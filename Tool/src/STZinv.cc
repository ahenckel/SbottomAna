// ===========================================================================
// 
//       Filename:  STZinv.cc
// 
//    Description:  A analysis for stop Zinv background studies
// 
//        Version:  1.0
//        Created:  11/16/2015 11:20:25
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#include "STZinv.h"

//----------------------------------------------------------------------------
//       Class:  STZinv
//      Method:  STZinv
// Description:  constructor
//----------------------------------------------------------------------------
STZinv::STZinv (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile, std::string spec_)
: ComAna(name, tr_, OutFile, spec_)
{
  InitCutOrder(name);
  if (spec_.find("Zinv") != std::string::npos)
  {

    jetVecLabel = "jetsLVecLepCleaned";
    CSVVecLabel = "recoJetsBtag_0_LepCleaned";
    METLabel = "cleanMetPt" + spec_;
    METPhiLabel = "cleanMetPhi" + spec_;
    if (spec_.find("M") != std::string::npos)
    {
      HLTstr.push_back("HLT_Mu45_eta2p1_v2");
      MCTrigstr.push_back(Label["PassDiMuonTrigger"]);
    }
    if (spec_.find("E") != std::string::npos)
    {
      HLTstr.push_back("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v2");
      HLTstr.push_back("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v3");
      MCTrigstr.push_back(Label["PassDiEleTrigger"]);
    }
  }
  //std::copy(MCTrigstr.begin(), MCTrigstr.end(), std::ostream_iterator<std::string>(std::cout, " "));
}  // -----  end of method STZinv::STZinv  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  STZinv
//      Method:  STZinv
// Description:  copy constructor
//----------------------------------------------------------------------------
STZinv::STZinv ( const STZinv &other ): ComAna(other)
{
}  // -----  end of method STZinv::STZinv  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  STZinv
//      Method:  ~STZinv
// Description:  destructor
//----------------------------------------------------------------------------
STZinv::~STZinv ()
{
}  // -----  end of method STZinv::-STZinv  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  STZinv
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  STZinv&
STZinv::operator = ( const STZinv &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method STZinv::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  STZinv::BookHistograms
//  Description:  
// ===========================================================================
bool STZinv::BookHistograms()
{
  ComAna::BookHistograms();
  BookTLVHistos("RecoZ");
  BookTLVHistos("RecoTop");

  his->AddTH1C("SearchBins", "Search Bins", 50, 0, 50);
  his->AddTH1C("JBT", "JBT", "JBT", "Events", 400, 0, 400);

  //Study the MET coorelation between
  his->AddTH1C("RawMET",    "RawMET",    "raw #slash{E}_{T} [GeV]",      "Events" , 200, 0,  800);
  his->AddTH1C("dPhiZMET" , "dPhiZMET" , "#phi(Z, #slash{E}_{T})" , "Events" , 20 , -5  , 5);
  his->AddTH2C("ZPTNJet" , "ZPTNJet" , "Z p_{T} [GeV]", "No. of Jets" , 100 , 0  , 1000, 10, 0, 10 );
  his->AddTH2C("ZPTRawMET" , "ZPTRawMET" , "Z p_{T} [GeV]", "raw #slash{E}_{T} [GeV]" , 100 , 0  , 1000, 200, 0 , 800 );
  his->AddTH2C("ZPTdPhiRawMET" , "ZPTdPhiRawMET" , "Z p_{T} [GeV]", "#phi(Z, #slash{E}_{T})" , 100 , 0  , 1000, 20, -5 , 5 );
  return true;

}       // -----  end of function STZinv::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  STZinv::InitCutOrder
//  Description:  
// ===========================================================================
bool STZinv::InitCutOrder(std::string ana)
{
  //Clear the objects
  CutOrder.clear();
  CutMap.clear();

  //Add name and order of the cutflow
  CutOrder.push_back("NoCut");
  CutOrder.push_back("Filter");
  CutOrder.push_back("Trigger");
  CutOrder.push_back("nJets");
  CutOrder.push_back("2Leps");
  CutOrder.push_back("HasZ");
  CutOrder.push_back("dPhis");
  CutOrder.push_back("HT");
  CutOrder.push_back("0bLoose");
  CutOrder.push_back("0bTight");
  CutOrder.push_back("1bLoose");
  CutOrder.push_back("1bTight");

  //CutOrder.push_back("BJets");
  //CutOrder.push_back("MET");
  //CutOrder.push_back("Tagger");
  //CutOrder.push_back("MT2");

  //Set the cutbit of each cut
  CutMap["NoCut"]    = "00000000000000000";
  CutMap["Filter"]   = "00000000000000001";
  CutMap["Trigger"]  = "00000000000000011";
  CutMap["nJets"]    = "00000000000000111";
  CutMap["2Leps"]    = "00000000000001111";
  CutMap["HasZ"]     = "00000000000011111";
  CutMap["dPhis"]    = "00000000000111111";
  CutMap["HT"]       = "00000000001111111";
  CutMap["0bLoose"]  = "00000010011111111";
  CutMap["0bTight"]  = "00000001011111111";
  CutMap["1bLoose"]  = "00000010101111111";
  CutMap["1bTight"]  = "00000001101111111";
  CutMap["0bVLoose"] = "00000100011111111";
  CutMap["1bVLoose"] = "00000100011111111";

  //CutMap["BJets"]     = "00000000011111111";
  //CutMap["Tagger"]  = "00000000111111111";
  //CutMap["MT2"]     = "00000001111111111";
  //CutMap["MET"]      = "00000011111111111";

  assert(CutOrder.size() == CutMap.size());

  his->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));
  return true;
}       // -----  end of function STZinv::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  STZinv::CheckCut
//  Description:  
// ===========================================================================
bool STZinv::CheckCut()
{
  cutbit.reset();
  cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));
  cutbit.set(1 , PassTrigger());
  cutbit.set(2 , tr->getVar<bool>(Label["passnJets"]));
  cutbit.set(3 , tr->getVar<int>(Label["nMuons_Base"]) + tr->getVar<int>(Label["nElectrons_Base"]) == 2 );
  cutbit.set(4 , tr->getVec<TLorentzVector>(Label["recoZVec"]).size() == 1);
  cutbit.set(5 , tr->getVar<bool>(Label["passdPhis"]));
  cutbit.set(6 , tr->getVar<double>(Label["HT"]) >= 200);

  // Nbtag bits
  cutbit.set(7 , tr->getVar<int>(Label["cntCSVS"]) == 0);
  cutbit.set(8 , tr->getVar<int>(Label["cntCSVS"]) > 0);

  // MET region: tight and loose
  cutbit.set(9 , tr->getVar<double>(METLabel) >= 200);
  cutbit.set(10 , tr->getVar<double>(METLabel) >= 100 && tr->getVar<double>(METLabel) < 200);
  cutbit.set(11 , tr->getVar<double>(METLabel) >= 50 && tr->getVar<double>(METLabel) < 200);

  return true;
}       // -----  end of function STZinv::CheckCut  -----

// ===  FUNCTION  ============================================================
//         Name:  STZinv::FillCut
//  Description:  
// ===========================================================================
bool STZinv::FillCut()
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
    ComAna::CheckLeadingLeptons(i);
    FillSearchBins(i);
    FillZMET(i);

    for(auto &z: tr->getVec<TLorentzVector>(Label["recoZVec"]))
    {
      FillTLVHistos(i, "RecoZ", z);
    }

    for(auto &z: tr->getVec<TLorentzVector>(Label["vTops"]))
    {
      FillTLVHistos(i, "RecoTop", z);
    }

    if (i+1 == CutOrder.size()) 
    {
      passcuts = true;
    }
  }

  return passcuts;
}       // -----  end of function STZinv::FillCut  -----

// ===  FUNCTION  ============================================================
//         Name:  STZinv::FillSearchBins
//  Description:  
// ===========================================================================
bool STZinv::FillSearchBins(int NCut)
{
  int searchbin_id = find_Binning_Index( tr->getVar<int>(Label["cntCSVS"]), tr->getVar<int>(Label["nTopCandSortedCnt"]), 
      tr->getVar<double>(Label["best_had_brJet_MT2"]), tr->getVar<double>(METLabel));
  if( searchbin_id >= 0 )
  {
    his->FillTH1(NCut, "SearchBins", searchbin_id);
  }
  return true;
}       // -----  end of function STZinv::FillSearchBins  -----

// ===  FUNCTION  ============================================================
//         Name:  STZinv::FillZMET
//  Description:  To study the correlation between regions
// ===========================================================================
bool STZinv::FillZMET(int NCut)
{
  his->FillTH1(NCut, "RawMET", tr->getVar<double>("met"));

  if (tr->getVec<TLorentzVector>(Label["recoZVec"]).size() == 1)
  {
    TLorentzVector METLV(0, 0, 0, 0);
    METLV.SetPtEtaPhiE(tr->getVar<double>("met"), 0, tr->getVar<double>("metphi"), 0);
    TLorentzVector recoZ =  tr->getVec<TLorentzVector>(Label["recoZVec"]).at(0);
    his->FillTH1(NCut, "dPhiZMET", recoZ.DeltaPhi(METLV));
    his->FillTH2(NCut, "ZPTNJet", recoZ.Pt(), tr->getVar<int>("cntNJetsPt30Eta24"));
    his->FillTH2(NCut, "ZPTRawMET", recoZ.Pt(), tr->getVar<double>("met"));
    his->FillTH2(NCut, "ZPTdPhiRawMET", recoZ.Pt(), recoZ.DeltaPhi(METLV));
  }
  return true;
}       // -----  end of function STZinv::FillZMET  -----

