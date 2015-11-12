#!usr/bin/env python
# encoding: utf-8

# File        : PyAna.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Jul 24
#
# Description : A common analyzer for manipulating PyHist, PyDraw, PyProcess.
# Dedicated studies should has its own class inherit or morm from this class

from PyProcess import PyProcess
from Config import Prodmap as PRODMA
from Config import Lumi
import re
import os
import glob


class PyAna():
    def __init__(self, dirname):
        self.Prodmap = PRODMA
        self.AllProds = {}
        self.Directory = dirname
        self.Lumi = Lumi
        self.dirnames = []
        self.FormProcesses()
        self.DumpContent()

    def FormProcesses(self):
        for key, vdict in self.Prodmap.items():
            tempPro = PyProcess(key, ["%s/%s" % (self.Directory, file) for file in vdict["file"]], vdict)
            if hasattr(tempPro, "ProList") and len(tempPro.ProList) != 0:
                self.AllProds[key] = tempPro

# ============================================================================#
# ----------------------------     User Access     ---------------------------#
# ============================================================================#
    def GetProNames(self, ptype=""):
        if ptype != "":
            return [signal for signal in self.AllProds.keys() if self.AllProds[signal].stype == ptype]
        else:
            return self.AllProds.keys()

    def GetDirNames(self):
        return(self.AllProds.itervalues().next().GetDirnames())

    def GetHistNames(self, directory):
        return(self.AllProds.itervalues().next().GetHistnames(directory))

    def GetHist(self, proname_, dirname_, histname_, norm="Lumi", BaseName="NBase", **kw):
        hists = []
        proname, dirname, histname = self.FormListofHist(proname_, dirname_, histname_)

        for hpro in proname:
            if hpro in ["Data", "Background", "Signal"]:
                if hpro in self.AllProds:
                    self.AllProds.pop(hpro, None)

                self.MergeProcesses(hpro)
            for hdist in dirname:
                for hhist in histname:
                    # for key, value in self.AllProds.items():
                    lhist = self.AllProds[hpro].GetHist(hdist, hhist, norm, BaseName, **kw)
                    lhist.proname = hpro
                    hists.append(lhist)
        self.SmartLegendEntry(hists, proname, dirname, histname)
        return hists

    def FormListofHist(self, proname_, dirname_, histname_):
        if not isinstance(proname_, list):
            proname = [proname_]
        else:
            if len(proname_) == 0:
                proname = list(self.AllProds.keys())
            else:
                proname = proname_
        tmp = []
        tmpProds = self.AllProds.keys() + ["Data", "Background", "Signal"]
        for reg in proname:
            if '+' in reg or '*' in reg or '?' in reg:
                tmp+=[pro for pro in tmpProds if re.match(reg, pro)]
            else:
                tmp.append(reg)
        proname = tmp

        if not isinstance(dirname_, list):
            dirname = [dirname_]
        else:
            if len(dirname_) == 0:
                dirname = self.AllProds.itervalues().next().GetDirnames()
            else:
                dirname = dirname_
        tmp = []
        for reg in dirname:
            if '+' in reg or '*' in reg or '?' in reg:
                tmp+=[pro for pro in self.AllProds.itervalues().next().GetDirnames() if re.match(reg, pro)]
            else:
                tmp.append(reg)
        dirname = tmp

        if not isinstance(histname_, list):
            histname = [histname_]
        else:
            if len(histname_) == 0:
                histname = self.AllProds.itervalues().next().GetHistnames(dirname[0])
            else:
                histname = histname_
        tmp = []
        for reg in histname:
            if '+' in reg or '*' in reg or '?' in reg:
                tmp+=[pro for pro in self.AllProds.itervalues().next().GetHistnames(dirname[0]) if re.match(reg, pro)]
            else:
                tmp.append(reg)
        histname = tmp

        return(proname, dirname, histname)

    def MergeProcesses(self, name, prolist=[], **kw):
        if not isinstance(prolist, list):
            prolist = [prolist]

        if len(prolist) == 0:
            prolist = self.GetProNames(name)

        import copy
        self.AllProds[name] = copy.deepcopy(self.AllProds[prolist[0]])
        self.AllProds[name].ProName = name
        self.AllProds[name].ProList = []
        for subprocess in prolist:
            self.AllProds[name].ProList.extend(self.AllProds[subprocess].ProList)

        # Assign other attribue
        for keys, values in kw.iteritems():
            setattr(self.AllProds[name], keys, values)

    def SmartLegendEntry(self, hists, pronames, dirnames, histnames_):
        histnames = []
        for hname in histnames_:
            rematch = re.match("^(\w*)_(\d+)", hname)
            if rematch is None:
                histnames.append(hname)
            else:
                histnames.append(rematch.group(1))
        histnames = list(set(histnames))

        for lhist in hists:
            leglable = ""
            outname = ""
            # Label
            if len(pronames) != 1:
                if lhist.proname in self.Prodmap:
                    leglable += self.Prodmap[lhist.proname]["label"] + " "
                else:
                    leglable += lhist.proname + " "
            if len(dirnames) != 1:
                leglable += lhist.DirLabel + " " if hasattr(lhist, "DirLabel") else lhist.dirname.split("_")[-1] + " "
            if len(histnames) != 1:
                leglable += lhist.HistLabel + " " if hasattr(lhist, "HistLabel") else lhist.histname + " "
            if len(pronames) == 1 and len(dirnames) == 1 and len(histnames) == 1:
                leglable += lhist.cutname

            # Output filename
            if len(pronames) == 1:
                outname += lhist.proname + "_"
            if len(dirnames) == 1:
                outname += lhist.dirname.split("_")[-1] + "_"
            if len(histnames) == 1:
                outname += histnames[0] + "_"

            # Lagend Style
            # Can't do much here. This should be handled in PyDraw
            lhist.title = leglable.strip()
            lhist.outname = outname.strip("_")

    def DumpContent(self, filename=".Hist"):
        redo = False
        if os.path.isfile("%s.Pronames" % filename):
            f = open("%s.Pronames" % filename, 'r')
            for line in f.readlines():
                if "PyAna" in line:
                    rematch = re.match("PyAna: (.+)$", line)
                    if rematch is not None:
                        redo = (rematch.group(1) != self.Directory)
                        break
            f.close()
        else:
            redo = True

        if not redo:
            return

        # Redo all the Hist Dump, first remove all the old files
        [os.remove(file) for file in glob.glob("%s*" % filename)]

        f = open("%s.Pronames" % filename, 'w')
        f.write("PyAna: %s\n\n" % self.Directory)
        for d in sorted(self.GetProNames()):
            f.write("%s\n" % d)
        f.close()

        self.AllProds.itervalues().next().DumpContent(filename, True)

