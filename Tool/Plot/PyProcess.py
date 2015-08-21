#!/usr/bin/env python
# encoding: utf-8

# File        : PyProcess.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Jul 24
#
# Description : A class to handle different process, it will produce different
# histogram of the corresponding process

from PyHist import PyHist


class PyProcess():
    def __init__(self, name, filelist, attribute, lumi=10*1000):
        self.ProName = name
        self.ProList = []
        self.lumi = lumi
        for file in filelist:
            self.ProList.append(PyHist(file, Lumi=self.lumi))
        self.linecolor = attribute["linecolor"] if "linecolor" in attribute else 1
        self.linestyle = attribute["linestyle"] if "linestyle" in attribute else 1
        self.markercolor = attribute["markercolor"] if "markercolor" in attribute else 1
        self.markerstyle = attribute["markerstyle"] if "markerstyle" in attribute else 1
        self.fillcolor = attribute["fillcolor"] if "fillcolor" in attribute else 0
        self.fillstyle = attribute["fillstyle"] if "fillstyle" in attribute else 1001
        self.stype = attribute["type"] if "type" in attribute else "Signal"
        self.label = attribute["label"] if "label" in attribute else name

    def GetHist(self, dirname, histname, norm="Lumi", BaseName="NBase"):
        rehist = 0
        done = False
        if not done and histname.find("Efficiency") != -1:
            rehist = self.GetEfficiency(dirname, histname)
            done = True
        if not done:
            hists = [pros.GetHist(dirname, histname, norm, BaseName) for pros in self.ProList]
            for hist in hists:
                rehist += hist
            for var in vars(hists[0]):
                if var not in rehist:
                    setattr(rehist, var, getattr(hists[0], var))
            done = True

        if done and histname == "CutFlow":
            rehist.Scale(1/rehist.GetBinContent(1))

        rehist.ptype = self.stype
        rehist.norm = norm
        setattr(rehist, "Linecolor", self.linecolor)
        setattr(rehist, "Linestyle", self.linestyle)
        setattr(rehist, "Markercolor", self.markercolor)
        setattr(rehist, "Markerstyle", self.markerstyle)
        setattr(rehist, "Fillcolor", self.fillcolor)
        setattr(rehist, "Fillstyle", self.fillstyle)
        if hasattr(rehist, "HistFun"):
            rehist.HistFun(rehist)
        if hasattr(rehist, "DirFun"):
            rehist.DirFun(rehist)
        return rehist

    def GetDirnames(self):
        for paths, apples, objects in self.ProList[0].file.walk(maxdepth=0):
            return apples

    def GetHistnames(self, directory):
        for paths, apples, objects in self.ProList[0].file.walk():
            if paths == directory:
                return objects

    def GetEfficiency(self, dirname, histname):
        if histname.find("Efficiency") == -1:
            print("Wrong")
            return
        denorminator = histname.replace("Efficiency", "Denominator")
        numerator = histname.replace("Efficiency", "Numerator")
        hdenorminator = self.GetHist(dirname, denorminator, norm="None")
        hnumerator = self.GetHist(dirname, numerator, norm="None")
        hdenorminator.Sumw2()
        hnumerator.Sumw2()
        hdenorminator.Divide(hnumerator, hdenorminator, 1, 1, "B")
        hdenorminator.GetYaxis().SetTitle("Efficiency")
        return hdenorminator
