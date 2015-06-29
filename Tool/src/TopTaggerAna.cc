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
TopTaggerAna::TopTaggerAna (std::string &name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile)
:tr(tr_)
{
  his = new HistTool(OutFile, "", name);
  his->AddTH1("test", "TEST", 200, 0, 500);
  type3Ptr = new topTagger::type3TopTagger();
  type3Ptr->setnJetsSel(4); // same as  AnaConsts::nJetsSel
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
bool TopTaggerAna::Test() 
{
  genDecayLVec.clear();
  genDecayIdxVec.clear();    
  genDecayPdgIdVec.clear();  
  genDecayMomIdxVec.clear(); 
  
  //std::cout << " test joe " <<  tr->getVar<double>("joe")<< std::endl;
  retval = tr->getVar<double>("met") + tr->getVar<double>("mht");

  his->FillTH1("test", tr->getVar<double>("met"));
  //std::cout << " test joe " <<  tr->getVar<double>("joe")<< std::endl;
  genDecayLVec      = tr->getVec<TLorentzVector> ("genDecayLVec");
  genDecayIdxVec    = tr->getVec<int>            ("genDecayIdxVec");
  genDecayPdgIdVec  = tr->getVec<int>            ("genDecayPdgIdVec");
  genDecayMomIdxVec = tr->getVec<int>            ("genDecayMomIdxVec");
  genDecayStrVec    = tr->getVec<std::string>    ("genDecayStrVec");

  for(unsigned int i=0; i < genDecayStrVec.size(); ++i)
  {
    std::cout << genDecayStrVec.at(i) << std::endl;
  }

  //for (int i = 0; i < genDecayMomIdxVec.size(); ++i)
  //{
    //std::cout << " Idx" << genDecayIdxVec[i] <<" Pdg " << genDecayPdgIdVec[i] <<" Mom " << genDecayMomIdxVec[i] << " pt " << genDecayLVec[i].Pt() << std::endl;

  //}
  GetGenTop();

  std::vector<TLorentzVector> taggedtops = GetT3TopTagger(30, "jetsLVec", "recoJetsBtag", "met");

  return true;

}       // -----  end of function TopTaggerAna::Test  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::EndTest
//  Description:  
// ===========================================================================
bool TopTaggerAna::EndTest()
{
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
  return true;
}       // -----  end of function TopTaggerAna::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::GetGenTop
//  Description:  
// ===========================================================================
bool TopTaggerAna::GetGenTop() 
{
  vTops.clear();
  for (int i = 0; i < genDecayMomIdxVec.size(); ++i)
  {
    std::cout << " Idx" << genDecayIdxVec[i] <<" Pdg " << genDecayPdgIdVec[i] <<" Mom " << genDecayMomIdxVec[i] << " pt " << genDecayLVec[i].Pt() << std::endl;
    if (fabs(genDecayPdgIdVec[i]) == 6)
    {
      TopDecay temp;
      temp.topidx_ = i;
      temp.bidx_ = GetChild(i, {5});
      temp.Widx_ = GetChild(i, {24});
      temp.Lepidx_ = GetChild(temp.Widx_, {11, 13, 15});
      temp.neuidx_ = GetChild(temp.Widx_, {12, 14, 16});
      if (temp.Lepidx_ != -1)
      {
        temp.isLeptonic_ = true;
      }
      else
      {
        std::vector<int> out = GetChilds( temp.Widx_, {1, 2,3, 4, 5});
        assert(out.size() == 2);
        temp.had1idx_ = out.at(0);
        temp.had2idx_ = out.at(1);
      }
      
      vTops.push_back(temp);
    }
  }

  return true;
}       // -----  end of function TopTaggerAna::GetGenTop  -----


// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::GetChild
//  Description:  
// ===========================================================================
int TopTaggerAna::GetChild(int parent, std::vector<int> pdgs) const
{
  
  for (int i = 0; i < genDecayMomIdxVec.size(); ++i)
  {
    if (fabs(genDecayMomIdxVec[i]) == parent)
    {
      for(unsigned int j=0; j < pdgs.size(); ++j)
      {
        if (fabs(genDecayPdgIdVec.at(i)) == pdgs.at(j))
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
    if (fabs(genDecayMomIdxVec[i]) == parent)
    {
      for(unsigned int j=0; j < pdgs.size(); ++j)
      {
        if (fabs(genDecayPdgIdVec.at(i)) == pdgs.at(j))
        {
          outs.push_back(i)<##>;
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
std::vector<int> TopTaggerAna::GetT3TopTagger(double ptcut, 
    std::string jetstr, std::string bjstr, std::string metstr ) const
{
  std::vector<TLorentzVector> jetsforTT;
  std::vector<double> bjsforTT;

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

  type3Ptr->processEvent(jetsforTT, bjsforTT, tr->getVar<TLorentzVector>(metstr));

  std::vector<int> taggedtops;
  for (size_t j = 0; j < type3TopTaggerPtr->finalCombfatJets.size(); ++j)
  {
    if (PassType3TopCrite(type3TopTaggerPtr, j))
    {
      taggedtops.push_back(j);
    }
  }
  return taggedtops;
}       // -----  end of function TopTaggerAna::GetT3TopTagger  -----

// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::PassType3TopCrite
//  Description:  
// ===========================================================================
bool TopTaggerAna::PassType3TopCrite(topTagger::type3TopTagger* type3TopTaggerPtr, int ic) const
{
    double fatJetm123 = type3TopTaggerPtr->fatJetMassVec[ic];
    // Find a top fat jet passing at least one of the three criteria
    std::vector<int> fatJetPassStatusVec;
    int isTopJet = type3TopTaggerPtr->checkTopCriteria(type3TopTaggerPtr->finalCombfatJets[ic], 
        *oriJetsVec, *recoJetsBtagCSVS, type3TopTaggerPtr->fatJetSubMassVec[ic], fatJetm123, fatJetPassStatusVec);

    if (isTopJet != 1) return false;

    TLorentzVector jjjTop(0, 0, 0, 0);
    for (size_t k = 0; k < type3TopTaggerPtr->finalCombfatJets.at(ic).size(); ++k)
    {
      jjjTop += oriJetsVec->at(type3TopTaggerPtr->finalCombfatJets.at(ic).at(k));
    }
    if (jjjTop.M() < 80 || jjjTop.M() > 270 ) return false;
    return true;
}       // -----  end of function TopTaggerAna::PassType3TopCrite  -----


// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::CalTaggerEff
//  Description:  
// ===========================================================================
bool TopTaggerAna::CalTaggerEff(std::vector<int> toptags) const
{
  for (int i = 0; i < toptags.size(); ++i)
  {
    
  }

  return true;
}       // -----  end of function TopTaggerAna::CalTaggerEff  -----

