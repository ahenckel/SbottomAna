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


class PyHist:
    """
    A class to get all the histogram from the root file
    Normalized to lumi or unit. It assumes the cross section is stored in XS and
    the initial number of event is stored in "NEvent"
    """
    def __init__(self, filename, Lumi=10*1000, XSName="XS", EventName="NEvent"):
        self.file = root_open(filename, "read")
        self.lumi = Lumi
        self.xs = self.file.Get(XSName).GetBinContent(2)
        self.Nevent = self.file.Get(EventName).GetBinContent(2)

    def GetCutFlow(self, dirname=""):
        if dirname == "":
            cutflowhist = self.file.Get("CutFlow")
        else:
            cutflowhist = self.file.Get("%s/CutFlow" % dirname)
        cutflowhist.Scale(1/cutflowhist.GetBinContent(1))
        return cutflowhist

    def GetHist(self, dirname, histname, norm="Lumi", BaseName = "NBase"):
        if histname == "CutFlow":
            return self.GetCutFlow(histname)
        nBase = self.file.Get("%s/%s" % (dirname, BaseName)).GetBinContent(2)
        hist =  self.file.Get("%s/%s" % (dirname, histname))
        hist.SetLineWidth(3)
        if hist.Integral() == 0:
            return hist
        if norm == "Lumi":
            scale = float(self.lumi) * self.xs * nBase / self.Nevent
            hist.Scale(scale/hist.Integral())
        elif norm == "Unit":
            hist.Scale(1/hist.Integral())
        else:
            print("No scaling")
        return hist

