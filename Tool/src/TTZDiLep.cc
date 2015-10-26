// ===========================================================================
// 
//       Filename:  TTZDiLep.cc
// 
//    Description:  An analysis for TTZ background in the 3 lepton channel
// 
//        Version:  1.0
//        Created:  10/07/2015 11:02:23
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================


#include "TTZDiLep.h"

//----------------------------------------------------------------------------
//       Class:  TTZDiLep
//      Method:  TTZDiLep
// Description:  constructor
//----------------------------------------------------------------------------
TTZDiLep::TTZDiLep (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile)
: ComAna(name, tr_, OutFile)
{
  InitCutOrder(name);
  his->AddTH1("NBase", "Number of Events passed baseline", 2, 0, 2);
}  // -----  end of method TTZDiLep::TTZDiLep  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  TTZDiLep
//      Method:  TTZDiLep
// Description:  copy constructor
//----------------------------------------------------------------------------
TTZDiLep::TTZDiLep ( const TTZDiLep &other ): ComAna(other)
{
}  // -----  end of method TTZDiLep::TTZDiLep  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  TTZDiLep
//      Method:  ~TTZDiLep
// Description:  destructor
//----------------------------------------------------------------------------
TTZDiLep::~TTZDiLep ()
{
}  // -----  end of method TTZDiLep::-TTZDiLep  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  TTZDiLep
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  TTZDiLep&
TTZDiLep::operator = ( const TTZDiLep &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method TTZDiLep::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  TTZDiLep::BookHistograms
//  Description:  
// ===========================================================================
bool TTZDiLep::BookHistograms()
{
  ComAna::BookHistograms();
  return true;
}       // -----  end of function TTZDiLep::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZDiLep::InitCutOrder
//  Description:  
// ===========================================================================
bool TTZDiLep::InitCutOrder(std::string ana)
{
  //Clear the objects
  CutOrder.clear();
  CutMap.clear();

  //Add name and order of the cutflow
  CutOrder.push_back("NoCut");
  CutOrder.push_back("nJets");
  CutOrder.push_back("dPhis");
  CutOrder.push_back("BJets");
  CutOrder.push_back("Tagger");
  CutOrder.push_back("Filter");
  CutOrder.push_back("HasZ");

  //Set the cutbit of each cut
  CutMap["NoCut"]  = "00000000000000000";
  CutMap["nJets"]  = "00000000000000001";
  CutMap["dPhis"]  = "00000000000000011";
  CutMap["BJets"]  = "00000000000000111";
  CutMap["Tagger"] = "00000000000001111";
  CutMap["Filter"] = "00000000000011111";
  CutMap["HasZ"]   = "00000000000111111";

  assert(CutOrder.size() == CutMap.size());

  his->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));
  return true;
}       // -----  end of function TTZDiLep::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZDiLep::CheckCut
//  Description:  
// ===========================================================================
bool TTZDiLep::CheckCut()
{
  
  cutbit.set(0 , tr->getVar<bool>("passnJets"));
  cutbit.set(1 , tr->getVar<bool>("passdPhis"));
  cutbit.set(2 , tr->getVar<int>("cntCSVS") == 2);
  cutbit.set(3 , tr->getVar<int>("nTopCandSortedCnt") == 2);
  cutbit.set(4 , tr->getVar<bool>("passNoiseEventFilter"));
  cutbit.set(5 , tr->getVar<bool>("passMuZinvSel"));

  return true;
}       // -----  end of function TTZDiLep::CheckCut  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZDiLep::FillCut
//  Description:  
// ===========================================================================
bool TTZDiLep::FillCut()
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
      his->FillTH1("NBase", 1);
      passcuts = true;
    }
  }

  return passcuts;
}       // -----  end of function TTZDiLep::FillCut  -----


