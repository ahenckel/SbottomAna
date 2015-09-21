// ===========================================================================
// 
//       Filename:  PassCut.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  09/21/2015 12:20:50
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

#include "PassCut.h"

//----------------------------------------------------------------------------
//       Class:  PassCut
//      Method:  PassCut
// Description:  constructor
//----------------------------------------------------------------------------
PassCut::PassCut (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile)
: ComAna(name, tr_, OutFile)
{
  ComAna::BookHistograms();
  his->AddTH1("NBase", "Number of Events passed baseline", 2, 0, 2);
}  // -----  end of method PassCut::PassCut  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  PassCut
//      Method:  PassCut
// Description:  copy constructor
//----------------------------------------------------------------------------
PassCut::PassCut ( const PassCut &other ): ComAna(other)
{
}  // -----  end of method PassCut::PassCut  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  PassCut
//      Method:  ~PassCut
// Description:  destructor
//----------------------------------------------------------------------------
PassCut::~PassCut ()
{
}  // -----  end of method PassCut::-PassCut  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  PassCut
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  PassCut&
PassCut::operator = ( const PassCut &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method PassCut::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  PassCut::FillCut
//  Description:  
// ===========================================================================
bool PassCut::FillCut() 
{
  ComAna::RunEvent();
  ComAna::FillCut(-1);
  return true;
}       // -----  end of function PassCut::FillCut  -----
