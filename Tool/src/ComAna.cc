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
  type3Ptr = new topTagger::type3TopTagger();
  type3Ptr->setnJetsSel(4); // same as  AnaConsts::nJetsSel
  //BookHistograms();
}  // -----  end of method ComAna::ComAna  (constructor)  -----


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

  // Ntops
  his->AddTH1C("NRecoTops"            , "NRecoTops"            , "No. of Reco Tops"        , "Events"        , 5 , 0  , 5);
  

  // RM from arXiv:1506.00653
  his->AddTH1C("RM"            , "RM"            , "#frac{MET}{ISR Jet}"        , "Events"        , 100, 0, 1);
  //
  his->AddTH1C("dPhiJ1MET", "dPhiJ1MET", "#phi(J1, MET)", "Events",  20, -5, 5);

  // Search bins
  his->AddTH1C("NBJets", "NBJets", 4, 0, 4);
  his->AddTH1C("NTops", "NTops", 4, 0, 4);
  his->AddTH1C("MT2", "MT2", 300, 0, 1500);
  his->AddTH1C("MET", "MET", 300, 0, 1500);
  return true;
}       // -----  end of function ComAna::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  ComAna::InitCutOrder
//  Description:  
// ===========================================================================
bool ComAna::InitCutOrder(std::string ana)
{
  (void)ana;
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

  // NTops
  his->FillTH1(NCut, "NRecoTops", tr->getVar<int>("nTopCandSortedCnt"));
  //his->FillTH1(NCut, "NRecoTops", int(vRecoTops.size()));

  // RM
  if( tr->getVec<TLorentzVector> ("jetsLVec").size() > 0 ) 
  {

    his->FillTH1(NCut, "RM", tr->getVar<double>("met") / tr->getVec<TLorentzVector> ("jetsLVec").at(0).Pt());
    TLorentzVector METLV(0, 0, 0, 0);
    METLV.SetPtEtaPhiE(tr->getVar<double>("met"), 0, tr->getVar<double>("metphi"), 0);
    his->FillTH1(NCut, "dPhiJ1MET", tr->getVec<TLorentzVector> ("jetsLVec").at(0).DeltaPhi(METLV));
  }

  his->FillTH1(NCut, "NBJets", tr->getVar<int>("cntCSVS"));
  his->FillTH1(NCut, "NTops", tr->getVar<int>("nTopCandSortedCnt"));
  his->FillTH1(NCut, "MT2", tr->getVar<double>("best_had_brJet_MT2"));
  his->FillTH1(NCut, "MET", tr->getVar<double>("met"));

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
  //GetType3TopTagger();

  return true;
}       // -----  end of function ComAna::RunEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  ComAna::CountJets
//  Description:  
// ===========================================================================
int ComAna::CountJets(double jetPt) const
{
  int jcount = 0;
  for (unsigned int i = 0; i < tr->getVec<TLorentzVector> ("jetsLVec").size(); ++i)
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
  his->AddTH1C(ss.str(), ss.str(), "Mass", ylabel.str(),  100, 0, 500);

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

// ===  FUNCTION  ============================================================
//         Name:  ComAna::GetType3TopTagger
//  Description:  
// ===========================================================================
int ComAna::GetType3TopTagger()
{
  vRecoTops.clear();
  std::vector<TLorentzVector> jetsforTT;
  std::vector<double> bjsforTT;
  std::vector<int> vToptagged;

  const std::string jetstr= "jetsLVec";
  const std::string bjstr = "recoJetsBtag_0";
  const double ptcut =30;

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
  if(jetsforTT.size () <= 3) return vRecoTops.size();

  // Run the Top Tagger
  //type3Ptr->setdoTopVeto(true);
  type3Ptr->runOnlyTopTaggerPart(jetsforTT, bjsforTT);

  //Get Pt order jet list, passing the requirement
  boost::bimap<int, double > topdm;
  for (size_t j = 0; j < type3Ptr->finalCombfatJets.size(); ++j)
  {
    TLorentzVector jjjTop(0, 0, 0, 0);
    for (size_t k = 0; k < type3Ptr->finalCombfatJets.at(j).size(); ++k)
    {
      jjjTop += jetsforTT.at(type3Ptr->finalCombfatJets.at(j).at(k));
    }

    if (PassType3TopCrite(type3Ptr, jetsforTT, bjsforTT, j))
    {
      vToptagged.push_back(j);
      topdm.insert(boost::bimap<int, double >::value_type(j, fabs(jjjTop.M() - 172.5)));
    }
  }
  vToptagged = SortToptager(topdm);

  for(unsigned int j=0; j < vToptagged.size(); ++j)
  {
    TLorentzVector jjjTop(0, 0, 0, 0);
    for (size_t k = 0; k < type3Ptr->finalCombfatJets.at(vToptagged.at(j)).size(); ++k)
    {
      jjjTop +=  jetsforTT.at(type3Ptr->finalCombfatJets.at(vToptagged.at(j)).at(k));
    }
    vRecoTops.push_back(jjjTop);
  }

  return vRecoTops.size();
}       // -----  end of function ComAna::GetType3TopTagger  -----

// ===  FUNCTION  ============================================================
//         Name:  ComAna::PassType3TopCrite
//  Description:  
// ===========================================================================
bool ComAna::PassType3TopCrite(topTagger::type3TopTagger* type3TopTaggerPtr, std::vector<TLorentzVector>& oriJetsVec,
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
}       // -----  end of function ComAna::PassType3TopCrite  -----

// ===  FUNCTION  ============================================================
//         Name:  ComAna::SortToptager
//  Description:  
// ===========================================================================
std::vector<int> ComAna::SortToptager( boost::bimap<int, double >  dm_bimap) 
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

  return sortedtagger;
}       // -----  end of function ComAna::SortToptager  -----


// ===  FUNCTION  ============================================================
//         Name:  ComAna::SetEvtWeight
//  Description:  
// ===========================================================================
bool ComAna::SetEvtWeight(double weight) const
{
   his->SetWeight(weight);
  return true;
}       // -----  end of function ComAna::SetEvtWeight  -----

