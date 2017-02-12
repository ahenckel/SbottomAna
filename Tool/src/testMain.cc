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
#include "GlobalObj.h"
#include "ComAna.h"
#include "VarPerEvent.h"
#include "SBDiJet.h"
#include "SBISR.h"
#include "STISR.h"
#include "STZinv.h"
#include "STRM.h"
#include "STTagger.h"
#include "SBMulti.h"
#include "StopAna.h"
#include "TTZDiLep.h"
#include "TTZ3Lep.h"
#include "TriggerAna.h"
#include "LepTrigSF.h"

// SusyAnaTools
#include "SusyAnaTools/Tools/baselineDef.h"
#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/EventListFilter.h"
#include "SusyAnaTools/Tools/PDFUncertainty.h"
#include "SusyAnaTools/Tools/PileupWeights.h"
#include "SusyAnaTools/Tools/BTagCorrector.h"
#include "SusyAnaTools/Tools/ISRCorrector.h"

bool DefSysComAnd(std::map<std::string, std::vector<std::string> > &SysMap, 
    std::map<std::string, ComAna*> &AnaMap, 
    std::vector<std::string> vSysAna={},
    std::shared_ptr<TFile> OutFile=NULL);

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
  const char *fastsim   = "";
  if (argc > 3)
  {
    fastsim = argv[3];
  }

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
  his->AddTH1("NEvent", "Number of Events", 2, 0, 2);
  his->AddTH1("Weight", "Number of Events", 200, -1, 1);

  std::map<std::string, double> SamplePro = GetXS(inputFileList);
  std::vector<std::string> Probins= { "key", "xsec", "lumi", "kfactor", "nEvts", "color" };
  his->AddTPro("XS", "Cross Section", Probins);
  for(unsigned int i=0; i < Probins.size(); ++i)
  {
    std::string &binlabel = Probins.at(i);
    std::cout <<i << " "<< binlabel <<" " << SamplePro[binlabel]<< std::endl;
    assert(SamplePro.find(binlabel) != SamplePro.end());
    his->FillTPro("XS", static_cast<int>(i), SamplePro[binlabel]);
  }

  std::string  proname = GetProcName(SamplePro);
  std::string filterstring = "";
  if (strcmp(fastsim, "fastsim") == 0 || strstr(inputFileList, "Fastsim") != NULL)
    filterstring =  "fastsim";
  std::cout << "Process " <<proname <<  " Sample is  " << filterstring << std::endl;
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Setup Global Object ~~~~~
  //Dirty fixed for missing DYJetsToLL_HT_70to100 in ISR and BTag SF
  if (proname =="DYJetsToLL_HT_70to100") proname = "DYJetsToLL_HT_100to200";

  LHAPDF::setVerbosity(LHAPDF::Verbosity::SILENT);
  PDFUncertainty pdfs;  //PDF 
  ISRCorrector isrCorr("allINone_ISRJets.root", "", "", proname);
  BTagCorrector btagcorr("allINone_bTagEff.root", "", filterstring == "fastsim", proname);
  Pileup_Sys pileup("PileupHistograms_0121_69p2mb_pm4p6.root");
  //ExternObj Gobj;

  //clock to monitor the run time
  size_t t0 = clock();
  NTupleReader tr(fChain);
  tr.setReThrow(false);
  tr.registerFunction(pdfs);
  tr.registerFunction(isrCorr);
  tr.registerFunction(btagcorr);
  tr.registerFunction(pileup);

  LepTrigSF lepTrigSF(&tr, "allINone_leptonSF_Moriond17.root");
//**************************************************************************//
//                         Prepare Baseline Classes                         //
//**************************************************************************//
  std::map<std::string, BaselineVessel*> blvMap;
  //blvMap["Default"] = new BaselineVessel(tr, "", filterstring);
  blvMap["MedEle"] = new BaselineVessel(tr, "MedEle", filterstring);
  blvMap["MedEle"]->elesFlagIDLabel = "elesFlagMedium";

  for(auto blv : blvMap)
  {
    tr.registerFunction(*blv.second);
  }
  //tr.registerFunction(boost::bind(GetTopPtReweight, _1, SamplePro));
  //tr.registerFunction(boost::bind(passBaselineZinv, _1, "001")); // bit : TEM
  //tr.registerFunction(boost::bind(passBaselineZinv, _1, "010")); // bit : TEM
  //tr.registerFunction(boost::bind(passBaselineZinv, _1, "100")); // bit : TEM
  //tr.registerFunction(boost::bind(passBaselineTTZ, _1, blvMap, "01", 0)); // bit : EM
  //tr.registerFunction(boost::bind(passBaselineTTZ, _1, "01", 1)); // bit : EM

  //tr.registerFunction(boost::bind(GetNbNjReweighting, _1, "", dynamic_cast<TH2*>(Gobj.Get("STZinv15.root:STZinvT_NbNjWeight")))); 
  //tr.registerFunction(boost::bind(GetNbNjReweighting, _1, "ZinvT", dynamic_cast<TH2*>(Gobj.Get("STZinv15.root:STZinvT_NbNjWeight")))); 
  //tr.registerFunction(boost::bind(GetNbNjReweighting, _1, "ZinvM", dynamic_cast<TH2*>(Gobj.Get("STZinv15.root:STZinvM_NbNjWeight")))); 
  //tr.registerFunction(boost::bind(RegisterDefaultAllSpecs<double>, _1, "NbNjReweight", 1.0));

  //**************************************************************************//
  //                           Prepare the analysis                           //
  //**************************************************************************//
  std::map<std::string, ComAna*> AnaMap;
  //AnaMap["Stop"] = new StopAna("Stop", &tr, OutFile);
  //AnaMap["TrigStop"] = new TriggerAna("TrigStop", &tr, OutFile, "MedEle");
  //AnaMap["TrigQCD"]  = new TriggerAna("TrigQCD",  &tr, OutFile, "MedEle");
  //AnaMap["TrigMuon"] = new TriggerAna("TrigMuon", &tr, OutFile, "MedEle");
  //AnaMap["TrigEle"] = new TriggerAna("TrigEle", &tr, OutFile, "MedEle");
  for (int i = 0; i < 4; ++i)
  {
    AnaMap["Tagger"+std::to_string(i)] = new STTagger("Tagger"+std::to_string(i), &tr, OutFile, "MedEle", i);
    AnaMap["Tagger"+std::to_string(i)+"WT"] = new STTagger("Tagger"+std::to_string(i)+"WT", &tr, OutFile, "MedEle", i);
  }
  //AnaMap["Tagger"] = new STTagger("Tagger", &tr, OutFile, "TTZM"); // DataMCSF
  //AnaMap["Tagger_Up"] = new STTagger("TaggerUp", &tr, OutFile, "TTZMJECup");
  //AnaMap["Tagger_Dn"] = new STTagger("TaggerDn", &tr, OutFile, "TTZMJECdn");
  //AnaMap["STISR"] = new STISR("STISR", &tr, OutFile);
  //AnaMap["STRM"] = new STRM("STRM", &tr, OutFile);
  //AnaMap["STZinvM"] = new STZinv("STZinvM", &tr, OutFile,"ZinvM");
  //AnaMap["STZinvE"] = new STZinv("STZinvE", &tr, OutFile,"ZinvE");
  //AnaMap["STZinvT"] = new STZinv("STZinvT", &tr, OutFile,"ZinvT");
  //AnaMap["TTZ3LepM"] = new TTZ3Lep("TTZ3LepM", &tr, OutFile, "TTZM");
  AnaMap["TTZ3SUSYE"] = new TTZ3Lep("SUSYE", &tr, OutFile, "MedEle");
  AnaMap["TTZ3SUSYM"] = new TTZ3Lep("SUSYM", &tr, OutFile, "MedEle");
  AnaMap["WTTZ3SUSYE"] = new TTZ3Lep("WSUSYE", &tr, OutFile, "MedEle");
  AnaMap["WTTZ3SUSYM"] = new TTZ3Lep("WSUSYM", &tr, OutFile, "MedEle");
  //AnaMap["TTZ3LepE"] = new TTZ3Lep("TTZ3LepE", &tr, OutFile, "TTZE");
  //AnaMap["TTZDiLepM"] = new TTZDiLep("TTZDiLepM", &tr, OutFile, "TTZM");
  //AnaMap["TTZDiLepE"] = new TTZDiLep("TTZDiLepE", &tr, OutFile, "TTZE");
  //AnaMap["TTZ3Lep"] = new TTZ3Lep("TTZ3Lep", &tr, OutFile, "TTZM");
  //AnaMap["SBDJ"] = new SBDiJet("SBDJ", &tr, OutFile);
  //AnaMap["SBISR"] = new SBISR("SBISR", &tr, OutFile);
  //AnaMap["SBMulti"] = new SBMulti("SBMulti", &tr, OutFile);

  //**************************************************************************//
  //                      Setup Systematics with Weights                      //
  //**************************************************************************//
  // Systematic_Name, sysbit (rate, shape), registerName
  std::map<std::string, std::vector<std::string> > SysMap;
  // Bit(Rate, Shape), "Scale Up/Down", "Central correction is has"
  SysMap["PDF_up"]     = {"11", "NNPDF_From_Median_Up"};
  SysMap["PDF_down"]   = {"11", "NNPDF_From_Median_Down"};
  SysMap["Scale_up"]   = {"11", "Scaled_Variations_Up"};
  SysMap["Scale_down"] = {"11", "Scaled_Variations_Down"};
  SysMap["PU_up"]      = {"01", "_PUSysUp",                "_PUweightFactor"};
  SysMap["PU_down"]    = {"01", "_PUSysDown",              "_PUweightFactor"};
  //DefSysComAnd(SysMap, AnaMap, {"Stop"});

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
    if (tr.isFirstEvent() && tr.getVar<int>("run")!= 1)
    {
      for (auto& name: tr.getVec<std::string>("TriggerNames"))
        std::cout << name << std::endl;
    }

    //if (tr.getEvtNum() > 20000 ) break;
    if (tr.getEvtNum() % 20000 == 0)
    {
      std::cout << "Processed Events: " << tr.getEvtNum() << std::endl;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ testing Zone ~~~~~

    //**************************************************************************//
    //                             Set Event Weight                             //
    //**************************************************************************//
    double evtWeight = tr.getVar<double>("evtWeight"); // For shape 
    if (tr.getVar<int>("run") != 1) // Set weight to 1 for Data
      evtWeight = 1;
    double rateWeight = evtWeight; // For rate (event count) -> NEvent & NBase
    his->FillTH1("NEvent", 1, evtWeight);

    if (tr.getVar<int>("run") == 1) // Set weight to MC
    {
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting weight for rate, but not shape ~~~~~
      rateWeight *= tr.getVar<double>("isr_Unc_Cent"); 
      rateWeight *= tr.getVar<double>("bTagSF_EventWeightSimple_Central"); 

      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting weight for shape, but not rate ~~~~~
      evtWeight *= tr.getVar<double>("isr_Unc_Cent"); 
      evtWeight *= tr.getVar<double>("bTagSF_EventWeightSimple_Central"); 
      evtWeight *= tr.getVar<double>("_PUweightFactor"); 
      //evtWeight *= tr.getVar<double>("TopPtReweight"); // TopPtReweight, apply to shape, not rate
      //evtWeight *= tr.getVar<double>("NbNjReweight"); // TopPtReweight, apply to shape, not rate
    }


    his->FillTH1("Weight", evtWeight);

    //**************************************************************************//
    //                                 Fill Cuts                                //
    //**************************************************************************//
    for( auto &it : AnaMap )
    {
      float leptrig = 1.0;
      if (it.first.find("WT") != std::string::npos)
        leptrig = lepTrigSF.GetEventSF(it.second, "10");
      else
        leptrig = lepTrigSF.GetEventSF(it.second, "11");
      double temprate = rateWeight *leptrig;
      double tempevt = evtWeight *leptrig;
      it.second->SetRateWeight(temprate);
      it.second->SetEvtWeight(tempevt);
      it.second->FillCut();
    }

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
  OutFile->Close();
  fChain->Reset();
  return 0;
}


// ===  FUNCTION  ============================================================
//         Name:  DefSysComAnd
//  Description:  /* cursor */
// ===========================================================================
bool DefSysComAnd(std::map<std::string, std::vector<std::string> > &SysMap, 
    std::map<std::string, ComAna*> &AnaMap, 
    std::vector<std::string> vSysAna,
    std::shared_ptr<TFile> OutFile)
{
  std::vector<std::string> anaNames;
  if (vSysAna.empty())
  {
    for(auto &ana : AnaMap)
    {
      anaNames.push_back(ana.first);
    }
  }
  else anaNames = vSysAna;

  for(auto &sys : SysMap)
  {
    for(auto &ananame : anaNames)
    {
      std::stringstream ss;
      ss << ananame <<"_" << sys.first;
      AnaMap[ss.str()] = AnaMap[ananame]->Clone(ss.str(), OutFile);
      if (sys.second.size() == 2)
        AnaMap[ss.str()]->SetSysVar(sys.second.at(0), sys.second.at(1));
      if (sys.second.size() == 3)
        AnaMap[ss.str()]->SetSysVar(sys.second.at(0), sys.second.at(1), sys.second.at(2));
    }
  }
  return true;
}       // -----  end of function DefSysComAnd  -----

