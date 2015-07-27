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
    def __init__(self, name, filelist, lumi = 10*1000, label_="process", linecolor_=1, linestyle_=1, markercolor_=1, markerstyle_=1, fillcolor_=1, fillstyle_=1):
        self.ProName = name
        self.ProList = []
        self.lumi = lumi
        for file in filelist:
            self.ProList.append(PyHist(file, Lumi=self.lumi))
        self.linecolor = linecolor_
        self.linestyle = linestyle_
        self.markercolor = markercolor_
        self.markerstyle = markerstyle_
        self.fillcolor = fillcolor_
        self.fillstyle = fillstyle_


    def GetHist(self, dirname, histname, norm="Lumi", BaseName = "NBase"):
        hists = [pros.GetHist(dirname, histname, norm, BaseName) for pros in self.ProList]
        rehist = 0
        for hist in hists:
            rehist += hist
        return rehist


    def GetDirnames(self):
        # print self.ProList[0].file.walk(maxdepth=0)
        for paths, apples, objects in self.ProList[0].file.walk(maxdepth=0):
            return apples
            # print paths
            # print apples
            # print objects

        # return self.ProList[0].file.walk(depth=0)

