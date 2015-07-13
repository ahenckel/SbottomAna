// ===========================================================================
// 
//       Filename:  testMain.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/15/2015 11:52:50
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

// Stdlib
#include <iostream>
//#include <algorithm>
//#include <cstring>
#include <string>
//#include <map>
//#include <cmath>
//#include <set>
#include <cstdio>
//#include <sstream>
//#include <fstream>
//#include <vector>
#include <ctime>
#include <memory>

// ROOT
//#include "TGraph.h"
//#include "TCanvas.h"
//#include "TH1F.h"
//#include "TH1D.h"
//#include "THStack.h"
//#include "TLatex.h"
//#include "TLegend.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TError.h"

// SusyAnaTools
//#include "samples.h"
//#include "customize.h"
#include "baselineDef.h"

// UserTools
#include "RootTools.h"
#include "NTupleReader.h"
#include "HistTool.hh"

#include "TopTaggerAna.h"

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
  //TChain *fChain = new TChain("AUX");
  
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
  his->AddTH1("NBase", "Number of Events passed baseline", 2, 0, 2);
  his->FillTPro("XS", 1, GetXS(inputFileList));

  //clock to monitor the run time
  size_t t0 = clock();

  NTupleReader tr(fChain);
  ////initialize the type3Ptr defined in the customize.h
  //AnaFunctions::prepareTopTagger();
  ////The passBaselineFunc is registered here
  //tr.registerFunction(&passBaselineFunc);

  //std::string name = "ToTopptest";
  //TopTaggerAna topana(name, &tr, OutFile);
  //std::string name = "ToTopptest";
  TopTaggerAna topana("T3Top30", &tr, OutFile);
  TopTaggerAna top20ana("T3Top20", &tr, OutFile);

  TopTaggerAna CMSTopana("CMSTop", &tr, OutFile);
  TopTaggerAna HEPTopana("HEPTop", &tr, OutFile);
  TopTaggerAna SoftDropTopana("SoftDrop", &tr, OutFile);

  //TopTaggerAna topbase("afterbaseline30", &tr, OutFile);
  //TopTaggerAna top20base("afterbaseline20", &tr, OutFile);
  //TopTaggerAna puppiTop("PUPPITopptest", &tr, OutFile);

  //first loop, to generate Acc, reco and Iso effs and also fill expected histgram
  std::cout<<"First loop begin: "<<std::endl;

  while(tr.getNextEvent())
  {
    his->FillTH1("NEvent", 1);
    if(tr.getEvtNum()%20000 == 0)
      std::cout << tr.getEvtNum() << "\t" << ((clock() - t0)/1000000.0) << std::endl;
    //if(tr.getEvtNum() == 1000 ) break;
    //if(tr.getEvtNum()%20000 == 0) std::cout << tr.getEvtNum() << "\t" << ((clock() - t0)/1000000.0) << std::endl;

    topana.GetT3TopTagger(30, "jetsLVec", "recoJetsBtag", "met");
    top20ana.GetT3TopTagger(20, "jetsLVec", "recoJetsBtag", "met");
    //topana.GetT3TopTagger(30, "jetsLVec", "recoJetsBtag_0", "met");
    //top20ana.GetT3TopTagger(20, "jetsLVec", "recoJetsBtag_0", "met");
    CMSTopana.GetFatTopTagger("CMSTopLVec");
    HEPTopana.GetFatTopTagger("HEPV2TopLVec");
    SoftDropTopana.GetFatTopTagger("AK8SoftDropLVec");

    topana.RunTagger();
    top20ana.RunTagger();
    CMSTopana.RunTagger();
    HEPTopana.RunTagger();
    SoftDropTopana.RunTagger();



    //topana.GetT3TopTagger(30, "PUPPIjetsLVec", "recoPUPPIJetsBtag", "met");
    //top20ana.GetT3TopTagger(20, "PUPPIjetsLVec", "recoPUPPIJetsBtag", "met");
    //topana.RunTagger(30, "jetsLVec", "recoJetsBtag_0", "met");
    //top20ana.RunTagger(20, "jetsLVec", "recoJetsBtag_0", "met");
    //bool passBaseline=tr.getVar<bool>("passBaseline");
    //if (passBaseline)
    //{
      //his->FillTH1("NBase", 1);
      ////topbase.RunTagger(30, "jetsLVec", "recoJetsBtag_0", "met");
      ////top20base.RunTagger(20, "jetsLVec", "recoJetsBtag_0", "met");
      //topbase.RunTagger(30, "PUPPIjetsLVec", "recoPUPPIJetsBtag", "met");
      //top20base.RunTagger(20, "PUPPIjetsLVec", "recoPUPPIJetsBtag", "met");
    //}

    //PUPPITopptest.RunTagger(30, "jetsLVec", "recoJetsBtag_0", "met");

  }

  his->WriteTPro();
  topana.EndTest();
  //topbase.EndTest();
  top20ana.EndTest();
  //top20base.EndTest();
  CMSTopana.EndTest();
  HEPTopana.EndTest();
  SoftDropTopana.EndTest();


  return 0;
}

