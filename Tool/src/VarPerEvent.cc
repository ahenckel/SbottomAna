// ===========================================================================
// 
//       Filename:  VarPerEvent.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  10/13/2015 16:21:55
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#include "VarPerEvent.h"

// ===  FUNCTION  ============================================================
//         Name:  RegisterVarPerEvent
//  Description:  
// ===========================================================================
void RegisterVarPerEvent(NTupleReader &tr, topTagger::type3TopTagger * type3Ptr)
{
  VarPerEvent var(&tr, type3Ptr);
  var.RunPerEvent();
}       // -----  end of function RegisterVarPerEvent  -----

//----------------------------------------------------------------------------
//       Class:  VarPerEvent
//      Method:  VarPerEvent
// Description:  constructor
//----------------------------------------------------------------------------
VarPerEvent::VarPerEvent (NTupleReader *tr_, topTagger::type3TopTagger * type3Ptr_)
:tr(tr_),type3Ptr(type3Ptr_)
{
}  // -----  end of method VarPerEvent::VarPerEvent  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  VarPerEvent
//      Method:  VarPerEvent
// Description:  copy constructor
//----------------------------------------------------------------------------
VarPerEvent::VarPerEvent ( const VarPerEvent &other )
{
}  // -----  end of method VarPerEvent::VarPerEvent  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  VarPerEvent
//      Method:  ~VarPerEvent
// Description:  destructor
//----------------------------------------------------------------------------
VarPerEvent::~VarPerEvent ()
{
}  // -----  end of method VarPerEvent::-VarPerEvent  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  VarPerEvent
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  VarPerEvent&
VarPerEvent::operator = ( const VarPerEvent &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method VarPerEvent::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  VarPerEvent::RunPerEvent
//  Description:  
// ===========================================================================
bool VarPerEvent::RunPerEvent() const
{
  GetnTops();

  return true;
}       // -----  end of function VarPerEvent::RunPerEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  VarPerEvent::GetnTops
//  Description:  
// ===========================================================================
bool VarPerEvent::GetnTops() const
{
  int nTops = tr->getVar<int>("nTopCandSortedCnt");
  std::vector<TLorentzVector> *vTops = new std::vector<TLorentzVector>();
  std::map<int, std::vector<TLorentzVector> > *vTopJets = new std::map<int, std::vector<TLorentzVector> >();

  for(int it=0; it<nTops; it++)
  {
    TLorentzVector topLVec = type3Ptr->buildLVec(tr->getVec<TLorentzVector>("jetsLVec_forTagger"), 
        type3Ptr->finalCombfatJets[type3Ptr->ori_pickedTopCandSortedVec[it]]);
    vTops->push_back(topLVec);

    std::vector<TLorentzVector> temp;
    std::vector<int> indexCombVec = type3Ptr->finalCombfatJets[type3Ptr->ori_pickedTopCandSortedVec[it]];
    for (size_t k = 0; k < indexCombVec.size(); ++k)
    {
      temp.push_back( (tr->getVec<TLorentzVector>("jetsLVec_forTagger")).at(indexCombVec.at(k)));
    }
    vTopJets->insert(std::make_pair(it, temp));
  }

  tr->registerDerivedVec("vTops", vTops);
  tr->registerDerivedVec("mTopJets", vTopJets);

  return true;
}       // -----  end of function VarPerEvent::GetnTops  -----
