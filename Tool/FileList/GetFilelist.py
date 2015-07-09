#!/usr/bin/env python
# encoding: utf-8

# File        : GetFilelist.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Jun 11
#
# Description :

from __future__ import print_function
import glob
import fnmatch
import os

hostname = os.uname()[1]
if hostname == 'macbook':
    NtupleDir = "/Users/benwu/Data/Dataset/PHY14_Ntuple/PHYS14_720_Mar14_2014_v2/"
if hostname == 'nbay04.fnal.gov':
    NtupleDir = "/data/nbay04/c/benwu/PHY14_Ntuple/PHYS14_720_Mar14_2014_v2/"

NameDir = {
    "TTbar"                                      : "PU20bx25_TTJets_MSDecaysCKM_madgraph-tauola",
    "WJetsToLNu_HT_600toInf"                     : "PU20bx25_WJetsToLNu_HT-600toInf_madgraph-tauola",
    "WJetsToLNu_HT_400to600"                     : "PU20bx25_WJetsToLNu_HT-400to600_madgraph-tauola" ,
    "WJetsToLNu_HT_200to400"                     : "PU20bx25_WJetsToLNu_HT-200to400_madgraph-tauola" ,
    "WJetsToLNu_HT_100to200"                     : "PU20bx25_WJetsToLNu_HT-100to200_madgraph-tauola",
    "ZJetsToNuNu_HT_600toInf"                    : "PU20bx25_ZJetsToNuNu_HT-600toInf_madgraph-tauola",
    "ZJetsToNuNu_HT_400to600"                    : "PU20bx25_ZJetsToNuNu_HT-400to600_madgraph-tauola",
    "ZJetsToNuNu_HT_200to400"                    : "PU20bx25_ZJetsToNuNu_HT-200to400_madgraph-tauola",
    "ZJetsToNuNu_HT_100to200"                    : "PU20bx25_ZJetsToNuNu_HT-100to200_madgraph-tauola",
    "DYJetsToLL_HT_600toInf"                     : "DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola",
    "DYJetsToLL_HT_400to600"                     : "DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola",
    "DYJetsToLL_HT_200to400"                     : "DYJetsToLL_M-50_HT-200to400_Tune4C_13TeV-madgraph-tauola",
    "DYJetsToLL_HT_100to200"                     : "DYJetsToLL_M-50_HT-100to200_Tune4C_13TeV-madgraph-tauola",
    #"QCD_HT_1000toInf",
    #"QCD_HT_500to1000",
    #"QCD_HT_250to500",
    "T_tW"                                       : "PU20bx25_T_tW-channel-DR_powheg-tauola",
    "Tbar_tW"                                    : "PU20bx25_Tbar_tW-channel-DR_powheg-tauola",
    "TTZ"                                        : "PU20bx25_TTZJets_madgraph-tauola",
    "Signal_T1tttt_mGluino1200_mLSP800"          : "PU20bx25_T1tttt_2J_mGl-1200_mLSP-800_madgraph-tauola",
    "Signal_T1tttt_mGluino1500_mLSP100"          : "PU20bx25_T1tttt_2J_mGl-1500_mLSP-100_madgraph-tauola",
    "Signal_T5tttt_mGluino1300_mStop300_mChi280" : "T5ttttDeg_mGo1300_mStop300_mChi280_4bodydec_v2",
    "Signal_T5tttt_mGluino1300_mStop300_mCh285"  : "T5ttttDeg_mGo1300_mStop300_mCh285_mChi280_23bodydec_v2",
    "Signal_T1bbbb_mGluino1000_mLSP900"          : "PU20bx25_T1bbbb_2J_mGl-1000_mLSP-900_madgraph-tauola",
    "Signal_T1bbbb_mGluino1500_mLSP100"          : "PU20bx25_T1bbbb_2J_mGl-1500_mLSP-100_madgraph-tauola",
    "Signal_T2tt_mStop425_mLSP325"               : "PU20bx25_T2tt_2J_mStop-425_mLSP-325_madgraph-tauola",
    "Signal_T2tt_mStop500_mLSP325"               : "PU20bx25_T2tt_2J_mStop-500_mLSP-325_madgraph-tauola",
    "Signal_T2tt_mStop650_mLSP325"               : "PU20bx25_T2tt_2J_mStop-650_mLSP-325_madgraph-tauola",
    "Signal_T2tt_mStop850_mLSP100"               : "PU20bx25_T2tt_2J_mStop-850_mLSP-100_madgraph-tauola",
    "Signal_T2bb_mSbottom600_mLSP580"            : "PU20bx25_T2bb_2J_mStop-600_mLSP-580_madgraph-tauola",
    "Signal_T2bb_mSbottom900_mLSP100"            : "PU20bx25_T2bb_2J_mStop-900_mLSP-100_madgraph-tauola",

    ## Unmatched ntuples
    #"DYJetsToLL_M-50_13TeV-madgraph-pythia8"
    #"PU20bx25_GJets_HT-100to200_madgraph-tauola"
    #"PU20bx25_GJets_HT-200to400_madgraph-tauola"
    #"PU20bx25_GJets_HT-400to600_madgraph-tauola"
    #"PU20bx25_GJets_HT-600toInf_madgraph-tauola"
    #"PU20bx25_QCD_Pt-1000to1400_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-120to170_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-1400to1800_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-15to30_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-170to300_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-1800toInf_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-300to470_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-30to50_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-470to600_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-50to80_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-600to800_Tune4C_13TeV_pythia8"
    #"PU20bx25_QCD_Pt-800to1000_Tune4C_13TeV_pythia8"
#"PU20bx25_QCD_Pt-80to120_Tune4C_13TeV_pythia8"
#"PU20bx25_TTWJets_madgraph-tauola"
#"PU20bx25_WJetsToLNu_madgraph-tauola"
#"TTDMDMJets_M1000GeV"
#"TTDMDMJets_M600GeV"
#"ZJetsToNuNu_HT-100to200_Tune4C_13TeV-madgraph-tauola"
#"ZJetsToNuNu_HT-200to400_Tune4C_13TeV-madgraph-tauola"
#"ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola"
#"ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola"
}

if __name__ == "__main__":
    for key, value in NameDir.items():
        matches = []
        file = open ("%s.list" % key, "w")
        for root, dirnames, filenames in os.walk( "%s/%s/" % (NtupleDir, value) ):
            for filename in fnmatch.filter(filenames, '*.root'):
                file.write("%s\n" % os.path.join(root, filename))
        file.close()




