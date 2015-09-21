// ===========================================================================
// 
//       Filename:  STISR.h
// 
//    Description:  Based on http://arxiv.org/abs/1506.00653
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

#ifndef  __STISR_INC__
#define  __STISR_INC__

// stdlib
#include <cassert>

// ROOT

// User class
#include "ComAna.h"
#include "HistTool.hh"
#include "NTupleReader.h"
#include "RootTools.h"
// ===========================================================================
//        Class:  STISR
//  Description:  S
// ===========================================================================
class STISR : public ComAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    STISR (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile);
    // constructor
    STISR ( const STISR &other );   // copy constructor
    ~STISR ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool InitCutOrder(std::string ana);
    bool CheckCut();
    bool BookHistograms();
    bool FillCut();

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    STISR& operator = ( const STISR &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================
    std::bitset<NBITS> cutbit;

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================
    std::vector<std::string> CutOrder;
    std::map<std::string, std::string>  CutMap;

}; // -----  end of class STISR  -----

#endif   // ----- #ifndef __STISR_INC__  -----
