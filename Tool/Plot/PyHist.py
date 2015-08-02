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

    def GetCutFlow(self, dirname="", Norm=None):
        if dirname == "":
            cutflowhist = self.file.Get("CutFlow")
        else:
            cutflowhist = self.file.Get("%s/CutFlow" % dirname)
        if Norm is None:
            cutflowhist.Scale(1/cutflowhist.GetBinContent(1))
        else:
            if "NBase" in self.file and self.file.Get("NBase").GetBinContent(2) != 0:
                cutflowhist.Scale((self.xs * self.lumi * self.file.Get("NBase").GetBinContent(2) / self.Nevent)
                                  / cutflowhist.GetBinContent(1))
            else:
                cutflowhist.Scale(self.xs * self.lumi/cutflowhist.GetBinContent(1))
        return cutflowhist

    def GetHist(self, dirname, histname, norm="Lumi", BaseName="NBase"):
        if histname == "CutFlow":
            hist = self.GetCutFlow(dirname, Norm=norm)
        else:
            if "%s/%s" % (dirname, BaseName) in self.file:
                nBase = self.file.Get("%s/%s" % (dirname, BaseName)).GetBinContent(2)
            else:
                nBase = 0

            if dirname != "":
                hist = self.file.Get("%s/%s" % (dirname, histname))
            else:
                hist = self.file.Get("%s" % histname)

        hist.dirname = dirname
        hist.histname = histname

        if hist.Integral() != 0 and histname != "CutFlow":
            if norm == "Lumi" and self.xs != 0 and self.Nevent != 0:
                scale = float(self.lumi) * self.xs * nBase / self.Nevent
                hist.Scale(scale/hist.Integral())
            elif norm == "Unit":
                hist.Scale(1/hist.Integral())

        return self.SetHistProperty(hist)

    def SetHistProperty(self, hist):
        hist.SetLineWidth(3)
        hist.SetMarkerSize(1)

        if hist.histname in HIST_Proper:
            for keys, values in HIST_Proper[hist.histname].iteritems():
                setattr(hist, keys, values)

        if hist.dirname in DIR_Proper:
            for keys, values in DIR_Proper[hist.dirname].iteritems():
                setattr(hist, keys, values)

        if "RebinX" in hist:
            self.Hist1DRebinX(hist.histname, hist)

        return hist

    def Hist1DRebinX(self, histname, hist):
        label = hist.GetYaxis().GetTitle()
        if "RebinX" in HIST_Proper[histname]:
            hist.Rebin(HIST_Proper[histname]['RebinX'])
        avgbinwidth = (hist.GetXaxis().GetXmax() - hist.GetXaxis().GetXmin()) / hist.GetNbinsX()
        avgbinwidth = "0.1f".format(avgbinwidth) if avgbinwidth - int(avgbinwidth) > 0 else int(avgbinwidth)
        import re
        matchout = re.sub(r"(\d+)", str(avgbinwidth), label)
        hist.SetYTitle(matchout)
