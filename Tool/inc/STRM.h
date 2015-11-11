// ===========================================================================
// 
//       Filename:  STRM.h
// 
//    Description:  An analysis implementing arXiv: 1506.00653
// 
//        Version:  1.0
//        Created:  11/03/2015 16:36:40
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  MY_STRM_INC
#define  MY_STRM_INC

// stdlib
#include <cassert>
#include <functional>   // std::bind

// ROOT
#include "TMath.h"

// User class
#include "ComAna.h"
#include "HistTool.hh"
#include "TopTaggerAna.h"
#include "RootTools.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
// ===========================================================================
//        Class:  STRM
//  Description:  
// ===========================================================================
class STRM : public ComAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    STRM (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile);
    // constructor
    ~STRM ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool InitCutOrder(std::string ana);
    bool CheckCut();
    bool BookHistograms();
    bool WriteHistogram();
    bool FillCut();

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    STRM& operator = ( const STRM &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

}; // -----  end of class STRM  -----

#endif   // ----- #ifndef MY_STRM_INC  -----
