// ===========================================================================
// 
//       Filename:  TTZDiLep.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  10/07/2015 11:05:16
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  __TTZDiLep_INC__
#define  __TTZDiLep_INC__

// stdlib
#include <cassert>

// ROOT

// User class
#include "ComAna.h"
#include "HistTool.hh"
#include "RootTools.h"

#include "SusyAnaTools/Tools/NTupleReader.h"

// ===========================================================================
//        Class:  TTZDiLep
//  Description:  S
// ===========================================================================
class TTZDiLep : public ComAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    TTZDiLep (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile);                             
    // constructor
    TTZDiLep ( const TTZDiLep &other );   // copy constructor
    ~TTZDiLep ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool InitCutOrder(std::string ana);
    bool CheckCut();
    bool BookHistograms();
    bool FillCut();

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    TTZDiLep& operator = ( const TTZDiLep &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

}; // -----  end of class TTZDiLep  -----

#endif   // ----- #ifndef __TTZDiLep_INC__  -----
