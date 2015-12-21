// ===========================================================================
// 
//       Filename:  StopAna.cc
// 
//    Description:  A standard stop analysis
// 
//        Version:  1.0
//        Created:  10/13/2015 16:05:36
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#include "StopAna.h"

//----------------------------------------------------------------------------
//       Class:  StopAna
//      Method:  StopAna
// Description:  constructor
//----------------------------------------------------------------------------
StopAna::StopAna (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile, std::string spec_)
: ComAna(name, tr_, OutFile, spec_)
{
  InitCutOrder(name);
  his->AddTH1("NBase", "Number of Events passed baseline", 2, 0, 2);
}  // -----  end of method StopAna::StopAna  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  StopAna
//      Method:  StopAna
// Description:  copy constructor
//----------------------------------------------------------------------------
StopAna::StopAna ( const StopAna &other ): ComAna(other)
{
}  // -----  end of method StopAna::StopAna  (copy constructor)  -----

// ===  FUNCTION  ============================================================
//         Name:  StopAna::Clone
//  Description:  /* cursor */
// ===========================================================================
StopAna* StopAna::Clone(std::string newname, std::shared_ptr<TFile> OutFile_) 
{
  if (OutFile_ == NULL)
    return new StopAna(newname, tr, OutFile, spec);
  else
    return new StopAna(newname, tr, OutFile_, spec);
}       // -----  end of function StopAna::Clone  -----

//----------------------------------------------------------------------------
//       Class:  StopAna
//      Method:  ~StopAna
// Description:  destructor
//----------------------------------------------------------------------------
StopAna::~StopAna ()
{
}  // -----  end of method StopAna::-StopAna  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  StopAna
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  StopAna&
StopAna::operator = ( const StopAna &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method StopAna::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  StopAna::BookHistograms
//  Description:  
// ===========================================================================
bool StopAna::BookHistograms()
{
  ComAna::BookHistograms();
  BookTLVHistos("RecoTop");
  his->AddTH1C("SearchBinsStat" , "Search Bins Stat;Search Bin;Events"        , 45 , 0    , 45);
  his->AddTH1C("hSearchBins" , "Search Bins;Search Bin;Events"        , 45 , 0    , 45);
  his->AddTH1C("hNJets30"    , "NJets30;N_{jets} (p_{T} > 30);Events" , 10 , 0    , 10);   // "cntNJetsPt30Eta24"
  his->AddTH1C("hNJets50"    , "NJets50;N_{jets} (p_{T} > 50);Events" , 10 , 0    , 10);   // "cntNJetsPt50Eta24"
  his->AddTH1C("hNTops"      , "NTops;N_{tops};Events"                , 5  , 0    , 5);    // "nTopCandSortedCnt"
  his->AddTH1C("hNbJets"     , "NbJets;N_{bjets};Events"              , 5  , 0    , 5);    // "cntCSVS"
  his->AddTH1C("hMET"        , "MET;#slash{E}_{T} [GeV];Events"       , 24 , 200  , 800);  // "met"
  his->AddTH1C("hMT2"        , "MT2;M_{T2} [GeV];Events"              , 24 , 200  , 800);  // "best_had_brJet_MT2"
  his->AddTH1C("hHT"         , "HT;H_{T} [GeV];Events"                , 20 , 500  , 1000); // "HT"

  return true;
}       // -----  end of function StopAna::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  StopAna::InitCutOrder
//  Description:  
// ===========================================================================
bool StopAna::InitCutOrder(std::string ana)
{
  //Clear the objects
  CutOrder.clear();
  CutMap.clear();

  //Add name and order of the cutflow
  CutOrder.push_back("NoCut");
  CutOrder.push_back("Filter");
  CutOrder.push_back("nJets");
  CutOrder.push_back("MuonVeto");
  CutOrder.push_back("EleVeto");
  CutOrder.push_back("IskVeto");
  CutOrder.push_back("dPhis");
  CutOrder.push_back("BJets");
  CutOrder.push_back("MET");
  CutOrder.push_back("Tagger");
  CutOrder.push_back("MT2");
  CutOrder.push_back("HT");
  CutOrder.push_back("Baseline");

  //Set the cutbit of each cut
  CutMap["NoCut"]    = "00000000000000000";
  CutMap["Filter"]   = "00000000000000001";
  CutMap["nJets"]    = "00000000000000011";
  CutMap["MuonVeto"] = "00000000000000111";
  CutMap["EleVeto"]  = "00000000000001111";
  CutMap["IskVeto"]  = "00000000000011111";
  CutMap["dPhis"]    = "00000000000111111";
  CutMap["BJets"]    = "00000000001111111";
  CutMap["MET"]      = "00000000011111111";
  CutMap["Tagger"]   = "00000000111111111";
  CutMap["MT2"]      = "00000001111111111";
  CutMap["HT"]       = "00000011111111111";
  CutMap["Baseline"] = "00000111111111111";

  assert(CutOrder.size() == CutMap.size());

  his->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));
  return true;
}       // -----  end of function StopAna::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  StopAna::CheckCut
//  Description:  
// ===========================================================================
bool StopAna::CheckCut()
{
  cutbit.reset();

  cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));
  cutbit.set(1 , tr->getVar<bool>(Label["passnJets"]));
  cutbit.set(2 , tr->getVar<bool>(Label["passMuonVeto"]));
  cutbit.set(3 , tr->getVar<bool>(Label["passEleVeto"]));
  cutbit.set(4 , tr->getVar<bool>(Label["passIsoTrkVeto"]));
  cutbit.set(5 , tr->getVar<bool>(Label["passdPhis"]));
  cutbit.set(6 , tr->getVar<bool>(Label["passBJets"]));
  cutbit.set(7 , tr->getVar<bool>(Label["passMET"]));
  cutbit.set(8 , tr->getVar<bool>(Label["passTagger"]));
  cutbit.set(9 , tr->getVar<bool>(Label["passMT2"]));
  cutbit.set(10, tr->getVar<bool>(Label["passHT"]));
  cutbit.set(11, tr->getVar<bool>(Label["passBaseline"]));

  return true;
}       // -----  end of function StopAna::CheckCut  -----

// ===  FUNCTION  ============================================================
//         Name:  StopAna::FillCut
//  Description:  
// ===========================================================================
bool StopAna::FillCut()
{

//----------------------------------------------------------------------------
//  Check cut and fill cut-based plots
//----------------------------------------------------------------------------
  CheckCut();
  ComAna::RunEvent();
  bool passcuts = false;

  for (unsigned int i = 0; i < CutOrder.size(); ++i)
  {
    std::bitset<NBITS> locbit(CutMap[CutOrder.at(i)]);
    if ( (cutbit & locbit) != locbit) continue;

    his->FillTH1("CutFlow", int(i)); 
    ComAna::FillCut(i);
    FillSearchBins(i);

    if (i+1 == CutOrder.size()) 
    {
      assert(tr->getVar<bool>(Label["passBaseline"]));
      passcuts = true;
    }
  }
  return passcuts;
}       // -----  end of function StopAna::FillCut  -----

// ===  FUNCTION  ============================================================
//         Name:  StopAna::FillSearchBins
//  Description:  
// ===========================================================================
bool StopAna::FillSearchBins(int NCut)
{
  int searchbin_id = find_Binning_Index( tr->getVar<int>(Label["cntCSVS"]), tr->getVar<int>(Label["nTopCandSortedCnt"]), 
      tr->getVar<double>(Label["best_had_brJet_MT2"]), tr->getVar<double>(METLabel));
  if( searchbin_id >= 0 )
  {
    his->FillTH1(NCut, "hSearchBins", searchbin_id);
    his->FillTH1(NCut, "SearchBinsStat", searchbin_id, tr->getVar<double>("stored_weight") >= 0 ? 1 : -1);
  }

  his->FillTH1(NCut, "hNJets30", tr->getVar<int>(Label["cntNJetsPt30Eta24"]));
  his->FillTH1(NCut, "hNJets50", tr->getVar<int>(Label["cntNJetsPt50Eta24"]));
  his->FillTH1(NCut, "hNTops", tr->getVar<int>(Label["nTopCandSortedCnt"]));
  his->FillTH1(NCut, "hNbJets", tr->getVar<int>(Label["cntCSVS"]));
  his->FillTH1(NCut, "hMET", tr->getVar<double>(METLabel));
  his->FillTH1(NCut, "hMT2", tr->getVar<double>(Label["best_had_brJet_MT2"]));
  his->FillTH1(NCut, "hHT", tr->getVar<double>(Label["HT"]));

  if (tr->getVar<int>(Label["nTopCandSortedCnt"]) == 1)
  {
    for(auto &z: tr->getVec<TLorentzVector>(Label["vTops"]))
    {
      FillTLVHistos(NCut, "RecoTop", z);
    }
  }

  return true;
}       // -----  end of function StopAna::FillSearchBins  -----
