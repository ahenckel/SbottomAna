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


class PyAna():
    def __init__(self, dirname):
        self.Prodmap = PRODMA
        self.AllProds = {}
        self.Directory = dirname
        self.Lumi = Lumi
        self.dirnames = []
        self.FormProcesses()

    def FormProcesses(self):
        for key, vdict in self.Prodmap.items():
            self.AllProds[key] = PyProcess(key, ["%s/%s" % (self.Directory, file) for file in vdict["file"]], vdict)

# ============================================================================#
# ----------------------------     User Access     ---------------------------#
# ============================================================================#
    def GetProNames(self, ptype=""):
        if ptype != "":
            return [signal for signal in self.AllProds.keys() if self.AllProds[signal]["type"] == ptype]
        else:
            return self.AllProds.keys()

    def GetDirNames(self):
        return(self.AllProds.itervalues().next().GetDirnames())

    def GetHistNames(self, directory):
        return(self.AllProds.itervalues().next().GetHistnames(directory))

    def GetHist(self, proname_, dirname_, histname_, norm="Lumi", BaseName="NBase"):
        hists = []
        proname, dirname, histname = self.FormListofHist(proname_, dirname_, histname_)

        for hpro in proname:
            if hpro in ["Data", "Background", "Signal"]:
                self.MergeProcesses(hpro)
            for hdist in dirname:
                for hhist in histname:
                    # for key, value in self.AllProds.items():
                    lhist = self.AllProds[hpro].GetHist(hdist, hhist, norm, BaseName)
                    lhist.proname = hpro
                    self.SmartLegendEntry(lhist, proname, dirname, histname)
                    hists.append(lhist)
        return hists

    def FormListofHist(self, proname_, dirname_, histname_):
        # localProd = self.AllProds.copy()
        if not isinstance(proname_, list):
            proname = [proname_]
        else:
            if len(proname_) == 0:
                proname = list(self.AllProds.keys())
            else:
                proname = proname_

        if not isinstance(dirname_, list):
            dirname = [dirname_]
        else:
            if len(dirname_) == 0:
                dirname = self.AllProds.itervalues().next().GetDirnames()
            else:
                dirname = dirname_

        if not isinstance(histname_, list):
            histname = [histname_]
        else:
            if len(histname_) == 0:
                histname = self.AllProds.itervalues().next().GetHistnames(dirname[0])
            else:
                histname = histname_
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
        self.AllProds[name].ProList.extend(self.AllProds[subprocess].ProList for subprocess in prolist)

        # Assign other attribue
        for keys, values in kw.iteritems():
            setattr(self.AllProds[name], keys, values)

    def SmartLegendEntry(self, lhist, pronames, dirnames, histnames):
        leglable = ""
        outname = ""

        # Label
        if len(pronames) != 1:
            leglable += self.Prodmap[lhist.proname]["label"] + " "
        if len(dirnames) != 1:
            leglable += lhist.DirLabel + " " if hasattr(lhist, "DirLabel") else lhist.dirname.split("_")[-1] + " "
        if len(histnames) != 1:
            leglable += lhist.HistLabel + " " if hasattr(lhist, "HistLabel") else lhist.histname + " "
        if len(pronames) == 1 and len(dirnames) == 1 and len(histnames) == 1:
            leglable += self.Prodmap[lhist.proname]["label"] + " "

        # Output filename
        if len(pronames) == 1:
            outname += lhist.proname + "_"
        if len(dirnames) == 1:
            outname += lhist.dirname.split("_")[-1] + "_"
        if len(histnames) == 1:
            outname += lhist.histname + "_"

        # Lagend Style
        # Can't do much here. This should be handled in PyDraw
        lhist.title = leglable.strip()
        lhist.outname = outname.strip("_")

    def GetSignalBackground(self, proname_, dirname_, histname_, style="line", norm="Lumi", BaseName="NBase"):
        pass
