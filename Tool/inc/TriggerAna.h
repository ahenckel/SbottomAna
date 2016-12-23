// ===========================================================================
// 
//       Filename:  TriggerAna.h
// 
//    Description:  Class for analysis trigger efficiency 
// 
//        Version:  1.0
//        Created:  11/28/16 16:23:00
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  MY_TRIGGERANA_INC
#define  MY_TRIGGERANA_INC

// stdlib
#include <cassert>

// ROOT

// User class
#include "ComAna.h"
#include "HistTool.hh"
#include "RootTools.h"

#include "SusyAnaTools/Tools/NTupleReader.h"


// ===========================================================================
//        Class:  TriggerAna
//  Description:  
// ===========================================================================
class TriggerAna : public ComAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    TriggerAna (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile, std::string spec_="");                             // constructor
    TriggerAna ( const TriggerAna &other );   // copy constructor
    ~TriggerAna ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool InitCutOrder(std::string ana);
    bool CheckCut();
    bool BookHistograms();
    bool FillCut();

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    TriggerAna& operator = ( const TriggerAna &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    bool FillMETEff(int NCut);
    bool FillMuonEff(int NCut);

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

}; // -----  end of class TriggerAna  -----


#endif   // ----- #ifndef MY_TRIGGERANA_INC  -----
