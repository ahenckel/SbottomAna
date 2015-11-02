// ===========================================================================
// 
//       Filename:  testMain.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/21/2015 13:58:19
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

// Stdlib
#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>
#include <memory>
#include <functional>

// ROOT
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TError.h"

// Boost
#include "boost/bind.hpp"

// UserTools
#include "RootTools.h"
#include "HistTool.hh"
#include "ComAna.h"
#include "VarPerEvent.h"
#include "SBDiJet.h"
#include "SBISR.h"
#include "STISR.h"
#include "PassCut.h"
#include "SBMulti.h"
#include "StopAna.h"
#include "TTZDiLep.h"
#include "TTZ3Lep.h"

// SusyAnaTools
#include "SusyAnaTools/Tools/baselineDef.h"
#include "SusyAnaTools/Tools/NTupleReader.h"


int main(int argc, char* argv[])
{

  //----------------------------------------------------------------------------
  //  Getting input information
  //----------------------------------------------------------------------------
  if (argc < 2)
  {
    std::cerr <<"Please give 2 arguments " << "runList " << " " << "outputFileName" << std::endl;
    std::cerr <<" Valid configurations are " << std::endl;
    std::cerr <<" ./LostLepton_MuCS_TTbar runlist_ttjets.txt isoplots.root" << std::endl;
    return -1;
  }

  //----------------------------------------------------------------------------
  //  Input and output files
  //----------------------------------------------------------------------------
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];

  TChain *fChain = new TChain("stopTreeMaker/AUX");
  
  // :WARNING:06/16/2015 11:41:12:benwu: Can't work with passing TChain
  // through a function. Why?
  std::fstream input(inputFileList);
  if(!input.is_open())
  {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return false;
  }
  for(std::string line; getline(input, line);)
  {
    if (line[0] == '#') continue;
    boost::algorithm::trim_right(line);
    std::cout << "Add File: " << line << std::endl;
    fChain->Add(line.c_str());
  }
  //std::cout << "No. of Entries in this tree : " << fChain->GetEntries() << std::endl;

  // To remove the warning from TFile::Append
  // -->  Replacing existing TH1: CrossSection (Potential memory leak)
  // We need same histname for later on comparison
  gErrorIgnoreLevel = kFatal;
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Initialize output file ~~~~~
  std::shared_ptr<TFile> OutFile(new TFile(outFileName, "RECREATE"));
  HistTool* his = new HistTool(OutFile, "d");
  his->AddTPro("XS", "Cross Section", 2, 0, 2);
  his->AddTH1("NEvent", "Number of Events", 2, 0, 2);
  his->AddTH1("Weight", "Number of Events", 200, -1, 1);
  his->FillTPro("XS", 1, GetXS(inputFileList));

  //clock to monitor the run time
  size_t t0 = clock();
  NTupleReader tr(fChain);
  tr.registerFunction(&passBaselineFunc);
  tr.registerFunction(&passBaselineTTZ);
  tr.registerFunction(&RegisterVarPerEvent);

  //first loop, to generate Acc, reco and Iso effs and also fill expected histgram

  //**************************************************************************//
  //                           Prepare the analysis                           //
  //**************************************************************************//
  std::map<std::string, ComAna*> AnaMap;
  AnaMap["Stop"] = new StopAna("Stop", &tr, OutFile);
  AnaMap["STISR"] = new STISR("STISR", &tr, OutFile);
  AnaMap["TTZDiLep"] = new TTZDiLep("TTZDiLep", &tr, OutFile, "TTZ");
  AnaMap["TTZ3Lep"] = new TTZ3Lep("TTZ3Lep", &tr, OutFile, "TTZ");
  //AnaMap["SBDJ"] = new SBDiJet("SBDJ", &tr, OutFile);
  //AnaMap["SBISR"] = new SBISR("SBISR", &tr, OutFile);
  //AnaMap["SBMulti"] = new SBMulti("SBMulti", &tr, OutFile);
  //AnaMap["PassCut"] = new PassCut("LeftOver", &tr, OutFile);
  for( auto &it : AnaMap )
  {
    it.second->SaveCutHist(true);
    it.second->BookHistograms();
  }

  //**************************************************************************//
  //                            Start Event Looping                           //
  //**************************************************************************//
  std::cout << "First loop begin: " << std::endl;
  while(tr.getNextEvent())
  {
    if (tr.getEvtNum() % 20000 == 0)
      std::cout << tr.getEvtNum() << "\t" << ((clock() - t0) / 1000000.0) << std::endl;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Set Event Weight ~~~~~
    double stored_weight = -999;
    try {
      stored_weight = tr.getVar<double>("stored_weight");
    } catch (std::string var) {
    }
    if (stored_weight == -999) stored_weight = 0;
    int evtWeight = stored_weight >= 0 ? 1 : -1;
    his->FillTH1("NEvent", 1, evtWeight);
    his->FillTH1("Weight", stored_weight);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Fill Cuts ~~~~~
    for( auto &it : AnaMap )
    {
      it.second->SetEvtWeight(evtWeight);
      it.second->FillCut();
    }

    //AnaMap["STISR"] -> FillCut();
    //bool passcuts = false;
    //bool passDJ =  AnaMap["SBDJ"]->FillCut();
    //bool passISR = AnaMap["SBISR"]->FillCut();
    //bool passMulti = AnaMap["SBMulti"]->FillCut();
    //his->FillTH1("TestBit", 0);
    //if (passDJ) his->FillTH1("TestBit", 1);
    //if (passISR) his->FillTH1("TestBit", 2);
    //if (passDJ && passISR) his->FillTH1("TestBit", 3);
    //if (passDJ || passISR) his->FillTH1("TestBit", 4);
    //if (passMulti) his->FillTH1("TestBit", 5);
    ////passcuts = AnaMap["SBDJ"]->FillCut() || passcuts ;
    ////passcuts = AnaMap["SBISR"]->FillCut() || passcuts;
    //if (! (passDJ || passISR))
    //{
      //AnaMap["PassCut"]->FillCut();
    //}
  }

//----------------------------------------------------------------------------
//  Write the output
//----------------------------------------------------------------------------
  for(std::map<std::string, ComAna*>::const_iterator it=AnaMap.begin();
      it!=AnaMap.end(); ++it)
  {
    it->second->WriteHistogram();
  }
  his->WriteTPro();
  his->WriteTH1();

  return 0;
}
