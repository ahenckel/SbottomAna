// ===========================================================================
// 
//       Filename:  SBISR.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  09/20/2015 20:25:53
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

#ifndef  __SBISR_INC__
#define  __SBISR_INC__

// stdlib
#include <cassert>

// ROOT

// User class
#include "ComAna.h"
#include "HistTool.hh"
#include "RootTools.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
// ===========================================================================
//        Class:  SBISR
//  Description:  S
// ===========================================================================
class SBISR : public ComAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    SBISR (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile);
    // constructor
    SBISR ( const SBISR &other );   // copy constructor
    ~SBISR ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool InitCutOrder(std::string ana);
    bool CheckCut();
    bool BookHistograms();
    bool FillCut();

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    SBISR& operator = ( const SBISR &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

}; // -----  end of class SBISR  -----

#endif   // ----- #ifndef __SBISR_INC__  -----
