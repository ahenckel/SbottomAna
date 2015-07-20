#!/usr/bin/env python
# coding: utf-8

import os
import re
import time
import subprocess
import glob

DelExe    = '../testMain'
OutDir = '/eos/uscms/store/user/benwu/Stop/TopTagger/Jul16/'

tempdir = ''
UserEMAIL = 'benwu@fnal.gov'
ProjectName = 'GenJet2'
Process = {
    #'DYJetsToLL_HT_100to200'                     : ['../FileList/DYJetsToLL_HT_100to200.list',                     1],
    #'DYJetsToLL_HT_200to400'                     : ['../FileList/DYJetsToLL_HT_200to400.list',                     1],
    #'DYJetsToLL_HT_400to600'                     : ['../FileList/DYJetsToLL_HT_400to600.list',                     1],
    #'DYJetsToLL_HT_600toInf'                     : ['../FileList/DYJetsToLL_HT_600toInf.list',                     1],
    #'Signal_T1bbbb_mGluino1000_mLSP900'          : ['../FileList/Signal_T1bbbb_mGluino1000_mLSP900.list',          1],
    #'Signal_T1bbbb_mGluino1500_mLSP100'          : ['../FileList/Signal_T1bbbb_mGluino1500_mLSP100.list',          1],
    #'Signal_T1tttt_mGluino1200_mLSP800'          : ['../FileList/Signal_T1tttt_mGluino1200_mLSP800.list',          1],
    #'Signal_T1tttt_mGluino1500_mLSP100'          : ['../FileList/Signal_T1tttt_mGluino1500_mLSP100.list',          1],
    #'Signal_T2bb_mSbottom600_mLSP580'            : ['../FileList/Signal_T2bb_mSbottom600_mLSP580.list',            1],
    #'Signal_T2bb_mSbottom900_mLSP100'            : ['../FileList/Signal_T2bb_mSbottom900_mLSP100.list',            1],
    #'Signal_T2tt_mStop425_mLSP325'                : ['../FileList/Signal_T2tt_mStop425_mLSP325.list',               1],
    #'Signal_T2tt_mStop500_mLSP325'                : ['../FileList/Signal_T2tt_mStop500_mLSP325.list',               1],
    #'Signal_T2tt_mStop650_mLSP325'                : ['../FileList/Signal_T2tt_mStop650_mLSP325.list',               1],
    #'Signal_T2tt_mStop850_mLSP100'                : ['../FileList/Signal_T2tt_mStop850_mLSP100.list',               1],
    ##'Signal_T5tttt_mGluino1300_mStop300_mCh285'  : ['../FileList/Signal_T5tttt_mGluino1300_mStop300_mCh285.list',  1],
    ##'Signal_T5tttt_mGluino1300_mStop300_mChi280' : ['../FileList/Signal_T5tttt_mGluino1300_mStop300_mChi280.list', 1],
    ##'TTZ'                                        : ['../FileList/TTZ.list',                                        1],
    'TTbar'                                       : ['../FileList/TTbar.list',                                      44],
    'QCD'                                         : ['../FileList/QCD_HT_500to1000.list',                           7],
    ##'T_tW'                                       : ['../FileList/T_tW.list',                                       2],
    ##'Tbar_tW'                                    : ['../FileList/Tbar_tW.list',                                    1],
    ##'WJetsToLNu_HT_100to200'                     : ['../FileList/WJetsToLNu_HT_100to200.list',                     1],
    ##'WJetsToLNu_HT_200to400'                     : ['../FileList/WJetsToLNu_HT_200to400.list',                     1],
    ##'WJetsToLNu_HT_400to600'                     : ['../FileList/WJetsToLNu_HT_400to600.list',                     1],
    ##'WJetsToLNu_HT_600toInf'                     : ['../FileList/WJetsToLNu_HT_600toInf.list',                     1],
    #'ZJetsToNuNu_HT_100to200'                     : ['../FileList/ZJetsToNuNu_HT_100to200.list',                    13],
    #'ZJetsToNuNu_HT_200to400'                     : ['../FileList/ZJetsToNuNu_HT_200to400.list',                    10],
    #'ZJetsToNuNu_HT_400to600'                     : ['../FileList/ZJetsToNuNu_HT_400to600.list',                    10],
    'ZJetsToNuNu_HT_600toInf'                     : ['../FileList/ZJetsToNuNu_HT_600toInf.list',                    14],
}



def Condor_Sub(condor_file):
    ## Since we run with xrootd, setup proxy
    hasproxy = True
    proxyfile = ''
    if os.environ.has_key('X509_USER_PROXY'):
        proxyfile = os.path.abspath(os.environ['X509_USER_PROXY'])
    else:
        hasproxy = False
    if not hasproxy or not os.path.exists(proxyfile) or (time.time() - os.stat(proxyfile).st_ctime) / 60/24 > 1:
        print "Proxy file is at least one day old. Requesting new proxy"
        os.system("voms-proxy-init -valid 168:00 -voms cms")

    curdir = os.path.abspath(os.path.curdir)
    os.chdir(os.path.dirname(condor_file))
    print "To submit condor with " + condor_file
    os.system("condor_submit " + condor_file)
    os.chdir(curdir)


def SplitPro(key, file, fraction):
    splitedfiles = []
    filelistdir = tempdir + '/' + "FileList"
    try:
        os.makedirs(filelistdir)
    except OSError:
        pass


    filename = os.path.abspath(file)
    if fraction == 1:
        splitedfiles.append(os.path.abspath(filename))
        return splitedfiles

    f = open(filename, 'r')
    lines = f.readlines()
    lineperfile = len(lines) / fraction

    for i in range(0, fraction):
        outf = open("%s/%s.%d.list" % (filelistdir, key, i), 'w')
        splitedfiles.append(os.path.abspath("%s/%s.%d.list" % (filelistdir, key, i)))
        if i == fraction - 1 :
            outf.writelines(lines[lineperfile*i :])
        else:
            outf.writelines(lines[lineperfile*i : lineperfile*(i+1)])
        outf.close()

    return splitedfiles

def my_process():
    ## temp dir for submit
    global tempdir
    tempdir = '/tmp/' + os.getlogin() + "/" + ProjectName +  "/"
    try:
        os.makedirs(tempdir)
    except OSError:
        pass

    ## Create the output directory
    outdir = OutDir +  "/" + ProjectName + "/"
    try:
        os.makedirs(outdir)
    except OSError:
        pass

    ## Update RunHT.csh with DelDir and pileups
    RunHTFile = tempdir + "/" + "RunExe.csh"
    with open(RunHTFile, "wt") as outfile:
        for line in open("RunExe.csh", "r"):
            #line = line.replace("DELDIR", os.environ['PWD'])
            line = line.replace("DELDIR", os.environ['CMSSW_BASE'])
            line = line.replace("DELEXE", DelExe.split('/')[-1])
            line = line.replace("OUTDIR", outdir)
            outfile.write(line)

    ### Update condor files
    for key, value in Process.items():
        if not os.path.isfile(value[0]):
            continue
        npro = GetProcess(key, value)
        if len(npro) > 1:
            arg = "\nArguments = %s.$(Process).list %s_$(Process).root \nQueue %d \n" % (key, key, len(npro))
        else:
            arg = "\nArguments = %s.list %s.root \n Queue\n" % (key, key)

        npro.append(os.path.abspath(DelExe))
        tranferfiles = ", ".join(npro)
        ## Prepare the condor file
        condorfile = tempdir + "/" + "condor_" + ProjectName +"_" + key
        with open(condorfile, "wt") as outfile:
            for line in open("condor_template", "r"):
                line = line.replace("EXECUTABLE", os.path.abspath(RunHTFile))
                #line = line.replace("DELDIR", os.environ['CMSSW_BASE'])
                line = line.replace("TARFILES", tranferfiles)
                line = line.replace("TEMPDIR", tempdir)
                line = line.replace("PROJECTNAME", ProjectName)
                line = line.replace("ARGUMENTS", arg)
                outfile.write(line)

        Condor_Sub(condorfile)

def GetProcess(key, value):
    if len(value) == 1:
        return SplitPro(key, value[0], 1)
    else :
        return SplitPro(key, value[0], value[1])

if __name__ == "__main__":
    my_process()
