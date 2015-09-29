// ===========================================================================
// 
//       Filename:  testBottom.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/21/2015 13:58:19
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

// Stdlib
#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>
#include <memory>

// ROOT
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TError.h"

// UserTools
#include "RootTools.h"
#include "baselineDef.h"
#include "NTupleReader.h"
#include "HistTool.hh"
#include "ComAna.h"
#include "SBDiJet.h"
#include "SBISR.h"
#include "STISR.h"
#include "PassCut.h"
#include "SBMulti.h"

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
  his->AddTH1("TestBit", "TestBit", 10, 0, 10);
  his->AddTH1("NEvent", "Number of Events", 2, 0, 2);
  his->FillTPro("XS", 1, GetXS(inputFileList));

  //clock to monitor the run time
  size_t t0 = clock();
  NTupleReader tr(fChain);
  AnaFunctions::prepareTopTagger();
  tr.registerFunction(&passBaselineFunc);
  //first loop, to generate Acc, reco and Iso effs and also fill expected histgram

  std::map<std::string, ComAna*> AnaMap;
  AnaMap["SBDJ"] = new SBDiJet("SBDJ", &tr, OutFile);
  AnaMap["SBISR"] = new SBISR("SBISR", &tr, OutFile);
  AnaMap["SBMulti"] = new SBMulti("SBMulti", &tr, OutFile);
  AnaMap["PassCut"] = new PassCut("LeftOver", &tr, OutFile);
  AnaMap["STISR"] = new STISR("STISR", &tr, OutFile);

  std::cout<<"First loop begin: "<<std::endl;
  while(tr.getNextEvent())
  {
    his->FillTH1("NEvent", 1);
    if(tr.getEvtNum()%20000 == 0)
      std::cout << tr.getEvtNum() << "\t" << ((clock() - t0)/1000000.0) << std::endl;

    // CutFlow
    his->FillTH1("CutFlow", 0);
    AnaMap["STISR"] -> FillCut();
    bool passcuts = false;
    bool passDJ =  AnaMap["SBDJ"]->FillCut();
    bool passISR = AnaMap["SBISR"]->FillCut();
    bool passMulti = AnaMap["SBMulti"]->FillCut();
    his->FillTH1("TestBit", 0);
    if (passDJ) his->FillTH1("TestBit", 1);
    if (passISR) his->FillTH1("TestBit", 2);
    if (passDJ && passISR) his->FillTH1("TestBit", 3);
    if (passDJ || passISR) his->FillTH1("TestBit", 4);
    if (passMulti) his->FillTH1("TestBit", 5);
    //passcuts = AnaMap["SBDJ"]->FillCut() || passcuts ;
    //passcuts = AnaMap["SBISR"]->FillCut() || passcuts;
    if (! (passDJ || passISR))
    {
      AnaMap["PassCut"]->FillCut();
    }
  }

  for(std::map<std::string, ComAna*>::const_iterator it=AnaMap.begin();
      it!=AnaMap.end(); ++it)
  {
    it->second->WriteHistogram();
  }

//----------------------------------------------------------------------------
//  Write the output
//----------------------------------------------------------------------------
  his->WriteTPro();
  his->WriteTH1();

  return 0;
}
