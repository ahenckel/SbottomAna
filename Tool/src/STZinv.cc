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
  his->AddTH1C("JBT", "JBT", "JBT", "Events", 400, 0, 400);
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
  CutOrder.push_back("HasZ");
  CutOrder.push_back("2Leps");
  CutOrder.push_back("nJets");
  CutOrder.push_back("BJets");
  CutOrder.push_back("Tagger");
  CutOrder.push_back("MET70");
  CutOrder.push_back("BinTop");

  //Set the cutbit of each cut
  CutMap["NoCut"]   = "00000000000000000";
  CutMap["Filter"]  = "00000000000000001";
  CutMap["Trigger"] = "00000000100000001";
  CutMap["HasZ"]    = "00000000100000011";
  CutMap["2Leps"]   = "00000000100000111";
  CutMap["nJets"]   = "00000000100001111";
  CutMap["BJets"]   = "00000000100011111";
  CutMap["Tagger"]  = "00000000100111111";
  CutMap["MET70"]   = "00000000101111111";
  CutMap["BinTop"]  = "00000000111111111";

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
  
  cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));

  cutbit.set(1 , tr->getVec<TLorentzVector>(Label["recoZVec"]).size() == 1);

  cutbit.set(2 ,  tr->getVar<int>("nMuons_Base") + tr->getVar<int>("nElectrons_Base") == 2 );

  cutbit.set(3 , tr->getVec<TLorentzVector>(Label["jetsLVec_forTagger"]).size() >= 4);

  cutbit.set(4 , tr->getVar<int>(Label["cntCSVS"]) >= 1);
  cutbit.set(5 , tr->getVar<int>(Label["nTopCandSortedCnt"]) == 2);
  cutbit.set(6 , tr->getVar<double>(METLabel) < 70);

  cutbit.set(8 , tr->getVar<bool>(Label["PassDiMuonTrigger"]) || tr->getVar<bool>(Label["PassDiEleTrigger"]));
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

    if (i+1 == CutOrder.size()) 
    {
      passcuts = true;
    }
  }

  return passcuts;
}       // -----  end of function STZinv::FillCut  -----
