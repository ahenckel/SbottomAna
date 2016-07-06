// ===========================================================================
// 
//       Filename:  STZinv.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  11/16/2015 11:30:57
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  MY_STZINV_INC
#define  MY_STZINV_INC

// stdlib
#include <cassert>
#include <algorithm>
#include <string>
#include <cstring>

// ROOT

// User class
#include "ComAna.h"
#include "HistTool.hh"
#include "RootTools.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/baselineDef.h"
#include "SusyAnaTools/Tools/searchBins.h"

// ===========================================================================
//        Class:  STZinv
//  Description:  
// ===========================================================================
class STZinv : public ComAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    STZinv (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile, std::string spec_="");                             // constructor
    STZinv ( const STZinv &other );   // copy constructor
    ~STZinv ();                            // destructor
    STZinv* Clone(std::string newname, std::shared_ptr<TFile> OutFile_);

    // ====================  ACCESSORS     ===============================

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    STZinv& operator = ( const STZinv &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    bool InitCutOrder(std::string ana);
    bool CheckCut();
    bool BookHistograms();
    bool FillCut();
    bool FillSearchBins(int NCut);
    // ====================  DATA MEMBERS  ===============================
    bool FillZMET(int NCut);

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================
    SearchBins sb;

}; // -----  end of class STZinv  -----

#endif   // ----- #ifndef MY_STZINV_INC  -----
