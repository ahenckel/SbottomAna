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
  his->AddTH1C("TrigMET_Denominator" , "TrigMET_Denominator" , "MET [GeV]" , "Denominator" , 200, 0, 800);
  his->AddTH1C("TrigMET_Numerator"   , "TrigMET_Numerator"   , "MET [GeV]" , "Numerator"   , 200, 0, 800);
  his->AddTH1C("TrigHT_Denominator" , "TrigHT_Denominator" , "HT [GeV]" , "Denominator" , 200, 0, 800);
  his->AddTH1C("TrigHT_Numerator"   , "TrigHT_Numerator"   , "HT [GeV]" , "Numerator"   , 200, 0, 800);
  return true;
}       // -----  end of function TriggerAna::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  TriggerAna::InitCutOrder
//  Description:  
// ===========================================================================
bool TriggerAna::InitCutOrder(std::string ana)
{
  //Clear the objects
  CutOrder.clear();
  CutMap.clear();

  if (AnaName.find("Ele") != std::string::npos)
  {
    // Use this trigger as denominator
    HLTstr.push_back("HLT_Ele27_eta2p1_WPLoose_Gsf_v\\d");

    //Add name and order of the cutflow
    //Default is the singleEle trigger pass
    CutOrder.push_back("NoCut");
    CutOrder.push_back("Filter");
    CutOrder.push_back("EleTrig");
    CutOrder.push_back("NEle");
    CutOrder.push_back("nJets");
    CutOrder.push_back("BJets");
    CutOrder.push_back("MET");
    CutOrder.push_back("HT");

    //Set the cutbit of each cut
    CutMap["NoCut"]   = "00000000000000000";
    CutMap["Filter"]  = "00000000000000001";
    CutMap["EleTrig"] = "00000000000000011";
    CutMap["NEle"]    = "00000000000000111";
    CutMap["nJets"]   = "00000000000001111";
    CutMap["BJets"]   = "00000000000011111";
    CutMap["MET"]     = "00000000000111111";
    CutMap["HT"]      = "00000000001011111";
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

  if (AnaName.find("Ele") != std::string::npos)
  {
    cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));
    cutbit.set(1 , PassTrigger());
    cutbit.set(2 , tr->getVar<int>(Label["nElectrons_Base"]) >= 1);
    cutbit.set(3 , tr->getVar<bool>(Label["passnJets"]));
    cutbit.set(4 , tr->getVar<bool>(Label["passBJets"]));
    cutbit.set(5 , tr->getVar<bool>(Label["passMET"]));
    cutbit.set(6 , tr->getVar<bool>(Label["passHT"]));
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
  CheckCut();
  ComAna::RunEvent();
  bool passcuts = false;

  for (unsigned int i = 0; i < CutOrder.size(); ++i)
  {
    std::bitset<NBITS> locbit(CutMap[CutOrder.at(i)]);
    if ( (cutbit & locbit) != locbit) continue;

    his->FillTH1("CutFlow", int(i)); 
    FillMETEff(i);
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
  his->FillTH1(NCut, "TrigMET_Denominator", tr->getVar<double>(METLabel));
  his->FillTH1(NCut, "TrigHT_Denominator", tr->getVar<double>(Label["HT"]));

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
    his->FillTH1(NCut, "TrigHT_Numerator", tr->getVar<double>(Label["HT"]));
  }

  return true;
}       // -----  end of function TriggerAna::FillMETEff  -----
