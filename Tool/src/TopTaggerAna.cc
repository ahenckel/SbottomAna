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
  his = new HistTool(OutFile, "", name);
  type3Ptr = new topTagger::type3TopTagger();
  type3Ptr->setnJetsSel(4); // same as  AnaConsts::nJetsSel

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
  genDecayLVec.clear();
  genDecayIdxVec.clear();    
  genDecayPdgIdVec.clear();  
  genDecayMomIdxVec.clear(); 
  
  //std::cout << " test joe " <<  tr->getVar<double>("joe")<< std::endl;
  //retval = tr->getVar<double>("met") + tr->getVar<double>("mht");

  //his->FillTH1("test", tr->getVar<double>("met"));
  //std::cout << " test joe " <<  tr->getVar<double>("joe")<< std::endl;
  genDecayLVec      = tr->getVec<TLorentzVector> ("genDecayLVec");
  genDecayIdxVec    = tr->getVec<int>            ("genDecayIdxVec");
  genDecayPdgIdVec  = tr->getVec<int>            ("genDecayPdgIdVec");
  genDecayMomIdxVec = tr->getVec<int>            ("genDecayMomIdxVec");
  
  int ditop = GetGenTop();
  FillGenTop();

  if (ditop == 2 )
  {
    CalTaggerEff();
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
  his->AddTH1("GenTopPT"             , "GenTopPT"             , "p_{T}^{gen} [GeV]"       , "Events / 10GeV", 100, 0  , 1000);
  his->AddTH1("GenTopEta"            , "GenTopEta"            , "#eta^{gen}"              , "Events"        , 20 , -5 , 5);
  his->AddTH1("GenTopMass"           , "GenTopMass"           , "m^{gen}"                 , "Events"        , 100, 150 , 200);

  his->AddTH1("RecoTopMass"          , "RecoTopMass"          , "m^{reco} [GeV]"      , "Events"        , 100, 0  , 300);
  his->AddTH1("MatchedRecoTopMass"   , "MatchedRecoTopMass"   , "Matched m^{reco} [GeV]"  , "Events"        , 100, 0  , 300);

  his->AddTH1("MissTopTagPT"         , "MissTopTagPT"         , "NoReco p_{T}^{gen} [GeV]", "Events / 10GeV", 100, 0  , 1000);
  his->AddTH1("MissTopTagEta"        , "MissTopTagEta"        , "NoReco #eta^{gen}"       , "Events"        , 20 , -5 , 5);
  his->AddTH1("NRecoTops"            , "NRecoTops"            , "No. of Reco Tops"        , "Events"        , 10 , 0  , 10);
  his->AddTH1("NSortedType3TopTagger", "NSortedType3TopTagger", "No. of Type3 Tops"       , "Events"        , 10 , 0  , 10);
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
  return true;

}       // -----  end of function TopTaggerAna::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::GetGenTop
//  Description:  
// ===========================================================================
int TopTaggerAna::GetGenTop() 
{
  vTops.clear();
  int Nhad = 0;
  for (int i = 0; i < genDecayMomIdxVec.size(); ++i)
  {
    //std::cout << " idx " << i <<" ID " << genDecayPdgIdVec[i]  << std::endl;
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

  std::vector<TLorentzVector> jets =  tr->getVec<TLorentzVector>(jetstr);
  std::vector<double> bjets  = tr->getVec<double>(bjstr);
  //assert(jets.size() == bjets.size());
  
  for(unsigned int i=0; i < jets.size(); ++i)
  {
    if (jets.at(i).Pt() > ptcut)
    {
      jetsforTT.push_back(jets.at(i));
      bjsforTT.push_back(0);
    }
  } 
  // Some event selection cuts
  //assert(jetsforTT.size () > 3);

  // Form TLorentzVector of MET
  //TLorentzVector metLVec(tr->getVar<double>("met"), 0, tr->getVar<double>("metphi"), 0);
  type3Ptr->setdoTopVeto(true);
  type3Ptr->runOnlyTopTaggerPart(jetsforTT, bjsforTT);
  //type3Ptr->processEvent(jetsforTT, bjsforTT, metLVec);
  
  
  //Get Pt order jet list, passing the requirement
  boost::bimap<int, double > topdm;

  for (size_t j = 0; j < type3Ptr->finalCombfatJets.size(); ++j)
  {
    TLorentzVector jjjTop(0, 0, 0, 0);
    for (size_t k = 0; k < type3Ptr->finalCombfatJets.at(j).size(); ++k)
    {
      jjjTop += jetsforTT.at(type3Ptr->finalCombfatJets.at(j).at(k));
    }
    his->FillTH1("Type3TopTaggerMass", jjjTop.M());
    his->FillTH1("Type3TopTaggerPt", jjjTop.Pt());

    if (PassType3TopCrite(type3Ptr, jetsforTT, bjsforTT, j))
    {
      vToptagged.push_back(j);
      topdm.insert(boost::bimap<int, double >::value_type(j, fabs(jjjTop.M() - 172.5)));
    }
  }


  his->FillTH1("NSortedType3TopTagger", int(vToptagged.size()));
  SortToptager(topdm);
  his->FillTH1("NRecoTops", int(vToptagged.size()));

  for(unsigned int j=0; j < vToptagged.size(); ++j)
  {
    TLorentzVector jjjTop(0, 0, 0, 0);
    for (size_t k = 0; k < type3Ptr->finalCombfatJets.at(vToptagged.at(j)).size(); ++k)
    {
      jjjTop +=  jetsforTT.at(type3Ptr->finalCombfatJets.at(vToptagged.at(j)).at(k));
    }
    RecoTops.push_back(jjjTop);
    if (jjjTop.M() < 110 )
    {
      std::cout << j << " pass? " << PassType3TopCrite(type3Ptr, jetsforTT, bjsforTT, j) << std::endl;
    }
    his->FillTH1("RecoTopMass", jjjTop.M());
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
    if (MatchGenReco.size() == 2) 
    {
      //std::cout << " first " << MatchGenReco[0] << " second " << MatchGenReco[1] << std::endl;
      if (MatchGenReco[0] != MatchGenReco[1])
        his->FillTH1("TopTagdR_Numerator", genDecayLVec[vTops.at(0).topidx_].DeltaR(genDecayLVec[vTops.at(1).topidx_]));
    }
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


  return true;
}       // -----  end of function TopTaggerAna::CalMisTag()  -----
