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
#include "STZinv.h"
#include "STRM.h"
#include "PassCut.h"
#include "SBMulti.h"
#include "StopAna.h"
#include "TTZDiLep.h"
#include "TTZ3Lep.h"

// SusyAnaTools
#include "SusyAnaTools/Tools/baselineDef.h"
#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/EventListFilter.h"
#include "SusyAnaTools/Tools/PDFUncertainty.h"

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
  PDFUncertainty pdfs;
  //clock to monitor the run time
  size_t t0 = clock();
  NTupleReader tr(fChain);
  tr.registerFunction(pdfs);
  tr.registerFunction(&passBaselineFunc);
  tr.registerFunction(boost::bind(GetTopPtReweight, _1, SamplePro));
  tr.registerFunction(boost::bind(passBaselineZinv, _1, "001")); // bit : TEM
  tr.registerFunction(boost::bind(passBaselineZinv, _1, "010")); // bit : TEM
  tr.registerFunction(boost::bind(passBaselineZinv, _1, "100")); // bit : TEM
  tr.registerFunction(boost::bind(passBaselineTTZ, _1, "01")); // bit : EM
  tr.registerFunction(boost::bind(passBaselineTTZ, _1, "10")); // bit : EM

  //first loop, to generate Acc, reco and Iso effs and also fill expected histgram

  //**************************************************************************//
  //                           Prepare the analysis                           //
  //**************************************************************************//
  std::map<std::string, ComAna*> AnaMap;
  AnaMap["Stop"] = new StopAna("Stop", &tr, OutFile);
  //AnaMap["STISR"] = new STISR("STISR", &tr, OutFile);
  //AnaMap["STRM"] = new STRM("STRM", &tr, OutFile);
  AnaMap["STZinvM"] = new STZinv("STZinvM", &tr, OutFile,"ZinvM");
  AnaMap["STZinvE"] = new STZinv("STZinvE", &tr, OutFile,"ZinvE");
  AnaMap["STZinvT"] = new STZinv("STZinvT", &tr, OutFile,"ZinvT");
  AnaMap["TTZ3LepM"] = new TTZ3Lep("TTZ3LepM", &tr, OutFile, "TTZM");
  //AnaMap["TTZ3LepE"] = new TTZ3Lep("TTZ3LepE", &tr, OutFile, "TTZE");
  AnaMap["TTZDiLepM"] = new TTZDiLep("TTZDiLepM", &tr, OutFile, "TTZM");
  //AnaMap["TTZDiLepE"] = new TTZDiLep("TTZDiLepE", &tr, OutFile, "TTZE");
  //AnaMap["TTZ3Lep"] = new TTZ3Lep("TTZ3Lep", &tr, OutFile, "TTZ");
  //AnaMap["SBDJ"] = new SBDiJet("SBDJ", &tr, OutFile);
  //AnaMap["SBISR"] = new SBISR("SBISR", &tr, OutFile);
  //AnaMap["SBMulti"] = new SBMulti("SBMulti", &tr, OutFile);
  //AnaMap["PassCut"] = new PassCut("LeftOver", &tr, OutFile);

  //**************************************************************************//
  //                      Setup Systematics with Weights                      //
  //**************************************************************************//
  // Systematic_Name, sysbit (rate, shape), registerName
  std::map<std::string, std::pair<std::string, std::string> > SysMap;
  SysMap["PDF_up"] = std::make_pair("11", "PDF_Unc_Up"); // as shape uncertainty
  SysMap["PDF_down"] = std::make_pair("11", "PDF_Unc_Down"); // as shape uncertainty
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
    double stored_weight = -999;
    double evtWeight = 1.0; // For shape 
    double rateWeight = 1.0; // For rate (event count) -> NEvent & NBase

    //~~~~~~~~~~~~~~~~~~~~~ Getting weight that apply to both shape and rate ~~~~~
    try {
      stored_weight = tr.getVar<double>("stored_weight");
    } catch (std::string var) {
    }
    if (stored_weight == -999) stored_weight = 1;
    evtWeight = stored_weight >= 0 ? 1 : -1;
    if (tr.getVar<int>("run") != 1) // Set weight to 1 for Data
      evtWeight = 1;
    rateWeight = evtWeight;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting weight for rate, but not shape ~~~~~

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getting weight for shape, but not rate ~~~~~
    evtWeight *= tr.getVar<double>("TopPtReweight"); // TopPtReweight, apply to shape, not rate

    his->FillTH1("NEvent", 1, rateWeight);
    his->FillTH1("Weight", stored_weight);

    //**************************************************************************//
    //                                 Fill Cuts                                //
    //**************************************************************************//
    for( auto &it : AnaMap )
    {
      it.second->SetRateWeight(rateWeight);
      it.second->SetEvtWeight(evtWeight);
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

