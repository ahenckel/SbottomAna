// ===========================================================================
// 
//       Filename:  STTagger.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/09/2016 07:51:05
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#include "STTagger.h"

//----------------------------------------------------------------------------
//       Class:  STTagger
//      Method:  STTagger
// Description:  constructor
//----------------------------------------------------------------------------
STTagger::STTagger (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile, std::string spec_)
  : ComAna(name, tr_, OutFile, spec_)
{
  InitCutOrder(name);
  BookHistograms();
}  // -----  end of method STTagger::STTagger  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  STTagger
//      Method:  STTagger
// Description:  copy constructor
//----------------------------------------------------------------------------
STTagger::STTagger ( const STTagger &other ) : ComAna(other)
{
}  // -----  end of method STTagger::STTagger  (copy constructor)  -----

// ===  FUNCTION  ============================================================
//         Name:  STTagger::Clone
//  Description:  
// ===========================================================================
STTagger*  STTagger::Clone(std::string newname, std::shared_ptr<TFile> OutFile_) 
{
  if (OutFile_ == NULL)
    return new STTagger(newname, tr, OutFile, spec);
  else
    return new STTagger(newname, tr, OutFile_, spec);
}       // -----  end of function STTagger::Clone  -----


//----------------------------------------------------------------------------
//       Class:  STTagger
//      Method:  ~STTagger
// Description:  destructor
//----------------------------------------------------------------------------
STTagger::~STTagger ()
{
}  // -----  end of method STTagger::-STTagger  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  STTagger
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  STTagger&
STTagger::operator = ( const STTagger &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method STTagger::operator =  (assignment operator)  ---


// ===  FUNCTION  ============================================================
//         Name:  STTagger::BookHistograms
//  Description:  
// ===========================================================================
bool STTagger::BookHistograms()
{
  his->AddTH1("GenTopPT"             , "GenTopPT"             , "p_{T}^{gen} [GeV]"       , "Events / 10GeV", 100, 0  , 1000);
  his->AddTH1("GenTopEta"            , "GenTopEta"            , "#eta^{gen}"              , "Events"        , 20 , -5 , 5);
  his->AddTH1("GenTopMass"           , "GenTopMass"           , "m^{gen}"                 , "Events"        , 100, 150 , 200);
  his->AddTH1("RecoTopPT"             , "RecoTopPT"             , "p_{T}^{Reco} [GeV]"       , "Events / 10GeV", 100, 0  , 1000);
  his->AddTH1("RecoTopEta"            , "RecoTopEta"            , "#eta^{Reco}"              , "Events"        , 20 , -5 , 5);
  his->AddTH1("RecoTopMass"           , "RecoTopMass"           , "m^{Reco}"                 , "Events"        , 100, 150 , 200);


  his->AddTH1("TopTagPT_Denominator" , "TopTagPT_Denominator" , "p_{T}^{gen} [GeV]"       , "Denominator"   , 60, 0  , 1200);
  his->AddTH1("TopTagPT_Numerator"   , "TopTagPT_Numerator"   , "p_{T}^{gen} [GeV]"       , "Numerator"     , 60, 0  , 1200);
  his->AddTH1("TopTagPT_Efficiency"  , "TopTagPT_Efficiency"  , "p_{T}^{gen} [GeV]"       , "Efficiency"    , 60, 0  , 1200);
  his->AddTH1("TopTagEta_Denominator", "TopTagEta_Denominator", "#eta^{gen} [GeV]"       , "Denominator"   , 20, -5, 5);
  his->AddTH1("TopTagEta_Numerator"  , "TopTagEta_Numerator"  , "#eta^{gen} [GeV]"       , "Numerator"     , 20, -5, 5);
  his->AddTH1("TopTagEta_Efficiency" , "TopTagEta_Efficiency" , "#eta^{gen} [GeV]"       , "Efficiency"    , 20, -5, 5);
  his->AddTH1("TopTagdR_Denominator" , "TopTagdR_Denominator" , "#Delta R^{gen} [GeV]"       , "Denominator"   , 50, 0 , 5);
  his->AddTH1("TopTagdR_Numerator"   , "TopTagdR_Numerator"   , "#Delta R^{gen} [GeV]"       , "Numerator"     , 50, 0 , 5);
  his->AddTH1("TopTagdR_Efficiency"  , "TopTagdR_Efficiency"  , "#Delta R^{gen} [GeV]"       , "Efficiency"    , 50, 0 , 5);

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

  BookJMEHist();
  return true;
}       // -----  end of function STTagger::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  STTagger::InitCutOrder
//  Description:  
// ===========================================================================
bool STTagger::InitCutOrder(std::string ana)
{
  //Clear the objects
  CutOrder.clear();
  CutMap.clear();

  //Add name and order of the cutflow
  CutOrder.push_back("NoCut");
  CutOrder.push_back("Filter");
  CutOrder.push_back("nJets");
  CutOrder.push_back("MuonSel");
  CutOrder.push_back("EleVeto");
  CutOrder.push_back("HTLep");
  CutOrder.push_back("dPhis");
  CutOrder.push_back("BJets");
  CutOrder.push_back("MET");
  CutOrder.push_back("nComb");

  //Set the cutbit of each cut
  CutMap["NoCut"]   = "00000000000000000";
  CutMap["Filter"]  = "00000000000000001";
  CutMap["nJets"]   = "00000000000000011";
  CutMap["MuonSel"] = "00000000000000111";
  CutMap["EleVeto"] = "00000000000001111";
  CutMap["HTLep"]   = "00000000000011111";
  CutMap["dPhis"]   = "00000000000111111";
  CutMap["BJets"]   = "00000000001111111";
  CutMap["MET"]     = "00000000011111111";
  CutMap["nComb"]   = "00000000111111111";


/*  Stop cut flow
 *  //Add name and order of the cutflow
 *  CutOrder.push_back("NoCut");
 *  CutOrder.push_back("Filter");
 *  CutOrder.push_back("nJets");
 *  CutOrder.push_back("MuonVeto");
 *  CutOrder.push_back("EleVeto");
 *  CutOrder.push_back("IskVeto");
 *  CutOrder.push_back("dPhis");
 *  CutOrder.push_back("BJets");
 *  CutOrder.push_back("MET");
 *
 *  //Set the cutbit of each cut
 *  CutMap["NoCut"]    = "00000000000000000";
 *  CutMap["Filter"]   = "00000000000000001";
 *  CutMap["nJets"]    = "00000000000000011";
 *  CutMap["MuonVeto"] = "00000000000000111";
 *  CutMap["EleVeto"]  = "00000000000001111";
 *  CutMap["IskVeto"]  = "00000000000011111";
 *  CutMap["dPhis"]    = "00000000000111111";
 *  CutMap["BJets"]    = "00000000001111111";
 *  CutMap["MET"]      = "00000000011111111";
 *
 */
  assert(CutOrder.size() == CutMap.size());

  his->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));
  return true;
}       // -----  end of function STTagger::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  STTagger::CheckCut
//  Description:  
// ===========================================================================
bool STTagger::CheckCut()
{
  cutbit.reset();

  cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));
  cutbit.set(1 , tr->getVar<bool>(Label["passnJets"]));

  // Exactly one muon with pt > 45 and |eta| < 2.1
  cutbit.set(2 , GetMuon45());

  cutbit.set(3 , tr->getVar<bool>(Label["passEleVeto"]));

  // MTW
  cutbit.set(4 , GetHTLep() > 150);

  cutbit.set(5 , tr->getVar<bool>(Label["passdPhis"]));
  cutbit.set(6 , tr->getVar<bool>(Label["passBJets"]));

  cutbit.set(7 , tr->getVar<double>(METLabel) > 20);

  cutbit.set(8 , tr->getVec<TLorentzVector>(Label["vCombs"]).size() > 0);

  return true;
}       // -----  end of function STTagger::CheckCut  -----

// ===  FUNCTION  ============================================================
//         Name:  STTagger::FillCut
//  Description:  
// ===========================================================================
bool STTagger::FillCut()
{

//----------------------------------------------------------------------------
//  Check cut and fill cut-based plots
//----------------------------------------------------------------------------
  CheckCut();
  GetGenTop();
  GetRecoTops();

  bool passcuts = false;

  for (unsigned int i = 0; i < CutOrder.size(); ++i)
  {
    std::bitset<NBITS> locbit(CutMap[CutOrder.at(i)]);
    if ( (cutbit & locbit) != locbit) continue;

    his->FillTH1("CutFlow", int(i)); 

    if (i+1 == CutOrder.size()) 
    {
      passcuts = true;
    }
  }

  if (passcuts)
  {
    FillJMEEff();
    FillGenTop();
    CalTaggerEff();
  }
  return passcuts;
}       // -----  end of function STTagger::FillCut  -----

// ===  FUNCTION  ============================================================
//         Name:  STTagger::GetGenTop
//  Description:  
// ===========================================================================
bool STTagger::GetGenTop()
{
  genDecayLVec.clear();
  genDecayIdxVec.clear();    
  genDecayPdgIdVec.clear();  
  genDecayMomIdxVec.clear(); 
  
  try{
    genDecayLVec      = tr->getVec<TLorentzVector> ("genDecayLVec");
    genDecayIdxVec    = tr->getVec<int>            ("genDecayIdxVec");
    genDecayPdgIdVec  = tr->getVec<int>            ("genDecayPdgIdVec");
    genDecayMomIdxVec = tr->getVec<int>            ("genDecayMomIdxVec");
  }
  catch(std::string &var)
  {
    genDecayLVec.clear();
    genDecayIdxVec.clear();    
    genDecayPdgIdVec.clear();  
    genDecayMomIdxVec.clear(); 
    return 0;
  }

  vGenTops.clear();
  int Nhad = 0;
  for (unsigned int i = 0; i < genDecayMomIdxVec.size(); ++i)
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
      
      vGenTops.push_back(temp);

    }
  }


  return true;
}       // -----  end of function STTagger::GetGenTop  -----

// ===  FUNCTION  ============================================================
//         Name:  STTagger::GetRecoTops
//  Description:  
// ===========================================================================
bool STTagger::GetRecoTops() 
{
  vRecoTops.clear();
  vRecoTops = tr->getVec<TLorentzVector>("vTops");
  for(auto t : vRecoTops)
  {
    his->FillTH1("RecoTopPT", t.Pt());
    his->FillTH1("RecoTopMass", t.M());
    his->FillTH1("RecoTopEta", t.Eta());
  }
  return true;
}       // -----  end of function STTagger::GetRecoTops  -----

// ===  FUNCTION  ============================================================
//         Name:  STTagger::GetChild
//  Description:  
// ===========================================================================
int STTagger::GetChild(int parent, std::vector<int> pdgs) const
{
  
  for (unsigned int i = 0; i < genDecayMomIdxVec.size(); ++i)
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
}       // -----  end of function STTagger::GetChild  -----

// ===  FUNCTION  ============================================================
//         Name:  STTagger::GetChilds
//  Description:  
// ===========================================================================
std::vector<int> STTagger::GetChilds(int parent, std::vector<int> pdgs) const
{
  std::vector<int> outs;
  
  for (unsigned int i = 0; i < genDecayMomIdxVec.size(); ++i)
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
}       // -----  end of function STTagger::GetChild  -----


// ===  FUNCTION  ============================================================
//         Name:  STTagger::CalTaggerEff
//  Description:  
// ===========================================================================
bool STTagger::CalTaggerEff()
{
  std::map<int, int> MatchGenReco;

  for(unsigned int i=0; i < vGenTops.size(); ++i)
  {
    TopDecay gentop = vGenTops.at(i);

    if (gentop.isLeptonic_) continue;
    his->FillTH1("TopTagPT_Denominator", genDecayLVec[gentop.topidx_].Pt());
    his->FillTH1("TopTagEta_Denominator", genDecayLVec[gentop.topidx_].Eta());

    bool tagged = false;
    for(unsigned int j=0; j < vRecoTops.size(); ++j)
    {
      TLorentzVector jjjTop = vRecoTops.at(j);

      if (jjjTop.DeltaR(genDecayLVec[gentop.topidx_]) < 0.4) // match
      {
        his->FillTH1("TopTagPT_Numerator", genDecayLVec[gentop.topidx_].Pt());
        his->FillTH1("TopTagEta_Numerator", genDecayLVec[gentop.topidx_].Eta());
        tagged = true;
        MatchGenReco[i] = j;
        break;
      }
    }
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DiTop DR ~~~~~
  if (vGenTops.size() == 2)
  {
    his->FillTH1("TopTagdR_Denominator", genDecayLVec[vGenTops.at(0).topidx_].DeltaR(genDecayLVec[vGenTops.at(1).topidx_]));
    if (MatchGenReco.size() == 2 && MatchGenReco[0] != MatchGenReco[1])
      his->FillTH1("TopTagdR_Numerator", genDecayLVec[vGenTops.at(0).topidx_].DeltaR(genDecayLVec[vGenTops.at(1).topidx_]));
  }


  double HT = tr->getVar<double>(Label["HT"]);
//----------------------------------------------------------------------------
//  As funtion of HT
//----------------------------------------------------------------------------
  his->FillTH1("TopTagHT_Denominator", HT);
  if (vRecoTops.size() >=1 )
  {
    his->FillTH1("TopTagHT_Numerator", HT);
  }
  if (vRecoTops.size() == 1)
  {
    his->FillTH1("TopTagHT_1Top_Numerator", HT);
  }
  if (vRecoTops.size() > 1)
  {
    his->FillTH1("TopTagHT_2Top_Numerator", HT);
  }


  int nAK4Js = tr->getVar<int>(Label["cntNJetsPt30Eta24"]);
//----------------------------------------------------------------------------
//  As a function of nJets 
//----------------------------------------------------------------------------
  his->FillTH1("TopTagnAK4Js_Denominator", nAK4Js);
  if (vRecoTops.size() >=1 )
  {
    his->FillTH1("TopTagnAK4Js_Numerator", nAK4Js);
  }
  if (vRecoTops.size() == 1)
  {
    his->FillTH1("TopTagnAK4Js_1Top_Numerator", nAK4Js);
  }
  if (vRecoTops.size() > 1)
  {
    his->FillTH1("TopTagnAK4Js_2Top_Numerator", nAK4Js);
  }

  int nBJs = tr->getVar<int>(Label["cntCSVS"]);
//----------------------------------------------------------------------------
//  As a function of nJets  -  nGenJets
//----------------------------------------------------------------------------
  his->FillTH1("TopTagnBJs_Denominator", nBJs);
  if (vRecoTops.size() >=1 )
  {
    his->FillTH1("TopTagnBJs_Numerator", nBJs);
  }
  if (vRecoTops.size() == 1)
  {
    his->FillTH1("TopTagnBJs_1Top_Numerator", nBJs);
  }
  if (vRecoTops.size() > 1)
  {
    his->FillTH1("TopTagnBJs_2Top_Numerator", nBJs);
  }


//----------------------------------------------------------------------------
//  As a funtion as MET
//----------------------------------------------------------------------------
  double  MET = tr->getVar<double>(METLabel);
  his->FillTH1("TopTagMET_Denominator", MET);
  if (vRecoTops.size() >=1 )
  {
    his->FillTH1("TopTagMET_Numerator", MET);
  }
  if (vRecoTops.size() == 1)
  {
    his->FillTH1("TopTagMET_1Top_Numerator", MET);
  }
  if (vRecoTops.size() > 1)
  {
    his->FillTH1("TopTagMET_2Top_Numerator", MET);
  }

  return true;
}       // -----  end of function STTagger::CalTaggerEff  -----


// ===  FUNCTION  ============================================================
//         Name:  STTagger::FillGenTop
//  Description:  
// ===========================================================================
bool STTagger::FillGenTop()
{
  for(unsigned int i=0; i < vGenTops.size(); ++i)
  {
    TopDecay gentop = vGenTops.at(i);
    his->FillTH1("GenTopPT", genDecayLVec[gentop.topidx_].Pt());
    his->FillTH1("GenTopMass", genDecayLVec[gentop.topidx_].M());
    his->FillTH1("GenTopEta", genDecayLVec[gentop.topidx_].Eta());
  }

  return true;
}       // -----  end of function STTagger::FillGenTop  -----


// ===  FUNCTION  ============================================================
//         Name:  STTagger::WriteHistogram
//  Description:  
// ===========================================================================
bool STTagger::WriteHistogram() 
{
  his->CalEfficiency("TopTagHT_Efficiency", "TopTagHT_Numerator", "TopTagHT_Denominator");
  his->CalEfficiency("TopTagPT_Efficiency", "TopTagPT_Numerator", "TopTagPT_Denominator");
  his->CalEfficiency("TopTagEta_Efficiency", "TopTagEta_Numerator", "TopTagEta_Denominator");
  his->CalEfficiency("TopTagdR_Efficiency", "TopTagdR_Numerator", "TopTagdR_Denominator");
  his->CalEfficiency("TopTagHT_Efficiency", "TopTagHT_Numerator", "TopTagHT_Denominator");
  his->CalEfficiency("TopTagnAK4Js_Efficiency", "TopTagnAK4Js_Numerator", "TopTagnAK4Js_Denominator");
  his->CalEfficiency("TopTagnBJs_Efficiency", "TopTagnBJs_Numerator", "TopTagnBJs_Denominator");
  his->CalEfficiency("TopTagMET_Efficiency", "TopTagMET_Numerator", "TopTagMET_Denominator");
  his->CalEfficiency("TopTagdPhiJ0_Efficiency", "TopTagdPhiJ0_Numerator", "TopTagdPhiJ0_Denominator");
  his->CalEfficiency("TopTagdPhiJ1_Efficiency", "TopTagdPhiJ1_Numerator", "TopTagdPhiJ1_Denominator");
  his->CalEfficiency("TopTagdPhiJ2_Efficiency", "TopTagdPhiJ2_Numerator", "TopTagdPhiJ2_Denominator");

  his->CalEfficiency("TagPT_Efficiency", "TagPT_Numerator", "TagPT_Denominator");

  ComAna::WriteHistogram();
  return true;
}       // -----  end of function STTagger::WriteHistogram  -----
// ===  FUNCTION  ============================================================
//         Name:  STTagger::GetMuon45
//  Description:  
// ===========================================================================
bool STTagger::GetMuon45()
{
  vMuon45.clear();
  for(auto mu : tr->getVec<TLorentzVector>(Label["cutMuVec"]))
  {
    if (mu.Pt() > 45 && fabs(mu.Eta()) < 2.1)
    {
      vMuon45.push_back(mu);
    }
  }
  
  return vMuon45.size() == 1;
}       // -----  end of function STTagger::GetMuon45  -----
// ===  FUNCTION  ============================================================
//         Name:  STTagger::GetHTLep
//  Description:  
// ===========================================================================
double STTagger::GetHTLep() const
{
  if (vMuon45.size() == 0) return -1;
  TLorentzVector muon = vMuon45.front();
  return muon.Pt() + tr->getVar<double>(METLabel);
}       // -----  end of function STTagger::GetHTLep  -----


// ===  FUNCTION  ============================================================
//         Name:  STTagger::BookJMEHist
//  Description:  
// ===========================================================================
bool STTagger::BookJMEHist()
{
  his->AddTH1("TagPT_Denominator" , "TagPT_Denominator" , "p_{T}^{reco} [GeV]"       , "Denominator"   , 60, 0  , 1200);
  his->AddTH1("TagPT_Numerator"   , "TagPT_Numerator"   , "p_{T}^{reco} [GeV]"       , "Numerator"     , 60, 0  , 1200);
  his->AddTH1("TagPT_Efficiency"  , "TagPT_Efficiency"  , "p_{T}^{reco} [GeV]"       , "Efficiency"    , 60, 0  , 1200);

  return true;
}       // -----  end of function STTagger::BookJMEHist  -----
// ===  FUNCTION  ============================================================
//         Name:  STTagger::FillJMEEff
//  Description:  For myself, I still not buying this method ...
// ===========================================================================
bool STTagger::FillJMEEff()
{
  const std::vector<TLorentzVector> &vTops = tr->getVec<TLorentzVector>(Label["vTops"]);

  if (vTops.size() == 0)
  {
    TLorentzVector comb = GetBestComb();
    his->FillTH1("TagPT_Denominator", comb.Pt());
    return false;
  }

  //Get Pt order jet list, passing the requirement
  boost::bimap<int, double > topdm;

  for(unsigned int i=0; i < vTops.size(); ++i)
  {
    TLorentzVector itop = vTops.at(i);
    topdm.insert(boost::bimap<int, double >::value_type(i, fabs(itop.M() - 172.5)));
  }
  TLorentzVector goodtop = vTops.at(topdm.right.begin()->second);
  his->FillTH1("TagPT_Denominator", goodtop.Pt());
  his->FillTH1("TagPT_Numerator", goodtop.Pt());

  return true;
}       // -----  end of function STTagger::FillJMEEff  -----


// ===  FUNCTION  ============================================================
//         Name:  STTagger::GetBestComb
//  Description:  
// ===========================================================================
TLorentzVector STTagger::GetBestComb()
{
  const std::vector<TLorentzVector> &vCombs = tr->getVec<TLorentzVector>(Label["vCombs"]);
  if (vCombs.size() == 0)
  {
    std::cout << "This is fucked so big!!! " << tr->getVar<double>(METLabel)<< std::endl;
    return TLorentzVector(0,0,0,0);
  }
  
  //Get Pt order jet list, passing the requirement
  boost::bimap<int, double > topdm;

  for(unsigned int i=0; i < vCombs.size(); ++i)
  {
    TLorentzVector itop = vCombs.at(i);
    topdm.insert(boost::bimap<int, double >::value_type(i, fabs(itop.M() - 172.5)));
  }
  return vCombs.at(topdm.right.begin()->second);
}       // -----  end of function STTagger::GetBestComb  -----
