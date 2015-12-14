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
#include "SBMulti.h"
#include "StopAna.h"
#include "TTZDiLep.h"
#include "TTZ3Lep.h"

// SusyAnaTools
#include "SusyAnaTools/Tools/baselineDef.h"
#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/EventListFilter.h"
#include "SusyAnaTools/Tools/PDFUncertainty.h"
#include "SusyAnaTools/Tools/Weights.h"

bool DefSysComAnd(std::map<std::string, std::pair<std::string, std::string> > &SysMap, 
    std::map<std::string, ComAna*> &AnaMap, std::shared_ptr<TFile> OutFile=NULL);

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
  his->AddTH1("NEvent", "Number of Events", 2, 0, 2);
  his->AddTH1("Weight", "Number of Events", 200, -1, 1);

  std::map<std::string, double> SamplePro = GetXS(inputFileList);
  const std::string ProName = GetProcName(SamplePro);
  std::vector<std::string> Probins= { "key", "xsec", "lumi", "kfactor", "nEvts", "color" };
  his->AddTPro("XS", "Cross Section", Probins);
  for(unsigned int i=0; i < Probins.size(); ++i)
  {
    std::string &binlabel = Probins.at(i);
    std::cout <<i << " "<< binlabel <<" " << SamplePro[binlabel]<< std::endl;
    assert(SamplePro.find(binlabel) != SamplePro.end());
    his->FillTPro("XS", static_cast<int>(i), SamplePro[binlabel]);
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Setup Global Object ~~~~~
  LHAPDF::setVerbosity(LHAPDF::Verbosity::SILENT);
  PDFUncertainty pdfs;  //PDF 
  Pileup_Sys pileup;
  ExternObj Gobj;

  //clock to monitor the run time
  size_t t0 = clock();
  NTupleReader tr(fChain);
  tr.registerFunction(pdfs);
  tr.registerFunction(pileup);
  tr.registerFunction(&passBaselineFunc);
  tr.registerFunction(boost::bind(GetTopPtReweight, _1, SamplePro));
  tr.registerFunction(boost::bind(passBaselineZinv, _1, "001")); // bit : TEM
  tr.registerFunction(boost::bind(passBaselineZinv, _1, "010")); // bit : TEM
  tr.registerFunction(boost::bind(passBaselineZinv, _1, "100")); // bit : TEM
  tr.registerFunction(boost::bind(passBaselineTTZ, _1, "01")); // bit : EM
  tr.registerFunction(boost::bind(passBaselineTTZ, _1, "10")); // bit : EM

  tr.registerFunction(boost::bind(GetNbNjReweighting, _1, "ZinvM", dynamic_cast<TH2*>(Gobj.Get("STZinv15.root:STZinvM_NbNjWeight")))); 
  if (ProName == "TTbar")
  {
    tr.registerFunction(boost::bind(GetNbNjReweighting, _1, "ZinvT", dynamic_cast<TH2*>(Gobj.Get("STZinv15.root:STZinvM_NbNjWeight")))); 
  }
  tr.registerFunction(boost::bind(RegisterDefaultAllSpecs<double>, _1, "NbNjReweight", 1.0));

  //first loop, to generate Acc, reco and Iso effs and also fill expected histgram

  //**************************************************************************//
  //                           Prepare the analysis                           //
  //**************************************************************************//
  std::map<std::string, ComAna*> AnaMap;
  AnaMap["Stop"] = new StopAna("Stop", &tr, OutFile);
  //AnaMap["STISR"] = new STISR("STISR", &tr, OutFile);
  //AnaMap["STRM"] = new STRM("STRM", &tr, OutFile);
  //AnaMap["STZinvM"] = new STZinv("STZinvM", &tr, OutFile,"ZinvM");
  //AnaMap["STZinvE"] = new STZinv("STZinvE", &tr, OutFile,"ZinvE");
  //AnaMap["STZinvT"] = new STZinv("STZinvT", &tr, OutFile,"ZinvT");
  //AnaMap["TTZ3LepM"] = new TTZ3Lep("TTZ3LepM", &tr, OutFile, "TTZM");
  //AnaMap["TTZ3LepE"] = new TTZ3Lep("TTZ3LepE", &tr, OutFile, "TTZE");
  //AnaMap["TTZDiLepM"] = new TTZDiLep("TTZDiLepM", &tr, OutFile, "TTZM");
  //AnaMap["TTZDiLepE"] = new TTZDiLep("TTZDiLepE", &tr, OutFile, "TTZE");
  //AnaMap["TTZ3Lep"] = new TTZ3Lep("TTZ3Lep", &tr, OutFile, "TTZ");
  //AnaMap["SBDJ"] = new SBDiJet("SBDJ", &tr, OutFile);
  //AnaMap["SBISR"] = new SBISR("SBISR", &tr, OutFile);
  //AnaMap["SBMulti"] = new SBMulti("SBMulti", &tr, OutFile);

  //**************************************************************************//
  //                      Setup Systematics with Weights                      //
  //**************************************************************************//
  // Systematic_Name, sysbit (XS, eff, shape), registerName
  std::map<std::string, std::pair<std::string, std::string> > SysMap;
  SysMap["PDF_up"] = std::make_pair("111", "PDF_Unc_Up"); // as shape uncertainty
  SysMap["PDF_down"] = std::make_pair("111", "PDF_Unc_Down"); // as shape uncertainty
  SysMap["Scale_up"] = std::make_pair("111", "Scaled_Variations_Up"); // as shape uncertainty
  SysMap["Scale_down"] = std::make_pair("111", "Scaled_Variations_Down"); // as shape uncertainty
  SysMap["PileUp_up"] = std::make_pair("011", "_PUSysUp"); // as shape uncertainty
  SysMap["PileUp_down"] = std::make_pair("011", "_PUSysDown"); // as shape uncertainty
  DefSysComAnd(SysMap, AnaMap);

  for( auto &it : AnaMap )
  {
    it.second->SaveCutHist(true);
    it.second->BookHistograms();
  }

  //**************************************************************************//
  //                            Start Event Looping                           //
  //**************************************************************************//
  std::cout << "First loop begin: " << std::endl;
  double vm, rss;
  while(tr.getNextEvent())
  {
    if (tr.IsFirstEvent() && tr.getVar<int>("run")!= 1)
    {
      for (auto& name: tr.getVec<std::string>("TriggerNames"))
        std::cout << name << std::endl;
    }

    //if (tr.getEvtNum() > 10000) break;
    if (tr.getEvtNum() % 20000 == 0)
    {
      process_mem_usage(vm, rss);
      std::cout << tr.getEvtNum() << "\t" << ((clock() - t0) / 1000000.0) <<"\tVM:" << vm <<"\tRSS:" << rss<< std::endl;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ testing Zone ~~~~~

    //**************************************************************************//
    //                             Set Event Weight                             //
    //**************************************************************************//
    // yeild = xs * lumi * eff (NBase/Ntotal) * xs_scale(NxsSF/Nevent)
    // In total 5 places for weight, included ShapeWeight
    // Nevent      : NLO
    // NxsSF       : NLO, PDF, Scale,
    // Ntotal      : NLO, PDF, Scale, PileUp, NbNj, 
    // NBase       : NLO, PDF, Scale, PileUp, NbNj, 
    // ShapeWeight : NLO, PDF, Scale, PileUp, NbNj, TopPt, 
    double stored_weight = -999; // From ntuple
    double evtWeight = 1.0;      // For all, mostly for mc@NLO
    double xsWeight = 1.0;       // For variation of XS -> NxsSF/NEvent
    double effWeight = 1.0;      // For variation of efficiency -> Ntotal, Nbase
    double shapeWeight = 1.0;    // For variation in shape

    //~~~~~~~~~~~~~~~~~~~~~ Getting weight that apply to both shape and rate ~~~~~
    try {
      stored_weight = tr.getVar<double>("stored_weight");
    } catch (std::string var) {
    }
    if (stored_weight == -999) stored_weight = 1;
    evtWeight = stored_weight >= 0 ? 1 : -1;
    if (tr.getVar<int>("run") != 1) // Set weight to 1 for Data
      evtWeight = 1;
    xsWeight = effWeight = shapeWeight = evtWeight;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Weight for eff. ~~~~~
    effWeight *= tr.getVar<double>("_PUweightFactor");

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Weight for shape ~~~~~
    shapeWeight *= tr.getVar<double>("_PUweightFactor");
    shapeWeight *= tr.getVar<double>("TopPtReweight");

    his->FillTH1("NEvent", 1, evtWeight);
    his->FillTH1("Weight", stored_weight);

    //**************************************************************************//
    //                                 Fill Cuts                                //
    //**************************************************************************//
    for( auto &it : AnaMap )
    {
      it.second->SetWeights(evtWeight, xsWeight, effWeight, shapeWeight);
      //it.second->SetEffShapeWeight("NbNjReweight");
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

  return 0;
}


// ===  FUNCTION  ============================================================
//         Name:  DefSysComAnd
//  Description:  /* cursor */
// ===========================================================================
bool DefSysComAnd(std::map<std::string, std::pair<std::string, std::string> > &SysMap, 
  std::map<std::string, ComAna*> &AnaMap, std::shared_ptr<TFile> OutFile)
{
  std::vector<std::string> anaNames;
  for(auto &ana : AnaMap)
  {
    anaNames.push_back(ana.first);
  }

  for(auto &sys : SysMap)
  {
    for(auto &ananame : anaNames)
    {
      std::stringstream ss;
      ss << ananame <<"_" << sys.first;
      AnaMap[ss.str()] = AnaMap[ananame]->Clone(ss.str(), OutFile);
      AnaMap[ss.str()]->SetSysVar(sys.second.first, sys.second.second);
    }
  }
  return true;
}       // -----  end of function DefSysComAnd  -----

