// ===========================================================================
// 
//       Filename:  SBMulti.cc
// 
//    Description:  Analysis for SBottom ISR
//https://indico.cern.ch/event/439574/contribution/0/attachments/1142929/1637677/Sbottom_13Tev.pdf
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

#include "SBMulti.h"

//----------------------------------------------------------------------------
//       Class:  SBMulti
//      Method:  SBMulti
// Description:  constructor
//----------------------------------------------------------------------------
SBMulti::SBMulti (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile)
: ComAna(name, tr_, OutFile)
{
  InitCutOrder(name);
  his->AddTH1("NBase", "Number of Events passed baseline", 2, 0, 2);
}  // -----  end of method SBMulti::SBMulti  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  SBMulti
//      Method:  SBMulti
// Description:  copy constructor
//----------------------------------------------------------------------------
SBMulti::SBMulti ( const SBMulti &other ): ComAna(other)
{
}  // -----  end of method SBMulti::SBMulti  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  SBMulti
//      Method:  ~SBMulti
// Description:  destructor
//----------------------------------------------------------------------------
SBMulti::~SBMulti ()
{
}  // -----  end of method SBMulti::-SBMulti  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  SBMulti
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  SBMulti&
SBMulti::operator = ( const SBMulti &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method SBMulti::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  SBMulti::BookHistograms
//  Description:  
// ===========================================================================
bool SBMulti::BookHistograms()
{
  ComAna::BookHistograms();
  return true;
}       // -----  end of function SBMulti::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  SBMulti::InitCutOrder
//  Description:  
// ===========================================================================
bool SBMulti::InitCutOrder(std::string ana)
{
  //Clear the objects
  CutOrder.clear();
  CutMap.clear();

  //Add name and order of the cutflow
  CutOrder.push_back("NoCut");
  CutOrder.push_back("EleVeto");
  CutOrder.push_back("MuonVeto");
  CutOrder.push_back("IskVeto");
  CutOrder.push_back("MET200");
  CutOrder.push_back("dPhiJMET");
  CutOrder.push_back("TopVeto");

  //CutOrder.push_back("HT250");
  //CutOrder.push_back("PTNonB");
  //CutOrder.push_back("MCT");
  //CutOrder.push_back("TriJets");
  //CutOrder.push_back("DiJet70");
  //CutOrder.push_back("1JetNotB");
  //CutOrder.push_back("23JetB");

  //Set the cutbit of each cut

  CutMap["NoCut"]    = "00000000000000000";
  CutMap["EleVeto"]  = "00000000000000001";
  CutMap["MuonVeto"] = "00000000000000011";
  CutMap["IskVeto"]  = "00000000000000111";
  CutMap["MET200"]   = "00000000000001111";
  CutMap["dPhiJMET"] = "00000000000011111";
  CutMap["TopVeto"]  = "00000000000111111";


  //CutMap["DiJet70"] = "00000000000111111";
  //CutMap["1JetNotB"]  = "00000000001111111";
  //CutMap["HT250"]    = "00000000011111111";
  //CutMap["23JetB"]   = "00000000111111111";
  //CutMap["TriJets"] = "00000001111111111";
  //CutMap["PTNonB"]   = "00000001111111111";
  //CutMap["MCT"]      = "00000001111111111";

  assert(CutOrder.size() == CutMap.size());

  his->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));
  return true;
}       // -----  end of function SBMulti::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  SBMulti::CheckCut
//  Description:  
// ===========================================================================
bool SBMulti::CheckCut()
{
  //const double CVS = 0.814;
  cutbit.reset();

  // Eletron Veto
  cutbit.set(0, tr->getVar<bool>("passEleVeto"));
  
  // Muon Veto
  cutbit.set(1, tr->getVar<bool>("passMuonVeto"));

  // IsoTrack Veto
  cutbit.set(2, tr->getVar<bool>("passIsoTrkVeto"));

  // MET > 200 GeV
  cutbit.set(3, tr->getVar<double>("met") > 200);

  // dPhi 1, 2, 3 for QCD rejection
  cutbit.set(4, tr->getVar<bool>("passdPhis"));

  // Veto Top using Type3 
  cutbit.set(5, ComAna::GetType3TopTagger() == 0);
/*
 *
 *  // Exactly 3 jet with pT > 30GeV and |eta| < 2.5
 *  int jet30count = 0;
 *  for (int i = 0; i < tr->getVec<TLorentzVector> ("jetsLVec").size(); ++i)
 *  {
 *    if (tr->getVec<TLorentzVector> ("jetsLVec").at(i).Pt() > 30) 
 *    {
 *      jet30count++;
 *    }
 *  }
 *  cutbit.set(0, jet30count == 3);
 *
 *  // Two leading jets pT > 70 GeV
 *  bool Dijet70 = false;
 *  if (tr->getVec<TLorentzVector> ("jetsLVec").size() >=2)
 *  {
 *    if (tr->getVec<TLorentzVector> ("jetsLVec").at(0).Pt() > 70 
 *        && tr->getVec<TLorentzVector> ("jetsLVec").at(1).Pt() > 70) 
 *      Dijet70 = true;
 *  }
 *  cutbit.set(1, Dijet70);
 *
 *  // First Jet is not tagged as a b-jet
 *  cutbit.set(2, tr->getVec<TLorentzVector> ("jetsLVec").size() > 0 && tr->getVec<double>("recoJetsBtag_0").at(0) <  CVS );
 *
 *  // Second or third or both jets are b-jet
 *  bool bjets23 = false;
 *  if (tr->getVec<TLorentzVector> ("jetsLVec").size() >=3)
 *  {
 *    if ( tr->getVec<double>("recoJetsBtag_0").at(1)  > CVS || tr->getVec<double>("recoJetsBtag_0").at(2) >  CVS )
 *      bjets23 = true;
 *  }
 *  cutbit.set(3, bjets23);
 *
 *  // HT(scalar sum of jet pt) > 250GeV
 *  cutbit.set(7, tr->getVar<double>("mht") > 250);
 *
 *  
 *
 *  // Pt (non-b jet) > 250GeV
 *  double nonbHT = 0;
 *  for (int i = 0; i < tr->getVec<TLorentzVector> ("jetsLVec").size(); ++i)
 *  {
 *    
 *    if(tr->getVec<double>("recoJetsBtag_0").at(i) > CVS)
 *      nonbHT += tr->getVec<TLorentzVector> ("jetsLVec").at(i).Pt();
 *  }
 *  cutbit.set(10, nonbHT > 250);
 *
 *
 */
  return true;
}       // -----  end of function SBMulti::CheckCut  -----

// ===  FUNCTION  ============================================================
//         Name:  SBMulti::FillCut
//  Description:  
// ===========================================================================
bool SBMulti::FillCut()
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
}       // -----  end of function SBMulti::FillCut  -----
