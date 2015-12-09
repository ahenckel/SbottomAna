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
#include <functional>   // std::bind

// ROOT

// User class
#include "ComAna.h"
#include "HistTool.hh"
#include "TopTaggerAna.h"
#include "RootTools.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
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
    STISR* Clone(std::string newname, std::shared_ptr<TFile> OutFile_);

    // ====================  ACCESSORS     ===============================
    bool InitCutOrder(std::string ana);
    bool CheckCut();
    bool BookHistograms();
    bool WriteHistogram();
    bool FillCut();

    std::map<std::string, bool> JetOrgAna(int NCut, std::string name, TLorentzVector &Jet);
    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    STISR& operator = ( const STISR &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================
    bool LJinTops();
    double LJMIso(double coneSize) const;
    double LJPTIso(double coneSize) const;
    bool FillLJAna(int i);

    // ====================  DATA MEMBERS  ===============================
    std::vector<std::string> JetType;
    TopTaggerAna *topAna;

    std::vector<TLorentzVector> jetsforTT;
    std::vector<TLorentzVector> jetsforTTplt;
}; // -----  end of class STISR  -----

#endif   // ----- #ifndef __STISR_INC__  -----
