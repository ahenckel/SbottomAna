#!/usr/bin/env python
# coding: utf-8

import os
import re
import time
import subprocess
import glob

DelExe    = '../testMain'
#OutDir = '/store/user/benwu/Stop/TTZ/'
#OutDir = '/store/user/benwu/Stop/Comb/'

tempdir = ''
UserEMAIL = 'benwu@fnal.gov'
ProjectName = 'Test0'
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
    #'TTbar'                                       : ['../FileList/TTbar.list',                                      40],
    #'QCD'                                         : ['../FileList/QCD_HT_500to1000.list',                           7],
    ##'T_tW'                                       : ['../FileList/T_tW.list',                                       2],
    ##'Tbar_tW'                                    : ['../FileList/Tbar_tW.list',                                    1],
    #'WJetsToLNu_HT_100to200'                     : ['../FileList/WJetsToLNu_HT_100to200.list',                     10],
    #'WJetsToLNu_HT_200to400'                     : ['../FileList/WJetsToLNu_HT_200to400.list',                     10],
    #'WJetsToLNu_HT_400to600'                     : ['../FileList/WJetsToLNu_HT_400to600.list',                     10],
    #'WJetsToLNu_HT_600toInf'                     : ['../FileList/WJetsToLNu_HT_600toInf.list',                     10],
    #'ZJetsToNuNu_HT_100to200'                     : ['../FileList/ZJetsToNuNu_HT_100to200.list',                    13],
    #'ZJetsToNuNu_HT_200to400'                     : ['../FileList/ZJetsToNuNu_HT_200to400.list',                    10],
    #'ZJetsToNuNu_HT_400to600'                     : ['../FileList/ZJetsToNuNu_HT_400to600.list',                    10],
    #'ZJetsToNuNu_HT_600toInf'                     : ['../FileList/ZJetsToNuNu_HT_600toInf.list',                    14],
    #"WJetsToLNu_HT_600toInf"            : ['',6],

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ f ~~~~~
    #"TTbarInc"                          : ['',35],
    #"WJetsToLNu_HT_100to200"            : ['',30],
    #"WJetsToLNu_HT_200to400"            : ['',17],
    #"WJetsToLNu_HT_400to600"            : ['',10],
    #"WJetsToLNu_HT_600to800"            : ['',15],
    #"WJetsToLNu_HT_800to1200"           : ['',11],
    #"WJetsToLNu_HT_1200to2500"          : ['',7],
    #"WJetsToLNu_HT_2500toInf"           : ['',7],

    #"ZJetsToNuNu_HT_600toInf"           : ['',10],
    #"ZJetsToNuNu_HT_400to600"           : ['',7],
    #"ZJetsToNuNu_HT_200to400"           : ['',20],
    #"ZJetsToNuNu_HT_100to200"           : ['',20],

    #"TTZToLLNuNu"                       : ['',4],
    #"TTZToQQ"                           : ['',4],
    #"TTWJetsToLNu"                      : ['',4],
    #"TTWJetsToQQ"                       : ['',7],
    #"DYJetsToLL_HT_600toInf"            : ['../FileList/TopProj-DYJetsToLL_HT_600toInf.list',10],
    #"DYJetsToLL_HT_400to600"            : ['../FileList/TopProj-DYJetsToLL_HT_400to600.list',10],
    #"DYJetsToLL_HT_200to400"            : ['../FileList/TopProj-DYJetsToLL_HT_200to400.list',10],
    #"DYJetsToLL_HT_100to200"            : ['../FileList/TopProj-DYJetsToLL_HT_100to200.list',10],

    #"TTbar_HT-600to800"                 : ['',20],
    #"TTbar_HT-800to1200"                : ['',4],
    #"TTbar_HT-1200to2500"               : ['',4],
    #"TTbar_HT-2500toInf"                : ['',4],
    #"Signal_T2tt_mStop425_mLSP325"      : ['',1],
    #"Signal_T2tt_mStop500_mLSP325"      : ['',1],
    "Signal_T2tt_mStop650_mLSP325"      : ['',1],
    #"Signal_T2tt_mStop850_mLSP100"      : ['',1],
    #"QCD_HT100to200"                    : ['',100],
    #"QCD_HT200to300"                    : ['',50],
    #"QCD_HT300to500"                    : ['',50],
    #"QCD_HT500to700"                    : ['',50],
    #"QCD_HT700to1000"                   : ['',40],
    #"QCD_HT1000to1500"                  : ['',8],
    #"QCD_HT1500to2000"                  : ['',6],
    #"QCD_HT2000toInf"                   : ['',4],

    #"tW_top"                            : ['../FileList/TopProj-tW_top.list'                ,4],
    #"tW_antitop"                        : ['../FileList/TopProj-tW_antitop.list'            ,4],
    #"TTbarDiLep"                        : ['../FileList/TopProj-TTbarDiLep.list'            ,60],
    #"TTZToLLNuNu"                       : ['../FileList/TopProj-TTZToLLNuNu.list'           ,4],
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ f ~~~~~

    ##"IncDYSampleSet"                    : ['',4],
    ##"QCD_Pt_10to15"                     : ['',4],
    ##"QCD_Pt_15to30"                     : ['',4],
    ##"QCD_Pt_30to50"                     : ['',4],
    ##"QCD_Pt_50to80"                     : ['',4],
    ##"QCD_Pt_80to120"                    : ['',4],
    ##"QCD_Pt_120to170"                   : ['',4],
    ##"QCD_Pt_170to300"                   : ['',4],
    ##"QCD_Pt_300to470"                   : ['',4],
    ##"QCD_Pt_470to600"                   : ['',4],
    ##"QCD_Pt_600to800"                   : ['',4],
    ##"QCD_Pt_800to1000"                  : ['',4],
    ##"QCD_Pt_1000to1400"                 : ['',4],
    ##"QCD_Pt_1400to1800"                 : ['',4],
    ##"QCD_Pt_1800to2400"                 : ['',4],
    ##"QCD_Pt_2400to3200"                 : ['',4],
    ##"QCD_Pt_3200toInf"                  : ['',4],
    #"Data_SingleElectron_2015B"         : ['',4],
    #"Data_SingleElectron_2015C"         : ['',4],
    #"Data_SingleElectron_2015D"         : ['',4],
    #"Data_DoubleMuon_2015B"             : ['',4],
    #"Data_DoubleMuon_2015C"             : ['',4],
    #"Data_DoubleMuon_2015D"             : ['',4],
    #"Data_DoubleEG_2015B"               : ['',4],
    #"Data_DoubleEG_2015C"               : ['',4],
    #"Data_DoubleEG_2015D"               : ['',4],
    #"Data_HTMHT_2015B"                  : ['',4],
    #"Data_HTMHT_2015C"                  : ['',4],
    #"Data_HTMHT_2015D"                  : ['',4],
    #"Signal_T1tttt_mGluino1200_mLSP800" : ['',1],
    #"Signal_T1tttt_mGluino1500_mLSP100" : ['',1],
    #"Signal_T1bbbb_mGluino1000_mLSP900" : ['',1],
    #"Signal_T1bbbb_mGluino1500_mLSP100" : ['',1],
    #"Signal_T2bb_mSbottom600_mLSP580"   : ['',1],
    #"Signal_T2bb_mSbottom900_mLSP100"   : ['',1],
    #"Signal_TTDMDMJets_M600GeV"         : ['',1],
    #"Signal_TTDMDMJets_M1000GeV"        : ['',1],




    #"tW_top"                            : ['../FileList/TopProj-tW_top.list'                ,4],
    #"tW_antitop"                        : ['../FileList/TopProj-tW_antitop.list'            ,4],
    #"TTbarDiLep"                        : ['../FileList/TopProj-TTbarDiLep.list'            ,60],
    #"TTZToLLNuNu"                       : ['../FileList/TopProj-TTZToLLNuNu.list'           ,4],
}

Mergeblock = """#!/usr/bin/env python

# File        : merge.py
# Author      : Ben Wu
# Contact     : benwu@fnal.gov
# Date        : 2015 Jul 20
#
# Description : Code to merge output hists

import re
import glob
import os
import subprocess
import multiprocessing

def MergeFile(prod):
    print "Processing %s" % prod
    g = glob.glob("%s*.root" % prod)
    logfile = open("%s.log" % prod, 'w')
    sub = re.compile(r'^%s_\d+\.root$' % prod)
    allfile = set()
    goodfile = set()
    for f in g:
        if sub.match(f) is not None:
            allfile.add(f)
            if os.path.getsize(f) != 0:
                goodfile.add(f)
    run = "hadd -f %s.root " % prod
    run += " ".join(goodfile)
    process = subprocess.Popen(run, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = process.communicate()
    logfile.write(out)
    logfile.write(err)
    logfile.close()

    mv = "mv "
    mv += " ".join(allfile)
    mv += " %s.log" % prod
    mv += " backup"
    subprocess.call(mv, shell=True)

if __name__ == "__main__":
    cmd_exists = lambda x: any(os.access(os.path.join(path, x), os.X_OK)
                               for path in os.environ["PATH"].split(os.pathsep))
    if cmd_exists('hadd'):
        if not os.path.isdir("backup"):
            os.mkdir("backup")
    else:
        HEADER = '[95m'
        OKBLUE = '[94m'
        OKGREEN = '[92m'
        WARNING = '[93m'
        FAIL = '[91m'
        ENDC = '[0m'
        BOLD = '[1m'
        UNDERLINE = '[4m'
        print(FAIL + "Warning: no hadd available! Please setup ROOT!!" + ENDC)
        exit()

    pattern = re.compile(r'^(.*)_\d+\.root$')
    g = glob.glob("*")

    ## Get all the process
    process = set()
    for files in g:
        match = pattern.match(files)
        if match is not None:
            process.add(match.group(1))

    ## Run with multiprocessing Pool
    pool = multiprocessing.Pool(processes = multiprocessing.cpu_count()/3)
    pool.map(MergeFile, process)
"""

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
    if len(lines) <= fraction:
        lineperfile = 1
        fraction = len(lines)
    else:
        lineperfile = len(lines) / fraction
        if len(lines) % fraction > 0:
            lineperfile += 1


    for i in range(0, fraction):
        wlines = []
        if i == fraction - 1 :
            wlines = lines[lineperfile*i :]
        else:
            wlines = lines[lineperfile*i : lineperfile*(i+1)]
        if len(wlines) > 0:
            outf = open("%s/%s.%d.list" % (filelistdir, key, i), 'w')
            outf.writelines(wlines)
            splitedfiles.append(os.path.abspath("%s/%s.%d.list" % (filelistdir, key, i)))
        outf.close()

    return splitedfiles

def my_process():
    ## temp dir for submit
    global tempdir
    global Mergeblock
    global ProjectName
    ProjectName = time.strftime('%b%d') + ProjectName
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

    ## Script for merging output histograms
    MergeFile = tempdir + "/" + "merge.py"
    f = open("%s/merge.py" % tempdir, 'wt')
    f.writelines(Mergeblock)
    f.close()
    import shutil
    shutil.copy2("%s/merge.py" % tempdir, "%s/merge.py" % outdir)

    ### Update condor files
    for key, value in Process.items():
        if value[0] == "":
            value[0] = "../FileList/"+key+".list"
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
