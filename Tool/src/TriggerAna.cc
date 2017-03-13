// ===========================================================================
// 
//       Filename:  TriggerAna.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  11/28/16 16:22:52
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#include "TriggerAna.h"

//----------------------------------------------------------------------------
//       Class:  TriggerAna
//      Method:  TriggerAna
// Description:  constructor
//----------------------------------------------------------------------------
TriggerAna::TriggerAna (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile, std::string spec_)
: ComAna(name, tr_, OutFile, spec_)
{
  InitCutOrder(name);
}  // -----  end of method TriggerAna::TriggerAna  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  TriggerAna
//      Method:  TriggerAna
// Description:  copy constructor
//----------------------------------------------------------------------------
TriggerAna::TriggerAna ( const TriggerAna &other ): ComAna(other)
{
}  // -----  end of method TriggerAna::TriggerAna  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  TriggerAna
//      Method:  ~TriggerAna
// Description:  destructor
//----------------------------------------------------------------------------
TriggerAna::~TriggerAna ()
{
}  // -----  end of method TriggerAna::-TriggerAna  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  TriggerAna
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  TriggerAna&
TriggerAna::operator = ( const TriggerAna &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method TriggerAna::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  TriggerAna::BookHistograms
//  Description:  
// ===========================================================================
bool TriggerAna::BookHistograms()
{

  if (AnaName.find("Electron") != std::string::npos)
    BookEleHistograms();
  else if (AnaName.find("Muon") != std::string::npos)
    BookMuonHistograms();
  else
    BookStopHistograms();

  return true;
}       // -----  end of function TriggerAna::BookHistograms  -----


// ===  FUNCTION  ============================================================
//         Name:  TriggerAna::BookEleHistograms
//  Description:  
// ===========================================================================
bool TriggerAna::BookEleHistograms()
{
  his->AddTH1C("TrigEle_Denominator"      , "TrigEle_Denominator"    , "ElePT [GeV]"     , "Denominator" , 250, 0,  500);
  his->AddTH1C("TrigEle_Numerator"        , "TrigEle_Numerator"      , "ElePT [GeV]"     , "Numerator"   , 250, 0,  500);
  his->AddTH1C("TrigEleEta40_Denominator" , "TrigEleEta40_Denominator" , "EleEta(PT > 40)" , "Denominator" , 60,  -3, 3);
  his->AddTH1C("TrigEleEta40_Numerator"   , "TrigEleEta40_Numerator"   , "EleEta(PT > 40)" , "Numerator"   , 60,  -3, 3);
  return true;
}       // -----  end of function TriggerAna::BookEleHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  TriggerAna::BookMuonHistograms
//  Description:  
// ===========================================================================
bool TriggerAna::BookMuonHistograms()
{
  his->AddTH1C("Trig50Muon_Denominator"      , "Trig50Muon_Denominator"    , "MuonPT [GeV]"     , "Denominator" , 250, 0,  500);
  his->AddTH1C("Trig50Muon_Numerator"        , "Trig50Muon_Numerator"      , "MuonPT [GeV]"     , "Numerator"   , 250, 0,  500);
  his->AddTH1C("Trig50MuonEta50_Denominator" , "Trig50MuonEta50_Denominator" , "MuonEta(PT > 50)" , "Denominator" , 60,  -3, 3);
  his->AddTH1C("Trig50MuonEta50_Numerator"   , "Trig50MuonEta50_Numerator"   , "MuonEta(PT > 50)" , "Numerator"   , 60,  -3, 3);

  his->AddTH1C("Trig50DiMuMuon_Denominator"      , "Trig50DiMuMuon_Denominator"    , "MuonPT [GeV]"     , "Denominator" , 250, 0,  500);
  his->AddTH1C("Trig50DiMuMuon_Numerator"        , "Trig50DiMuMuon_Numerator"      , "MuonPT [GeV]"     , "Numerator"   , 250, 0,  500);
  his->AddTH1C("Trig50DiMuMuonEta50_Denominator" , "Trig50DiMuMuonEta50_Denominator" , "MuonEta(PT > 50)" , "Denominator" , 60,  -3, 3);
  his->AddTH1C("Trig50DiMuMuonEta50_Numerator"   , "Trig50DiMuMuonEta50_Numerator"   , "MuonEta(PT > 50)" , "Numerator"   , 60,  -3, 3);

  his->AddTH1C("TrigMuon_Denominator"      , "TrigMuon_Denominator"    , "MuonPT [GeV]"     , "Denominator" , 250, 0,  500);
  his->AddTH1C("TrigMuon_Numerator"        , "TrigMuon_Numerator"      , "MuonPT [GeV]"     , "Numerator"   , 250, 0,  500);
  his->AddTH1C("TrigMuonEta45_Denominator" , "TrigMuonEta45_Denominator" , "MuonEta(PT > 45)" , "Denominator" , 60,  -3, 3);
  his->AddTH1C("TrigMuonEta45_Numerator"   , "TrigMuonEta45_Numerator"   , "MuonEta(PT > 45)" , "Numerator"   , 60,  -3, 3);
  his->AddTH1C("TrigMuonEta40_Denominator" , "TrigMuonEta40_Denominator" , "MuonEta(PT > 40)" , "Denominator" , 60,  -3, 3);
  his->AddTH1C("TrigMuonEta40_Numerator"   , "TrigMuonEta40_Numerator"   , "MuonEta(PT > 40)" , "Numerator"   , 60,  -3, 3);
  his->AddTH1C("TrigMuonEta50_Denominator" , "TrigMuonEta50_Denominator" , "MuonEta(PT > 50)" , "Denominator" , 60,  -3, 3);
  his->AddTH1C("TrigMuonEta50_Numerator"   , "TrigMuonEta50_Numerator"   , "MuonEta(PT > 50)" , "Numerator"   , 60,  -3, 3);
  return true;
}       // -----  end of function TriggerAna::BookMuonHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  TriggerAna::BookStopHistograms
//  Description:  
// ===========================================================================
bool TriggerAna::BookStopHistograms()
{
  his->AddTH1C("TrigMET_Denominator" , "TrigMET_Denominator" , "MET [GeV]" , "Denominator" , 200, 0, 1000);
  his->AddTH1C("TrigMET_Numerator"   , "TrigMET_Numerator"   , "MET [GeV]" , "Numerator"   , 200, 0, 1000);
  his->AddTH1C("TrigMET_HTLess1000_Denominator" , "TrigMET_HTLess1000_Denominator" , "MET [GeV]" , "Denominator" , 200, 0, 1000);
  his->AddTH1C("TrigMET_HTLess1000_Numerator"   , "TrigMET_HTLess1000_Numerator"   , "MET [GeV]" , "Numerator"   , 200, 0, 1000);
  his->AddTH1C("TrigMET_HTMore1000_Denominator" , "TrigMET_HTMore1000_Denominator" , "MET [GeV]" , "Denominator" , 200, 0, 1000);
  his->AddTH1C("TrigMET_HTMore1000_Numerator"   , "TrigMET_HTMore1000_Numerator"   , "MET [GeV]" , "Numerator"   , 200, 0, 1000);

  his->AddTH1C("TrigHT_Denominator"    , "TrigHT_Denominator"    , "HT [GeV]" , "Denominator" , 300, 0, 1500);
  his->AddTH1C("TrigHT_Numerator"      , "TrigHT_Numerator"      , "HT [GeV]" , "Numerator"   , 300, 0, 1500);
  his->AddTH1C("TrigNJets_Denominator" , "TrigNJets_Denominator" , "NJets"    , "Denominator" , 8,   0, 8);
  his->AddTH1C("TrigNJets_Numerator"   , "TrigNJets_Numerator"   , "NJets"    , "Numerator"   , 8,   0, 8);
  his->AddTH1C("TrigNBs_Denominator"   , "TrigNBs_Denominator"   , "NBs"      , "Denominator" , 4,   0, 4);
  his->AddTH1C("TrigNBs_Numerator"     , "TrigNBs_Numerator"     , "NBs"      , "Numerator"   , 4,   0, 4);

  return true;
}       // -----  end of function TriggerAna::BookStopHistograms  -----


// ===  FUNCTION  ============================================================
//         Name:  TriggerAna::InitCutOrder
//  Description:  
// ===========================================================================
bool TriggerAna::InitCutOrder(std::string ana)
{
  //Clear the objects
  CutOrder.clear();
  CutMap.clear();
  HLTstr.clear();



//**************************************************************************//
//                          Stop Trigger Efficiency                         //
//**************************************************************************//
  if (AnaName == "TrigEle_Stop" || AnaName == "TrigEle_QCD")
  {
    HLTstr.push_back("HLT_Ele27_WPTight_Gsf_v\\d");

    CutOrder.push_back("NoCut");
    CutOrder.push_back("Filter");
    CutOrder.push_back("EleTrig");
    CutOrder.push_back("NEle");
    CutOrder.push_back("MuonVeto");
    CutOrder.push_back("nJets30");
    CutOrder.push_back("nJets50");
    CutOrder.push_back("BJets");
    CutOrder.push_back("HT");
    CutOrder.push_back("dDPhi");
    CutOrder.push_back("WithMuon");

    //Set the cutbit of each cut
    CutMap["NoCut"]    = "00000000000000000";
    CutMap["Filter"]   = "00000000000000001";
    CutMap["EleTrig"]  = "00000000000000011";
    CutMap["NEle"]     = "00000000000000111";
    CutMap["MuonVeto"] = "00000000000001111";
    CutMap["nJets30"]  = "00000000000011111";
    CutMap["nJets50"]  = "00000000000111111";
    CutMap["BJets"]    = "00000000001111111";
    CutMap["HT"]       = "00000000011111111";
    CutMap["dDPhi"]    = "00000000111111111";
    CutMap["WithMuon"] = "00000001111110111";
  }
  if (AnaName == "TrigMu_Stop" || AnaName == "TrigMu_QCD" )
  {
    HLTstr.push_back("HLT_Mu50_v\\d");

    CutOrder.push_back("NoCut");
    CutOrder.push_back("Filter");
    CutOrder.push_back("MuTrig");
    CutOrder.push_back("NMu");
    CutOrder.push_back("EleVeto");
    CutOrder.push_back("nJets30");
    CutOrder.push_back("nJets50");
    CutOrder.push_back("BJets");
    CutOrder.push_back("HT");
    CutOrder.push_back("dDPhi");
    CutOrder.push_back("WithEle");

    //Set the cutbit of each cut
    CutMap["NoCut"]   = "00000000000000000";
    CutMap["Filter"]  = "00000000000000001";
    CutMap["MuTrig"]  = "00000000000000011";
    CutMap["NMu"]     = "00000000000000111";
    CutMap["EleVeto"] = "00000000000001111";
    CutMap["nJets30"] = "00000000000011111";
    CutMap["nJets50"] = "00000000000111111";
    CutMap["BJets"]   = "00000000001111111";
    CutMap["HT"]      = "00000000011111111";
    CutMap["dDPhi"]   = "00000000111111111";
    CutMap["WithEle"] = "00000001111110111";
  }
  if (AnaName == "TrigHT_Stop" ||AnaName == "TrigHT_QCD")
  {
    HLTstr.push_back("HLT_PFHT125_v\\d");
    HLTstr.push_back("HLT_PFHT200_v\\d");
    HLTstr.push_back("HLT_PFHT250_v\\d");
    HLTstr.push_back("HLT_PFHT300_PFMET100_v\\d");
    HLTstr.push_back("HLT_PFHT300_v\\d");
    HLTstr.push_back("HLT_PFHT350_v\\d");
    HLTstr.push_back("HLT_PFHT400_v\\d");
    HLTstr.push_back("HLT_PFHT475_v\\d");
    HLTstr.push_back("HLT_PFHT600_v\\d");
    HLTstr.push_back("HLT_PFHT650_v\\d");
    HLTstr.push_back("HLT_PFHT800_v\\d");
    HLTstr.push_back("HLT_PFHT900_v\\d");
    HLTstr.push_back("HLT_PFJet450_v\\d");
    HLTstr.push_back("HLT_CaloJet500_NoJetID_v\\d");



    CutOrder.push_back("NoCut");
    CutOrder.push_back("Filter");
    CutOrder.push_back("HTTrig");
    CutOrder.push_back("MuVeto");
    CutOrder.push_back("EleVeto");
    CutOrder.push_back("nJets30");
    CutOrder.push_back("nJets50");
    CutOrder.push_back("BJets");
    CutOrder.push_back("HT");
    CutOrder.push_back("dDPhi");
    CutOrder.push_back("WithEle");
    CutOrder.push_back("WithMu");

    //Set the cutbit of each cut
    CutMap["NoCut"]   = "00000000000000000";
    CutMap["Filter"]  = "00000000000000001";
    CutMap["HTTrig"]  = "00000000000000011";
    CutMap["MuVeto"]  = "00000000000000111";
    CutMap["EleVeto"] = "00000000000001111";
    CutMap["nJets30"] = "00000000000011111";
    CutMap["nJets50"] = "00000000000111111";
    CutMap["BJets"]   = "00000000001111111";
    CutMap["HT"]      = "00000000011111111";
    CutMap["dDPhi"]   = "00000000111111111";
    CutMap["WithEle"] = "00000001111110111";
    CutMap["WithMu"]  = "00000010111111011";
  }




//**************************************************************************//
//                                  Others                                  //
//**************************************************************************//
  if (AnaName == "TrigEle_Muon")
  {
    // Use this trigger as denominator
    HLTstr.push_back("HLT_Ele27_WPTight_Gsf_v\\d");

    //Add name and order of the cutflow
    //Default is the singleEle trigger pass
    CutOrder.push_back("NoCut");
    CutOrder.push_back("Filter");
    CutOrder.push_back("EleTrig");
    CutOrder.push_back("NEle");
    CutOrder.push_back("NMuon");
    CutOrder.push_back("nJets30");
    CutOrder.push_back("nJets50");
    CutOrder.push_back("BJets");
    CutOrder.push_back("HT");

    //Set the cutbit of each cut
    CutMap["NoCut"]   = "00000000000000000";
    CutMap["Filter"]  = "00000000000000001";
    CutMap["EleTrig"] = "00000000000000011";
    CutMap["NEle"]    = "00000000000000111";
    CutMap["NMuon"]   = "00000000000001111";
    CutMap["nJets30"] = "00000000000011111";
    CutMap["nJets50"] = "00000000000111111";
    CutMap["BJets"]   = "00000000001111111";
    CutMap["HT"]      = "00000000011111111";
  }

  if (AnaName == "TrigMu_Electron")
  {
    // Use this trigger as denominator
    HLTstr.push_back("HLT_Mu50_v\\d");

    //Add name and order of the cutflow
    //Default is the singleEle trigger pass
    CutOrder.push_back("NoCut");
    CutOrder.push_back("Filter");
    CutOrder.push_back("MuTrig");
    CutOrder.push_back("NMuon");
    CutOrder.push_back("NEle");
    CutOrder.push_back("nJets30");
    CutOrder.push_back("nJets50");
    CutOrder.push_back("BJets");
    CutOrder.push_back("HT");

    //Set the cutbit of each cut
    CutMap["NoCut"]   = "00000000000000000";
    CutMap["Filter"]  = "00000000000000001";
    CutMap["MuTrig"]  = "00000000000000011";
    CutMap["NMuon"]   = "00000000000000111";
    CutMap["NEle"]    = "00000000000001111";
    CutMap["nJets30"] = "00000000000011111";
    CutMap["nJets50"] = "00000000000111111";
    CutMap["BJets"]   = "00000000001111111";
    CutMap["HT"]      = "00000000011111111";
  }


  if (AnaName == "TrigEle_HT")
  {
    // Use this trigger as denominator
    HLTstr.push_back("HLT_Ele27_WPTight_Gsf_v\\d");

    //Add name and order of the cutflow
    //Default is the singleEle trigger pass
    CutOrder.push_back("NoCut");
    CutOrder.push_back("Filter");
    CutOrder.push_back("EleTrig");
    CutOrder.push_back("NEle");
    CutOrder.push_back("MuonVeto");
    CutOrder.push_back("nJets30");
    CutOrder.push_back("nJets50");
    CutOrder.push_back("BJets");
    CutOrder.push_back("HT");

    //Set the cutbit of each cut
    CutMap["NoCut"]   = "00000000000000000";
    CutMap["Filter"]  = "00000000000000001";
    CutMap["EleTrig"] = "00000000000000011";
    CutMap["NEle"]    = "00000000000000111";
    CutMap["MuonVeto"]= "00000000000001111";
    CutMap["nJets30"] = "00000000000011111";
    CutMap["nJets50"] = "00000000000111111";
    CutMap["BJets"]   = "00000000001111111";
    CutMap["HT"]      = "00000000011111111";
  }

  if (AnaName == "TrigMu_HT")
  {
    // Use this trigger as denominator
    HLTstr.push_back("HLT_Mu50_v\\d");

    //Add name and order of the cutflow
    //Default is the singleEle trigger pass
    CutOrder.push_back("NoCut");
    CutOrder.push_back("Filter");
    CutOrder.push_back("MuTrig");
    CutOrder.push_back("NMuon");
    CutOrder.push_back("EleVeto");
    CutOrder.push_back("nJets30");
    CutOrder.push_back("nJets50");
    CutOrder.push_back("BJets");
    CutOrder.push_back("HT");

    //Set the cutbit of each cut
    CutMap["NoCut"]   = "00000000000000000";
    CutMap["Filter"]  = "00000000000000001";
    CutMap["MuTrig"]  = "00000000000000011";
    CutMap["NMuon"]   = "00000000000000111";
    CutMap["EleVeto"] = "00000000000001111";
    CutMap["nJets30"] = "00000000000011111";
    CutMap["nJets50"] = "00000000000111111";
    CutMap["BJets"]   = "00000000001111111";
    CutMap["HT"]      = "00000000011111111";
  }

  assert(CutOrder.size() == CutMap.size());

  his->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));
  return true;

}       // -----  end of function TriggerAna::InitCutOrder  -----


// ===  FUNCTION  ============================================================
//         Name:  TriggerAna::CheckCut
//  Description:  
// ===========================================================================
bool TriggerAna::CheckCut()
{
  cutbit.reset();

  if (AnaName == "TrigEle_Stop" || AnaName == "TrigEle_QCD")
  {
    cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));
    cutbit.set(1 , PassTrigger());
    cutbit.set(2 , tr->getVec<TLorentzVector>(Label["cutEleVec"]).size() >= 1 && 
        tr->getVec<TLorentzVector>(Label["cutEleVec"]).front().Pt() > 30);
    cutbit.set(3 , tr->getVar<int>(Label["nMuons_Base"]) == 0 );
    cutbit.set(4 , tr->getVar<int>(Label["cntNJetsPt30Eta24"]) > 3 );
    cutbit.set(5 , tr->getVar<int>(Label["cntNJetsPt50Eta24"]) > 1);
    cutbit.set(6 , tr->getVar<bool>(Label["passBJets"]));
    cutbit.set(7 , tr->getVar<bool>(Label["passHT"]));
    if (AnaName.find("QCD") != std::string::npos)
      cutbit.set(8 , !tr->getVar<bool>(Label["passdPhis"]));
    else
      cutbit.set(8 , tr->getVar<bool>(Label["passdPhis"]));
    cutbit.set(9 , tr->getVar<int>(Label["nMuons_Base"]) >= 1);
    
  }
  if (AnaName == "TrigMu_Stop" || AnaName == "TrigMu_QCD" )
  {
    cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));
    cutbit.set(1 , PassTrigger());
    cutbit.set(2 , tr->getVec<TLorentzVector>(Label["cutMuVec"]).size() >= 1 && 
        tr->getVec<TLorentzVector>(Label["cutMuVec"]).front().Pt() > 50);
    cutbit.set(3 , tr->getVar<int>(Label["nElectrons_Base"]) == 0 );
    cutbit.set(4 , tr->getVar<int>(Label["cntNJetsPt30Eta24"]) > 3 );
    cutbit.set(5 , tr->getVar<int>(Label["cntNJetsPt50Eta24"]) > 1);
    cutbit.set(6 , tr->getVar<bool>(Label["passBJets"]));
    cutbit.set(7 , tr->getVar<bool>(Label["passHT"]));
    if (AnaName.find("QCD") != std::string::npos)
      cutbit.set(8 , !tr->getVar<bool>(Label["passdPhis"]));
    else
      cutbit.set(8 , tr->getVar<bool>(Label["passdPhis"]));
    cutbit.set(9 , tr->getVar<int>(Label["nElectrons_Base"]) >= 1);
    
  }

  if (AnaName == "TrigHT_Stop" ||AnaName == "TrigHT_QCD")
  {
    cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));
    cutbit.set(1 , PassTrigger());
    cutbit.set(2 , tr->getVar<int>(Label["nMuons_Base"]) == 0 );
    cutbit.set(3 , tr->getVar<int>(Label["nElectrons_Base"]) == 0 );
    cutbit.set(4 , tr->getVar<int>(Label["cntNJetsPt30Eta24"]) > 3 );
    cutbit.set(5 , tr->getVar<int>(Label["cntNJetsPt50Eta24"]) > 1);
    cutbit.set(6 , tr->getVar<bool>(Label["passBJets"]));
    cutbit.set(7 , tr->getVar<bool>(Label["passHT"]));
    if (AnaName.find("QCD") != std::string::npos)
      cutbit.set(8 , !tr->getVar<bool>(Label["passdPhis"]));
    else
      cutbit.set(8 , tr->getVar<bool>(Label["passdPhis"]));
    cutbit.set(9 , tr->getVar<int>(Label["nElectrons_Base"]) >= 1);
    cutbit.set(10 , tr->getVar<int>(Label["nMuons_Base"]) >= 1);
    
  }

//**************************************************************************//
//                                  Others                                  //
//**************************************************************************//
  if (AnaName == "TrigEle_Muon")
  {
    cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));
    cutbit.set(1 , PassTrigger());
    cutbit.set(2 , tr->getVec<TLorentzVector>(Label["cutEleVec"]).size() >= 1 && 
        tr->getVec<TLorentzVector>(Label["cutEleVec"]).front().Pt() > 30);
    cutbit.set(3 , tr->getVar<int>(Label["nMuons_Base"]) >= 1 );
    cutbit.set(4 , tr->getVar<int>(Label["cntNJetsPt30Eta24"]) > 3 );
    cutbit.set(5 , tr->getVar<int>(Label["cntNJetsPt50Eta24"]) > 1);
    cutbit.set(6 , tr->getVar<bool>(Label["passBJets"]));
    cutbit.set(7 , tr->getVar<bool>(Label["passHT"]));
  }

  if (AnaName == "TrigMu_Electron")
  {
    cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));
    cutbit.set(1 , PassTrigger());
    cutbit.set(2 , tr->getVec<TLorentzVector>(Label["cutMuVec"]).size() >= 1 && 
        tr->getVec<TLorentzVector>(Label["cutMuVec"]).front().Pt() > 50);
    cutbit.set(3 , tr->getVar<int>(Label["nElectrons_Base"]) >= 1 );
    cutbit.set(4 , tr->getVar<int>(Label["cntNJetsPt30Eta24"]) > 3 );
    cutbit.set(5 , tr->getVar<int>(Label["cntNJetsPt50Eta24"]) > 1);
    cutbit.set(6 , tr->getVar<bool>(Label["passBJets"]));
    cutbit.set(7 , tr->getVar<bool>(Label["passHT"]));
  }

  if (AnaName == "TrigEle_HT")
  {
    cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));
    cutbit.set(1 , PassTrigger());
    cutbit.set(2 , tr->getVec<TLorentzVector>(Label["cutEleVec"]).size() >= 1 && 
        tr->getVec<TLorentzVector>(Label["cutEleVec"]).front().Pt() > 30);
    cutbit.set(3 , tr->getVar<int>(Label["nMuons_Base"]) == 0 );
    cutbit.set(4 , tr->getVar<int>(Label["cntNJetsPt30Eta24"]) > 3 );
    cutbit.set(5 , tr->getVar<int>(Label["cntNJetsPt50Eta24"]) > 1);
    cutbit.set(6 , tr->getVar<bool>(Label["passBJets"]));
    cutbit.set(7 , tr->getVar<bool>(Label["passHT"]));
  }

  if (AnaName == "TrigMu_HT")
  {
    cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));
    cutbit.set(1 , PassTrigger());
    cutbit.set(2 , tr->getVec<TLorentzVector>(Label["cutMuVec"]).size() >= 1 && 
        tr->getVec<TLorentzVector>(Label["cutMuVec"]).front().Pt() > 50);
    cutbit.set(3 , tr->getVar<int>(Label["nElectrons_Base"]) == 0 );
    cutbit.set(4 , tr->getVar<int>(Label["cntNJetsPt30Eta24"]) > 3 );
    cutbit.set(5 , tr->getVar<int>(Label["cntNJetsPt50Eta24"]) > 1);
    cutbit.set(6 , tr->getVar<bool>(Label["passBJets"]));
    cutbit.set(7 , tr->getVar<bool>(Label["passHT"]));
  }

  return true;
}       // -----  end of function TriggerAna::CheckCut  -----

// ===  FUNCTION  ============================================================
//         Name:  TriggerAna::FillCut
//  Description:  
// ===========================================================================
bool TriggerAna::FillCut()
{
//----------------------------------------------------------------------------
//  Check cut and fill cut-based plots
//----------------------------------------------------------------------------
  HLTIdx.clear();
  //if (ComAna::IsUpdateHLT()) HLTIdx.clear();
  ComAna::RunEvent();
  CheckCut();
  bool passcuts = false;

  for (unsigned int i = 0; i < CutOrder.size(); ++i)
  {
    std::bitset<NBITS> locbit(CutMap[CutOrder.at(i)]);
    if ( (cutbit & locbit) != locbit) continue;

    his->FillTH1("CutFlow", int(i)); 
    if (AnaName.find("Stop") != std::string::npos || AnaName.find("QCD") != std::string::npos )
      FillMETEff(i);
    if (AnaName.find("Muon") != std::string::npos)
      FillMuonEff(i);
    if (AnaName.find("Electron") != std::string::npos)
      FillEleEff(i);
    if (AnaName.find("_HT") != std::string::npos)
      FillHTEff(i);
  }

  return true;
}       // -----  end of function TriggerAna::FillCut  -----l

// ===  FUNCTION  ============================================================
//         Name:  TriggerAna::FillMETEff
//  Description:  
// ===========================================================================
bool TriggerAna::FillMETEff(int NCut)
{
  // Denominator
  his->FillTH1(NCut, "TrigMET_Denominator",   tr->getVar<double>(METLabel));
  his->FillTH1(NCut, "TrigHT_Denominator",    tr->getVar<double>(Label["HT"]));
  his->FillTH1(NCut, "TrigNJets_Denominator", tr->getVar<int>(Label["cntNJetsPt30"]));
  his->FillTH1(NCut, "TrigNBs_Denominator",   tr->getVar<int>(Label["cntCSVS"]));
  if (tr->getVar<double>(Label["HT"]) > 1000)
    his->FillTH1(NCut, "TrigMET_HTMore1000_Denominator", tr->getVar<double>(METLabel));
  else
    his->FillTH1(NCut, "TrigMET_HTLess1000_Denominator", tr->getVar<double>(METLabel));

  std::vector<std::string> METHLT;
  METHLT.push_back("HLT_PFMET100_PFMHT100_IDTight_v\\d");
  METHLT.push_back("HLT_PFMET110_PFMHT110_IDTight_v\\d");
  METHLT.push_back("HLT_PFMET120_PFMHT120_IDTight_v\\d");
  METHLT.push_back("HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_v\\d");
  METHLT.push_back("HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v\\d");
  METHLT.push_back("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v\\d");
  if (PassTrigger(METHLT))
  {
    his->FillTH1(NCut, "TrigMET_Numerator", tr->getVar<double>(METLabel));
    if (tr->getVar<double>(Label["HT"]) > 1000)
      his->FillTH1(NCut, "TrigMET_HTMore1000_Numerator", tr->getVar<double>(METLabel));
    else
      his->FillTH1(NCut, "TrigMET_HTLess1000_Numerator", tr->getVar<double>(METLabel));

    his->FillTH1(NCut, "TrigNJets_Numerator", tr->getVar<int>(Label["cntNJetsPt30"]));
    his->FillTH1(NCut, "TrigNBs_Numerator",   tr->getVar<int>(Label["cntCSVS"]));
  }

  return true;
}       // -----  end of function TriggerAna::FillMETEff  -----

// ===  FUNCTION  ============================================================
//         Name:  TriggerAna::FillMuonEff
//  Description:  
// ===========================================================================
bool TriggerAna::FillMuonEff(int NCut)
{
  const std::vector<TLorentzVector> &muonsLVec   = tr->getVec<TLorentzVector>(Label["cutMuVec"]);
  if (muonsLVec.empty()) return false;
  double LeadingPt =  -1;
  double LeadingEta40 =  -999;
  double LeadingEta45 =  -999;
  double LeadingEta50 =  -999;
  for(auto i : muonsLVec)
  {
    //if (fabs(i.Eta()) > 2.1) continue;
    if (i.Pt() > LeadingPt)
    {
      LeadingPt = i.Pt();
      if (i.Pt() > 40)
        LeadingEta40 = i.Eta();
      if (i.Pt() > 45)
        LeadingEta45 = i.Eta();
      if (i.Pt() > 50)
        LeadingEta50 = i.Eta();
    }
  }

  his->FillTH1(NCut, "TrigMuon_Denominator", LeadingPt);
  his->FillTH1(NCut, "Trig50Muon_Denominator", LeadingPt);
  his->FillTH1(NCut, "Trig50MuonEta50_Denominator", LeadingEta50);
  his->FillTH1(NCut, "Trig50DiMuMuon_Denominator", LeadingPt);
  his->FillTH1(NCut, "Trig50DiMuMuonEta50_Denominator", LeadingEta50);
  his->FillTH1(NCut, "TrigMuonEta40_Denominator", LeadingEta40);
  his->FillTH1(NCut, "TrigMuonEta45_Denominator", LeadingEta45);
  his->FillTH1(NCut, "TrigMuonEta50_Denominator", LeadingEta50);

  std::vector<std::string> MuonHLT;
  MuonHLT.push_back("HLT_Mu50_v\\d");
  if (PassTrigger(MuonHLT))
  {
    his->FillTH1(NCut, "Trig50Muon_Numerator", LeadingPt);
    his->FillTH1(NCut, "Trig50MuonEta50_Numerator", LeadingEta50);
  }

  std::vector<std::string> DiMuHLT = MuonHLT;
  DiMuHLT.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v\\d");
  DiMuHLT.push_back("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v\\d");
  if (PassTrigger(DiMuHLT))
  {
    his->FillTH1(NCut, "Trig50DiMuMuon_Numerator", LeadingPt);
    his->FillTH1(NCut, "Trig50DiMuMuonEta50_Numerator", LeadingEta50);
  }

  MuonHLT.push_back("HLT_IsoMu24_v\\d");
  MuonHLT.push_back("HLT_IsoTKMu24_v\\d");
  if (PassTrigger(MuonHLT))
  {
    his->FillTH1(NCut, "TrigMuon_Numerator", LeadingPt);
    his->FillTH1(NCut, "TrigMuonEta40_Numerator", LeadingEta40);
    his->FillTH1(NCut, "TrigMuonEta45_Numerator", LeadingEta45);
    his->FillTH1(NCut, "TrigMuonEta50_Numerator", LeadingEta50);
  }
  return true;
}       // -----  end of function TriggerAna::FillMuonEff  -----
// ===  FUNCTION  ============================================================
//         Name:  TriggerAna::FillEleEff
//  Description:  
// ===========================================================================
bool TriggerAna::FillEleEff(int NCut)
{
  
  const std::vector<TLorentzVector> &elesLVec   = tr->getVec<TLorentzVector>(Label["cutEleVec"]);
  if (elesLVec.empty()) return false;
  double LeadingPt =  -1;
  double LeadingEta40 =  -999;

  for(auto i : elesLVec)
  {
    //if (fabs(i.Eta()) > 2.1) continue;
    if (i.Pt() > LeadingPt)
    {
      LeadingPt = i.Pt();
      if (i.Pt() > 40)
        LeadingEta40 = i.Eta();
    }
  }
  his->FillTH1(NCut, "TrigEle_Denominator", LeadingPt);
  his->FillTH1(NCut, "TrigEleEta40_Denominator", LeadingEta40);

  std::vector<std::string> EleHLT;
  EleHLT.push_back("HLT_Ele27_WPTight_Gsf_v\\d");
  EleHLT.push_back("HLT_Ele105_CaloIdVT_GsfTrkIdT_v\\d");
  EleHLT.push_back("HLT_Ele115_CaloIdVT_GsfTrkIdT_v\\d");
  if (PassTrigger(EleHLT))
  {
    his->FillTH1(NCut, "TrigEle_Numerator", LeadingPt);
    his->FillTH1(NCut, "TrigEleEta40_Numerator", LeadingEta40);
  }
  return true;
}       // -----  end of function TriggerAna::FillEleEff  -----

// ===  FUNCTION  ============================================================
//         Name:  TriggerAna::FillHTEff
//  Description:  
// ===========================================================================
bool TriggerAna::FillHTEff(int NCut)
{
  his->FillTH1(NCut, "TrigMET_Denominator",   tr->getVar<double>(METLabel));
  his->FillTH1(NCut, "TrigHT_Denominator",    tr->getVar<double>(Label["HT"]));
  his->FillTH1(NCut, "TrigNJets_Denominator", tr->getVar<int>(Label["cntNJetsPt30"]));
  his->FillTH1(NCut, "TrigNBs_Denominator",   tr->getVar<int>(Label["cntCSVS"]));
  if (tr->getVar<double>(Label["HT"]) > 1000)
    his->FillTH1(NCut, "TrigMET_HTMore1000_Denominator", tr->getVar<double>(METLabel));
  else
    his->FillTH1(NCut, "TrigMET_HTLess1000_Denominator", tr->getVar<double>(METLabel));

  std::vector<std::string> HTHLT;
  HTHLT.push_back("HLT_PFHT125_v\\d");
  HTHLT.push_back("HLT_PFHT200_v\\d");
  HTHLT.push_back("HLT_PFHT250_v\\d");
  HTHLT.push_back("HLT_PFHT300_PFMET100_v\\d");
  HTHLT.push_back("HLT_PFHT300_v\\d");
  HTHLT.push_back("HLT_PFHT350_v\\d");
  HTHLT.push_back("HLT_PFHT400_v\\d");
  HTHLT.push_back("HLT_PFHT475_v\\d");
  HTHLT.push_back("HLT_PFHT600_v\\d");
  HTHLT.push_back("HLT_PFHT650_v\\d");
  HTHLT.push_back("HLT_PFHT800_v\\d");
  HTHLT.push_back("HLT_PFHT900_v\\d");
  HTHLT.push_back("HLT_PFJet450_v\\d");
  HTHLT.push_back("HLT_CaloJet500_NoJetID_v\\d");

  if (PassTrigger(HTHLT))
  {
    his->FillTH1(NCut, "TrigMET_Numerator", tr->getVar<double>(METLabel));
    if (tr->getVar<double>(Label["HT"]) > 1000)
      his->FillTH1(NCut, "TrigMET_HTMore1000_Numerator", tr->getVar<double>(METLabel));
    else
      his->FillTH1(NCut, "TrigMET_HTLess1000_Numerator", tr->getVar<double>(METLabel));

    his->FillTH1(NCut, "TrigNJets_Numerator", tr->getVar<int>(Label["cntNJetsPt30"]));
    his->FillTH1(NCut, "TrigNBs_Numerator",   tr->getVar<int>(Label["cntCSVS"]));
  }

  return true;
}       // -----  end of function TriggerAna::FillHTEff  -----
