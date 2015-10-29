// ===========================================================================
// 
//       Filename:  TTZ3Lep.cc
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


#include "TTZ3Lep.h"

//----------------------------------------------------------------------------
//       Class:  TTZ3Lep
//      Method:  TTZ3Lep
// Description:  constructor
//----------------------------------------------------------------------------
TTZ3Lep::TTZ3Lep (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile, std::string spec_)
: ComAna(name, tr_, OutFile)
{
  InitCutOrder(name);
  jetVecLabel = "prodJetsNoMu_jetsLVec";
  CSVVecLabel = "recoJetsBtag_0_MuCleaned";
}  // -----  end of method TTZ3Lep::TTZ3Lep  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  TTZ3Lep
//      Method:  TTZ3Lep
// Description:  copy constructor
//----------------------------------------------------------------------------
TTZ3Lep::TTZ3Lep ( const TTZ3Lep &other ): ComAna(other)
{
}  // -----  end of method TTZ3Lep::TTZ3Lep  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  TTZ3Lep
//      Method:  ~TTZ3Lep
// Description:  destructor
//----------------------------------------------------------------------------
TTZ3Lep::~TTZ3Lep ()
{
}  // -----  end of method TTZ3Lep::-TTZ3Lep  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  TTZ3Lep
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  TTZ3Lep&
TTZ3Lep::operator = ( const TTZ3Lep &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method TTZ3Lep::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  TTZ3Lep::BookHistograms
//  Description:  
// ===========================================================================
bool TTZ3Lep::BookHistograms()
{
  ComAna::BookHistograms();
  return true;
}       // -----  end of function TTZ3Lep::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZ3Lep::InitCutOrder
//  Description:  
// ===========================================================================
bool TTZ3Lep::InitCutOrder(std::string ana)
{
  //Clear the objects
  CutOrder.clear();
  CutMap.clear();

  //Add name and order of the cutflow
  CutOrder.push_back("NoCut");
  CutOrder.push_back("Filter");
  CutOrder.push_back("HasZ");
  CutOrder.push_back("nJets");
  CutOrder.push_back("BJets");
  CutOrder.push_back("Tagger");
  CutOrder.push_back("MET70");

  //Set the cutbit of each cut
  CutMap["NoCut"]  = "00000000000000000";
  CutMap["Filter"] = "00000000000000001";
  CutMap["HasZ"]   = "00000000000000011";
  CutMap["nJets"]  = "00000000000000111";
  CutMap["BJets"]  = "00000000000001111";
  CutMap["Tagger"] = "00000000000011111";
  CutMap["MET70"]  = "00000000000111111";

  assert(CutOrder.size() == CutMap.size());

  his->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));
  return true;
}       // -----  end of function TTZ3Lep::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZ3Lep::CheckCut
//  Description:  
// ===========================================================================
bool TTZ3Lep::CheckCut()
{
  cutbit.set(0 , tr->getVar<bool>("passNoiseEventFilterTTZ"));
  cutbit.set(1 , tr->getVar<bool>("passMuZinvSel"));
  cutbit.set(2 , tr->getVec<TLorentzVector>("jetsLVec_forTaggerTTZ").size() >= 4);
  cutbit.set(3 , tr->getVar<int>("cntCSVSTTZ") >= 1);
  cutbit.set(4 , tr->getVar<int>("nTopCandSortedCntTTZ") == 2);
  cutbit.set(5 , tr->getVar<double>(METLabel) < 70);


  return true;
}       // -----  end of function TTZ3Lep::CheckCut  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZ3Lep::FillCut
//  Description:  
// ===========================================================================
bool TTZ3Lep::FillCut()
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
}       // -----  end of function TTZ3Lep::FillCut  -----
