// ===========================================================================
// 
//       Filename:  VarPerEvent.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  10/13/2015 16:22:25
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  __VARPEREVENT_INC__
#define  __VARPEREVENT_INC__

#include "boost/bimap.hpp"
#include "boost/bimap/multiset_of.hpp"
#include "boost/foreach.hpp"

#include "TRandom3.h"
#include "TMath.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/baselineDef.h"
#include "SusyAnaTools/TopTagger/interface/Type3TopTagger.h"

void RegisterVarPerEvent(NTupleReader &tr);
typedef std::map<unsigned int, std::pair<unsigned int, unsigned int> > TypeZLepIdx;
// ===========================================================================
//        Class:  VarPerEvent
//  Description:  A local class which will be called per event, to calculate
//  local variables, following baselineDef structure
// ===========================================================================
class VarPerEvent
{
  public:

    // ====================  LIFECYCLE     ===============================
    VarPerEvent (NTupleReader *tr_);
    ~VarPerEvent ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool RunPerEvent() const;

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    VarPerEvent& operator = ( const VarPerEvent &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    bool GetJetsNoMu() const;

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================
    bool PassDiMuonTrigger() const;
    bool PassDiEleTrigger() const;
    bool GetRecoZ() const;
    bool GetMuInfo(std::vector<TLorentzVector>* recoZVec, TypeZLepIdx *ZLepIdx) const;
    bool GetEleZ(std::vector<TLorentzVector>* recoZVec, TypeZLepIdx *ZLepIdx) const;

    // ====================  DATA MEMBERS  ===============================
    NTupleReader *tr;

}; // -----  end of class VarPerEvent  -----


#endif   // ----- #ifndef __VARPEREVENT_INC__  -----
