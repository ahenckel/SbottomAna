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

// Have to include searchBins.h here, to avoid duplicate definition in main...
#include "SusyAnaTools/Tools/searchBins.h"
//----------------------------------------------------------------------------
//       Class:  StopAna
//      Method:  StopAna
// Description:  constructor
//----------------------------------------------------------------------------
StopAna::StopAna (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile)
: ComAna(name, tr_, OutFile)
{
  InitCutOrder(name);
  BookHistograms();
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
  his->AddTH1C("SearchBins", "Search Bins", 50, 0, 50);
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
  CutOrder.push_back("MuonVeto");
  CutOrder.push_back("EleVeto");
  CutOrder.push_back("IskVeto");
  CutOrder.push_back("nJets");
  CutOrder.push_back("dPhis");
  CutOrder.push_back("BJets");
  CutOrder.push_back("MET");
  CutOrder.push_back("MT2");
  CutOrder.push_back("HT");
  CutOrder.push_back("Tagger");
  CutOrder.push_back("Filter");

  //Set the cutbit of each cut
  CutMap["NoCut"]    = "00000000000000000";
  CutMap["MuonVeto"] = "00000000000000001";
  CutMap["EleVeto"]  = "00000000000000011";
  CutMap["IskVeto"]  = "00000000000000111";
  CutMap["nJets"]    = "00000000000001111";
  CutMap["dPhis"]    = "00000000000011111";
  CutMap["BJets"]    = "00000000000111111";
  CutMap["MET"]      = "00000000001111111";
  CutMap["MT2"]      = "00000000011111111";
  CutMap["HT"]       = "00000000111111111";
  CutMap["Tagger"]   = "00000001111111111";
  CutMap["Filter"]   = "00000011111111111";

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

  cutbit.set(0 , tr->getVar<bool>("passMuonVeto"));
  cutbit.set(1 , tr->getVar<bool>("passEleVeto"));
  cutbit.set(2 , tr->getVar<bool>("passIsoTrkVeto"));
  cutbit.set(3 , tr->getVar<bool>("passnJets"));
  cutbit.set(4 , tr->getVar<bool>("passdPhis"));
  cutbit.set(5 , tr->getVar<bool>("passBJets"));
  cutbit.set(6 , tr->getVar<bool>("passMET"));
  cutbit.set(7 , tr->getVar<bool>("passMT2"));
  cutbit.set(8 , tr->getVar<bool>("passHT"));
  cutbit.set(9 , tr->getVar<bool>("passTagger"));
  cutbit.set(10, tr->getVar<bool>("passNoiseEventFilter"));

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
      assert(tr->getVar<bool>("passBaseline"));
      his->FillTH1("NBase", 1);
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
  int searchbin_id = find_Binning_Index( tr->getVar<int>("cntCSVS"), tr->getVar<int>("nTopCandSortedCnt"), 
      tr->getVar<double>("best_had_brJet_MT2"), tr->getVar<double>("met"));
  if( searchbin_id >= 0 )
  {
    his->FillTH1(NCut, "SearchBins", searchbin_id);
  }
  return true;
}       // -----  end of function StopAna::FillSearchBins  -----
