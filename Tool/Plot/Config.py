#!/usr/bin/env python
# encoding: utf-8

# File        : Config.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Jul 28
#
# Description :

import ROOT

#============================================================================#
#--------------------------     Drawing Config     --------------------------#
#============================================================================#
PyColors = [
    ROOT.kBlack,
    ROOT.kRed,
    ROOT.kBlue,
    ROOT.kGreen-2,
    ROOT.kOrange,
    ROOT.kCyan,
    ROOT.kMagenta,
    ROOT.kYellow,
    ROOT.kGreen
]

PyOutPut = ["png", "root"]

Lumi = 10 * 1000

#============================================================================#
#---------------------------     Processes Map     --------------------------#
#============================================================================#
Prodmap = {
    "T2tt_425_325" : { "file" : ["Signal_T2tt_mStop425_mLSP325.root"],
                      "label" : "T2tt(425, 325)",
                      "type" : "Signal",
                      "linecolor" : ROOT.kRed-2,
                      "linestyle" : 2,
                      "markercolor" : ROOT.kRed-2,
                      "markerstyle" : 1,
                      "fillcolor" : ROOT.kRed-2,
                      "fillstyle" : 1001,
                      },
    "T2tt_500_325" : { "file" : ["Signal_T2tt_mStop500_mLSP325.root"],
                      "label" : "T2tt(500, 325)",
                      "type" : "Signal",
                      "linecolor" : ROOT.kRed,
                      "linestyle" : 1,
                      "markercolor" : ROOT.kRed,
                      "markerstyle" : 1,
                      "fillcolor" : ROOT.kRed,
                      "fillstyle" : 1001,
                      },
    "T2tt_650_325" : { "file" : ["Signal_T2tt_mStop650_mLSP325.root"],
                      "label" : "T2tt(650, 325)",
                      "type" : "Signal",
                      "linecolor" : ROOT.kRed+2,
                      "linestyle" : 1,
                      "markercolor" : ROOT.kRed+2,
                      "markerstyle" : 1,
                      "fillcolor" : ROOT.kRed+2,
                      "fillstyle" : 1001,
                      },
    "T2tt_850_100" : { "file" : ["Signal_T2tt_mStop850_mLSP100.root"],
                      "type" : "Signal",
                      "label" : "T2tt(850, 100)",
                      "linecolor" : ROOT.kRed+4,
                      "linestyle" : 1,
                      "markercolor" : ROOT.kRed+4,
                      "markerstyle" : 1,
                      "fillcolor" : ROOT.kRed+4,
                      "fillstyle" : 1001,
                      },
    "TTbar"        : { "file" : ["TTbar.root"],
                      "type" : "Background",
                      "label" : "t#bar{t}",
                      "linecolor" : ROOT.kGreen,
                      "linestyle" : 1,
                      "markercolor" : ROOT.kGreen,
                      "markerstyle" : 1,
                      "fillcolor" : ROOT.kGreen,
                      "fillstyle" : 1001,
                      },
"WJetsToLNu"   : { "file" : [ "WJetsToLNu_HT_100to200.root", "WJetsToLNu_HT_200to400.root",
                             "WJetsToLNu_HT_400to600.root", "WJetsToLNu_HT_600toInf.root"],
                  "label" : "W(l#nu)",
                  "type" : "Background",
                  "linecolor" : ROOT.kMagenta+1,
                  "linestyle" : 1,
                  "markercolor" : ROOT.kMagenta+1,
                  "markerstyle" : 1,
                  "fillcolor" : ROOT.kMagenta+1,
                  "fillstyle" : 1001,
                  },
"ZJetsToNuNu"  : { "file" : [ "ZJetsToNuNu_HT_100to200.root", "ZJetsToNuNu_HT_200to400.root",
                             "ZJetsToNuNu_HT_400to600.root", "ZJetsToNuNu_HT_600toInf.root" ],
                  "label" : "Z(#nu#nu)",
                  "type" : "Background",
                  "linecolor" : ROOT.kBlue,
                  # "linecolor" : ROOT.kTeal+4,
                  "linestyle" : 1,
                  "markercolor" : ROOT.kBlue,
                  # "markercolor" : ROOT.kTeal+4,
                  "markerstyle" : 1,
                  "fillcolor" : ROOT.kBlue,
                  # "fillcolor" : ROOT.kTeal+4,
                  "fillstyle" : 1001,
                  },
}

#============================================================================#
#--------------------------     User Functions     --------------------------#
#============================================================================#


def PrintHist(hist):
    for bins in range(1, hist.GetNbinsX()):
        print(bins, hist.GetBinCenter(bins), hist.GetBinContent(bins))

#============================================================================#
#-----------------------     Histogram properties     -----------------------#
#============================================================================#
HIST_Proper = {
    "MissTopTagPT": {
        "RebinX": 5,
        "HistLabel": "----",
        "Logy": False,
        "Logx": False,
        "YScale": 1.2,
        "YMax": 0,
        # "DrawOpt": "P text",
        # "legloc" : "None",
        # "HistFun": PrintHist
    }
}

DIR_Proper = {
    "Top_G3Top30": {"DirLabel": "TT"}
}
