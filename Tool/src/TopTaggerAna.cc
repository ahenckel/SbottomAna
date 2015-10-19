// ===========================================================================
// 
//       Filename:  TopTaggerAna.cc
// 
//    Description:  Code for Top tagger analyzer
// 
//        Version:  1.0
//        Created:  06/16/2015 04:14:31 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  University of Illinois at Chicago, CMS@LPC
// 
// ===========================================================================

#include "TopTaggerAna.h"

//----------------------------------------------------------------------------
//       Class:  TopTaggerAna
//      Method:  TopTaggerAna
// Description:  constructor
//----------------------------------------------------------------------------
TopTaggerAna::TopTaggerAna ()
{
}  // -----  end of method TopTaggerAna::TopTaggerAna  (constructor)  -----


//----------------------------------------------------------------------------
//       Class:  TopTaggerAna
//      Method:  TopTaggerAna
// Description:  constructor
//----------------------------------------------------------------------------
//TopTaggerAna::TopTaggerAna (char *name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile)
TopTaggerAna::TopTaggerAna (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile)
:tr(tr_)
{
  RunType3BHad = false;
  his = new HistTool(OutFile, "Top", name);
  type3Ptr = new topTagger::type3TopTagger();
  type3Ptr->setnJetsSel(4); // same as  AnaConsts::nJetsSel
  t3bhadPtr = new Type3Bhad();

  BookHistograms();
}  // -----  end of method TopTaggerAna::TopTaggerAna  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  TopTaggerAna
//      Method:  TopTaggerAna
// Description:  copy constructor
//----------------------------------------------------------------------------
TopTaggerAna::TopTaggerAna ( const TopTaggerAna &other )
{
}  // -----  end of method TopTaggerAna::TopTaggerAna  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  TopTaggerAna
//      Method:  ~TopTaggerAna
// Description:  destructor
//----------------------------------------------------------------------------
TopTaggerAna::~TopTaggerAna ()
{
}  // -----  end of method TopTaggerAna::-TopTaggerAna  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  TopTaggerAna
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  TopTaggerAna&
TopTaggerAna::operator = ( const TopTaggerAna &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method TopTaggerAna::operator =  (assignment operator)  ---


// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::Test
//  Description:  
// ===========================================================================
bool TopTaggerAna::RunTagger()
{
  his->FillTH1("NBase", 1);
  
  int ditop = GetGenTop();
  FillGenTop();

  if (ditop == 2 )
  {
    CalTaggerEff();
  }

  if (RecoTops.size() == 0)
  {
    ZeroTopTagStudy();
  }

  CalMisTag();

  return true;

}       // -----  end of function TopTaggerAna::Test  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::EndTest
//  Description:  
// ===========================================================================
bool TopTaggerAna::EndTest()
{
  his->CalEfficiency("TopTagHT_Efficiency", "TopTagHT_Numerator", "TopTagHT_Denominator");
  his->CalEfficiency("TopTagPT_Efficiency", "TopTagPT_Numerator", "TopTagPT_Denominator");
  his->CalEfficiency("TopTagEta_Efficiency", "TopTagEta_Numerator", "TopTagEta_Denominator");
  his->CalEfficiency("TopTagdR_Efficiency", "TopTagdR_Numerator", "TopTagdR_Denominator");
  his->CalEfficiency("TopTagHT_Efficiency", "TopTagHT_Numerator", "TopTagHT_Denominator");
  his->CalEfficiency("TopTagnAK4Js_Efficiency", "TopTagnAK4Js_Numerator", "TopTagnAK4Js_Denominator");
  his->CalEfficiency("TopTagnGen4Js_Efficiency", "TopTagnGen4Js_Numerator", "TopTagnGen4Js_Denominator");
  his->CalEfficiency("TopTagnBJs_Efficiency", "TopTagnBJs_Numerator", "TopTagnBJs_Denominator");
  his->CalEfficiency("TopTagMET_Efficiency", "TopTagMET_Numerator", "TopTagMET_Denominator");
  his->CalEfficiency("TopTagdPhiJ0_Efficiency", "TopTagdPhiJ0_Numerator", "TopTagdPhiJ0_Denominator");
  his->CalEfficiency("TopTagdPhiJ1_Efficiency", "TopTagdPhiJ1_Numerator", "TopTagdPhiJ1_Denominator");
  his->CalEfficiency("TopTagdPhiJ2_Efficiency", "TopTagdPhiJ2_Numerator", "TopTagdPhiJ2_Denominator");
  his->WriteTH1();
  return true;
}       // -----  end of function TopTaggerAna::EndTest  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::BookHistograms
//  Description:  
// ===========================================================================
bool TopTaggerAna::BookHistograms()
{
  his->AddTH1("NBase", "Number of Events passed baseline", 2, 0, 2);
  his->AddTH1("GenTopPT"             , "GenTopPT"             , "p_{T}^{gen} [GeV]"       , "Events / 10GeV", 100, 0  , 1000);
  his->AddTH1("GenTopEta"            , "GenTopEta"            , "#eta^{gen}"              , "Events"        , 20 , -5 , 5);
  his->AddTH1("GenTopMass"           , "GenTopMass"           , "m^{gen}"                 , "Events"        , 100, 150 , 200);

  his->AddTH1("RecoTopMass"          , "RecoTopMass"          , "m^{reco} [GeV]"      , "Events"        , 100, 0  , 300);
  his->AddTH1("MatchedRecoTopMass"   , "MatchedRecoTopMass"   , "Matched m^{reco} [GeV]"  , "Events"        , 100, 0  , 300);

  his->AddTH1("MissTopTagPT"         , "MissTopTagPT"         , "NoReco p_{T}^{gen} [GeV]", "Events / 10GeV", 100, 0  , 1000);
  his->AddTH1("MissTopTagEta"        , "MissTopTagEta"        , "NoReco #eta^{gen}"       , "Events"        , 20 , -5 , 5);

  his->AddTH1("NRecoTops"            , "NRecoTops"            , "No. of Reco Tops"        , "Events"        , 10 , 0  , 10);
  his->AddTH1("NSortedType3TopTagger", "NSortedType3TopTagger", "No. of Type3 Tops"       , "Events"        , 10 , 0  , 10);
  his->AddTH1("NCombType3TopTagger", "NCombType3TopTagger", "No. of Type3 Tops Comb"       , "Events"        , 10 , 0  , 10);
  his->AddTH1("NMassType3TopTagger", "NMassType3TopTagger", "No. of Type3 Tops Mass"       , "Events"        , 10 , 0  , 10);

  his->AddTH1("Type3TopTaggerMass"   , "Type3TopTaggerMass"   , "Mass of Comb Tops"       , "Events"        , 80 , 100, 300);
  his->AddTH1("Type3TopTaggerPt"     , "Type3TopTaggerPt"     , "p_{T} of Comb Tops"      , "Events"        , 120, 0  , 1200);
  his->AddTH1("TopTagPT_Denominator" , "TopTagPT_Denominator" , "p_{T}^{gen} [GeV]"       , "Denominator"   , 120, 0  , 1200);
  his->AddTH1("TopTagPT_Numerator"   , "TopTagPT_Numerator"   , "p_{T}^{gen} [GeV]"       , "Numerator"     , 120, 0  , 1200);
  his->AddTH1("TopTagPT_Efficiency"  , "TopTagPT_Efficiency"  , "p_{T}^{gen} [GeV]"       , "Efficiency"    , 120, 0  , 1200);
  his->AddTH1("TopTagEta_Denominator", "TopTagEta_Denominator", "#eta^{gen} [GeV]"       , "Denominator"   , 20, -5, 5);
  his->AddTH1("TopTagEta_Numerator"  , "TopTagEta_Numerator"  , "#eta^{gen} [GeV]"       , "Numerator"     , 20, -5, 5);
  his->AddTH1("TopTagEta_Efficiency" , "TopTagEta_Efficiency" , "#eta^{gen} [GeV]"       , "Efficiency"    , 20, -5, 5);
  his->AddTH1("TopTagdR_Denominator" , "TopTagdR_Denominator" , "#Delta R^{gen} [GeV]"       , "Denominator"   , 50, 0 , 5);
  his->AddTH1("TopTagdR_Numerator"   , "TopTagdR_Numerator"   , "#Delta R^{gen} [GeV]"       , "Numerator"     , 50, 0 , 5);
  his->AddTH1("TopTagdR_Efficiency"  , "TopTagdR_Efficiency"  , "#Delta R^{gen} [GeV]"       , "Efficiency"    , 50, 0 , 5);

  his->AddTH1("DeltaPT_RecoTop", "DeltaPT_RecoTop", "p_{T}^{reco} - p_{T}^{gen} [GeV]", "Events", 100, -50, 50);
  his->AddTH1("DeltaEta_RecoTop", "DeltaEta_RecoTop", "#eta^{reco} - #eta^{gen}", "Events", 150, -0.5, 0.5);
  his->AddTH1("DeltaR_RecoTop", "DeltaR_RecoTop", "#Delta R(reco, gen)", "Events", 40, 0, 0.4);
  his->AddTH1("DeltaPT_GenTop", "DeltaPT_GenTop", "#frac{p_{T}^{reco} - p_{T}^{gen}}{p_{T}^{gen}}", "Events", 20, -2, 2);

  his->AddTH1("TopTagHT_Denominator"       , "TopTagHT_Denominator"       , "HT [GeV]"       , "Denominator"     , 500, 0, 2000);
  his->AddTH1("TopTagHT_Numerator"         , "TopTagHT_Numerator"         , "HT [GeV]"       , "#geq 1 Reco Tops", 500, 0, 2000);
  his->AddTH1("TopTagHT_1Top_Numerator"    , "TopTagHT_1Top_Numerator"    , "HT [GeV]"       , "1 Reco Tops"     , 500, 0, 2000);
  his->AddTH1("TopTagHT_2Top_Numerator"    , "TopTagHT_2Top_Numerator"    , "HT [GeV]"       , "#geq 2 Reco Tops", 500, 0, 2000);
  his->AddTH1("TopTagHT_Efficiency"        , "TopTagHT_Efficiency"        , "HT [GeV]"       , "Efficiency"      , 500, 0, 2000);

  his->AddTH1("TopTagnAK4Js_Denominator"   , "TopTagnAK4Js_Denominator"   , "No. of AK4 Jets", "Denominator"     , 40 , 0, 40);
  his->AddTH1("TopTagnAK4Js_Numerator"     , "TopTagnAK4Js_Numerator"     , "No. of AK4 Jets", "#geq 1 Reco Tops", 40 , 0, 40);
  his->AddTH1("TopTagnAK4Js_1Top_Numerator", "TopTagnAK4Js_1Top_Numerator", "No. of AK4 Jets", "1 Reco Tops"     , 40 , 0, 40);
  his->AddTH1("TopTagnAK4Js_2Top_Numerator", "TopTagnAK4Js_2Top_Numerator", "No. of AK4 Jets", "#geq 2 Reco Tops", 40 , 0, 40);
  his->AddTH1("TopTagnAK4Js_Efficiency"    , "TopTagnAK4Js_Efficiency"    , "No. of AK4 Jets", "Efficiency"      , 40 , 0, 40);

  his->AddTH1("TopTagnGen4Js_Denominator"   , "TopTagnGen4Js_Denominator"   , "No. of Gen4 Jets", "Denominator"     , 40 , 0, 40);
  his->AddTH1("TopTagnGen4Js_Numerator"     , "TopTagnGen4Js_Numerator"     , "No. of Gen4 Jets", "#geq 1 Reco Tops", 40 , 0, 40);
  his->AddTH1("TopTagnGen4Js_1Top_Numerator", "TopTagnGen4Js_1Top_Numerator", "No. of Gen4 Jets", "1 Reco Tops"     , 40 , 0, 40);
  his->AddTH1("TopTagnGen4Js_2Top_Numerator", "TopTagnGen4Js_2Top_Numerator", "No. of Gen4 Jets", "#geq 2 Reco Tops", 40 , 0, 40);
  his->AddTH1("TopTagnGen4Js_Efficiency"    , "TopTagnGen4Js_Efficiency"    , "No. of Gen4 Jets", "Efficiency"      , 40 , 0, 40);


  his->AddTH1("TopTagnBJs_Denominator"     , "TopTagnBJs_Denominator"     , "No. of b Jets"  , "Denominator"     , 10 , 0, 10);
  his->AddTH1("TopTagnBJs_Numerator"       , "TopTagnBJs_Numerator"       , "No. of b Jets"  , "#geq 1 Reco Tops", 10 , 0, 10);
  his->AddTH1("TopTagnBJs_1Top_Numerator"  , "TopTagnBJs_1Top_Numerator"  , "No. of b Jets"  , "1 Reco Tops"     , 10 , 0, 10);
  his->AddTH1("TopTagnBJs_2Top_Numerator"  , "TopTagnBJs_2Top_Numerator"  , "No. of b Jets"  , "#geq 2 Reco Tops", 10 , 0, 10);
  his->AddTH1("TopTagnBJs_Efficiency"      , "TopTagnBJs_Efficiency"      , "No. of b Jets"  , "Efficiency"      , 10 , 0, 10);

  his->AddTH1("TopTagMET_Denominator"      , "TopTagMET_Denominator"      , "MET [GeV]"      , "Denominator"     , 200, 0, 800);
  his->AddTH1("TopTagMET_Numerator"        , "TopTagMET_Numerator"        , "MET [GeV]"      , "#geq 1 Reco Tops", 200, 0, 800);
  his->AddTH1("TopTagMET_1Top_Numerator"   , "TopTagMET_1Top_Numerator"   , "MET [GeV]"      , "1 Reco Tops"     , 200, 0, 800);
  his->AddTH1("TopTagMET_2Top_Numerator"   , "TopTagMET_2Top_Numerator"   , "MET [GeV]"      , "#geq 2 Reco Tops", 200, 0, 800);
  his->AddTH1("TopTagMET_Efficiency"       , "TopTagMET_Efficiency"       , "MET [GeV]"      , "Efficiency"      , 200, 0, 800);

  his->AddTH1("TopTagdPhiJ0_Denominator",    "TopTagdPhiJ0_Denominator",    "dPhi(J0, MET)", "Denominator"     , 200, 0, 4);
  his->AddTH1("TopTagdPhiJ0_Numerator",      "TopTagdPhiJ0_Numerator",      "dPhi(J0, MET)", "#geq 1 Reco Tops", 200, 0, 4);
  his->AddTH1("TopTagdPhiJ0_1Top_Numerator", "TopTagdPhiJ0_1Top_Numerator", "dPhi(J0, MET)", "1 Reco Tops"     , 200, 0, 4);
  his->AddTH1("TopTagdPhiJ0_2Top_Numerator", "TopTagdPhiJ0_2Top_Numerator", "dPhi(J0, MET)", "#geq 2 Reco Tops", 200, 0, 4);
  his->AddTH1("TopTagdPhiJ0_Efficiency",     "TopTagdPhiJ0_Efficiency",     "dPhi(J0, MET)", "Efficiency"      , 200, 0, 4);

  his->AddTH1("TopTagdPhiJ1_Denominator",    "TopTagdPhiJ1_Denominator",    "dPhi(J1, MET)", "Denominator"     , 200, 0, 4);
  his->AddTH1("TopTagdPhiJ1_Numerator",      "TopTagdPhiJ1_Numerator",      "dPhi(J1, MET)", "#geq 1 Reco Tops", 200, 0, 4);
  his->AddTH1("TopTagdPhiJ1_1Top_Numerator", "TopTagdPhiJ1_1Top_Numerator", "dPhi(J1, MET)", "1 Reco Tops"     , 200, 0, 4);
  his->AddTH1("TopTagdPhiJ1_2Top_Numerator", "TopTagdPhiJ1_2Top_Numerator", "dPhi(J1, MET)", "#geq 2 Reco Tops", 200, 0, 4);
  his->AddTH1("TopTagdPhiJ1_Efficiency",     "TopTagdPhiJ1_Efficiency",     "dPhi(J1, MET)", "Efficiency"      , 200, 0, 4);


  his->AddTH1("TopTagdPhiJ2_Denominator",    "TopTagdPhiJ2_Denominator",   "dPhi(J2, MET)", "Denominator"     ,  200, 0, 4);
  his->AddTH1("TopTagdPhiJ2_Numerator",      "TopTagdPhiJ2_Numerator",     "dPhi(J2, MET)", "#geq 1 Reco Tops",  200, 0, 4);
  his->AddTH1("TopTagdPhiJ2_1Top_Numerator", "TopTagdPhiJ2_1Top_Numerator","dPhi(J2, MET)", "1 Reco Tops"     ,  200, 0, 4);
  his->AddTH1("TopTagdPhiJ2_2Top_Numerator", "TopTagdPhiJ2_2Top_Numerator","dPhi(J2, MET)", "#geq 2 Reco Tops",  200, 0, 4);
  his->AddTH1("TopTagdPhiJ2_Efficiency",     "TopTagdPhiJ2_Efficiency",    "dPhi(J2, MET)", "Efficiency"      ,  200, 0, 4);

  his->AddTH1("0T_GenTopPT"  , "0T_GenTopPT"  , "p_{T}^{gen} [GeV]"   , "Events / 10GeV", 100, 0  , 1000);
  his->AddTH1("0T_GenTopEta" , "0T_GenTopEta" , "#eta^{gen}"          , "Events"        , 20 , -5 , 5);
  his->AddTH1("0T_GenTopMass", "0T_GenTopMass", "m^{gen}"             , "Events"        , 100, 150, 200);
  his->AddTH1("0T_GenTopdR"  , "0T_GenTopdR"  , "#Delta R^{gen} [GeV]", "Events", 50, 0  , 5);
  his->AddTH1("0T_HT"        , "0T_HT"        , "HT [GeV]"            , "Events / 10GeV", 200, 0  , 2000);
  his->AddTH1("0T_MET"       , "0T_MET"       , "MET [GeV]"           , "Events / 10GeV", 200, 0  , 2000);
  his->AddTH1("0T_nAK4Js"    , "0T_nAK4Js"    , "No. of AK4 Jets"     , "Events", 20, 0  , 20);
  his->AddTH1("0T_nGen4Js"   , "0T_nGen4Js"   , "No. of Gen4 Jets"    , "Events", 10, 0, 10);
  his->AddTH1("0T_nBJs"      , "0T_nBJs"      , "No. of B Jets"       , "Events", 10, 0, 10);

  his->AddTH1("bhad_PT"      , "bhad_PT"      , "PT of b&had"       , "Events", 100, 0, 500);
  his->AddTH1("bhad_Eta"      , "bhad_Eta"      , "Eta of b&had"       , "Events", 20, -5, 5 );
  his->AddTH1("bhad_Phi"      , "bhad_Phi"      , "Phi of b&had"       , "Events", 20, -5, 5 );
  his->AddTH1("bhad_Mass"      , "bhad_Mass"      , "Mass of b&had"       , "Events", 100, 0, 500);
  his->AddTH1("bhad_Btag"      , "bhad_Btag"      , "Btag of b&had"       , "Events", 100, 0, 1);
  his->AddTH1("bhad_Btag70"      , "bhad_Btag70"      , "Btag of b&had for mass < 70"       , "Events", 100, 0, 1);
  his->AddTH1("bhad_dRbhad"      , "bhad_dRbhad"      , "DeltaR of b&had"       , "Events", 100, 0, 1);
  his->AddTH1("bhad_dRb"      , "bhad_dRb"      , "DeltaR of b"       , "Events", 100, 0, 1);
  his->AddTH1("bhad_dRhad"      , "bhad_dRhad"      , "DeltaR of had"       , "Events", 100, 0, 1);

  his->AddTH1("bhad_GenbPT"   , "bhad_GenbPT"   , "GenbPT of b&had"   , "Events", 100, 0,  500);
  his->AddTH1("bhad_GenbEta"  , "bhad_GenbEta"  , "GenbEta of b&had"  , "Events", 20,  -5, 5     );
  his->AddTH1("bhad_GenbPhi"  , "bhad_GenbPhi"  , "GenbPhi of b&had"  , "Events", 20,  -5, 5     );
  his->AddTH1("bhad_GenbMass" , "bhad_GenbMass" , "GenbMass of b&had" , "Events", 100, 0,  500);
  his->AddTH1("bhad_GenHadPT"   , "bhad_GenHadPT"   , "GenHadPT of b&had"   , "Events", 100, 0,  500);
  his->AddTH1("bhad_GenHadEta"  , "bhad_GenHadEta"  , "GenHadEta of b&had"  , "Events", 20,  -5, 5     );
  his->AddTH1("bhad_GenHadPhi"  , "bhad_GenHadPhi"  , "GenHadPhi of b&had"  , "Events", 20,  -5, 5     );
  his->AddTH1("bhad_GenHadMass" , "bhad_GenHadMass" , "GenHadMass of b&had" , "Events", 100, 0,  500);
  his->AddTH1("bhad_GenBHadPT"   , "bhad_GenBHadPT"   , "GenBHadPT of b&had"   , "Events", 100, 0,  500);
  his->AddTH1("bhad_GenBHadEta"  , "bhad_GenBHadEta"  , "GenBHadEta of b&had"  , "Events", 20,  -5, 5     );
  his->AddTH1("bhad_GenBHadPhi"  , "bhad_GenBHadPhi"  , "GenBHadPhi of b&had"  , "Events", 20,  -5, 5     );
  his->AddTH1("bhad_GenBHadMass" , "bhad_GenBHadMass" , "GenBHadMass of b&had" , "Events", 100, 0,  500);
  his->AddTH1("bhad_GenBHadMPT" , "bhad_GenBHadMPT" , "GenBHadMPT of b&had" , "Events", 100, 0,  0.3);
  his->AddTH1("BHH_GenBHHPT"   , "BHH_GenBHHPT"   , "GenBHHPT of b&had"   , "Events", 100, -500,  500);
  his->AddTH1("BHH_GenBHHEta"  , "BHH_GenBHHEta"  , "GenBHHEta of b&had"  , "Events", 20,  -5, 5     );
  his->AddTH1("BHH_GenBHHPhi"  , "BHH_GenBHHPhi"  , "GenBHHPhi of b&had"  , "Events", 20,  -5, 5     );
  his->AddTH1("BHH_GenBHHMass" , "BHH_GenBHHMass" , "GenBHHMass of b&had" , "Events", 100, -100,  500);
  his->AddTH1("BHH_GenBHHdR" , "BHH_GenBHHdR" , "GenBHH dR of b&had" , "Events", 100, 0,  4);

  his->AddTH1("T3bhad_PT"   , "T3bhad_PT"   , "PT of b&had"   , "Events", 100, 0,    800);
  his->AddTH1("T3bhad_Eta"  , "T3bhad_Eta"  , "Eta of b&had"  , "Events", 20,  -5,   5      );
  his->AddTH1("T3bhad_Phi"  , "T3bhad_Phi"  , "Phi of b&had"  , "Events", 20,  -5,   5      );
  his->AddTH1("T3bhad_Mass" , "T3bhad_Mass" , "Mass of b&had" , "Events", 100, 0,    500);
  his->AddTH1("T3bhad_TopPT"   , "T3bhad_TopPT"   , "PT of b&had"   , "Events", 100, 0,    800);
  his->AddTH1("T3bhad_TopEta"  , "T3bhad_TopEta"  , "Eta of b&had"  , "Events", 20,  -5,   5      );
  his->AddTH1("T3bhad_TopPhi"  , "T3bhad_TopPhi"  , "Phi of b&had"  , "Events", 20,  -5,   5      );
  his->AddTH1("T3bhad_TopMass" , "T3bhad_TopMass" , "Mass of b&had" , "Events", 100, 0,    500);

  his->AddTH1("T3bhad_MPT"  , "T3bhad_MPT"  , "MPT of b&had"  , "Events", 100, 0,    0.35);
  his->AddTH1("T3bhad_MPT2"  , "T3bhad_MPT2"  , "MPT2 of b&had"  , "Events", 100, 0,    0);
  his->AddTH1("T3bhad_dR"   , "T3bhad_dR"   , "Btag of b&had" , "Events", 100, 0,    2);
  his->AddTH1("T3bhad_dPT"  , "T3bhad_dPT"  , "dPT of b&had"  , "Events", 100, -500, 500);
  his->AddTH1("T3bhad_dM"   , "T3bhad_dPT"  , "dPT of b&had"  , "Events", 100, -500, 500);


  his->AddTH1("boost_dRhad1"      , "boost_dRhad1"      , "DeltaR of had1 in top rest"       , "Events", 100, 0, 1);
  his->AddTH1("boost_dRhad2"      , "boost_dRhad2"      , "DeltaR of had2 in top rest"       , "Events", 100, 0, 1);
  his->AddTH1("boost_GenboostPT"   , "boost_GenboostPT"   , "GenboostPT of b&had"   , "Events", 100, -10,  10);
  his->AddTH1("boost_GenboostEta"  , "boost_GenboostEta"  , "GenboostEta of b&had"  , "Events", 20,  -5, 5     );
  his->AddTH1("boost_GenboostPhi"  , "boost_GenboostPhi"  , "GenboostPhi of b&had"  , "Events", 20,  -5, 5     );
  his->AddTH1("boost_GenboostMass" , "boost_GenboostMass" , "GenboostMass of b&had" , "Events", 100, -100,  500);
  his->AddTH1("boost_GenboostdR" , "boost_GenboostdR" , "Genboost dR of b&had" , "Events", 100, 0,  4);

  his->AddTH1("T3bhad_boostdEta"   , "T3bhad_dEta"   , "Btag of b&had" , "Events", 100, 0,    2);
  his->AddTH1("T3bhad_boostdPhi"   , "T3bhad_dPhi"   , "Btag of b&had" , "Events", 100, 0,    2);
  his->AddTH1("T3bhad_boostdR"   , "T3bhad_dR"   , "Btag of b&had" , "Events", 100, 0,    2);
  his->AddTH1("T3bhad_boostdPT"  , "T3bhad_dPT"  , "dPT of b&had"  , "Events", 100, -10, 10);
  his->AddTH1("T3bhad_boostdM"   , "T3bhad_dPT"  , "dPT of b&had"  , "Events", 100, -500, 500);

  his->AddTH1("T3bhad_boostJ1PT"   , "T3bhad_PT"   , "PT of b&had"   , "Events", 100, 0,    800);
  his->AddTH1("T3bhad_boostJ1Eta"  , "T3bhad_Eta"  , "Eta of b&had"  , "Events", 20,  -5,   5      );
  his->AddTH1("T3bhad_boostJ1Phi"  , "T3bhad_Phi"  , "Phi of b&had"  , "Events", 20,  -5,   5      );
  his->AddTH1("T3bhad_boostJ1Mass" , "T3bhad_Mass" , "Mass of b&had" , "Events", 100, 0,    500);
  his->AddTH1("T3bhad_boostJ2PT"   , "T3bhad_PT"   , "PT of b&had"   , "Events", 100, 0,    800);
  his->AddTH1("T3bhad_boostJ2Eta"  , "T3bhad_Eta"  , "Eta of b&had"  , "Events", 20,  -5,   5      );
  his->AddTH1("T3bhad_boostJ2Phi"  , "T3bhad_Phi"  , "Phi of b&had"  , "Events", 20,  -5,   5      );
  his->AddTH1("T3bhad_boostJ2Mass" , "T3bhad_Mass" , "Mass of b&had" , "Events", 100, 0,    500);
  return true;

}       // -----  end of function TopTaggerAna::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::GetGenTop
//  Description:  
// ===========================================================================
int TopTaggerAna::GetGenTop() 
{
  genDecayLVec.clear();
  genDecayIdxVec.clear();    
  genDecayPdgIdVec.clear();  
  genDecayMomIdxVec.clear(); 
  
  genDecayLVec      = tr->getVec<TLorentzVector> ("genDecayLVec");
  genDecayIdxVec    = tr->getVec<int>            ("genDecayIdxVec");
  genDecayPdgIdVec  = tr->getVec<int>            ("genDecayPdgIdVec");
  genDecayMomIdxVec = tr->getVec<int>            ("genDecayMomIdxVec");
  vTops.clear();
  int Nhad = 0;
  for (int i = 0; i < genDecayMomIdxVec.size(); ++i)
  {
    if (abs(genDecayPdgIdVec[i]) == 6)
    {
      TopDecay temp;
      temp.topidx_ = i;
      temp.bidx_ = GetChild(genDecayIdxVec[i], {5});
      temp.Widx_ = GetChild(genDecayIdxVec[i], {24});
      if (temp.bidx_ == -1 || temp.Widx_ == 1)  // avoid temp top quark
        continue; 
      temp.Lepidx_ = GetChild(genDecayIdxVec[temp.Widx_], {11, 13, 15});
      temp.neuidx_ = GetChild(genDecayIdxVec[temp.Widx_], {12, 14, 16});
      if (temp.Lepidx_ != -1)
      {
        temp.isLeptonic_ = true;
      }
      else {
        std::vector<int> out = GetChilds( genDecayIdxVec[temp.Widx_], {1, 2,3, 4, 5});
        assert(out.size() == 2);
        Nhad ++;

        temp.had1idx_ = out.at(0);
        temp.had2idx_ = out.at(1);
      }
      
      vTops.push_back(temp);

    }
  }

  assert(Nhad <= vTops.size());

  return Nhad;
}       // -----  end of function TopTaggerAna::GetGenTop  -----


// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::GetChild
//  Description:  
// ===========================================================================
int TopTaggerAna::GetChild(int parent, std::vector<int> pdgs) const
{
  
  for (int i = 0; i < genDecayMomIdxVec.size(); ++i)
  {
    if (abs(genDecayMomIdxVec[i]) == parent)
    {
      for(unsigned int j=0; j < pdgs.size(); ++j)
      {
        if (abs(genDecayPdgIdVec.at(i)) == pdgs.at(j))
        {
          return i;
        }
      }
    }
  }
  return -1;
}       // -----  end of function TopTaggerAna::GetChild  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::GetChilds
//  Description:  
// ===========================================================================
std::vector<int> TopTaggerAna::GetChilds(int parent, std::vector<int> pdgs) const
{
  std::vector<int> outs;
  
  for (int i = 0; i < genDecayMomIdxVec.size(); ++i)
  {
    if (abs(genDecayMomIdxVec[i]) == parent)
    {
      for(unsigned int j=0; j < pdgs.size(); ++j)
      {
        if (abs(genDecayPdgIdVec.at(i)) == pdgs.at(j))
        {
          outs.push_back(i);
        }
      }
    }
  }
  return outs;
}       // -----  end of function TopTaggerAna::GetChild  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::GetT3TopTagger
//  Description:  /* cursor */
// ===========================================================================
bool TopTaggerAna::GetT3TopTagger(double ptcut, std::string jetstr, std::string bjstr, std::string metstr )
{
  jetsforTT.clear();
  bjsforTT.clear();
  vToptagged.clear();
  RecoTops.clear();
  Type3Jets.clear();

//----------------------------------------------------------------------------
//  Get Jets for tagger
//----------------------------------------------------------------------------
  std::vector<TLorentzVector> jets = tr->getVec<TLorentzVector>(jetstr);
  std::vector<double> bjets  = tr->getVec<double>(bjstr);
  assert(jets.size() == bjets.size());
  
  for(unsigned int i=0; i < jets.size(); ++i)
  {
    if (jets.at(i).Pt() > ptcut)
    {
      jetsforTT.push_back(jets.at(i));
      bjsforTT.push_back(bjets.at(i));
    }
  } 

  // Some event selection cuts
  assert(jetsforTT.size () > 3);

  // Run the Top Tagger
  type3Ptr->setdoTopVeto(true);
  type3Ptr->runOnlyTopTaggerPart(jetsforTT, bjsforTT);
  if (RunType3BHad)
  {
    t3bhadPtr->RunTagger(jetsforTT, bjsforTT);
  }
  
  //Get Pt order jet list, passing the requirement
  boost::bimap<int, double > topdm;
  his->FillTH1("NCombType3TopTagger", int(type3Ptr->finalCombfatJets.size()));

  // Get Top Tagger with deltaM
  int NmassT3 = 0;
  for (size_t j = 0; j < type3Ptr->finalCombfatJets.size(); ++j)
  {
    TLorentzVector jjjTop(0, 0, 0, 0);
    for (size_t k = 0; k < type3Ptr->finalCombfatJets.at(j).size(); ++k)
    {
      jjjTop += jetsforTT.at(type3Ptr->finalCombfatJets.at(j).at(k));
    }
    his->FillTH1("Type3TopTaggerMass", jjjTop.M());
    his->FillTH1("Type3TopTaggerPt", jjjTop.Pt());
    if (jjjTop.M() > 110 && jjjTop.M() < 240) NmassT3++;

    if (PassType3TopCrite(type3Ptr, jetsforTT, bjsforTT, j))
    {
      vToptagged.push_back(j);
      topdm.insert(boost::bimap<int, double >::value_type(j, fabs(jjjTop.M() - 172.5)));
    }
  }
  his->FillTH1("NMassType3TopTagger", NmassT3);
  
  if (RunType3BHad)
  {
    for(unsigned int i=0; i < t3bhadPtr->topCans.size();++i)
    {
      topdm.insert(boost::bimap<int, double >::value_type(i+1000, fabs(t3bhadPtr->topCans.at(i).M() - 172.5)));
    }
  }

  // Get the sorted Top list
  std::vector<int>  vT3BHadTagger;
  if (RunType3BHad)
  {
    his->FillTH1("NSortedType3TopTagger", int(vToptagged.size() + t3bhadPtr->topCans.size()));
    vT3BHadTagger = SortType3Tagger(topdm);
    his->FillTH1("NRecoTops", int(vToptagged.size() +  vT3BHadTagger.size()));
  } else {
    his->FillTH1("NSortedType3TopTagger", int(vToptagged.size()));
    SortToptager(topdm);
    his->FillTH1("NRecoTops", int(vToptagged.size()));
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ To Fix ~~~~~
  for(unsigned int j=0; j < vToptagged.size(); ++j)
  {
    TLorentzVector jjjTop(0, 0, 0, 0);
    for (size_t k = 0; k < type3Ptr->finalCombfatJets.at(vToptagged.at(j)).size(); ++k)
    {
      jjjTop +=  jetsforTT.at(type3Ptr->finalCombfatJets.at(vToptagged.at(j)).at(k));
      Type3Jets.push_back( type3Ptr->finalCombfatJets.at(vToptagged.at(j)).at(k));
    }
    RecoTops.push_back(jjjTop);
    if (jjjTop.M() < 110 )
    {
      std::cout << j << " pass? " << PassType3TopCrite(type3Ptr, jetsforTT, bjsforTT, j) << std::endl;
    }
    his->FillTH1("RecoTopMass", jjjTop.M());
  }

  if (RunType3BHad && vT3BHadTagger.size() > 0)
  {
    for(unsigned int i=0; i < vT3BHadTagger.size(); ++i)
    {
      RecoTops.push_back(t3bhadPtr->topCans.at(vT3BHadTagger.at(i)));
      his->FillTH1("RecoTopMass", t3bhadPtr->topCans.at(vT3BHadTagger.at(i)).M());
      Type3Jets.push_back( t3bhadPtr->toppair.at(vT3BHadTagger.at(i)).first);
      Type3Jets.push_back( t3bhadPtr->toppair.at(vT3BHadTagger.at(i)).second);
      // fill in some info
      TLorentzVector jet1 = jetsforTT.at( t3bhadPtr->toppair.at(vT3BHadTagger.at(i)).first );
      TLorentzVector jet2 = jetsforTT.at( t3bhadPtr->toppair.at(vT3BHadTagger.at(i)).second );
      his->FillTH1("T3bhad_PT", jet1.Pt());
      his->FillTH1("T3bhad_Eta", jet1.Eta());
      his->FillTH1("T3bhad_Phi", jet1.Phi());
      his->FillTH1("T3bhad_Mass", jet1.M());
      his->FillTH1("T3bhad_MPT", jet1.M()/jet1.Pt());
      his->FillTH1("T3bhad_MPT2", jet1.M()/(jet1.Pt() * jet1.Pt()));
      his->FillTH1("T3bhad_dR", jet1.DeltaR(jet2));
      his->FillTH1("T3bhad_dPT", jet1.Pt() - jet2.Pt());
      his->FillTH1("T3bhad_dM", jet1.M() - jet2.M());
      TLorentzVector ttop = jet1 + jet2;
      his->FillTH1("T3bhad_TopPT", ttop.Pt());
      his->FillTH1("T3bhad_TopEta", ttop.Eta());
      his->FillTH1("T3bhad_TopPhi", ttop.Phi());
      his->FillTH1("T3bhad_TopMass", ttop.M());

      jet1.Boost(-1 * ttop.BoostVector());
      jet2.Boost(-1 * ttop.BoostVector());
      his->FillTH1("T3bhad_boostJ1PT", jet1.Pt());
      his->FillTH1("T3bhad_boostJ1Eta", jet1.Eta());
      his->FillTH1("T3bhad_boostJ1Phi", jet1.Phi());
      his->FillTH1("T3bhad_boostJ1Mass", jet1.M());

      his->FillTH1("T3bhad_boostJ2PT", jet2.Pt());
      his->FillTH1("T3bhad_boostJ2Eta", jet2.Eta());
      his->FillTH1("T3bhad_boostJ2Phi", jet2.Phi());
      his->FillTH1("T3bhad_boostJ2Mass", jet2.M());
      his->FillTH1("T3bhad_boostd2ta", jet1.Eta() - jet2.Eta());
      his->FillTH1("T3bhad_boostdPhi", jet1.DeltaPhi(jet2));
      his->FillTH1("T3bhad_boostdR", jet1.DeltaR(jet2));
      his->FillTH1("T3bhad_boostdPT", jet1.Pt() - jet2.Pt());
      his->FillTH1("T3bhad_boostdM", jet1.M() - jet2.M());
    }
  }
  return true;
}       // -----  end of function TopTaggerAna::GetT3TopTagger  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::PassType3TopCrite
//  Description:  
// ===========================================================================
bool TopTaggerAna::PassType3TopCrite(topTagger::type3TopTagger* type3TopTaggerPtr, std::vector<TLorentzVector>& oriJetsVec,
    std::vector<double>& recoJetsBtagCSVS, int ic) const
{
    double fatJetm123 = type3TopTaggerPtr->fatJetMassVec[ic];
    // Find a top fat jet passing at least one of the three criteria
    std::vector<int> fatJetPassStatusVec;
    int isTopJet = type3TopTaggerPtr->checkTopCriteria(type3TopTaggerPtr->finalCombfatJets[ic], 
        oriJetsVec, recoJetsBtagCSVS, type3TopTaggerPtr->fatJetSubMassVec[ic], fatJetm123, fatJetPassStatusVec);

    if (isTopJet != 1) return false;

    TLorentzVector jjjTop(0, 0, 0, 0);
    for (size_t k = 0; k < type3TopTaggerPtr->finalCombfatJets.at(ic).size(); ++k)
    {
      jjjTop += oriJetsVec.at(type3TopTaggerPtr->finalCombfatJets.at(ic).at(k));
    }
    if (jjjTop.M() < 110 || jjjTop.M() > 240 ) return false;
    return true;
}       // -----  end of function TopTaggerAna::PassType3TopCrite  -----


// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::CalTaggerEff
//  Description:  
// ===========================================================================
bool TopTaggerAna::CalTaggerEff() const
{
  std::map<int, int> MatchGenReco;

  for(unsigned int i=0; i < vTops.size(); ++i)
  {
    TopDecay gentop = vTops.at(i);

    if (gentop.isLeptonic_) continue;
    his->FillTH1("TopTagPT_Denominator", genDecayLVec[gentop.topidx_].Pt());
    his->FillTH1("TopTagEta_Denominator", genDecayLVec[gentop.topidx_].Eta());

    bool tagged = false;
    for(unsigned int j=0; j < RecoTops.size(); ++j)
    {
      TLorentzVector jjjTop = RecoTops.at(j);

      if (jjjTop.DeltaR(genDecayLVec[gentop.topidx_]) < 0.4) // match
      {
      
        his->FillTH1("TopTagPT_Numerator", genDecayLVec[gentop.topidx_].Pt());
        his->FillTH1("TopTagEta_Numerator", genDecayLVec[gentop.topidx_].Eta());
        his->FillTH1("MatchedRecoTopMass", jjjTop.M());
        his->FillTH1("DeltaEta_RecoTop", jjjTop.Eta() - genDecayLVec[gentop.topidx_].Eta());
        his->FillTH1("DeltaR_RecoTop", jjjTop.DeltaR(genDecayLVec[gentop.topidx_]));
        his->FillTH1("DeltaPT_RecoTop", jjjTop.Pt() - genDecayLVec[gentop.topidx_].Pt());
        his->FillTH1("DeltaPT_GenTop", (jjjTop.Pt() - genDecayLVec[gentop.topidx_].Pt())/ genDecayLVec[gentop.topidx_].Pt());
        tagged = true;
        MatchGenReco[i] = j;
        break;
      }
    }

    if (! tagged)
    {
        his->FillTH1("MissTopTagPT", genDecayLVec[gentop.topidx_].Pt());
        his->FillTH1("MissTopTagEta", genDecayLVec[gentop.topidx_].Eta());
    }
    
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DiTop DR ~~~~~
  if (vTops.size() == 2)
  {
    his->FillTH1("TopTagdR_Denominator", genDecayLVec[vTops.at(0).topidx_].DeltaR(genDecayLVec[vTops.at(1).topidx_]));
    if (MatchGenReco.size() == 2 && MatchGenReco[0] != MatchGenReco[1])
      his->FillTH1("TopTagdR_Numerator", genDecayLVec[vTops.at(0).topidx_].DeltaR(genDecayLVec[vTops.at(1).topidx_]));
  }

  return true;
}       // -----  end of function TopTaggerAna::CalTaggerEff  -----


// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::FillGenTop
//  Description:  
// ===========================================================================
bool TopTaggerAna::FillGenTop()
{
  for(unsigned int i=0; i < vTops.size(); ++i)
  {
    TopDecay gentop = vTops.at(i);
    his->FillTH1("GenTopPT", genDecayLVec[gentop.topidx_].Pt());
    his->FillTH1("GenTopMass", genDecayLVec[gentop.topidx_].M());
    his->FillTH1("GenTopEta", genDecayLVec[gentop.topidx_].Eta());
  }

  return true;
}       // -----  end of function TopTaggerAna::FillGenTop  -----


// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::SortToptager
//  Description:  
// ===========================================================================
bool TopTaggerAna::SortToptager( boost::bimap<int, double >  dm_bimap) 
{
  std::vector<int> sortedtagger;
  std::set<int> jetset;
  
  double largerdm = -1;
  for(boost::bimap<int, double>::right_map::const_iterator it=dm_bimap.right.begin();
      it!=dm_bimap.right.end(); ++it)
  {
    //std::cout << "dm" << it->first <<" inex " << it->second << std::endl;
    assert(it->first >= largerdm);
    largerdm = it->first;
    bool foundduplicatedjets = false;
    for (size_t k = 0; k < type3Ptr->finalCombfatJets.at(it->second).size(); ++k)
    {
      if (!jetset.insert(type3Ptr->finalCombfatJets.at(it->second).at(k)).second)
      {
        foundduplicatedjets = true;
        break;
      }
    }
    if (!foundduplicatedjets)
    {
      sortedtagger.push_back(it->second);
    }
  }


  //for(unsigned int i=0; i < sortedtagger.size(); ++i)
  //{
    //std::cout << " index " << sortedtagger.at(i) <<" mass different " << dm_bimap.left.at(sortedtagger.at(i))<< std::endl;
  //}
  vToptagged = sortedtagger;

  return true;
}       // -----  end of function TopTaggerAna::SortToptager  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::GetCMSTopTagger
//  Description:  
// ===========================================================================
bool TopTaggerAna::GetFatTopTagger(std::string jetstr)
{
  RecoTops.clear();
  RecoTops =  tr->getVec<TLorentzVector>(jetstr);
  his->FillTH1("NRecoTops", int(RecoTops.size()));
  for(unsigned int j=0; j < RecoTops.size(); ++j)
  {
    TLorentzVector jjjTop = RecoTops.at(j);
    his->FillTH1("RecoTopMass", jjjTop.M());
  }

  return true;
}       // -----  end of function TopTaggerAna::GetCMSTopTagger  -----


// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::CheckRecoEvent
//  Description:  
// ===========================================================================
bool TopTaggerAna::CheckRecoEvent()
{
//----------------------------------------------------------------------------
//  AK4 jet
//----------------------------------------------------------------------------
  std::vector<TLorentzVector> jets =  tr->getVec<TLorentzVector>("jetsLVec");
  std::vector<double> bjets  = tr->getVec<double>("recoJetsBtag");
  int ak4count = 0;
  int ak4Bcount = 0;
  for(unsigned int i=0; i < jets.size(); ++i)
  {
    if (jets.at(i).Pt() >= 30)
    {
      ak4count++;
      if (bjets.at(i) > 0.814) ak4Bcount++;
    }
  } 
  if (ak4count < 4) return false;
  if (ak4Bcount < 1) return false;
  //if (tr->getVar<double>("met") < 200)  return false;

  //jets =  tr->getVec<TLorentzVector>("AK8LVec");
  //if (jets.size() == 0) return false;

  return true;
}       // -----  end of function TopTaggerAna::CheckRecoEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::CalHT
//  Description:  
// ===========================================================================
bool TopTaggerAna::CalHT() 
{
  HT = 0;
  std::vector<TLorentzVector> jets =  tr->getVec<TLorentzVector>("jetsLVec");
  for(unsigned int i=0; i < jets.size(); ++i)
  {
    HT += jets.at(i).Pt();
  } 
  return true;
}       // -----  end of function TopTaggerAna::CalHT  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::CalMisTag()
//  Description:  
// ===========================================================================
bool TopTaggerAna::CalMisTag()
{

  CalHT();
  int nAK4Js = 0;
  int nBJs = 0;
  boost::bimap<int, double > jetPtM;
  std::vector<TLorentzVector> jets =  tr->getVec<TLorentzVector>("jetsLVec");
  std::vector<double> bjets  = tr->getVec<double>("recoJetsBtag");
  for(unsigned int i=0; i < jets.size(); ++i)
  {
    if (jets.at(i).Pt() >= 30)  
    {
      nAK4Js++;
      jetPtM.insert(boost::bimap<int, double >::value_type(i, jets.at(i).Pt()));
      if (bjets.at(i) > 0.814) nBJs++;
    }

    if (vTops.size() == 0)
    {
      his->FillTH1("bhad_Eta"    , jets.at(i).Eta());
      his->FillTH1("bhad_Phi"    , jets.at(i).Phi());
      his->FillTH1("bhad_Mass"   , jets.at(i).M());
      his->FillTH1("bhad_Btag"   , bjets.at(i));
      if (jets.at(i).M() > 20 && jets.at(i).M() < 70)
      {
        his->FillTH1("bhad_Btag70"   , bjets.at(i));
        his->FillTH1("bhad_PT"     , jets.at(i).Pt());
      }
    }
  } 

  int nGen4Js = 0;
  std::vector<TLorentzVector> Genjets =  tr->getVec<TLorentzVector>("Gen4LVec");
  for(unsigned int i=0; i < Genjets.size(); ++i)
  {
    if (Genjets.at(i).Pt() >= 30)  nGen4Js++;
  } 

//----------------------------------------------------------------------------
//  As funtion of HT
//----------------------------------------------------------------------------
  his->FillTH1("TopTagHT_Denominator", HT);
  if (RecoTops.size() >=1 )
  {
    his->FillTH1("TopTagHT_Numerator", HT);
  }
  if (RecoTops.size() == 1)
  {
    his->FillTH1("TopTagHT_1Top_Numerator", HT);
  }
  if (RecoTops.size() > 1)
  {
    his->FillTH1("TopTagHT_2Top_Numerator", HT);
  }


//----------------------------------------------------------------------------
//  As a function of nJets 
//----------------------------------------------------------------------------
  his->FillTH1("TopTagnAK4Js_Denominator", nAK4Js);
  if (RecoTops.size() >=1 )
  {
    his->FillTH1("TopTagnAK4Js_Numerator", nAK4Js);
  }
  if (RecoTops.size() == 1)
  {
    his->FillTH1("TopTagnAK4Js_1Top_Numerator", nAK4Js);
  }
  if (RecoTops.size() > 1)
  {
    his->FillTH1("TopTagnAK4Js_2Top_Numerator", nAK4Js);
  }


//----------------------------------------------------------------------------
//  As a function of nJets  Gen
//----------------------------------------------------------------------------
  his->FillTH1("TopTagnGen4Js_Denominator", nGen4Js);
  if (RecoTops.size() >=1 )
  {
    his->FillTH1("TopTagnGen4Js_Numerator", nGen4Js);
  }
  if (RecoTops.size() == 1)
  {
    his->FillTH1("TopTagnGen4Js_1Top_Numerator", nGen4Js);
  }
  if (RecoTops.size() > 1)
  {
    his->FillTH1("TopTagnGen4Js_2Top_Numerator", nGen4Js);
  }

//----------------------------------------------------------------------------
//  As a function of nJets  -  nGenJets
//----------------------------------------------------------------------------
  his->FillTH1("TopTagnBJs_Denominator", nBJs);
  if (RecoTops.size() >=1 )
  {
    his->FillTH1("TopTagnBJs_Numerator", nBJs);
  }
  if (RecoTops.size() == 1)
  {
    his->FillTH1("TopTagnBJs_1Top_Numerator", nBJs);
  }
  if (RecoTops.size() > 1)
  {
    his->FillTH1("TopTagnBJs_2Top_Numerator", nBJs);
  }


//----------------------------------------------------------------------------
//  As a funtion as MET
//----------------------------------------------------------------------------
  double  MET = tr->getVar<double>("met");
  his->FillTH1("TopTagMET_Denominator", MET);
  if (RecoTops.size() >=1 )
  {
    his->FillTH1("TopTagMET_Numerator", MET);
  }
  if (RecoTops.size() == 1)
  {
    his->FillTH1("TopTagMET_1Top_Numerator", MET);
  }
  if (RecoTops.size() > 1)
  {
    his->FillTH1("TopTagMET_2Top_Numerator", MET);
  }

//----------------------------------------------------------------------------
//  As a funtion of deltaPhi between leading jet and MET
//----------------------------------------------------------------------------
  int jetcounting =  0;
  double dPhiJ0 = 0;
  double dPhiJ1 = 0;
  double dPhiJ2 = 0;
  for(boost::bimap<int, double>::right_map::const_iterator it=jetPtM.right.begin();
      it!=jetPtM.right.end(); ++it)
  {
    //std::cout << "dm" << it->first <<" inex " << it->second <<" test" << jets.at(it->second).Pt()<< std::endl;
    if (jetcounting == 0)
    {
      dPhiJ0 = fabs(TVector2::Phi_mpi_pi( jets.at(it->second).Phi() - tr->getVar<double>("metphi")));
      jetcounting++;
      continue;
    }

    if (jetcounting == 1)
    {
      dPhiJ1 = fabs(TVector2::Phi_mpi_pi( jets.at(it->second).Phi() - tr->getVar<double>("metphi")));
      jetcounting++;
      continue;
    }
    if (jetcounting == 2)
    {
      dPhiJ2 = fabs(TVector2::Phi_mpi_pi( jets.at(it->second).Phi() - tr->getVar<double>("metphi")));
      jetcounting++;
      break;
    }
  }

  his->FillTH1("TopTagdPhiJ0_Denominator", dPhiJ0);
  if (RecoTops.size() >=1 )
  {
    his->FillTH1("TopTagdPhiJ0_Numerator", dPhiJ0);
  }
  if (RecoTops.size() == 1)
  {
    his->FillTH1("TopTagdPhiJ0_1Top_Numerator", dPhiJ0);
  }
  if (RecoTops.size() > 1)
  {
    his->FillTH1("TopTagdPhiJ0_2Top_Numerator", dPhiJ0);
  }

  his->FillTH1("TopTagdPhiJ1_Denominator", dPhiJ1);
  if (RecoTops.size() >=1 )
  {
    his->FillTH1("TopTagdPhiJ1_Numerator", dPhiJ1);
  }
  if (RecoTops.size() == 1)
  {
    his->FillTH1("TopTagdPhiJ1_1Top_Numerator", dPhiJ1);
  }
  if (RecoTops.size() > 1)
  {
    his->FillTH1("TopTagdPhiJ1_2Top_Numerator", dPhiJ1);
  }

  his->FillTH1("TopTagdPhiJ2_Denominator", dPhiJ2);
  if (RecoTops.size() >=1 )
  {
    his->FillTH1("TopTagdPhiJ2_Numerator", dPhiJ2);
  }
  if (RecoTops.size() == 1)
  {
    his->FillTH1("TopTagdPhiJ2_1Top_Numerator", dPhiJ2);
  }
  if (RecoTops.size() > 1)
  {
    his->FillTH1("TopTagdPhiJ2_2Top_Numerator", dPhiJ2);
  }

  if (RecoTops.size() == 0)
  {
    his->FillTH1("0T_HT", HT);
    his->FillTH1("0T_MET", MET);
    his->FillTH1("0T_nAK4Js", nAK4Js);
    his->FillTH1("0T_nGen4Js", nGen4Js);
    his->FillTH1("0T_nBJs", nBJs);
  }

  return true;
}       // -----  end of function TopTaggerAna::CalMisTag()  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::GetNTops
//  Description:  
// ===========================================================================
int TopTaggerAna::GetNTops() const
{
  return RecoTops.size();
}       // -----  end of function TopTaggerAna::GetNTops  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::ZeroTopTagStudy
//  Description:  
// ===========================================================================
bool TopTaggerAna::ZeroTopTagStudy() const
{
  
  for(unsigned int i=0; i < vTops.size(); ++i)
  {
    TopDecay gentop = vTops.at(i);
    his->FillTH1("0T_GenTopPT", genDecayLVec[gentop.topidx_].Pt());
    his->FillTH1("0T_GenTopMass", genDecayLVec[gentop.topidx_].M());
    his->FillTH1("0T_GenTopEta", genDecayLVec[gentop.topidx_].Eta());
  }

  if (vTops.size() == 2)
  {
    his->FillTH1("0T_GenTopdR", genDecayLVec[vTops.at(0).topidx_].DeltaR(genDecayLVec[vTops.at(1).topidx_]));
  }

  return true;
}       // -----  end of function TopTaggerAna::ZeroTopTagStudy  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::GetBoverlapHad
//  Description:  
// ===========================================================================
bool TopTaggerAna::GetBoverlapHad() const
{
  bool temp = false;
  std::vector<std::pair<int, int> > bhad;
  std::vector<int> otherhad;
  for(unsigned int i=0; i < vTops.size(); ++i)
  {
    TopDecay gentop = vTops.at(i);
    if (gentop.bidx_ == -1)  continue;
    TLorentzVector bvec =  genDecayLVec[gentop.bidx_];
    for(unsigned int j=0; j < vTops.size(); ++j)
    {
      TopDecay hadtop = vTops.at(j);
      if (hadtop.had1idx_ != -1)  
        if (bvec.DeltaR(genDecayLVec[hadtop.had1idx_]) < 0.4)
        {
          bhad.push_back(std::make_pair(gentop.bidx_, hadtop.had1idx_));
          otherhad.push_back(hadtop.had2idx_);

        }
      if (hadtop.had2idx_ != -1) 
        if (bvec.DeltaR(genDecayLVec[hadtop.had2idx_]) < 0.4)
        {
          bhad.push_back(std::make_pair(gentop.bidx_, hadtop.had2idx_));
          otherhad.push_back(hadtop.had1idx_);
        }
    }

    //TopDecay gentop = vTops.at(i);
    //if (gentop.bidx_ == -1)  continue;
    //TLorentzVector bvec = genDecayLVec[gentop.bidx_];
    //if (gentop.had1idx_ != -1)  
    //if (bvec.DeltaR(genDecayLVec[gentop.had1idx_]) < 0.4)
    //bhad.push_back(std::make_pair(gentop.bidx_, gentop.had1idx_));
    //if (gentop.had2idx_ != -1) 
    //if (bvec.DeltaR(genDecayLVec[gentop.had2idx_]) < 0.4)
    //bhad.push_back(std::make_pair(gentop.bidx_, gentop.had2idx_));
  }

  if (bhad.size() == 0) return false;
  std::vector<TLorentzVector> jets =  tr->getVec<TLorentzVector>("jetsLVec");
  std::vector<double> bjets  = tr->getVec<double>("recoJetsBtag");
  std::vector<int> jetoverlap;
  std::vector<std::pair<int, int> > jetbhad;

  //std::cout << "number of overlap" << bhad.size() << std::endl;
  for (int i = 0; i < bhad.size(); ++i)
  {
    TLorentzVector b = genDecayLVec[bhad.at(i).first];
    TLorentzVector had = genDecayLVec[bhad.at(i).second];
    TLorentzVector bhad = b + had;
    for (int j = 0; j < jets.size() ; ++j)
    {
      if (jets.at(j).DeltaR(bhad) < 0.4) // jet closet to bhad
      {
        bool intype3 = false;

        for (int k = 0; k < RecoTops.size(); ++k)
        {
          if (jets.at(j).DeltaR(RecoTops.at(k)) < 1.5)
          {
            intype3 = true;
          }
        }
        if (! intype3) // Not counted in type3
        {
          jetoverlap.push_back(j);
          jetbhad.push_back(std::make_pair(j, i));
        }
      }
    }
  }

  if (jetoverlap.size() == 0) return false;

  for (int i = 0; i < bhad.size(); ++i)
  {
    TLorentzVector b = genDecayLVec[bhad.at(i).first];
    TLorentzVector had = genDecayLVec[bhad.at(i).second];
    TLorentzVector bhad = b + had;
    TLorentzVector hh = genDecayLVec[otherhad.at(i)];
    TLorentzVector ttop = bhad + hh;

    his->FillTH1("bhad_GenbPT"      , b.Pt());
    his->FillTH1("bhad_GenbEta"     , b.Eta());
    his->FillTH1("bhad_GenbPhi"     , b.Phi());
    his->FillTH1("bhad_GenbMass"    , b.M());
    his->FillTH1("bhad_GenHadPT"    , had.Pt());
    his->FillTH1("bhad_GenHadEta"   , had.Eta());
    his->FillTH1("bhad_GenHadPhi"   , had.Phi());
    his->FillTH1("bhad_GenHadMass"  , had.M());
    his->FillTH1("bhad_GenBHadPT"   , bhad.Pt());
    his->FillTH1("bhad_GenBHadEta"  , bhad.Eta());
    his->FillTH1("bhad_GenBHadPhi"  , bhad.Phi());
    his->FillTH1("bhad_GenBHadMass" , bhad.M());
    his->FillTH1("bhad_GenBHadMPT" , bhad.M()/bhad.Pt());
    his->FillTH1("BHH_GenBHHPT"   , bhad.Pt() - hh.Pt());
    his->FillTH1("BHH_GenBHHEta"  , bhad.Eta() - hh.Eta());
    his->FillTH1("BHH_GenBHHPhi"  , bhad.DeltaPhi(hh));
    his->FillTH1("BHH_GenBHHMass" , bhad.M() - hh.M());
    his->FillTH1("BHH_GenBHHdR"  , bhad.DeltaR(hh));
    hh.Boost(-1 * ttop.BoostVector() );
    bhad.Boost(-1 * ttop.BoostVector() );
    b.Boost(-1 * ttop.BoostVector() );
    had.Boost(-1 * ttop.BoostVector() );
    his->FillTH1("boost_dRhad1", b.DeltaR(had) );
    his->FillTH1("boost_dRhad2", b.DeltaR(hh) );
    his->FillTH1("boost_GenboostPT"   , bhad.Pt() - hh.Pt());
    his->FillTH1("boost_GenboostEta"  , bhad.Eta() - hh.Eta());
    his->FillTH1("boost_GenboostPhi"  , bhad.DeltaPhi(hh));
    his->FillTH1("boost_GenboostMass" , bhad.M() - hh.M());
    his->FillTH1("boost_GenboostdR"  , bhad.DeltaR(hh));
  }

  for (int j = 0; j < jetoverlap.size() ; ++j)
  {
    his->FillTH1("bhad_PT"     , jets.at(jetoverlap.at(j)).Pt());
    his->FillTH1("bhad_Eta"    , jets.at(jetoverlap.at(j)).Eta());
    his->FillTH1("bhad_Phi"    , jets.at(jetoverlap.at(j)).Phi());
    his->FillTH1("bhad_Mass"   , jets.at(jetoverlap.at(j)).M());
    his->FillTH1("bhad_Btag"   , bjets.at(jetoverlap.at(j)));

    TLorentzVector b = genDecayLVec[bhad.at(jetbhad.at(j).second).first];
    TLorentzVector had = genDecayLVec[bhad.at(jetbhad.at(j).second).second];
    TLorentzVector bhad = b + had;

    his->FillTH1("bhad_dRbhad" , jets.at(jetoverlap.at(j)).DeltaR(bhad));
    his->FillTH1("bhad_dRb"    , jets.at(jetoverlap.at(j)).DeltaR(b));
    his->FillTH1("bhad_dRhad"  , jets.at(jetoverlap.at(j)).DeltaR(had));

    if (jets.at(jetoverlap.at(j)).M() > 70) temp = true;
    else
      his->FillTH1("bhad_Btag70"   , bjets.at(jetoverlap.at(j)));

  }

  return temp;
}       // -----  end of function TopTaggerAna::GetBoverlapHad  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::SetType3BHad
//  Description:  
// ===========================================================================
bool TopTaggerAna::SetType3BHad(bool run)
{
  RunType3BHad = run;
  return RunType3BHad;
}       // -----  end of function TopTaggerAna::SetType3BHad  -----


// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::SortType3Tagger
//  Description:  
// ===========================================================================
std::vector<int> TopTaggerAna::SortType3Tagger( boost::bimap<int, double >  & dm_bimap)
{
  std::vector<int> sortedtagger;
  std::set<int> jetset;
  
  double largerdm = -1;
  for(boost::bimap<int, double>::right_map::const_iterator it=dm_bimap.right.begin();
      it!=dm_bimap.right.end(); ++it)
  {
    //std::cout << "dm" << it->first <<" inex " << it->second << std::endl;
    assert(it->first >= largerdm);
    largerdm = it->first;
    bool foundduplicatedjets = false;
    if (it->second < 1000) //default type3
    {
      for (size_t k = 0; k < type3Ptr->finalCombfatJets.at(it->second).size(); ++k)
      {
        if (!jetset.insert(type3Ptr->finalCombfatJets.at(it->second).at(k)).second)
        {
          foundduplicatedjets = true;
          break;
        }
      }
    } else {

      if (!jetset.insert(t3bhadPtr->toppair.at(it->second -1000).first).second)
      {
        foundduplicatedjets = true;
        break;
      }
      if (!jetset.insert(t3bhadPtr->toppair.at(it->second -1000).second).second)
      {
        foundduplicatedjets = true;
        break;
      }
    }
    if (!foundduplicatedjets)
    {
      sortedtagger.push_back(it->second);
    }
  }

  std::vector<int> vType3Tagger;
  std::vector<int> VT3BHadTagger;

  for(unsigned int i=0; i < sortedtagger.size(); ++i)
  {
    if (sortedtagger.at(i) >= 1000) 
      VT3BHadTagger.push_back(sortedtagger.at(i) - 1000);
    else
      vType3Tagger.push_back(sortedtagger.at(i));
    //std::cout << " index " << sortedtagger.at(i) <<" mass different " << dm_bimap.left.at(sortedtagger.at(i))<< std::endl;
  }
  vToptagged = vType3Tagger;

  return VT3BHadTagger;
}       // -----  end of function TopTaggerAna::SortType3Tagger  -----


// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::SetType3BhadWindow
//  Description:  
// ===========================================================================
bool TopTaggerAna::SetType3BhadWindow(int low, int high) const
{
  t3bhadPtr->SetTopWindow(low, high);
  return true;
}       // -----  end of function TopTaggerAna::SetType3BhadWindow  -----
