// ===========================================================================
// 
//       Filename:  StopAna.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  10/13/2015 16:05:55
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  __STOPANA_INC__
#define  __STOPANA_INC__

// stdlib
#include <cassert>

// ROOT

// User class
#include "ComAna.h"
#include "HistTool.hh"
#include "RootTools.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/searchBins.h"

// ===========================================================================
//        Class:  StopAna
//  Description:  
// ===========================================================================
class StopAna : public ComAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    StopAna (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile, std::string spec_="");                             // constructor
    StopAna ( const StopAna &other );   // copy constructor
    ~StopAna ();                            // destructor
    StopAna* Clone(std::string newname, std::shared_ptr<TFile> OutFile_);

    // ====================  ACCESSORS     ===============================
    bool InitCutOrder(std::string ana);
    bool CheckCut();
    bool BookHistograms();
    bool FillCut();

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    StopAna& operator = ( const StopAna &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================
    bool FillSearchBins(int NCut);
    bool IsGenWLep() const;
    bool IsGenZLep() const;

    SearchBins *sb;
    // ====================  DATA MEMBERS  ===============================

}; // -----  end of class StopAna  -----


#endif   // ----- #ifndef __STOPANA_INC__  -----
