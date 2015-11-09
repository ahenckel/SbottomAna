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
  topAna = new TopTaggerAna("top", tr_, OutFile);
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

  // Plots for Leading Jet Ana
  JetType.push_back("All");
  JetType.push_back("GenTop");
  JetType.push_back("GenB");
  JetType.push_back("GenW");
  JetType.push_back("GenHad");
  JetType.push_back("GenLep");
  JetType.push_back("Gens");
  JetType.push_back("T3Top");
  his->AddTH1C("LeadingJetType", "LeadingJetType", "Type", "Events", JetType);
  his->AddTH1C("SecondJetType",  "SecondJetType",  "Type", "Events", JetType);
  his->AddTH1C("ThirdJetType",   "ThirdJetType",   "Type", "Events", JetType);
  his->AddTH1C("ForthJetType",   "ForthJetType",   "Type", "Events", JetType);

  BookTLVHistos("Jet1NoGen");
  BookTLVHistos("Jet1NoT3");
  BookTLVHistos("Jet2NoGen");
  BookTLVHistos("Jet2NoT3");

  his->AddTH1C("Jet1NoGenMETdPhi", "Jet1NoGenMETdPhi", "#Delta #phi", "Events",  200, -5, 5);
  his->AddTH1C("Jet1NoGenMETdPT", "Jet1NoGenMETdPT", "#Delta p_{T}", "Events",  200, -500, 500);
  his->AddTH1C("Jet1NoT3METdPhi", "Jet1NoT3METdPhi", "#Delta #phi", "Events",  200, -5, 5);
  his->AddTH1C("Jet1NoT3METdPT", "Jet1NoT3METdPT", "#Delta p_{T}", "Events",  200, -500, 500);

  his->AddTH1C("Jet1METdPhi", "Jet1METdPhi", "#Delta #phi", "Events",  200, -5, 5);
  his->AddTH1C("Jet1METdPT", "Jet1METdPT", "#Delta p_{T}", "Events",  200, -500, 500);
  Book2TLVHistos("J1NGT1");
  Book2TLVHistos("J1NTT1");
  Book2TLVHistos("J1NGT2");
  Book2TLVHistos("J1NTT2");

  Book2TLVHistos("J2NGT1");
  Book2TLVHistos("J2NTT1");
  Book2TLVHistos("J2NGT2");
  Book2TLVHistos("J2NTT2");

  his->AddTH1C("LeadingJetPTIso15", "LeadingJetPTIso15", "PT Isolation 1.5", "Jets", 100, 0, 1);
  his->AddTH1C("LeadingJetPTIso8", "LeadingJetPTIso8", "PT Isolation 0.8", "Jets", 100, 0, 1);
  his->AddTH1C("LeadingJetPTIso4", "LeadingJetPTIso4", "PT Isolation 0.4", "Jets", 100, 0, 1);

  his->AddTH1C("LeadingJetMIso15", "LeadingJetMIso15", "Mass Isolation 1.5", "Jets", 100, 0, 1);
  his->AddTH1C("LeadingJetMIso8", "LeadingJetMIso8", "Mass Isolation 0.8", "Jets", 100, 0, 1);
  his->AddTH1C("LeadingJetMIso4", "LeadingJetMIso4", "Mass Isolation 0.4", "Jets", 100, 0, 1);

  his->AddTH2C("LJIsoVsTops", "LJIsoVsTops", "Jet Isolation", "nTops", 100, 0, 1, 5, -1, 4);
  his->AddTH2C("LJnoGenPTVsRM", "LJnoGenPTVsRM", "Jet PT", "RM", 100, 0, 1000, 100, 0, 1);
  his->AddTH2C("LJPTVsRM", "LJPTVsRM", "Jet PT", "RM", 100, 0, 1000, 100, 0, 1);
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
  // Base requirement
  CutOrder.push_back("EleVeto");
  CutOrder.push_back("MuonVeto");
  CutOrder.push_back("IskVeto");
  // From Trigger
  CutOrder.push_back("MET200");
  CutOrder.push_back("nJets");
  // Basic signiturae
  CutOrder.push_back("nTops");
  CutOrder.push_back("dPhiLJMET");
  CutOrder.push_back("dPhiLJTop");
  // Finding ISR Jets
  CutOrder.push_back("LJNonB");
  CutOrder.push_back("LJinTops");
  //CutOrder.push_back("LJetPT700");
  //CutOrder.push_back("LJetMass");
  //CutOrder.push_back("SubJet60");
  //CutOrder.push_back("SubJetB");
  //CutOrder.push_back("dPhiLJMET");
  //CutOrder.push_back("dPhiSubJMET");
  //CutOrder.push_back("LJetPT700");
  //CutOrder.push_back("LJetMass");

  //Set the cutbit of each cut
  CutMap["NoCut"]     = "00000000000000000";
  CutMap["EleVeto"]   = "00000000000000001";
  CutMap["MuonVeto"]  = "00000000000000011";
  CutMap["IskVeto"]   = "00000000000000111";
  CutMap["MET200"]    = "00000000000001111";
  CutMap["nJets"]     = "00000000000011111";
  CutMap["nTops"]     = "00000000000111111";
  CutMap["dPhiLJMET"] = "00000000001111111";
  CutMap["dPhiLJTop"] = "00000000011111111";
  CutMap["LJNonB"]    = "00000000111111111";
  CutMap["LJinTops"]  = "00000001111111111";

  //CutMap["SubJet60"]    = "00000000000111111";
  //CutMap["SubJetB"]     = "00000000001111111";
  //CutMap["dPhiLJMET"]   = "00000000011111111";
  //CutMap["dPhiSubJMET"] = "00000000111111111";
  //CutMap["LJetPT700"]   = "00000001111111111";
  //CutMap["LJetMass"]    = "00000001111111111";

  //CutMap["NoCut"]       = "00000000000000000";
  //CutMap["EleVeto"]     = "00000000000000001";
  //CutMap["MuonVeto"]    = "00000000000000011";
  //CutMap["IskVeto"]     = "00000000000000111";
  //CutMap["LJetPT700"]   = "00000000000001111";
  //CutMap["LJetMass"]    = "00000000000011111";
  //CutMap["SubJet60"]    = "00000000000111111";
  //CutMap["SubJetB"]     = "00000000001111111";
  //CutMap["dPhiLJMET"]   = "00000000011111111";
  //CutMap["dPhiSubJMET"] = "00000000111111111";
  //CutMap["MET200"]      = "00000001111111111";
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
  //const double CVS = 0.814;
  cutbit.reset();
  if( tr->getVec<TLorentzVector> (jetVecLabel).size() == 0 ) 
  {
    cutbit.set();
    cutbit.flip();
    return false;
  }

  // Eletron Veto
  cutbit.set(0, tr->getVar<bool>(Label["passEleVeto"]));
  
  // Muon Veto
  cutbit.set(1, tr->getVar<bool>(Label["passMuonVeto"]));

  // IsoTrack Veto
  cutbit.set(2, tr->getVar<bool>(Label["passIsoTrkVeto"]));

  // MET > 200 GeV
  cutbit.set(3, tr->getVar<double>(METLabel) > 200);

  cutbit.set(4 , tr->getVar<bool>(Label["passnJets"]));

  // At least one top tag
  cutbit.set(5 , tr->getVar<int>(Label["nTopCandSortedCnt"]) >= 1);

  TLorentzVector J1(0, 0, 0, 0);
  if( tr->getVec<TLorentzVector> (jetVecLabel).size() > 0 )
    J1 = tr->getVec<TLorentzVector> (jetVecLabel).at(0);

  // | phi_j0 - phi_MET - pi | < 0.5
  cutbit.set(6, fabs(fabs(J1.Phi() - tr->getVar<double>(METPhiLabel))- 3.14) < 0.5);

  // | phi_j0 - phi_Top - pi | < 0.5
  //std::vector<TLorentzVector> vTops;
  //vTops = tr->getVec<TLorentzVector>("vTops");
  //if (vTops.size() > 0)
    //cutbit.set(7, fabs(fabs(J1.Phi() - vTops.at(0).Phi()) - 3.14) < 0.5);
  //else
    //cutbit.set(7, false);
  cutbit.set(7, true);

  if( tr->getVec<TLorentzVector> (CSVVecLabel).size() > 0 )
    cutbit.set(8,  tr->getVec<double> (CSVVecLabel).at(0)  < 0.814 );
  else
    cutbit.set(8,  true);

  cutbit.set(9,  !LJinTops());

/*
 *
 *
 *  // Leading jet Pt > 700GeV!
 *  TLorentzVector J1(0, 0, 0, 0);
 *  if( tr->getVec<TLorentzVector> ("jetsLVec").size() > 0 )
 *    J1 = tr->getVec<TLorentzVector> ("jetsLVec").at(0);
 *
 *  cutbit.set(3, J1.Pt() > 700);
 *
 *  // Leading Jet mass > Mtop
 *  cutbit.set(4, J1.M() > 172.5);
 *
 *  // Subjet counting
 *  std::vector<int> SubJetIdx;
 *  int SubJetcount=0;
 *  int SubJetBcount=0;
 *  for (int i = 1; i < tr->getVec<TLorentzVector> ("jetsLVec").size(); ++i)
 *  {
 *    if (tr->getVec<TLorentzVector> ("jetsLVec").at(i).Pt() > 60) 
 *    {
 *      SubJetcount ++;
 *      SubJetIdx.push_back(i);
 *      if (tr->getVec<double>("recoJetsBtag_0").at(i) > CVS)
 *        SubJetBcount++;
 *    }
 *  }
 *  // At least three sub-leading jets
 *  cutbit.set(5, SubJetcount >= 3);
 *
 *  // At least one of the subjets is b-tagged
 *  cutbit.set(6, SubJetBcount >= 1);
 *
 *  // delta Phi 
 *  TLorentzVector METLV(0, 0, 0, 0);
 *  METLV.SetPtEtaPhiE(tr->getVar<double>("met"), 0, tr->getVar<double>("metphi"), 0);
 *
 *  // | phi_j0 - phi_MET - pi | < 0.15
 *  //cutbit.set(7, fabs(J1.Phi() - tr->getVar<double>("metphi") - 3.14) < 0.15);
 *  cutbit.set(7, fabs(fabs(J1.DeltaPhi(METLV)) - 3.14 ) < 0.15);
 *  
 *  // | phi_sub - phi_MET | > 0.2
 *  bool dPhiSub = true;
 *  for (int i = 0; i < SubJetIdx.size(); ++i)
 *  {
 *    dPhiSub = dPhiSub &&  fabs(tr->getVec<TLorentzVector> ("jetsLVec").at(SubJetIdx.at(i)).DeltaPhi(METLV)) > 0.2;
 *  }
 *  cutbit.set(8, dPhiSub);
 *  
 *  // MET > 200 GeV
 *  cutbit.set(9, tr->getVar<double>("met") > 200);
 */
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

  // ISR Jet Study
  topAna->GetGenTop();
  // Getting the jet list
  jetsforTT.clear();
  jetsforTTplt.clear();
  jetsforTT = tr->getVec<TLorentzVector>(Label["jetsLVec_forTagger"]);
  for(auto &it : tr->getMap<int, std::vector<TLorentzVector> >(Label["mTopJets"]))
  {
    jetsforTTplt.insert(jetsforTTplt.begin(), it.second.begin(), it.second.end());
  }

  for (unsigned int i = 0; i < CutOrder.size(); ++i)
  {
    std::bitset<NBITS> locbit(CutMap[CutOrder.at(i)]);
    if ( (cutbit & locbit) != locbit) continue;

    his->FillTH1("CutFlow", int(i)); 
    ComAna::FillCut(i);
    if (i+1 == CutOrder.size()) his->FillTH1("NBase", 1);
   
    FillLJAna(i);

    his->FillTH1(i, "LeadingJetPTIso15", LJPTIso(1.5));
    his->FillTH1(i, "LeadingJetPTIso8", LJPTIso(0.8));
    his->FillTH1(i, "LeadingJetPTIso4", LJPTIso(0.4));
    his->FillTH1(i, "LeadingJetMIso15", LJMIso(1.5));
    his->FillTH1(i, "LeadingJetMIso8", LJMIso(0.8));
    his->FillTH1(i, "LeadingJetMIso4", LJMIso(0.4));
  }

  return true;
}       // -----  end of function STISR::FillCut  -----

// ===  FUNCTION  ============================================================
//         Name:  STISR::JetOrgAna
//  Description:  
// ===========================================================================
std::map<std::string, bool> STISR::JetOrgAna(int NCut, std::string name, TLorentzVector &Jet)
{
  bool MatchGen = false;
  bool MatchT3 = false;
  std::map<std::string, bool> matched;
  matched["MatchGen"] = MatchGen;
  matched["MatchT3"] = MatchT3;

  //typedef &FindVectorIdX<std::vector<std::string>, std::string>() t;
  //auto fn_binIdX = std::bind (FindVectorIdX<std::vector<std::string>, std::string>, JetType, _1);

  if (Jet.Pt() < 30) return matched;

  //his->FillTH1(NCut, "LeadingJetType", fn_binIdX("All"));
  his->FillTH1(NCut, name, FindVectorIdX(JetType, "All"));
      //FindVectorIdX<std::vector<std::string>, std::string>(JetType, "All"));

  // Check origin of leading jet in Gen
  std::vector<TLorentzVector> genDecayLVec     = tr->getVec<TLorentzVector> ("genDecayLVec");
  std::vector<TLorentzVector> tempTop;
  std::vector<TLorentzVector> tempW;
  std::vector<TLorentzVector> tempb;
  std::vector<TLorentzVector> tempLep;
  std::vector<TLorentzVector> temphad;
  for(unsigned int i=0; i < topAna->vTops.size(); ++i)
  {
    TopDecay gentop = topAna->vTops.at(i);
    if (gentop.topidx_ != -1) tempTop.push_back(genDecayLVec[gentop.topidx_]);
    if (gentop.Widx_ != -1) tempW.push_back(genDecayLVec[gentop.Widx_]);
    if (gentop.bidx_ != -1) tempb.push_back(genDecayLVec[gentop.bidx_]);
    if (gentop.Lepidx_ != -1) tempLep.push_back(genDecayLVec[gentop.Lepidx_]);
    if (gentop.had1idx_ != -1 && static_cast<unsigned int>(gentop.had1idx_) <= genDecayLVec.size())
    {
      temphad.push_back(genDecayLVec[gentop.had1idx_]);
    }
    if (gentop.had2idx_ != -1 && static_cast<unsigned int>(gentop.had2idx_) <= genDecayLVec.size())
    {
      temphad.push_back(genDecayLVec[gentop.had2idx_]);
    }
  }

  if (HasTLV(Jet, tempTop) != -1) 
  {
    MatchGen=true;
    his->FillTH1(NCut, name, FindVectorIdX(JetType, "GenTop"));
  }
        //FindVectorIdX<std::vector<std::string>, std::string>(JetType, "GenTop"));

  if (HasTLV(Jet, tempb) != -1) 
  {
    MatchGen=true;
    his->FillTH1(NCut, name, FindVectorIdX(JetType, "GenB"));
  }
        //FindVectorIdX<std::vector<std::string>, std::string>(JetType, "GenB"));

  if (HasTLV(Jet, tempW) != -1) 
  {
    MatchGen=true;
    his->FillTH1(NCut, name, FindVectorIdX(JetType, "GenW"));
  }
        //FindVectorIdX<std::vector<std::string>, std::string>(JetType, "GenW"));
  
  if (HasTLV(Jet, temphad) != -1) 
  {
    MatchGen=true;
    his->FillTH1(NCut, name, FindVectorIdX(JetType, "GenHad"));
  }
        //FindVectorIdX<std::vector<std::string>, std::string>(JetType, "GenHad"));


  if (HasTLV(Jet, tempLep) != -1) 
  {
    MatchGen=true;
    his->FillTH1(NCut, name, FindVectorIdX(JetType, "GenLep"));
  }

  if(MatchGen) his->FillTH1(NCut, name, FindVectorIdX(JetType, "Gens"));

        //FindVectorIdX<std::vector<std::string>, std::string>(JetType, "GenLep"));

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Jet in Type3 Top Tagger ~~~~~
  if (jetsforTT.size() < 4) return matched;
  if (HasTLV(Jet, jetsforTTplt) != -1) 
  {
    MatchT3=true;
    his->FillTH1(NCut, name, FindVectorIdX(JetType, "T3Top"));
  }
  //FindVectorIdX<std::vector<std::string>, std::string>(JetType, "T3Top"));
  
  matched["MatchGen"] = MatchGen;
  matched["MatchT3"] = MatchT3;
  return matched;
}       // -----  end of function STISR::JetOrgAna  -----

// ===  FUNCTION  ============================================================
//         Name:  STISR::WriteHistogram
//  Description:  
// ===========================================================================
bool STISR::WriteHistogram()
{
  ComAna::WriteHistogram();
  his->WriteTH1();
  his->WriteTPro();
  his->WriteTH2();
  return true;
}       // -----  end of function STISR::WriteHistogram  -----


// ===  FUNCTION  ============================================================
//         Name:  STISR::LJinTops
//  Description:  
// ===========================================================================
bool STISR::LJinTops()
{
  TLorentzVector Jet1(0, 0, 0, 0);
  if( tr->getVec<TLorentzVector> (jetVecLabel).size() == 0 ) 
    return false;

  Jet1 =  tr->getVec<TLorentzVector> (jetVecLabel).at(0);

  std::map<int, std::vector<TLorentzVector> > vTopJets = tr->getMap<int, std::vector<TLorentzVector> >(Label["mTopJets"]);
  
  for(std::map<int, std::vector<TLorentzVector> >::const_iterator it=vTopJets.begin();
    it!=vTopJets.end(); ++it)
  {
    for(unsigned int i=0; i < it->second.size(); ++i)
    {
      if (Jet1.DeltaR(it->second.at(i))< 0.2)
      {
        return true;
      }
    }
  }

  return false;
}       // -----  end of function STISR::LJinTops  -----

// ===  FUNCTION  ============================================================
//         Name:  STISR::LJPTIso
//  Description:  ISR jet should be isolated from the rest system, idea from
//  Rick
// ===========================================================================
double STISR::LJPTIso(double coneSize) const
{
  const std::vector<TLorentzVector> vJets = tr->getVec<TLorentzVector> (jetVecLabel);
  if( vJets.size() == 0 ) 
    return 1;

  const TLorentzVector Jet1 = vJets.at(0);
  double SumPT = 0.0;

  for(unsigned int i=1; i < vJets.size(); ++i)
  {
    if (Jet1.DeltaR(vJets.at(i)) < coneSize)
    {
      SumPT += vJets.at(i).Pt();
    }
  }

  if (SumPT == 0.0)
    return 1;
  else
    return Jet1.Pt()/ SumPT;
}       // -----  end of function STISR::LJPTIso  -----


// ===  FUNCTION  ============================================================
//         Name:  STISR::LJMIso
//  Description:  ISR jet should be isolated from the rest system, idea from
//  Rick, checking mass relative isolation
// ===========================================================================
double STISR::LJMIso(double coneSize) const
{
  const std::vector<TLorentzVector> vJets = tr->getVec<TLorentzVector> (jetVecLabel);
  if( vJets.size() == 0 ) 
    return 1;

  const TLorentzVector Jet1 = vJets.at(0);
  TLorentzVector conejet(0,0,0,0);

  for(unsigned int i=1; i < vJets.size(); ++i)
  {
    if (Jet1.DeltaR(vJets.at(i)) < coneSize)
    {
      conejet +=vJets.at(i);
    }
  }

  if (conejet.M() == 0.0)
    return 1;
  else
    return Jet1.M()/ conejet.M();
}       // -----  end of function STISR::LJMIso  -----

// ===  FUNCTION  ============================================================
//         Name:  STISR::FillLJAna
//  Description:  
// ===========================================================================
bool STISR::FillLJAna(int i)
{
  //Jet original check
  std::map<std::string, bool> jet1Matched =  JetOrgAna(i, "LeadingJetType", Jet1);
  std::map<std::string, bool> jet2Matched =  JetOrgAna(i, "SecondJetType", Jet2);
  JetOrgAna(i, "ThirdJetType", Jet3);
  JetOrgAna(i, "ForthJetType", Jet4);

  int LJtype = int(LJinTops()) * 2 + int( jet1Matched["MatchGen"]);
  his->FillTH2(i, "LJIsoVsTops", LJPTIso(1.5), LJtype);

  his->FillTH1(i, "Jet1METdPhi", Jet1.Phi() - tr->getVar<double>(METPhiLabel));
  his->FillTH1(i, "Jet1METdPT", Jet1.Pt() - tr->getVar<double>(METLabel));
  his->FillTH2(i, "LJPTVsRM", Jet1.Pt(), tr->getVar<double>(METLabel) / Jet1.Pt());

  if (!jet1Matched["MatchGen"]) 
  {
    FillTLVHistos(i, "Jet1NoGen", Jet1);
    his->FillTH1(i, "Jet1NoGenMETdPhi", Jet1.Phi() - tr->getVar<double>(METPhiLabel));
    his->FillTH1(i, "Jet1NoGenMETdPT", Jet1.Pt() - tr->getVar<double>(METLabel));
    if (topAna->RecoTops.size() > 0)
      Fill2TLVHistos(i, "J1NGT1", Jet1, topAna->RecoTops.at(0));
    if (topAna->RecoTops.size() > 1)
      Fill2TLVHistos(i, "J1NGT1", Jet1, topAna->RecoTops.at(1));
    his->FillTH2(i, "LJnoGenPTVsRM", Jet1.Pt(), tr->getVar<double>(METLabel) / Jet1.Pt());
  }
  if (!jet1Matched["MatchT3"]) 
  {
    FillTLVHistos(i, "Jet1NoT3", Jet1);
    his->FillTH1(i, "Jet1NoT3METdPhi", Jet1.Phi() - tr->getVar<double>(METPhiLabel));
    his->FillTH1(i, "Jet1NoT3METdPT", Jet1.Pt() - tr->getVar<double>(METLabel));
    if (topAna->RecoTops.size() > 0)
      Fill2TLVHistos(i, "J1NTT1", Jet1, topAna->RecoTops.at(0));
    if (topAna->RecoTops.size() > 1)
      Fill2TLVHistos(i, "J1NTT1", Jet1, topAna->RecoTops.at(1));
  }


  if (!jet2Matched["MatchGen"]) 
  {
    FillTLVHistos(i, "Jet2NoGen", Jet2);
    if (topAna->RecoTops.size() > 0)
      Fill2TLVHistos(i, "J2NGT1", Jet2, topAna->RecoTops.at(0));
    if (topAna->RecoTops.size() > 1)
      Fill2TLVHistos(i, "J2NGT1", Jet2, topAna->RecoTops.at(1));
  }
  if (!jet2Matched["MatchT3"]) 
  {
    FillTLVHistos(i, "Jet2NoT3", Jet2);
    if (topAna->RecoTops.size() > 0)
      Fill2TLVHistos(i, "J2NTT1", Jet2, topAna->RecoTops.at(0));
    if (topAna->RecoTops.size() > 1)
      Fill2TLVHistos(i, "J2NTT1", Jet2, topAna->RecoTops.at(1));
  }

  return true;
}       // -----  end of function STISR::FillLJAna  -----
