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
    ROOT.kGreen,
    ROOT.kGray,
    ROOT.kSpring,
    ROOT.kTeal,
    ROOT.kAzure,
    ROOT.kViolet,
    ROOT.kPink
]

# (1001, 228,26,28),
# (1002, 55,126,184),
# (1003, 77,175,74),
# (1004, 152,78,163),
# (1005, 255,127,0),
# (1006, 255,255,51),
# (1007, 166,86,40),
# (1008, 247,129,191),
# (1009, 153,153,153)


PyOutPut = ["png", "root"]

Lumi = 3 * 1000

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
    "T2bb_600_580" : { "file" : ["Signal_T2bb_mSbottom600_mLSP580.root"],
                      "type" : "Signal",
                      "label" : "T2bb(600, 580)",
                      "linecolor" : ROOT.kRed,
                      "linestyle" : 1,
                      "markercolor" : ROOT.kRed,
                      "markerstyle" : 1,
                      "fillcolor" : ROOT.kRed,
                      "fillstyle" : 1001,
                     },
    "T2bb_900_100" : { "file" : ["Signal_T2bb_mSbottom900_mLSP100.root"],
                      "type" : "Signal",
                      "label" : "T2bb(900, 100)",
                      "linecolor" : ROOT.kRed+4,
                      "linestyle" : 1,
                      "markercolor" : ROOT.kRed+4,
                      "markerstyle" : 1,
                      "fillcolor" : ROOT.kRed+4,
                      "fillstyle" : 1001,
                     },
    "TTbarDiLep" : { "file" : ["TTbarDiLep.root"],
                    "type" : "Background",
                    "label" : "t#bar{t}(ll)",
                    "linecolor" : ROOT.kGreen,
                    "linestyle" : 1,
                    "markercolor" : ROOT.kGreen,
                    "markerstyle" : 1,
                    "fillcolor" : ROOT.kGreen,
                    "fillstyle" : 1001,
                   },
    "TTbar" : { "file" : ["TTbarInc.root"],
               "type" : "Background",
               "label" : "t#bar{t}",
               "linecolor" : ROOT.kGreen,
               "linestyle" : 1,
               "markercolor" : ROOT.kGreen,
               "markerstyle" : 1,
               "fillcolor" : ROOT.kGreen,
               "fillstyle" : 1001,
              },
    "WJetsToLNu" : { "file" : [
        "WJetsToLNu_HT_100to200.root",
        "WJetsToLNu_HT_200to400.root",
        "WJetsToLNu_HT_400to600.root",
        "WJetsToLNu_HT_600to800.root",
        "WJetsToLNu_HT_800to1200.root",
        "WJetsToLNu_HT_1200to2500.root",
        # "WJetsToLNu_HT_2500toInf.root",
        # "WJetsToLNu_HT_600toInf.root",
                 ],
                  "label" : "W(l#nu)",
                  "type" : "Background",
                  "linecolor" : ROOT.kMagenta+1,
                  "linestyle" : 1,
                  "markercolor" : ROOT.kMagenta+1,
                  "markerstyle" : 1,
                  "fillcolor" : ROOT.kMagenta+1,
                  "fillstyle" : 1001,
                 },
"DYJetsToLL" : { "file" : [
    "DYJetsToLL_HT_100to200.root",
    "DYJetsToLL_HT_200to400.root",
    "DYJetsToLL_HT_400to600.root",
    "DYJetsToLL_HT_600toInf.root"],
                  "label" : "DY(ll)",
                  "type" : "Background",
                  "linecolor" : ROOT.kRed,
                  # "linecolor" : ROOT.kYellow-7,
                  "linestyle" : 1,
                  "markercolor" : ROOT.kYellow-7,
                  "markerstyle" : 1,
                  "fillcolor" : ROOT.kYellow-7,
                  "fillstyle" : 1001,
                 },
"ZJetsToNuNu" : { "file" : [ "ZJetsToNuNu_HT_100to200.root", "ZJetsToNuNu_HT_200to400.root",
                             "ZJetsToNuNu_HT_400to600.root", "ZJetsToNuNu_HT_600toInf.root" ],
                  "label" : "Z(#nu#nu)",
                  "type" : "Background",
                  "linecolor" : ROOT.kTeal+4,
                  "linestyle" : 1,
                  "markercolor" : ROOT.kTeal+4,
                  "markerstyle" : 1,
                  "fillcolor" : ROOT.kTeal+4,
                  "fillstyle" : 1001,
                 },
"QCD"  : { "file" : ["QCD_HT1000to1500.root", "QCD_HT1500to2000.root", "QCD_HT2000toInf.root",
                     "QCD_HT200to300.root", "QCD_HT300to500.root",
                     "QCD_HT500to700.root", "QCD_HT700to1000.root"],
          "label" : "QCD",

          "type" : "Background",
          "linecolor" : ROOT.kBlue,
          "linestyle" : 1,
          "markercolor" : ROOT.kBlue,
          "markerstyle" : 1,
          "fillcolor" : ROOT.kBlue,
          "fillstyle" : 1001,
         },
# "TTZ"  : { "file" : [ "TTZToLLNuNu.root", "TTZToQQ.root"],
"TTZ"  : { "file" : [ "TTZToLLNuNu.root"],
          "label" : "t#bar{t}Z",
          "type" : "Signal",
          # "type" : "Background",
          "linecolor" : ROOT.kOrange+2,
          # "linecolor" : ROOT.kTeal+4,
          "linestyle" : 1,
          "markercolor" : ROOT.kOrange+2,
          # "markercolor" : ROOT.kTeal+4,
          "markerstyle" : 1,
          "fillcolor" : ROOT.kOrange+2,
          # "fillcolor" : ROOT.kTeal+4,
          "fillstyle" : 1001,
         },
"TTW"  : { "file" : [ "TTWJetsToQQ.root", "TTWJetsToLNu.root"],
          "label" : "t#bar{t}W",
          "type" : "Background",
          "linecolor" : ROOT.kSpring+2,
          "linestyle" : 1,
          "markercolor" : ROOT.kSpring+2,
          "markerstyle" : 1,
          "fillcolor" : ROOT.kSpring +2,
          "fillstyle" : 1001,
         },

"TW"  : { "file" : [ "tW_antitop.root", "tW_top.root"],
         "label" : "tW",
         "type" : "Background",
         "linecolor" : ROOT.kYellow,
         "linestyle" : 1,
         "markercolor" : ROOT.kYellow,
         "markerstyle" : 1,
         "fillcolor" : ROOT.kYellow,
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
        "Rebinx": 5,
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
