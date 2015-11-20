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
  if (spec.find("Zinv") != std::string::npos)
  {

    jetVecLabel = "jetsLVecLepCleaned";
    CSVVecLabel = "recoJetsBtag_0_LepCleaned";
    METLabel = "cleanMetPt" + spec_;
    METPhiLabel = "cleanMetPhi" + spec_;
    if (spec.find("M") != std::string::npos)
    {
      HLTstr.push_back("HLT_Mu45_eta2p1_v2");
      MCTrigstr.push_back(Label["PassDiMuonTrigger"]);
    }
    if (spec.find("E") != std::string::npos)
    {
      HLTstr.push_back("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v2");
      MCTrigstr.push_back(Label["PassDiEleTrigger"]);
    }
  }
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
  his->AddTH1C("SearchBins", "Search Bins", 50, 0, 50);
  his->AddTH1C("JBT", "JBT", "JBT", "Events", 400, 0, 400);
  his->AddTH1C("ZMET",    "MET",    "#slash{E}_{T} [GeV]",      "Events" , 200, 0,  800);
  his->AddTH1C("ZMETFake",    "METFake",    "#slash{E}_{T} [GeV]",      "Events" , 200, 0,  800);
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
  CutOrder.push_back("Trigger");
  CutOrder.push_back("HasZ");
  CutOrder.push_back("2Leps");
  CutOrder.push_back("nJets");
  CutOrder.push_back("dPhis");
  CutOrder.push_back("BJets");
  CutOrder.push_back("MET");
  CutOrder.push_back("MT2");
  CutOrder.push_back("HT");
  CutOrder.push_back("Tagger");
  CutOrder.push_back("Filter");

  //Set the cutbit of each cut
  CutMap["NoCut"]   = "00000000000000000";
  CutMap["Trigger"] = "00000000000000001";
  CutMap["HasZ"]    = "00000000000000011";
  CutMap["2Leps"]   = "00000000000000111";
  CutMap["nJets"]   = "00000000000001111";
  CutMap["dPhis"]   = "00000000000011111";
  CutMap["BJets"]   = "00000000000111111";
  CutMap["MET"]     = "00000000001111111";
  CutMap["MT2"]     = "00000000011111111";
  CutMap["HT"]      = "00000000111111111";
  CutMap["Tagger"]  = "00000001111111111";
  CutMap["Filter"]  = "00000011111111111";

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
  cutbit.set(0 , PassTrigger());

  cutbit.set(1 , tr->getVec<TLorentzVector>(Label["recoZVec"]).size() == 1);

  cutbit.set(2 ,  tr->getVar<int>(Label["nMuons_Base"]) + tr->getVar<int>(Label["nElectrons_Base"]) == 2 );

  cutbit.set(3 , tr->getVar<bool>(Label["passnJets"]));
  cutbit.set(4 , tr->getVar<bool>(Label["passdPhis"]));
  cutbit.set(5 , tr->getVar<bool>(Label["passBJets"]));
  cutbit.set(6 , tr->getVar<bool>(Label["passMET"]));
  cutbit.set(7 , tr->getVar<bool>(Label["passMT2"]));
  cutbit.set(8 , tr->getVar<bool>(Label["passHT"]));
  cutbit.set(9 , tr->getVar<bool>(Label["passTagger"]));
  cutbit.set(10, tr->getVar<bool>(Label["passNoiseEventFilter"]));

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
    FillSearchBins(i);

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
//  Description:  
// ===========================================================================
bool STZinv::FillZMET(int NCut) const
{
  if (tr->getVec<TLorentzVector>("recoZVecZinv").size() > 0)
  {
    his->FillTH1(NCut, "ZMET", tr->getVar<double>("met"));
    his->FillTH1(NCut, "ZMETFake", tr->getVar<double>(METLabel));
  }
  return true;
}       // -----  end of function STZinv::FillZMET  -----

// ===  FUNCTION  ============================================================
//         Name:  STZinv::PassTrigger
//  Description:  
// ===========================================================================
bool STZinv::PassTrigger()
{
  if (isData)
  {
    std::map<std::string, unsigned int> HLTIdx;
    const std::vector<std::string> &triggerName = tr->getVec<std::string>("TriggerNames");
    const std::vector<int> &triggerPass = tr->getVec<int>("PassTrigger");
    assert(triggerName.size() == triggerPass.size());
    // Get trigger index
    for(auto &hlt : HLTstr)
    {
      auto found = std::find(triggerName.begin(), triggerName.end(), hlt);
      HLTIdx[hlt] = found - triggerName.begin();
    }
    
    bool pass=false;
    for(auto &hlt : HLTIdx)
    {
      //std::cout << pass << " " << hlt.first<< " " << triggerPass.at(hlt.second)<< std::endl;
      pass = pass || triggerPass.at(hlt.second);
    }
    return pass;
  } else{
    bool pass=false;
    for(auto &mctrig : MCTrigstr)
    {
      pass = pass || tr->getVar<bool>(mctrig);

    }
    return pass;
  }

  return false;
}       // -----  end of function STZinv::PassTrigger  -----
