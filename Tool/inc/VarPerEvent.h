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


#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/TopTagger/interface/Type3TopTagger.h"

void RegisterVarPerEvent(NTupleReader &tr, topTagger::type3TopTagger * type3Ptr);
// ===========================================================================
//        Class:  VarPerEvent
//  Description:  A local class which will be called per event, to calculate
//  local variables, following baselineDef structure
// ===========================================================================
class VarPerEvent
{
  public:

    // ====================  LIFECYCLE     ===============================
    VarPerEvent (NTupleReader *tr_, topTagger::type3TopTagger * type3Ptr_);
    VarPerEvent ( const VarPerEvent &other );   // copy constructor
    ~VarPerEvent ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool RunPerEvent() const;

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    VarPerEvent& operator = ( const VarPerEvent &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    bool GetnTops() const;

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================
    NTupleReader *tr;
	topTagger::type3TopTagger * type3Ptr;

}; // -----  end of class VarPerEvent  -----


#endif   // ----- #ifndef __VARPEREVENT_INC__  -----
