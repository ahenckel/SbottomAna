// ===========================================================================
// 
//       Filename:  STISR.cc
// 
//    Description:  Based on http://arxiv.org/abs/1506.00653
// 
//        Version:  1.0
//        Created:  09/20/2015 20:25:06
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

#include "STISR.h"

//----------------------------------------------------------------------------
//       Class:  STISR
//      Method:  STISR
// Description:  constructor
//----------------------------------------------------------------------------
STISR::STISR (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile)
: ComAna(name, tr_, OutFile)
{
  InitCutOrder(name);
  BookHistograms();
  his->AddTH1("NBase", "Number of Events passed baseline", 2, 0, 2);
}  // -----  end of method STISR::STISR  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  STISR
//      Method:  STISR
// Description:  copy constructor
//----------------------------------------------------------------------------
STISR::STISR ( const STISR &other ): ComAna(other)
{
}  // -----  end of method STISR::STISR  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  STISR
//      Method:  ~STISR
// Description:  destructor
//----------------------------------------------------------------------------
STISR::~STISR ()
{
}  // -----  end of method STISR::-STISR  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  STISR
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  STISR&
STISR::operator = ( const STISR &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method STISR::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  STISR::BookHistograms
//  Description:  
// ===========================================================================
bool STISR::BookHistograms()
{
  ComAna::BookHistograms();
  return true;
}       // -----  end of function STISR::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  STISR::InitCutOrder
//  Description:  
// ===========================================================================
bool STISR::InitCutOrder(std::string ana)
{
  //Clear the objects
  CutOrder.clear();
  CutMap.clear();

  //Add name and order of the cutflow
  CutOrder.push_back("NoCut");
  CutOrder.push_back("EleVeto");
  CutOrder.push_back("MuonVeto");
  CutOrder.push_back("IskVeto");
  CutOrder.push_back("LJetPT700");
  CutOrder.push_back("LJetMass");
  CutOrder.push_back("SubJet60");
  CutOrder.push_back("SubJetB");
  CutOrder.push_back("dPhiLJMET");
  CutOrder.push_back("dPhiSubJMET");
  CutOrder.push_back("MET200");

  //Set the cutbit of each cut
  CutMap["NoCut"]       = "00000000000000000";
  CutMap["EleVeto"]     = "00000000000000001";
  CutMap["MuonVeto"]    = "00000000000000011";
  CutMap["IskVeto"]     = "00000000000000111";
  CutMap["LJetPT700"]   = "00000000000001111";
  CutMap["LJetMass"]    = "00000000000011111";
  CutMap["SubJet60"]    = "00000000000111111";
  CutMap["SubJetB"]     = "00000000001111111";
  CutMap["dPhiLJMET"]   = "00000000011111111";
  CutMap["dPhiSubJMET"] = "00000000111111111";
  CutMap["MET200"]      = "00000001111111111";

  assert(CutOrder.size() == CutMap.size());

  his->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));
  return true;
}       // -----  end of function STISR::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  STISR::CheckCut
//  Description:  
// ===========================================================================
bool STISR::CheckCut()
{
  const double CVS = 0.814;
  cutbit.reset();
  if( tr->getVec<TLorentzVector> ("jetsLVec").size() == 0 ) 
  {
    cutbit.set();
    cutbit.flip();
    return false;
  }

  // Eletron Veto
  cutbit.set(0, tr->getVar<bool>("passEleVeto"));
  
  // Muon Veto
  cutbit.set(1, tr->getVar<bool>("passMuonVeto"));

  // IsoTrack Veto
  cutbit.set(2, tr->getVar<bool>("passIsoTrkVeto"));


  // Leading jet Pt > 700GeV!
  TLorentzVector J1(0, 0, 0, 0);
  if( tr->getVec<TLorentzVector> ("jetsLVec").size() > 0 )
    J1 = tr->getVec<TLorentzVector> ("jetsLVec").at(0);

  cutbit.set(3, J1.Pt() > 700);

  // Leading Jet mass > Mtop
  cutbit.set(4, J1.M() > 172.5);

  // Subjet counting
  std::vector<int> SubJetIdx;
  int SubJetcount=0;
  int SubJetBcount=0;
  for (int i = 1; i < tr->getVec<TLorentzVector> ("jetsLVec").size(); ++i)
  {
    if (tr->getVec<TLorentzVector> ("jetsLVec").at(i).Pt() > 60) 
    {
      SubJetcount ++;
      SubJetIdx.push_back(i);
      if (tr->getVec<double>("recoJetsBtag_0").at(i) > CVS)
        SubJetBcount++;
    }
  }
  // At least three sub-leading jets
  cutbit.set(5, SubJetcount >= 3);

  // At least one of the subjets is b-tagged
  cutbit.set(6, SubJetBcount >= 1);

  // delta Phi 
  TLorentzVector METLV(0, 0, 0, 0);
  METLV.SetPtEtaPhiE(tr->getVar<double>("met"), 0, tr->getVar<double>("metphi"), 0);

  // | phi_j0 - phi_MET - pi | < 0.15
  cutbit.set(7, fabs(J1.DeltaPhi(METLV)) < 0.15);
  
  // | phi_sub - phi_MET | > 0.2
  bool dPhiSub = true;
  for (int i = 0; i < SubJetIdx.size(); ++i)
  {
    dPhiSub = dPhiSub &&  fabs(tr->getVec<TLorentzVector> ("jetsLVec").at(SubJetIdx.at(i)).DeltaPhi(METLV)) > 0.2;
  }
  cutbit.set(8, dPhiSub);
  
  // MET > 200 GeV
  cutbit.set(9, tr->getVar<double>("met") > 200);


  return true;
}       // -----  end of function STISR::CheckCut  -----

// ===  FUNCTION  ============================================================
//         Name:  STISR::FillCut
//  Description:  
// ===========================================================================
bool STISR::FillCut()
{

//----------------------------------------------------------------------------
//  Check cut and fill cut-based plots
//----------------------------------------------------------------------------
  CheckCut();
  ComAna::RunEvent();

  for (unsigned int i = 0; i < CutOrder.size(); ++i)
  {
    std::bitset<NBITS> locbit(CutMap[CutOrder.at(i)]);
    if ( (cutbit & locbit) != locbit) continue;

    his->FillTH1("CutFlow", int(i)); 
    ComAna::FillCut(i);

    if (i+1 == CutOrder.size()) his->FillTH1("NBase", 1);
    // Filling by functions
    //FillJets(i);
    //FillBJet(i);
    //FillMet(i);
  }

  return true;
}       // -----  end of function STISR::FillCut  -----
