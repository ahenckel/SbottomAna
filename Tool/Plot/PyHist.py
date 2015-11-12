#!/usr/bin/env python
# encoding: utf-8

# File        : GetHis.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Jul 17
#
# Description : Class to get the histogram with normalized among HT


from __future__ import print_function
from rootpy.io import root_open
from Config import HIST_Proper
from Config import DIR_Proper
import re
import rootpy


class PyHist:
    """
    A class to get all the histogram from the root file
    Normalized to lumi or unit. It assumes the cross section is stored in XS and
    the initial number of event is stored in "NEvent".
    """
    def __init__(self, filename, Lumi=10*1000, XSName="XS", EventName="NEvent"):
        self.file = root_open(filename, "read")
        self.lumi = Lumi
        self.xs = self.file.Get(XSName).GetBinContent(2) if XSName in self.file else 0
        self.Nevent = self.file.Get(EventName).GetBinContent(2) if EventName in self.file else 0
        self.cutpat = re.compile("^(\w*)_(\d+)")

    def GetCutFlow(self, dirname="", Norm=None):
        if dirname == "":
            cutflowhist = self.file.Get("CutFlow")
        else:
            cutflowhist = self.file.Get("%s/CutFlow" % dirname)
        if Norm is None:
            cutflowhist.Scale(1/cutflowhist.GetBinContent(1))
        elif Norm == "XS":
            cutflowhist.Scale(self.xs/cutflowhist.GetBinContent(1))
        else:
            if "NBase" in self.file and self.file.Get("NBase").GetBinContent(2) != 0:
                cutflowhist.Scale((self.xs * self.lumi * self.file.Get("NBase").GetBinContent(2) / self.Nevent)
                                  / cutflowhist.GetBinContent(1))
        return cutflowhist

    def GetHist(self, dirname, histname, norm="Lumi", BaseName="NBase", **kw):
        if histname == "CutFlow":
            hist = self.GetCutFlow(dirname, Norm=norm)
        else:
            if dirname != "":
                hist = self.file.Get("%s/%s" % (dirname, histname))
            else:
                hist = self.file.Get("%s" % histname)

        nBase = self.GetNBase(dirname, histname, BaseName)
        hist.cutname = self.GetCutName(dirname, histname)
        hist.dirname = dirname
        hist.histname = histname

        if hist.Integral() != 0 and histname != "CutFlow":
            if norm == "Lumi" and self.xs != 0 and self.Nevent != 0:
                scale = float(self.lumi) * self.xs * nBase / self.Nevent
                hist.Scale(scale/hist.Integral())
            elif norm == "Unit":
                hist.Scale(1/hist.Integral())

        return self.SetHistProperty(hist, kw)

    def SetHistProperty(self, hist, kw):
        ## Setting some default
        hist.SetLineWidth(3)
        hist.SetMarkerSize(1)
        if not hasattr(hist, "outname"):
            setattr(hist, "outname", hist.histname)
        if not hasattr(hist, "Linecolor"):
            setattr(hist, "Linecolor", 1)
        if not hasattr(hist, "Linestyle"):
            setattr(hist, "Linestyle", 1)
        if not hasattr(hist, "ptype"):
            setattr(hist, "ptype", "Signal")
        if not hasattr(hist, "proname"):
            setattr(hist, "proname", "X")

        if hist.histname in HIST_Proper:
            for keys, values in HIST_Proper[hist.histname].iteritems():
                setattr(hist, keys, values)

        if hist.dirname in DIR_Proper:
            for keys, values in DIR_Proper[hist.dirname].iteritems():
                setattr(hist, keys, values)

        for k, v in kw.iteritems():
            setattr(hist, k, v)

        # Set strip histname
        rematch = re.match("^(\w*)_(\d+)", hist.histname)
        if rematch is None:
            setattr(hist, "striphistname", hist.histname)
        else:
            setattr(hist, "striphistname", rematch.group(1))

        self.HistRebin(hist)
        return hist

    def HistRebin(self, hist):
        if hasattr(hist, "Rebiny") and isinstance(hist, rootpy.plotting.hist._Hist2D):
            hist.RebinY(hist.Rebiny)

        if hasattr(hist, "Rebinx"):
            hist.RebinX(hist.Rebinx)
            label = hist.GetYaxis().GetTitle()
            avgbinwidth = (hist.GetXaxis().GetXmax() - hist.GetXaxis().GetXmin()) / hist.GetNbinsX()
            avgbinwidth = "0.1f".format(avgbinwidth) if avgbinwidth - int(avgbinwidth) > 0 else int(avgbinwidth)
            matchout = re.sub(r"/\s*(\d+)", str(avgbinwidth), label)
            hist.SetYTitle(matchout)

    def GetDirnames(self):
        for paths, apples, objects in self.file.walk(maxdepth=0):
            return apples

    def GetHistnames(self, directory):
        for paths, apples, objects in self.file.walk():
            if paths == directory:
                return objects

    def GetNBase(self, dirname, histname, BaseName_):
        patmat = self.cutpat.match(histname)
        if patmat is None:
            BaseName = BaseName_
        else:
            BaseName = "%s_%s" % (BaseName_, patmat.group(2))

        if "%s/%s" % (dirname, BaseName) in self.file:
            if "NBase" in BaseName:
                nBase = self.file.Get("%s/%s" % (dirname, BaseName)).GetBinContent(2)
            else:
                nBase = self.file.Get("%s/%s" % (dirname, BaseName)).Integral()
        else:
            nBase = 0

        return nBase

    def DumpContent(self, filename=".Hist", append=False):
        if not append:
            import os
            import glob
            [os.remove(file) for file in glob.glob("%s*" % filename)]

        # Dir name
        f = open("%s.Dirnames" % filename, 'w')
        for d in sorted(self.GetDirnames()):
            f.write("%s\n" % d)
        f.write("\n\n\n")
        for paths, apples, objects in self.file.walk(maxdepth=0):
            [f.write("%s\n" % obj) for obj in objects]
        f.close()

        # Hist in Each Dir
        for d in sorted(self.GetDirnames()):
            if(len(self.GetHistnames(d)) == 0):
                continue
            f = open("%s.%s" % (filename, d), 'w')
            for h in sorted(self.GetHistnames(d)):
                f.write("%s\n" % h)
            f.close()

    def GetCutName(self, dirname, histname):
        patmat = self.cutpat.match(histname)
        if patmat is None:
            return ""
        else:
            cutbin = int(patmat.group(2))+1
            cutflowhist = self.file.Get("%s/CutFlow" % dirname)
            return cutflowhist.GetXaxis().GetBinLabel(cutbin)
