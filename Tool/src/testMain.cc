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

bool PrintTLorentz(int event, std::string name, std::vector<TLorentzVector> obj);
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

  std::vector<std::string> CutOrder = { "noCut", "nJets", "muonVeto", "eleVeto", "isoVeto", "dPhis", "BJets" , "MET", "nTops"};
  his->Cutorder("Stop", CutOrder, 0);

  //clock to monitor the run time
  size_t t0 = clock();

  NTupleReader tr(fChain);
  ////initialize the type3Ptr defined in the customize.h
  AnaFunctions::prepareTopTagger();
  ////The passBaselineFunc is registered here
  tr.registerFunction(&passBaselineFunc);
  
  std::map<std::string, TopTaggerAna*> TopMap;
  TopMap["T3Top"] = new TopTaggerAna("T3Top", &tr, OutFile);
  TopMap["T3bhad"] = new TopTaggerAna("T3bhad", &tr, OutFile);
  TopMap["T3bhad"]->SetType3BHad(true);
  TopMap["T3bhadTight"] = new TopTaggerAna("T3bhadTight", &tr, OutFile);
  TopMap["T3bhadTight"]->SetType3BHad(true);
  TopMap["T3bhadTight"]->SetType3BhadWindow(140, 200);
  //TopMap["T3Top20"] = new TopTagger;Ana("T3Top20", &tr, OutFile);
  //TopMap["G3Top30"] = new TopTaggerAna("G3Top30", &tr, OutFile);
  //TopMap["G3Top20"] = new TopTaggerAna("G3Top20", &tr, OutFile);
  TopMap["CMSTop"] = new TopTaggerAna("CMSTop", &tr, OutFile);
  TopMap["HEPTop"] = new TopTaggerAna("HEPTop", &tr, OutFile);
  //TopMap["AK8SoftDrop"] = new TopTaggerAna("AK8SoftDrop", &tr, OutFile);
  TopMap["CA15SoftDrop"] = new TopTaggerAna("CA15SoftDrop", &tr, OutFile);

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

    // CutFlow
    his->FillTH1("CutFlow", 0);
    //----------------------------------------------------------------------------
    //  Ugly nested if statement....
    //----------------------------------------------------------------------------
    if (tr.getVar<bool>("passnJets")) 
    {
      his->FillTH1("CutFlow", GetCutBin(CutOrder, "nJets"));
      if (tr.getVar<bool>("passMuonVeto")) 
      {
        his->FillTH1("CutFlow", GetCutBin(CutOrder, "muonVeto"));
        if (tr.getVar<bool>("passEleVeto")) 
        {
          his->FillTH1("CutFlow", GetCutBin(CutOrder, "eleVeto"));
          if (tr.getVar<bool>("passIsoTrkVeto")) 
          {
            his->FillTH1("CutFlow", GetCutBin(CutOrder, "isoVeto"));
            if (tr.getVar<bool>("passdPhis")) 
            {
              his->FillTH1("CutFlow", GetCutBin(CutOrder, "dPhis"));
              if (tr.getVar<bool>("passBJets")) 
              {
                his->FillTH1("CutFlow", GetCutBin(CutOrder, "BJets"));
                if (tr.getVar<bool>("passMET")) his->FillTH1("CutFlow", GetCutBin(CutOrder, "MET"));
              }
            }
          }
        }
      }
    }



    bool passBaseline=tr.getVar<bool>("passBaseline");
    if (! passBaseline) continue;

    //TopMap["T3Top20"]->GetT3TopTagger(20, "jetsLVec", "recoJetsBtag", "met");
    //TopMap["G3Top30"]->GetT3TopTagger(30, "Gen4LVec", "recoJetsBtag", "met");
    //TopMap["G3Top20"]->GetT3TopTagger(20, "Gen4LVec", "recoJetsBtag", "met");
    //TopMap["AK8SoftDrop"]->GetFatTopTagger("AK8SoftDropLVec");

    TopMap["T3Top"]->GetT3TopTagger(30, "jetsLVec", "recoJetsBtag", "met");
    TopMap["T3bhad"]->GetT3TopTagger(30, "jetsLVec", "recoJetsBtag", "met");
    TopMap["T3bhadTight"]->GetT3TopTagger(30, "jetsLVec", "recoJetsBtag", "met");
    TopMap["HEPTop"]->GetFatTopTagger("HEPV2TopLVec");
    TopMap["CMSTop"]->GetFatTopTagger("CMSTopLVec");
    TopMap["CA15SoftDrop"]->GetFatTopTagger("CA15SoftDropLVec");

    if (TopMap["T3Top"]->GetNTops() >= 1) his->FillTH1("CutFlow", GetCutBin(CutOrder, "nTops"));

    for(std::map<std::string, TopTaggerAna*>::const_iterator it=TopMap.begin();
      it!=TopMap.end(); ++it)
    {
      it->second->RunTagger();
    }

    TopMap["T3bhad"]->GetBoverlapHad();

    //if ( TopMap["T3Top30"]->GetBoverlapHad() && TopMap["HEPTop"]->GetNTops() == 2 && TopMap["T3Top30"]->GetNTops() < TopMap["HEPTop"]->GetNTops() )
    //if (tr.getEvtNum()  == 11260)
    //if ( TopMap["HEPTop"]->GetNTops() == 2 && TopMap["T3Top30"]->GetNTops() < TopMap["HEPTop"]->GetNTops() )
    //if ( TopMap["T3Top"]->GetBoverlapHad() )
    if (false)
    {

      int event = tr.getEvtNum();
      //PrintTLorentz(event, "HEPTop", TopMap["HEPTop"]->RecoTops);
      PrintTLorentz(event, "T3Top", TopMap["T3Top"]->RecoTops);
      PrintTLorentz(event, "AK4Jet", tr.getVec<TLorentzVector>("jetsLVec"));
      PrintTLorentz(event, "GEN4Jet", tr.getVec<TLorentzVector>("Gen4LVec"));


      std::vector<TLorentzVector> genDecayLVec     = tr.getVec<TLorentzVector> ("genDecayLVec");

      std::vector<TopDecay> vTops =  TopMap["T3Top"]->vTops;
      std::vector<TLorentzVector> tempTop;
      std::vector<TLorentzVector> tempW;
      std::vector<TLorentzVector> tempb;
      std::vector<TLorentzVector> tempLep;
      std::vector<TLorentzVector> temphad;
      for(unsigned int i=0; i < vTops.size(); ++i)
      {
        TopDecay gentop = vTops.at(i);
        if (gentop.topidx_ != -1) tempTop.push_back(genDecayLVec[gentop.topidx_]);
        if (gentop.Widx_ != -1) tempW.push_back(genDecayLVec[gentop.Widx_]);
        if (gentop.bidx_ != -1) tempb.push_back(genDecayLVec[gentop.bidx_]);
        if (gentop.Lepidx_ != -1) tempLep.push_back(genDecayLVec[gentop.Lepidx_]);
        if (gentop.had1idx_ != -1 && gentop.had1idx_ <= genDecayLVec.size())
        {
          temphad.push_back(genDecayLVec[gentop.had1idx_]);
        }
        if (gentop.had2idx_ != -1 && gentop.had2idx_ <= genDecayLVec.size())
        {
          temphad.push_back(genDecayLVec[gentop.had2idx_]);
        }
      }
      PrintTLorentz(event, "Top", tempTop);
      PrintTLorentz(event, "W", tempW);
      PrintTLorentz(event, "Lep", tempLep);
      PrintTLorentz(event, "B", tempb);
      PrintTLorentz(event, "Had", temphad);

      // Type3 Jets
      std::vector<TLorentzVector> jetsforTT;
      std::vector<TLorentzVector> jetsforTTplt;

      for(unsigned int i=0; i < tr.getVec<TLorentzVector>("jetsLVec").size(); ++i)
      {
        if (tr.getVec<TLorentzVector>("jetsLVec").at(i).Pt() > 30) 
          jetsforTT.push_back(tr.getVec<TLorentzVector>("jetsLVec").at(i));
      }
      
      for(unsigned int i=0; i < TopMap["T3Top"]->Type3Jets.size(); ++i)
      {
        jetsforTTplt.push_back(jetsforTT.at(TopMap["T3Top"]->Type3Jets.at(i)));
      }
      PrintTLorentz(event, "T3Jet", jetsforTTplt);

    }

  }

//----------------------------------------------------------------------------
//  Write the output
//----------------------------------------------------------------------------
  his->WriteTPro();
  his->WriteTH1();
  for(std::map<std::string, TopTaggerAna*>::const_iterator it=TopMap.begin();
      it!=TopMap.end(); ++it)
  {
    it->second->EndTest();
  }

  return 0;
}

// ===  FUNCTION  ============================================================
//         Name:  PrintTLorentz
//  Description:  /* cursor */
// ===========================================================================
bool PrintTLorentz(int event, std::string name, std::vector<TLorentzVector> obj) 
{
  for (int i = 0; i < obj.size(); ++i)
  {
    TLorentzVector temp = obj.at(i);
    if (temp.Pt()<3) continue;
    std::cout << event <<"," << name <<"," << temp.Px() <<"," << temp.Py() <<"," <<
      temp.Pz()<<"," << temp.E()<< std::endl;
  }

  return true;
}       // -----  end of function PrintTLorentz  -----
