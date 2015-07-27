#!/usr/bin/env python
# encoding: utf-8

# File        : PyAna.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Jul 24
#
# Description : A common analyzer for manipulating PyHist, PyDraw, PyProcess.
# Dedicated studies should has its own class inherit or morm from this class

from PyProcess import PyProcess
import ROOT

Prodmap = {
    "T2tt_425_325" : { "file" : ["Signal_T2tt_mStop425_mLSP325.root"],
                      "label" : "T2tt(425, 325)",
                      "type" : "Signal",
                      "linecolor" : 1,
                      "linestyle" : 1,
                      "markercolor" : 1,
                      "markerstyle" : 1,
                      "fillcolor" : 1,
                      "fillstyle" : 1,
                      },
    "T2tt_500_325" : { "file" : ["Signal_T2tt_mStop500_mLSP325.root"],
                      "label" : "T2tt(425, 325)",
                      "type" : "Signal",
                      "linecolor" : 1,
                      "linestyle" : 1,
                      "markercolor" : 1,
                      "markerstyle" : 1,
                      "fillcolor" : 1,
                      "fillstyle" : 1,
                      },
    "T2tt_650_325" : { "file" : ["Signal_T2tt_mStop650_mLSP325.root"],
                      "label" : "T2tt(425, 325)",
                      "type" : "Signal",
                      "linecolor" : 2,
                      "linestyle" : 1,
                      "markercolor" : 1,
                      "markerstyle" : 1,
                      "fillcolor" : 1,
                      "fillstyle" : 1,
                      },
    "T2tt_850_100" : { "file" : ["Signal_T2tt_mStop850_mLSP100.root"],
                      "label" : "T2tt(425, 325)",
                      "type" : "Signal",
                      "linecolor" : 3,
                      "linestyle" : 1,
                      "markercolor" : 1,
                      "markerstyle" : 1,
                      "fillcolor" : 1,
                      "fillstyle" : 1,
                      },
    "TTbar"        : { "file" : ["TTbar.root"],
                      "type" : "Signal",
                      "label" : "t#bar{t}",
                      "linecolor" : ROOT.kRed,
                      "linestyle" : 1,
                      "markercolor" : 1,
                      "markerstyle" : 1,
                      "fillcolor" : 1,
                      "fillstyle" : 1,
                      },
    "WJetsToLNu"   : { "file" : [ "WJetsToLNu_HT_100to200.root", "WJetsToLNu_HT_200to400.root",
                                "WJetsToLNu_HT_400to600.root", "WJetsToLNu_HT_600toInf.root"],
                    "label" : "T2tt(425, 325)",
                    "type" : "Signal",
                    "linecolor" : 1,
                    "linestyle" : 1,
                    "markercolor" : 1,
                    "markerstyle" : 1,
                    "fillcolor" : 1,
                    "fillstyle" : 1,
                    },
    "ZJetsToNuNu"  : { "file" : [ "ZJetsToNuNu_HT_100to200.root", "ZJetsToNuNu_HT_200to400.root",
                                "ZJetsToNuNu_HT_400to600.root", "ZJetsToNuNu_HT_600toInf.root" ],
                    "label" : "T2tt(425, 325)",
                    "type" : "Signal",
                    "linecolor" : 1,
                    "linestyle" : 1,
                    "markercolor" : 1,
                    "markerstyle" : 1,
                    "fillcolor" : 1,
                    "fillstyle" : 1,
                    },
    }

class PyAna():
    global Prodmap
    def __init__(self, dirname, lumi = 10*1000):
        self.AllProds = {}
        self.Directory = dirname
        self.Lumi = lumi
        self.dirnames = []


    def FormProcesses(self):
        for key, vdict in Prodmap.items():
            self.AllProds[key] = PyProcess(key, ["%s/%s" % (self.Directory, file) for file in vdict["file"]],
                                           lumi = self.Lumi, label_=vdict["label"], linecolor_=vdict["linecolor"],
                                           linestyle_=vdict["linestyle"] , markercolor_= vdict["markercolor"],
                                           markerstyle_= vdict["markerstyle"], fillcolor_=vdict["fillcolor"],
                                           fillstyle_=vdict["fillstyle"])

    def GetHist(self, proname_, dirname_, histname_, norm="Lumi", BaseName = "NBase"):
        hists = []
        # localProd = self.AllProds.copy()
        if not isinstance(proname_, list):
            proname = [proname_]
        else:
            if len(proname_) == 0:
                proname = list(self.AllProds.keys())
                print proname
            else:
                proname = proname_

        if not isinstance(dirname_, list):
            dirname = [dirname_]
        else:
            if len(dirname_) == 0:
                dirname = self.AllProds.itervalues().next().GetDirnames()
                print dirname
            else:
                dirname = dirname_

        if not isinstance(histname_, list):
            histname = [histname_]
        else:
            if len(histname_) == 0:
                histname = self.AllProds.itervalues().next().keys()
            else:
                histname = histname_


        for hpro in proname:
            for hdist in dirname:
                for hhist in histname:
                    # for key, value in self.AllProds.items():
                    lhist = self.AllProds[hpro].GetHist(hdist, hhist, norm, BaseName)
                    lhist.linecolor = Prodmap[hpro]["linecolor"]
                    leglable = ""
                    if len(proname) != 1:
                        leglable +=  Prodmap[hpro]["label"] + " "
                    if len(proname) == 1 and len(dirname) != 1:
                        leglable +=  hdist + " "
                    # if len(histname) != 1:
                    leglable +=  lhist.GetTitle() + " "
                    lhist.title = leglable
                    lhist.legendstyle='L'
                    hists.append(lhist)
        return hists



    # print "=---"
   # for key, value in test.AllProds:
       # print key, value.GetHist("T3Top20", "NBase")

