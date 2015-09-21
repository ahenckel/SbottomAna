// ===========================================================================
// 
//       Filename:  SBDiJet.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/21/2015 17:42:24
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================


#ifndef  __SBDIJET_INC__
#define  __SBDIJET_INC__

// stdlib
#include <cassert>

// ROOT

// User class
#include "ComAna.h"
#include "HistTool.hh"
#include "NTupleReader.h"
#include "RootTools.h"

// ===========================================================================
//        Class:  SBDiJet
//  Description:  
// ===========================================================================
class SBDiJet : public ComAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    SBDiJet (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile);
    SBDiJet ( const SBDiJet &other );   // copy constructor
    ~SBDiJet ();                            // destructor

    // ====================  ACCESSORS     ===============================

    bool InitCutOrder(std::string ana);
    bool CheckCut();
    bool BookHistograms();
    //bool WriteHistogram();
    bool FillCut();

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    SBDiJet& operator = ( const SBDiJet &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================
    std::bitset<NBITS> cutbit;

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================
    bool FillCut(int NCut);
    
    // ====================  DATA MEMBERS  ===============================
    std::vector<std::string> CutOrder;
    std::map<std::string, std::string>  CutMap;

    
    //  local variable
    double MTJ2MET ;
    
}; // -----  end of class SBDiJet  -----


#endif   // ----- #ifndef __SBDIJET_INC__  -----
