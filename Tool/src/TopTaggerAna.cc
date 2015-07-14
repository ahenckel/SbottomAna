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

  return true;

}       // -----  end of function TopTaggerAna::Test  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::EndTest
//  Description:  
// ===========================================================================
bool TopTaggerAna::EndTest()
{
  his->CalEfficiency("TopTagPT_Efficiency", "TopTagPT_Numerator", "TopTagPT_Denominator");
  his->CalEfficiency("TopTagEta_Efficiency", "TopTagEta_Numerator", "TopTagEta_Denominator");
  his->WriteTH1();
  return true;
}       // -----  end of function TopTaggerAna::EndTest  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::BookHistograms
//  Description:  
// ===========================================================================
bool TopTaggerAna::BookHistograms()
{
  his->AddTH1("test", "TEST", 200, 0, 500);
  his->AddTH1("test2", "TEST3", 200, 0, 1000);
  his->AddTH1("GenTopPT", "GenTopPT", 1000, 0, 1000);
  his->AddTH1("CHSTopPT", "CHSTopPT", 1000, 0, 1000);
  his->AddTH1("PUPPITopPT", "PUPPITopPT", 1000, 0, 1000);
  his->AddTH1("GenTopEta", "GenTopEta", 20, -5, 5);
  his->AddTH1("CHSTopEta", "CHSTopEta", 20, -5, 5);
  his->AddTH1("PUPPITopEta", "PUPPITopEta", 20, -5, 5);

  his->AddTH1("RecoTopMass", "RecoTopMass", 300, 0, 300);
  his->AddTH1("MatchedRecoTopMass", "MatchedRecoTopMass", 300, 0, 300);

  his->AddTH1("MissTopTagPT", "MissTopTagPT", 1000, 0, 1000);
  his->AddTH1("MissTopTagEta", "MissTopTagEta", 20, -5, 5);
  his->AddTH1("NRecoTops", "NRecoTops", 10, 0, 10);
  his->AddTH1("NSortedType3TopTagger", "NSortedType3TopTagger", 10, 0, 10);
  his->AddTH1("Type3TopTaggerMass", "Type3TopTaggerMass", 100, 100, 200);
  his->AddTH1("Type3TopTaggerPt", "Type3TopTaggerPt", 120, 0, 1200);
  his->AddTH1("TopTagPT_Denominator", "TopTagPT_Denominator", 1000, 0, 1000);
  his->AddTH1("TopTagPT_Numerator", "TopTagPT_Numerator", 1000, 0, 1000);
  his->AddTH1("TopTagEta_Denominator", "TopTagEta_Denominator", 20, -5, 5);
  his->AddTH1("TopTagEta_Numerator", "TopTagEta_Numerator", 20, -5, 5);
  his->AddTH1("TopTagPT_Efficiency", "TopTagPT_Efficiency", 1000, 0, 1000);
  his->AddTH1("TopTagEta_Efficiency", "TopTagEta_Efficiency", 20, -5, 5);

  his->AddTH1("DeltaPT_RecoTop", "DeltaPT_RecoTop", 100, -50, 50);
  his->AddTH1("DeltaEta_RecoTop", "DeltaEta_RecoTop", 100, -5, 5);
  his->AddTH1("DeltaR_RecoTop", "DeltaR_RecoTop", 40, 0, 0.4);
  his->AddTH1("DeltaPT_GenTop", "DeltaPT_GenTop", 20, -2, 2);
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
  if (jetsforTT.size () <= 3) return false;

  // Form TLorentzVector of MET
  //TLorentzVector metLVec(tr->getVar<double>("met"), 0, tr->getVar<double>("metphi"), 0);
  type3Ptr->setdoTopVeto(true);
  type3Ptr->runOnlyTopTaggerPart(jetsforTT, bjsforTT);
  //type3Ptr->processEvent(jetsforTT, bjsforTT, metLVec);
  
  
  //Get Pt order jet list, passing the requirement
  boost::bimap<int, double > topdm;

  for (size_t j = 0; j < type3Ptr->finalCombfatJets.size(); ++j)
  {
    if (PassType3TopCrite(type3Ptr, jetsforTT, bjsforTT, j))
    {
      vToptagged.push_back(j);
      TLorentzVector jjjTop(0, 0, 0, 0);
      for (size_t k = 0; k < type3Ptr->finalCombfatJets.at(j).size(); ++k)
      {
        jjjTop += jetsforTT.at(type3Ptr->finalCombfatJets.at(j).at(k));
      }
      his->FillTH1("Type3TopTaggerMass", jjjTop.M());
      his->FillTH1("Type3TopTaggerPt", jjjTop.Pt());
      topdm.insert(boost::bimap<int, double >::value_type(j, fabs(jjjTop.M() - 172.5)));
    }
  }


  SortToptager(topdm);
  his->FillTH1("NSortedType3TopTagger", int(vToptagged.size()));
  his->FillTH1("NRecoTops", int(vToptagged.size()));

  for(unsigned int j=0; j < vToptagged.size(); ++j)
  {
    TLorentzVector jjjTop(0, 0, 0, 0);
    for (size_t k = 0; k < type3Ptr->finalCombfatJets.at(j).size(); ++k)
    {
      jjjTop +=  jetsforTT.at(type3Ptr->finalCombfatJets.at(j).at(k));
    }
    RecoTops.push_back(jjjTop);
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
    //if (jjjTop.M() < 100 || jjjTop.M() > 250 ) return false;
    //if (jjjTop.M() < 80 || jjjTop.M() > 270 ) return false;
    return true;
}       // -----  end of function TopTaggerAna::PassType3TopCrite  -----


// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::CalTaggerEff
//  Description:  
// ===========================================================================
bool TopTaggerAna::CalTaggerEff() const
{
  for(unsigned int i=0; i < vTops.size(); ++i)
  {
    TopDecay gentop = vTops.at(i);
    //if (genDecayLVec[gentop.topidx_].Pt() < 10) continue;

    if (gentop.isLeptonic_) continue;
    his->FillTH1("TopTagPT_Denominator", genDecayLVec[gentop.topidx_].Pt());
    his->FillTH1("TopTagEta_Denominator", genDecayLVec[gentop.topidx_].Eta());

    bool tagged = false;
    for(unsigned int j=0; j < RecoTops.size(); ++j)
    {
      TLorentzVector jjjTop = RecoTops.at(j);
      
      //if (jjjTop.Pt() <=10) continue;

      //if (jjjTop.DeltaR(genDecayLVec[gentop.topidx_]) < 0.8) // match
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
        break;
      }
    }

    if (! tagged)
    {
        his->FillTH1("MissTopTagPT", genDecayLVec[gentop.topidx_].Pt());
        his->FillTH1("MissTopTagEta", genDecayLVec[gentop.topidx_].Eta());
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
    if (jets.at(i).Pt() > 30)
    {
      ak4count++;
    }
    if (bjets.at(i) > AnaConsts::cutCSVS) ak4Bcount++;
  } 
  if (ak4count < 4) return false;
  if (ak4Bcount < 1) return false;

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
