// ===========================================================================
// 
//       Filename:  ComAna.cc
// 
//    Description:  A common analysis class, aiming for cocmparison between
//    different cutflow and optimization
// 
//        Version:  1.0
//        Created:  08/24/2015 14:56:53
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================


#include "ComAna.h"

//----------------------------------------------------------------------------
//       Class:  ComAna
//      Method:  ComAna
// Description:  constructor
//----------------------------------------------------------------------------
ComAna::ComAna (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile):
  tr(tr_)
{
  his = new HistTool(OutFile, "Cut", name);
  //BookHistograms();
}  // -----  end of method ComAna::ComAna  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  ComAna
//      Method:  ComAna
// Description:  copy constructor
//----------------------------------------------------------------------------
ComAna::ComAna ( const ComAna &other )
{
}  // -----  end of method ComAna::ComAna  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  ComAna
//      Method:  ~ComAna
// Description:  destructor
//----------------------------------------------------------------------------
ComAna::~ComAna ()
{
}  // -----  end of method ComAna::-ComAna  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  ComAna
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  ComAna&
ComAna::operator = ( const ComAna &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method ComAna::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  ComAna::BookHistograms
//  Description:  
// ===========================================================================
bool ComAna::BookHistograms()
{
  // Jets
  his->AddTH1C("NJets", "NJets", 10, 0, 10);
  BookTLVHistos("Jet1");
  BookTLVHistos("Jet2");
  BookTLVHistos("Jet3");
  BookTLVHistos("Jet4");
  Book2TLVHistos("J1J2");

  //  MET
  his->AddTH1C("MET", "MET", "MET [GeV]", "Events"     , 200, 0, 800);
  his->AddTH1C("METPhi", "METPhi", "#phi MET [GeV]", "Events"     , 20, -5, 5);
  return true;
}       // -----  end of function ComAna::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  ComAna::InitCutOrder
//  Description:  
// ===========================================================================
bool ComAna::InitCutOrder(std::string ana) const
{
  return true;
}       // -----  end of function ComAna::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  ComAna::FillCut
//  Description:  
// ===========================================================================
bool ComAna::FillCut(int NCut)
{
  // Jet
  his->FillTH1(NCut, "NJets", j30count);
  FillTLVHistos(NCut, "Jet1", Jet1);
  FillTLVHistos(NCut, "Jet2", Jet2);
  FillTLVHistos(NCut, "Jet3", Jet3);
  FillTLVHistos(NCut, "Jet4", Jet4);
  Fill2TLVHistos(NCut, "J1J2", Jet1, Jet2);

  // MET
  his->FillTH1(NCut, "MET", tr->getVar<double>("met") );
  his->FillTH1(NCut, "METPhi", tr->getVar<double>("metphi") );
  return true;
}       // -----  end of function ComAna::FillCut  -----

// ===  FUNCTION  ============================================================
//         Name:  ComAna::WriteHistogram
//  Description:  
// ===========================================================================
bool ComAna::WriteHistogram()
{
  his->WriteTH1();
  his->WriteTPro();
  his->WriteTH2();
  return true;
}       // -----  end of function ComAna::WriteHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  ComAna::DrawHistogram
//  Description:  
// ===========================================================================
bool ComAna::DrawHistogram()
{
  
  return true;
}       // -----  end of function ComAna::DrawHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  ComAna::CheckCut
//  Description:  
// ===========================================================================
bool ComAna::CheckCut()
{
  return true;
}       // -----  end of function ComAna::CheckCut  -----


// ===  FUNCTION  ============================================================
//         Name:  ComAna::FillCut
//  Description:  
// ===========================================================================
bool ComAna::FillCut()
{
  //RunEvent();
  //FillCut(-1);
  return true;
}       // -----  end of function ComAna::FillCut  -----


// ===  FUNCTION  ============================================================
//         Name:  ComAna::RunEvent
//  Description:  
// ===========================================================================
bool ComAna::RunEvent()
{
  j30count = CountJets(30);
  GetLeadingJets();
  

  return true;
}       // -----  end of function ComAna::RunEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  ComAna::CountJets
//  Description:  
// ===========================================================================
int ComAna::CountJets(double jetPt) const
{
  int jcount = 0;
  for (int i = 0; i < tr->getVec<TLorentzVector> ("jetsLVec").size(); ++i)
  {
    if (tr->getVec<TLorentzVector> ("jetsLVec").at(i).Pt() > jetPt) 
      jcount++;
  }
  return jcount;
}       // -----  end of function ComAna::CountJets  -----

// ===  FUNCTION  ============================================================
//         Name:  ComAna::GetLeadingJets
//  Description:  
// ===========================================================================
bool ComAna::GetLeadingJets()
{
  Jet1.SetPtEtaPhiE(0.0, 0.0, 0.0, 0.0);
  Jet2.SetPtEtaPhiE(0.0, 0.0, 0.0, 0.0);
  Jet3.SetPtEtaPhiE(0.0, 0.0, 0.0, 0.0);
  Jet4.SetPtEtaPhiE(0.0, 0.0, 0.0, 0.0);

  if (tr->getVec<TLorentzVector> ("jetsLVec").size() > 0)
    Jet1 = tr->getVec<TLorentzVector> ("jetsLVec").at(0);
  
  if (tr->getVec<TLorentzVector> ("jetsLVec").size() > 1)
    Jet2 = tr->getVec<TLorentzVector> ("jetsLVec").at(1);

  if (tr->getVec<TLorentzVector> ("jetsLVec").size() > 2)
    Jet3 = tr->getVec<TLorentzVector> ("jetsLVec").at(2);

  if (tr->getVec<TLorentzVector> ("jetsLVec").size() > 3)
    Jet4 = tr->getVec<TLorentzVector> ("jetsLVec").at(3);

  return true;
}       // -----  end of function ComAna::GetLeadingJets  -----

// ===  FUNCTION  ============================================================
//         Name:  ComAna::BookTLVHistos
//  Description:  
// ===========================================================================
bool ComAna::BookTLVHistos(std::string name) 
{
  std::stringstream ss, ylabel;
  ss << name <<"PT";
  ylabel <<name<<" " << "Events";
  his->AddTH1C(ss.str(), ss.str(), "p_{T} [GeV]", ylabel.str(),  100, 0, 1000);
  ss.str("");
  ss << name <<"Eta";
  his->AddTH1C(ss.str(), ss.str(), "#eta", ylabel.str(),  20, -5, 5);
  ss.str("");
  ss << name <<"Phi";
  his->AddTH1C(ss.str(), ss.str(), "#phi", ylabel.str(),  20, -5, 5);
  ss.str("");
  ss << name <<"Mass";
  his->AddTH1C(ss.str(), ss.str(), "Mass", ylabel.str(),  50, 0, 50);

  return true;
}       // -----  end of function ComAna::BookTLVHistos  -----



// ===  FUNCTION  ============================================================
//         Name:  ComAna::FillTLVHistos
//  Description:  
// ===========================================================================
bool ComAna::FillTLVHistos(int NCut, std::string name, TLorentzVector TLV) 
{
  std::stringstream ss;
  ss << name <<"PT";
  his->FillTH1(NCut, ss.str(), TLV.Pt());
  ss.str("");
  ss << name <<"Eta";
  his->FillTH1(NCut, ss.str(), TLV.Eta());
  ss.str("");
  ss << name <<"Phi";
  his->FillTH1(NCut, ss.str(), TLV.Phi());
  ss.str("");
  ss << name <<"Mass";
  his->FillTH1(NCut, ss.str(), TLV.M());

  return true;
}       // -----  end of function ComAna::BookTLVHistos  -----

// ===  FUNCTION  ============================================================
//         Name:  ComAna::Book2TLVHistos
//  Description:  
// ===========================================================================
bool ComAna::Book2TLVHistos(std::string name)
{
  std::stringstream ss, ylabel;
  ss << name <<"delPT";
  ylabel <<name<<" " << "Events";
  his->AddTH1C(ss.str(), ss.str(), "p_{T} [GeV]", ylabel.str(),  100, -200, 200);

  ss.str("");
  ss << name <<"dPTRelFrac";
  his->AddTH1C(ss.str(), ss.str(), "Relative Fraction", ylabel.str(),  100, -2, 2);

  ss.str("");
  ss << name <<"dPTObj1Frac";
  his->AddTH1C(ss.str(), ss.str(), "#Delta p_{T} / Obj1 ", ylabel.str(),  100, -2, 2);

  ss.str("");
  ss << name <<"dPTObj1Frac";
  his->AddTH1C(ss.str(), ss.str(), "#Delta p_{T} / Obj2 ", ylabel.str(),  100, -2, 2);

  ss.str("");
  ss << name <<"delEta";
  his->AddTH1C(ss.str(), ss.str(), "#eta", ylabel.str(),  20, -5, 5);
  ss.str("");
  ss << name <<"delPhi";
  his->AddTH1C(ss.str(), ss.str(), "#phi", ylabel.str(),  20, -5, 5);
  ss.str("");
  ss << name <<"delMass";
  his->AddTH1C(ss.str(), ss.str(), "#Delta Mass", ylabel.str(),  50, 0, 50);

  ss.str("");
  ss << name <<"delR";
  his->AddTH1C(ss.str(), ss.str(), "#Delta R", ylabel.str(),  18, 0, 6);

  ss.str("");
  ss << name <<"Mass";
  his->AddTH1C(ss.str(), ss.str(), "Mass", ylabel.str(),  100, 0, 1000);

  ss.str("");
  ss << name <<"MCT";
  his->AddTH1C(ss.str(), ss.str(), "MCT", ylabel.str(),  100, 0, 1000);

  return true;
}       // -----  end of function ComAna::Book2TLVHistos  -----


// ===  FUNCTION  ============================================================
//         Name:  ComAna::Fill2TLVHistos
//  Description:  
// ===========================================================================
bool ComAna::Fill2TLVHistos(int NCut, std::string name, TLorentzVector LV1, TLorentzVector LV2)
{
  std::stringstream ss;
  ss << name <<"delPT";
  his->FillTH1(NCut, ss.str(), LV1.Pt() - LV2.Pt());

  ss.str("");
  ss << name <<"dPTRelFrac";
  his->FillTH1(NCut, ss.str(), (LV1.Pt() - LV2.Pt())/(LV1.Pt() + LV2.Pt()));

  ss.str("");
  ss << name <<"dPTObj1Frac";
  his->FillTH1(NCut, ss.str(), (LV1.Pt() - LV2.Pt())/LV1.Pt());

  ss.str("");
  ss << name <<"dPTObj2Frac";
  his->FillTH1(NCut, ss.str(), (LV1.Pt() - LV2.Pt())/LV2.Pt());

  ss.str("");
  ss << name <<"delEta";
  his->FillTH1(NCut, ss.str(), LV1.Eta() - LV2.Eta());

  ss.str("");
  ss << name <<"delPhi";
  his->FillTH1(NCut, ss.str(), LV1.DeltaPhi(LV2));

  ss.str("");
  ss << name <<"delMass";
  his->FillTH1(NCut, ss.str(), LV1.M() - LV2.M());

  ss.str("");
  ss << name <<"delR";
  his->FillTH1(NCut, ss.str(), LV1.DeltaR(LV2));

  ss.str("");
  ss << name <<"Mass";
  his->FillTH1(NCut, ss.str(), (LV1 + LV2).M());

  ss.str("");
  ss << name <<"MCT";
  his->FillTH1(NCut, ss.str(), CalMCT(LV1, LV2));

  return true;
}       // -----  end of function ComAna::Fill2TLVHistos  -----
