// ===========================================================================
// 
//       Filename:  MatchingTaggerGen.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/02/2015 11:37:38 AM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  University of Illinois at Chicago, CMS@LPC
// 
// ===========================================================================

#include "UserCode/SuperTopTagger/interface/MatchingTaggerGen.h"

//----------------------------------------------------------------------------
//       Class:  MatchingTaggerGen
//      Method:  MatchingTaggerGen
// Description:  constructor
//----------------------------------------------------------------------------
MatchingTaggerGen::MatchingTaggerGen (HistTool* hist_, TLorentzVector *metLVec_, 
    std::vector<TLorentzVector> *oriJetsVec_, std::vector<double> *recoJetsBtagCSVS_)
{
  hist = hist_;
  metLVec = metLVec_;
  oriJetsVec = oriJetsVec_;
  recoJetsBtagCSVS = recoJetsBtagCSVS_;
}  // -----  end of method MatchingTaggerGen::MatchingTaggerGen  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  MatchingTaggerGen
//      Method:  MatchingTaggerGen
// Description:  copy constructor
//----------------------------------------------------------------------------
MatchingTaggerGen::MatchingTaggerGen ( const MatchingTaggerGen &other )
{
}  // -----  end of method MatchingTaggerGen::MatchingTaggerGen  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  MatchingTaggerGen
//      Method:  ~MatchingTaggerGen
// Description:  destructor
//----------------------------------------------------------------------------
MatchingTaggerGen::~MatchingTaggerGen ()
{
}  // -----  end of method MatchingTaggerGen::-MatchingTaggerGen  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  MatchingTaggerGen
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  MatchingTaggerGen&
MatchingTaggerGen::operator = ( const MatchingTaggerGen &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method MatchingTaggerGen::operator =  (assignment operator)  ---


// ===  FUNCTION  ============================================================
//         Name:  MatchingTaggerGen::BookHistogram
//  Description:  
// ===========================================================================
bool MatchingTaggerGen::BookHistogram() const
{
  
  hist->AddTH1("OrgWPT", "Original GenW PT", 600, 0, 600);
  hist->AddTH1("MatchWPT", "Matched GenW PT", 600, 0, 600);
  hist->AddTH1("OrgWEta", "Original GenW Eta", 20, -5, 5);
  hist->AddTH1("MatchWEta", "Matched GenW Eta", 20, -5, 5);
  hist->AddTH1("EffWPT", "Efficiency GenW PT", 600, 0, 600);
  hist->AddTH1("EffWEta", "Efficiency GenW Eta", 20, -5, 5);
  hist->AddTH1("WMatchcount", "WMatchcount", 5, 0, 5);

  hist->AddTH1("RecoTopPT", "RecoTopPT", 600, 0, 600);
  hist->AddTH1("RecoTopM", "RecoTopM", 600, 0, 600);
  hist->AddTH1("MatchDeltaM", "Matched DeltaM with GenW ", 120, 60, 60);
  hist->AddTH2( "T3NotMactchWDRPT", "Not Matched W Decay dR Vs WPT", 400, 0, 400, 140, -7, 7);
  //hist->AddTH2( "T3NotMactchWDRPT", "Not Matched W Decay dR Vs WPT;RecoW PT;diJet deltaR", 400, 0, 400, 140, -7, 7);
  //hist->AddTH2( "T3NotMactchWDRPT", "Not Matched W Decay dR Vs WPT", "RecoW PT", "diJet deltaR", 400, 0, 400, 140, -7, 7);
  hist->AddTH2( "T3MactchWDRPT", "Matched W Decay dR Vs WPT", 400, 0, 400, 140, -7, 7);
  hist->AddTH2( "NT3NotMactchWDRPT", "Not Matched W Decay dR Vs WPT", 400, 0, 400, 140, -7, 7);
  hist->AddTH2( "NT3MactchWDRPT", "Matched W Decay dR Vs WPT", 400, 0, 400, 140, -7, 7);
  hist->AddTH2( "N2T3NotMactchWDRPT", "Not Matched W Decay dR Vs WPT", 400, 0, 400, 140, -7, 7);
  hist->AddTH2( "N2T3MactchWDRPT", "Matched W Decay dR Vs WPT", 400, 0, 400, 140, -7, 7);

  //hist->AddTH2( "T3MactchWDRPT", "Matched W Decay dR Vs WPT", "RecoW PT", "diJet deltaR", 400, 0, 400, 140, -7, 7);
  //hist->AddTH2( "NT3NotMactchWDRPT", "Not Matched W Decay dR Vs WPT", "RecoW PT", "Normalized diJet deltaR", 400, 0, 400, 140, -7, 7);
  //hist->AddTH2( "NT3MactchWDRPT", "Matched W Decay dR Vs WPT", "RecoW PT", "Normalized diJet deltaR", 400, 0, 400, 140, -7, 7);
  //hist->AddTH2( "N2T3NotMactchWDRPT", "Not Matched W Decay dR Vs WPT", "RecoW PT", "Normalized diJet deltaR", 400, 0, 400, 140, -7, 7);
  //hist->AddTH2( "N2T3MactchWDRPT", "Matched W Decay dR Vs WPT", "RecoW PT", "Normalized diJet deltaR", 400, 0, 400, 140, -7, 7);

  
  hist->AddTH2( "MatchSumPTWPT-WPT", "Match Sum PT/WPT  Vs WPT", 400, 0, 400, 140, 0, 0);
  hist->AddTH2( "NotMatchSumPTWPT-WPT", "Not Match Sum PT/WPT  Vs WPT", 400, 0, 400, 140, 0, 0);
  hist->AddTH2( "NotMatchWPT-TopPT", "WPT Vs Top PT", 400, 0, 400, 400, 0, 400);


  return true;
}       // -----  end of function MatchingTaggerGen::BookHistogram  -----


// ===  FUNCTION  ============================================================
//         Name:  MatchingTaggerGen::AnaType3WTag
//  Description:  
// ===========================================================================
bool MatchingTaggerGen::AnaType3WTag(topTagger::type3TopTagger* type3TopTaggerPtr, std::vector<WDecay> &Widx)
{
  
  for (size_t i = 0; i < Widx.size(); ++i)
  {
    WDecay wbos = Widx.at(i);
    if (wbos.isLeptonic_) continue;
    // Prep Gen
    TLorentzVector p;
    std::vector<TLorentzVector> pp;
    p.SetPtEtaPhiM( wbos.had1idx_.Pruned->pt(), wbos.had1idx_.Pruned->eta(), 
        wbos.had1idx_.Pruned->phi(), wbos.had1idx_.Pruned->mass());
    pp.push_back(p);
    p.SetPtEtaPhiM( wbos.had2idx_.Pruned->pt(), wbos.had2idx_.Pruned->eta(), 
        wbos.had2idx_.Pruned->phi(), wbos.had2idx_.Pruned->mass());
    pp.push_back(p);
    bool IsDijet = false;
      
    for (size_t j = 0; j < type3TopTaggerPtr->finalCombfatJets.size(); ++j)
    {

      if (! PassType3TopCrite(type3TopTaggerPtr, j)) continue;
      TLorentzVector jjjTop(0, 0, 0, 0);
      for (size_t k = 0; k < type3TopTaggerPtr->finalCombfatJets.at(j).size(); ++k)
      {
        jjjTop += oriJetsVec->at(type3TopTaggerPtr->finalCombfatJets.at(j).at(k));
      }
      if (jjjTop.M() < 80 || jjjTop.M() > 270 ) continue;


      std::vector<TLorentzVector> jj = GetType3WDijet(type3TopTaggerPtr->finalCombfatJets.at(j));
      int matchcount = MatchingWDijet(pp, jj);
      hist->FillTH1("WMatchcount", matchcount);
      if (jj.size() < 1) continue;
      if (jj.size() == 1 && (matchcount == -1 || matchcount == 3 ))
        break;


      if (jj.size() < 2) continue;
      IsDijet = true;
      double jjDeltaR = jj.at(0).DeltaR(jj.at(1));
      TLorentzVector jjW = jj.at(0) + jj.at(1);
      if (matchcount == 2)
      {
        hist->FillTH1("MatchDeltaM", jjW.M() - wbos.Widx_.Pruned->mass());
        hist->FillTH1("MatchWPT", wbos.Widx_.Pruned->pt());
        hist->FillTH1("MatchWEta", wbos.Widx_.Pruned->eta());
        hist->FillTH2("T3MactchWDRPT",  jjW.Pt(), jjDeltaR  );
        hist->FillTH2("NT3MactchWDRPT",  jjW.Pt(), jjDeltaR - 2 * jjW.M()/ jjW.Pt() );
        hist->FillTH2("N2T3MactchWDRPT",  jjW.Pt(), jjDeltaR - 2 * 80.3/ jjW.Pt() );

        hist->FillTH2("MatchSumPTWPT-WPT",  jjW.Pt(), (jj.at(0).Pt() + jj.at(1).Pt()) / jjW.Pt());
      }
      else
      {
        hist->FillTH2("T3NotMactchWDRPT",  jjW.Pt(), jjDeltaR);
        hist->FillTH2("NT3NotMactchWDRPT",  jjW.Pt(), jjDeltaR - 2 * jjW.M()/ jjW.Pt() );
        hist->FillTH2("N2T3NotMactchWDRPT",  jjW.Pt(), jjDeltaR - 2 * 80.3/ jjW.Pt() );
        hist->FillTH2("NotMatchSumPTWPT-WPT",  jjW.Pt(), (jj.at(0).Pt() + jj.at(1).Pt()) / jjW.Pt());
      }
    }

    if (IsDijet)
    {
      hist->FillTH1("OrgWPT", wbos.Widx_.Pruned->pt());
      hist->FillTH1("OrgWEta", wbos.Widx_.Pruned->eta());
    }
  }

//----------------------------------------------------------------------------
//  Get the efficiency
//----------------------------------------------------------------------------
  hist->HisMap["EffWPT"] = (TH1D*)hist->HisMap["MatchWPT"]->Clone("EffWPT");
  hist->HisMap["EffWPT"]->Divide(hist->HisMap["OrgWPT"]);
  hist->HisMap["EffWEta"] = (TH1D*)hist->HisMap["MatchWEta"]->Clone("EffWEta");
  hist->HisMap["EffWEta"]->Divide(hist->HisMap["OrgWEta"]);

  return true;
}       // -----  end of function MatchingTaggerGen::AnaType3WTag  -----


// ===  FUNCTION  ============================================================
//         Name:  MatchingTaggerGen::FakeType3WTag
//  Description:  Invert the algorithm from AnaType3WTag() to see the fake
//  rate of the Wtag in the QCD samples
// ===========================================================================
bool MatchingTaggerGen::FakeType3WTag(topTagger::type3TopTagger* type3TopTaggerPtr) 
{
  for (size_t j = 0; j < type3TopTaggerPtr->finalCombfatJets.size(); ++j)
  {
    if (! PassType3TopCrite(type3TopTaggerPtr, j)) continue;
    std::vector<TLorentzVector> jj = GetType3WDijet(type3TopTaggerPtr->finalCombfatJets.at(j));
    //int matchcount = MatchingWDijet(pp, jj);
    //if (jj.size() < 1) continue;
    //if (jj.size() == 1 && (matchcount == -1 || matchcount == 3 ))
    //break;

    //**************************************************************************//
    //                       Only consider W to dijet case                      //
    //**************************************************************************//
    if (jj.size() < 2) continue;
    TLorentzVector jjjTop(0, 0, 0, 0);
    for (size_t k = 0; k < type3TopTaggerPtr->finalCombfatJets.at(j).size(); ++k)
    {
      jjjTop += oriJetsVec->at(type3TopTaggerPtr->finalCombfatJets.at(j).at(k));
    }

    if (jjjTop.M() < 80 || jjjTop.M() > 270 ) continue;
    hist->FillTH1("RecoTopM", jjjTop.M());
    hist->FillTH1("RecoTopPT", jjjTop.Pt());

    double jjDeltaR = jj.at(0).DeltaR(jj.at(1));
    TLorentzVector jjW = jj.at(0) + jj.at(1);
    hist->FillTH2("T3NotMactchWDRPT",  jjW.Pt(), jjDeltaR);
    hist->FillTH2("NT3NotMactchWDRPT",  jjW.Pt(), jjDeltaR - 2 * jjW.M()/ jjW.Pt() );
    hist->FillTH2("N2T3NotMactchWDRPT",  jjW.Pt(), jjDeltaR - 2 * 80.3/ jjW.Pt() );
    hist->FillTH2("NotMatchSumPTWPT-WPT",  jjW.Pt(), (jj.at(0).Pt() + jj.at(1).Pt()) / jjW.Pt());
    hist->FillTH2("NotMatchWPT-TopPT",  jjW.Pt(), jjjTop.Pt());
  }
  
  return true;
}       // -----  end of function MatchingTaggerGen::FakeType3WTag  -----

// ===  FUNCTION  ============================================================
//         Name:  MatchingTaggerGen::AnaType3Gen
//  Description:  
// ===========================================================================
bool MatchingTaggerGen::AnaType3Gen(topTagger::type3TopTagger* type3TopTaggerPtr, GenParAna *AnaPtr) 
{
  AnaType3WTag(type3TopTaggerPtr, AnaPtr->Widx);
  FakeType3WTag(type3TopTaggerPtr);
  return true;
}       // -----  end of function MatchingTaggerGen::AnaType3Gen  -----

// ===  FUNCTION  ============================================================
//         Name:  MatchingTaggerGen::GetType3WDijet
//  Description:  
// ===========================================================================
std::vector<TLorentzVector> MatchingTaggerGen::GetType3WDijet(std::vector<int> T3TopComb) const
{
  //assert(T3TopComb.size() == 3);
//----------------------------------------------------------------------------
// Get the jet pair with closest mass to W mass
//----------------------------------------------------------------------------
  const double WMass = 80.385;
  std::vector<TLorentzVector> jj;


  // 1 top jet
  if (T3TopComb.size() == 1)
  {
    jj.push_back(oriJetsVec->at(T3TopComb.at(0)));
    return jj;
  }

  // 1 W jet + 1 b jet
  if (T3TopComb.size() == 2)
  {
    double minWMass = 999;
    for (size_t i = 0; i < T3TopComb.size(); ++i)
    {
      if (fabs(oriJetsVec->at(T3TopComb.at(i)).M() - WMass) < minWMass)
      {
        minWMass = fabs(oriJetsVec->at(T3TopComb.at(i)).M() - WMass);
        jj.clear();
        jj.push_back(oriJetsVec->at(T3TopComb.at(i)));
      }
    }
    return jj;
  }

  // 2 W jets + 1 b jet
  if (T3TopComb.size() > 2)
  {
    for (size_t i = 0; i < T3TopComb.size(); ++i)
    {
      double minWMass = 999;
      for (size_t j = i+1; j < T3TopComb.size(); ++j)
      {
        TLorentzVector j1 = oriJetsVec->at(T3TopComb.at(i));
        TLorentzVector j2 = oriJetsVec->at(T3TopComb.at(j));
        double mass = (j1+j2).M();
        if ( fabs(mass-WMass) < minWMass)
        {
          minWMass = fabs(mass-WMass);
          jj.clear();
          jj.push_back(j1);
          jj.push_back(j2);
        }
      }
    }
  }
  return jj;
}       // -----  end of function MatchingTaggerGen::GetType3WDijet  -----

// ===  FUNCTION  ============================================================
//         Name:  MatchingTaggerGen::MatchingWDijet
//  Description:  
// ===========================================================================
int MatchingTaggerGen::MatchingWDijet(std::vector<TLorentzVector> pp, std::vector<TLorentzVector> jj) const
{
//----------------------------------------------------------------------------
//  Matching to the W decays
//----------------------------------------------------------------------------
  assert(pp.size() == 2);
  int matchedcount = 0;

  if (jj.size() == 1)
  {
    TLorentzVector jet = jj.at(0);
    if (fabs(jet.M() - 173 ) < 30)
    {
      matchedcount = -1;
      return matchedcount;
    }
    
    TLorentzVector GenW = pp.at(0) + pp.at(1);

    if (fabs(jet.M() - 80 ) < 30 && GenW.DeltaR(jet) < 0.4)
    {
      matchedcount = 3;
      return matchedcount;
    }
  }

  if (jj.size() == 2)
  {
    BOOST_FOREACH(TLorentzVector p, pp)
    {
      BOOST_FOREACH(TLorentzVector j, jj)
      {
        // Match with cone 0.4!
        if (p.DeltaR(j) < 0.4)
        {
          matchedcount ++;
        }
      }
    }
  }
  return matchedcount;
}       // -----  end of function MatchingTaggerGen::MatchingWDijet  -----

// ===  FUNCTION  ============================================================
//         Name:  MatchingTaggerGen::PassType3TopCrite
//  Description:  
// ===========================================================================
bool MatchingTaggerGen::PassType3TopCrite(topTagger::type3TopTagger* type3TopTaggerPtr, int ic) const
{
    double fatJetm123 = type3TopTaggerPtr->fatJetMassVec[ic];
    // Find a top fat jet passing at least one of the three criteria
    std::vector<int> fatJetPassStatusVec;
    int isTopJet = type3TopTaggerPtr->checkTopCriteria(type3TopTaggerPtr->finalCombfatJets[ic], 
        *oriJetsVec, *recoJetsBtagCSVS, type3TopTaggerPtr->fatJetSubMassVec[ic], fatJetm123, fatJetPassStatusVec);

  return isTopJet == 1;
}       // -----  end of function MatchingTaggerGen::PassType3TopCrite  -----
