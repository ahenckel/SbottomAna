// ===========================================================================
// 
//       Filename:  Type3Bhad.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/13/2015 10:44:21
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

#include "Type3Bhad.h"

//----------------------------------------------------------------------------
//       Class:  Type3Bhad
//      Method:  Type3Bhad
// Description:  constructor
//----------------------------------------------------------------------------
Type3Bhad::Type3Bhad ()
{
  lowWin = 110;
  highWin = 240;
}  // -----  end of method Type3Bhad::Type3Bhad  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  Type3Bhad
//      Method:  ~Type3Bhad
// Description:  destructor
//----------------------------------------------------------------------------
Type3Bhad::~Type3Bhad ()
{
}  // -----  end of method Type3Bhad::-Type3Bhad  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  Type3Bhad
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  Type3Bhad&
Type3Bhad::operator = ( const Type3Bhad &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method Type3Bhad::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  Type3Bhad::RunTagger
//  Description:  
// ===========================================================================
int Type3Bhad::RunTagger(std::vector<TLorentzVector>  & jets, std::vector<double> & bjets ) 
{
  bhadCans.clear();
  topCans.clear();
  toppair.clear();
  
  std::vector<int> bhadIdx;
  for(unsigned int i=0; i < jets.size(); ++i)
  {
    TLorentzVector jet = jets.at(i);
    if (jet.M() > 20 && jet.M() < 170)
    {
      if (bjets.at(i) > 0.814 && fabs(jet.Eta()) < 2.4)
      {
        bhadCans.push_back(jet);
        bhadIdx.push_back(i);
      }
    }
  }

  for(unsigned int i=0; i < bhadCans.size(); ++i)
  {
    TLorentzVector bhad = bhadCans.at(i);
    for(unsigned int j=0; j < jets.size(); ++j)
    {
      TLorentzVector jet = jets.at(j);
      if (bhad.DeltaR(jet) > 1.5) continue;
      if (bhadIdx.at(i) == int(j)) continue;
      if (bhad.M() < jet.M() ) continue;
      if (bjets.at(j) > 0.814) continue;

      TLorentzVector topCan = bhad + jet;
      if (topCan.M() > lowWin && topCan.M() < highWin  )
      {
        toppair.push_back(std::make_pair(bhadIdx.at(i), j));
        topCans.push_back(topCan);
      }
    }
  }

  return topCans.size();
}       // -----  end of function Type3Bhad::RunTagger  -----

// ===  FUNCTION  ============================================================
//         Name:  Type3Bhad::SetTopWindow
//  Description:  
// ===========================================================================
bool Type3Bhad::SetTopWindow(int lowWin_, int highWin_)
{
  lowWin = lowWin_;
  highWin = highWin_;
  return true;
}       // -----  end of function Type3Bhad::SetTopWindow  -----
