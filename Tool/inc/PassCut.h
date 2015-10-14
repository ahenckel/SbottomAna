// ===========================================================================
// 
//       Filename:  PassCut.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  09/21/2015 12:21:10
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

#ifndef  __PASSCUT_INC__
#define  __PASSCUT_INC__

// stdlib
#include <cassert>

// ROOT

// User class
#include "ComAna.h"
#include "HistTool.hh"
#include "RootTools.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
// ===========================================================================
//        Class:  PassCut
//  Description:  S
// ===========================================================================
class PassCut : public ComAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    PassCut (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile);   
    // constructor
    PassCut ( const PassCut &other );   // copy constructor
    ~PassCut ();                            // destructor

    // ====================  ACCESSORS     ===============================

    bool FillCut();
    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    PassCut& operator = ( const PassCut &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

}; // -----  end of class PassCut  -----

#endif   // ----- #ifndef __PASSCUT_INC__  -----
