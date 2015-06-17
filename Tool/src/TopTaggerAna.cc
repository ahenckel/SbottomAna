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
  std::cout<<"Run to \033[0;31m"<<__func__<<"\033[0m at \033[1;36m"<< __FILE__<<"\033[0m, line \033[0;34m"<< __LINE__<<"\033[0m"<< std::endl; 
  //std::cout << " test joe " <<  tr->getVar<double>("joe")<< std::endl;
  genDecayLVec      = tr->getVec<TLorentzVector> ("genDecayLVec");
  genDecayIdxVec    = tr->getVec<int>            ("genDecayIdxVec");
  genDecayPdgIdVec  = tr->getVec<int>            ("genDecayPdgIdVec");
  genDecayMomIdxVec = tr->getVec<int>            ("genDecayMomIdxVec");

  for (int i = 0; i < genDecayMomIdxVec.size(); ++i)
  {
    std::cout << " Idx" << genDecayIdxVec[i] <<" Pdg " << genDecayPdgIdVec[i] <<" Mom " << genDecayMomIdxVec[i] << " pt " << genDecayLVec[i].Pt() << std::endl;
  }

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

//// ===  FUNCTION  ============================================================
////         Name:  TopTaggerAna::BookHistograms
////  Description:  
//// ===========================================================================
//bool TopTaggerAna::BookHistograms()
//{
  //his->AddTH1("test", "TEST", 200, 0, 500);
  //his->AddTH1("test2", "TEST3", 200, 0, 1000);
  //return true;
//}       // -----  end of function TopTaggerAna::BookHistograms  -----
//
// ===  FUNCTION  ============================================================
//         Name:  TopTaggerAna::GetGenTop
//  Description:  
// ===========================================================================
bool TopTaggerAna::GetGenTop() 
{
  for (int i = 0; i < genDecayMomIdxVec.size(); ++i)
  {
    std::cout << " Idx" << genDecayIdxVec[i] <<" Pdg " << genDecayPdgIdVec[i] <<" Mom " << genDecayMomIdxVec[i] << " pt " << genDecayLVec[i].Pt() << std::endl;
    if (genDecayPdgIdVec[i] == 6)
    {
      
    }
  }

  return true;
}       // -----  end of function TopTaggerAna::GetGenTop  -----
