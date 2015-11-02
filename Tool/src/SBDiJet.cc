// ===========================================================================
// 
//       Filename:  SBDiJet.cc
// 
//    Description:  A cut flow study for Sbottom Dijet
// 
//        Version:  1.0
//        Created:  08/21/2015 17:33:55
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================


#include "SBDiJet.h"

//----------------------------------------------------------------------------
//       Class:  SBDiJet
//      Method:  SBDiJet
// Description:  constructor
//----------------------------------------------------------------------------
SBDiJet::SBDiJet (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile)
: ComAna(name, tr_, OutFile)
{
  InitCutOrder(name);
  his->AddTH1("NBase", "Number of Events passed baseline", 2, 0, 2);
}  // -----  end of method SBDiJet::SBDiJet  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  SBDiJet
//      Method:  SBDiJet
// Description:  copy constructor
//----------------------------------------------------------------------------
SBDiJet::SBDiJet ( const SBDiJet &other ) : ComAna(other)
{
}  // -----  end of method SBDiJet::SBDiJet  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  SBDiJet
//      Method:  ~SBDiJet
// Description:  destructor
//----------------------------------------------------------------------------
SBDiJet::~SBDiJet ()
{
}  // -----  end of method SBDiJet::-SBDiJet  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  SBDiJet
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  SBDiJet&
SBDiJet::operator = ( const SBDiJet &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method SBDiJet::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  SBDiJet::BookHistograms
//  Description:  
// ===========================================================================
bool SBDiJet::BookHistograms()
{
  ComAna::BookHistograms();
  his->AddTH1C("MTJ2MET", "MT of Jet2 and MET", 500, 0, 1000);
  return true;
}       // -----  end of function SBDiJet::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  SBDiJet::InitCutOrder
//  Description:  
// ===========================================================================
bool SBDiJet::InitCutOrder(std::string ana)
{
  //Clear the objects
  CutOrder.clear();
  CutMap.clear();

  //Add name and order of the cutflow
  CutOrder.push_back("NoCut");
  CutOrder.push_back("DiJet70");
  CutOrder.push_back("DiJetB");
  CutOrder.push_back("3JetVeto");
  CutOrder.push_back("EleVeto");
  CutOrder.push_back("MuonVeto");
  CutOrder.push_back("IskVeto");
  CutOrder.push_back("MHT250");
  CutOrder.push_back("MET175");
  CutOrder.push_back("dPhi25");
  CutOrder.push_back("MTJ2MET");
  CutOrder.push_back("TopVeto");

  //Set the cutbit of each cut
  CutMap["NoCut"]    = "00000000000000000";
  CutMap["DiJet70"]  = "00000000000000001";
  CutMap["DiJetB"]   = "00000000000000011";
  CutMap["3JetVeto"] = "00000000000000111";
  CutMap["EleVeto"]  = "00000000000001111";
  CutMap["MuonVeto"] = "00000000000011111";
  CutMap["IskVeto"]  = "00000000000111111";
  CutMap["MHT250"]   = "00000000001111111";
  CutMap["MET175"]   = "00000000011111111";
  CutMap["dPhi25"]   = "00000000111111111";
  CutMap["MTJ2MET"]  = "00000001111111111";
  CutMap["TopVeto"]  = "00000011111111111";

  assert(CutOrder.size() == CutMap.size());

  his->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));
  return true;
}       // -----  end of function SBDiJet::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  SBDiJet::CheckCut
//  Description:  
// ===========================================================================
bool SBDiJet::CheckCut()
{
  cutbit.reset();
  // two jets with pT > 70 GeV (Trigger requirement)
  int jet70count = 0;
  double diJetHT = 0;
  std::vector<int> jet70Idx;
  
  for (unsigned int i = 0; i < tr->getVec<TLorentzVector> (jetVecLabel).size(); ++i)
  {
    if (tr->getVec<TLorentzVector> (jetVecLabel).at(i).Pt() > 70 && fabs(tr->getVec<TLorentzVector> (jetVecLabel).at(i).Eta()) < 2.4)
    {
      jet70count++;
      diJetHT +=  tr->getVec<TLorentzVector> (jetVecLabel).at(i).Pt();
      jet70Idx.push_back(i);
    }
  }
  cutbit.set(0, jet70count >= 2);

  // One or both leading jets tagged as a b-jet
  int bjet70count = 0;
  for (unsigned int i = 0; i < jet70Idx.size(); ++i)
  {
    if (tr->getVec<double>(CSVVecLabel).at(jet70Idx.at(i)) > 0.814)
      bjet70count++;
  }
  cutbit.set(1, bjet70count >= 1);

  // Veto event with if a third leading jet with pT > 50GeV, |eta| < 5.0
  bool jet3Veto = false;
  if (tr->getVec<TLorentzVector> (jetVecLabel).size() > 2)
  {
    TLorentzVector jet3 = tr->getVec<TLorentzVector> (jetVecLabel).at(2);
    //if (jet3.Pt() > 50 && fabs(jet3.Eta()) < 5) // 8TeV
    if (jet3.Pt() > 70 && fabs(jet3.Eta()) < 5) // 13TeV
    {
      jet3Veto = true;
    }
  }
  cutbit.set(2, !jet3Veto);

  // Eletron Veto
  cutbit.set(3, tr->getVar<bool>(Label["passEleVeto"]));
  
  // Muon Veto
  cutbit.set(4, tr->getVar<bool>(Label["passMuonVeto"]));

  // IsoTrack Veto
  cutbit.set(5, tr->getVar<bool>(Label["passIsoTrkVeto"]));

  // HT(scalar sum of jet pt) > 250GeV
  cutbit.set(6, diJetHT > 250);

  // MET > 175 GeV
  cutbit.set(7, tr->getVar<double>(METLabel) > 175);

  // Delta Phi (b1, b2) < 2.5
  TLorentzVector Jet1(0, 0, 0, 0);
  TLorentzVector Jet2(0, 0, 0, 0);

  if (tr->getVec<TLorentzVector> (jetVecLabel).size() > 0 ) 
    Jet1 = tr->getVec<TLorentzVector> (jetVecLabel).at(0);
  if (tr->getVec<TLorentzVector> (jetVecLabel).size() > 1 ) 
    Jet2 = tr->getVec<TLorentzVector> (jetVecLabel).at(1);
  cutbit.set(8, tr->getVec<TLorentzVector> (jetVecLabel).size() > 1 && fabs(Jet1.DeltaPhi(Jet2))<2.5);

  //MT(J2, MET) > 200GeV
  MTJ2MET = 0;
  TLorentzVector METLV(0, 0, 0, 0);
  METLV.SetPtEtaPhiE(tr->getVar<double>(METLabel), 0, tr->getVar<double>(METPhiLabel), 0);
  MTJ2MET = CalMT(Jet2, METLV);
  double MTJ1MET = CalMT(Jet1, METLV); // 13TeV
  MTJ2MET = MTJ2MET < MTJ1MET ? MTJ2MET : MTJ1MET;
  cutbit.set(9, tr->getVec<TLorentzVector> (jetVecLabel).size() > 1 && MTJ2MET > 200);

  cutbit.set(10, tr->getVar<int>(Label["nTopCandSortedCnt"]));
  //----------------------------------------------------------------------------
  //  Always fill in the event cutbits information
  //----------------------------------------------------------------------------
  std::vector<bool> Vbits;
  for (std::size_t i = 0; i < cutbit.size(); ++i)
    Vbits.push_back(cutbit.test(i));
  his->FillCutBits(Vbits);

  return true;
}       // -----  end of function SBDiJet::CheckCut  -----

// ===  FUNCTION  ============================================================
//         Name:  SBDiJet::FillCut
//  Description:  
// ===========================================================================
bool SBDiJet::FillCut()
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
    if (i+1 == CutOrder.size()) 
    {
      his->FillTH1("NBase", 1);
      passcuts = true;
    }
    his->FillTH1("CutFlow", int(i)); 
    ComAna::FillCut(i);
    FillCut(i);

    // Filling by functions
    //FillJets(i);
    //FillBJet(i);
    //FillMet(i);
  }

  return passcuts;
}       // -----  end of function SBDiJet::FillCut  -----


// ===  FUNCTION  ============================================================
//         Name:  SBDiJet::FillCut
//  Description:  
// ===========================================================================
bool SBDiJet::FillCut(int NCut)
{
  his->FillTH1(NCut, "MTJ2MET", MTJ2MET);
  return true;
}       // -----  end of function SBDiJet::FillCut  -----
