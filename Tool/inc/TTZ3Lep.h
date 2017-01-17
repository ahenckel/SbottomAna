// ===========================================================================
// 
//       Filename:  TTZ3Lep.h
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

#ifndef  __TTZ3LEP_INC__
#define  __TTZ3LEP_INC__

// stdlib
#include <cassert>

// ROOT

// User class
#include "ComAna.h"
#include "HistTool.hh"
#include "RootTools.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/customize.h"

// ===========================================================================
//        Class:  TTZ3Lep
//  Description:  S
// ===========================================================================
class TTZ3Lep : public ComAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    TTZ3Lep (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile, std::string spec_="");                             
    // constructor
    TTZ3Lep ( const TTZ3Lep &other );   // copy constructor
    ~TTZ3Lep ();                            // destructor
    TTZ3Lep* Clone(std::string newname, std::shared_ptr<TFile> OutFile_);

    // ====================  ACCESSORS     ===============================
    bool InitCutOrder(std::string ana);
    bool CheckCut();
    bool BookHistograms();
    //bool WriteHistogram();
    bool FillCut();

    bool HasZ();
    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    TTZ3Lep& operator = ( const TTZ3Lep &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    bool Pass3Leps(int pt1, int pt2, int pt3, bool isEle);

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================
    bool Check3rdLep(int NCut);

    // ====================  DATA MEMBERS  ===============================

}; // -----  end of class TTZ3Lep  -----

#endif   // ----- #ifndef __TTZ3LEP_INC__  -----
