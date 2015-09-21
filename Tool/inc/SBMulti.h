// ===========================================================================
// 
//       Filename:  SBMulti.h
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

#ifndef  __SBMULTI_INC__
#define  __SBMULTI_INC__

// stdlib
#include <cassert>

// ROOT

// User class
#include "ComAna.h"
#include "HistTool.hh"
#include "NTupleReader.h"
#include "RootTools.h"

// ===========================================================================
//        Class:  SBMulti
//  Description:  S
// ===========================================================================
class SBMulti : public ComAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    SBMulti (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile);
    // constructor
    SBMulti ( const SBMulti &other );   // copy constructor
    ~SBMulti ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool InitCutOrder(std::string ana);
    bool CheckCut();
    bool BookHistograms();
    bool FillCut();

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    SBMulti& operator = ( const SBMulti &other ); // assignment operator

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

}; // -----  end of class SBMulti  -----

#endif   // ----- #ifndef __SBMULTI_INC__  -----
