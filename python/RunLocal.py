#!/usr/bin/env python
# encoding: utf-8

# File        : RunLocal.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Feb 03
#
# Description :


import subprocess
import datetime
import os
maxEvents = -1
testname = "Test"
cfgfile = "SupperTopTagger_cfg.py"


procmap = {
    "ttjets"        : ["/data/nbay04/c/benwu/PHY14/TTJets_PU20bx25_PHYS14.root"],
    "QCDFlat"       : ["/data/nbay04/c/benwu/PHY14/QCD_FlatPt_PU20bx25_PHYS14.root"],
    "QCDHT1"        : ["/data/nbay04/c/benwu/PHY14/QCD_HT_250To500_PU20bx25_PHYS14.root"],
    "QCDHT2"        : ["/data/nbay04/c/benwu/PHY14/QCD_HT-500To1000_PU20bx25_PHYS14.root"],
    "QCDHT3"        : ["/data/nbay04/c/benwu/PHY14/QCD_HT_1000ToInf_PU20bx25_PHYS14.root"],
    "T14t_1500-100" : ["/data/nbay04/c/benwu/PHY14/T1tttt1500_100_PU40bx25_PHYS14.root"],

}

def RunProcess(key, value):
    inputs = ""
    for x in value[:-2]:
        inputs += "file:%s, " % x
    inputs += "file:%s " % value[-1]
    output = key
    cms = "cmsRun ../%s" % cfgfile
    cms = cms + " inputFiles=%s" % inputs
    cms = cms + " outputFile=%s" % output
    cms = cms + " maxEvents=%d" % maxEvents
    logfile = "%s-%s.log" % (key, now.strftime("%m%d-%H%M"))
    cms = cms + " >& %s &" % logfile
    subprocess.call(cms, shell=True)


if __name__ == "__main__":
    now = datetime.datetime.now()
    dirname = testname + "-"+ now.strftime("%m%d-%H%M")
    if not os.path.isdir(dirname):
        os.mkdir(dirname)
    pwd = os.getcwd()
    os.chdir(dirname)
    for key, value in procmap.items():
        RunProcess(key, value)
    os.chdir(pwd)
