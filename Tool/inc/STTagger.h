// ===========================================================================
// 
//       Filename:  STTagger.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/09/2016 07:51:12
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  MY_STTAGGER_INC
#define  MY_STTAGGER_INC

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
//        Class:  STTagger
//  Description:  
// ===========================================================================
class STTagger : public ComAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    STTagger (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile, std::string spec_="");              // constructor
    STTagger ( const STTagger &other );   // copy constructor
    ~STTagger ();                            // destructor

    // ====================  ACCESSORS     ===============================

    // ====================  MUTATORS      ===============================
    STTagger* Clone(std::string newname, std::shared_ptr<TFile> OutFile_);
    bool BookHistograms();
    bool InitCutOrder(std::string ana);
    bool CheckCut();
    bool FillCut();
    bool WriteHistogram();
    bool GetEff();

    bool GetRecoTops() const;
    bool FillGenTop();
    bool CalTaggerEff();
    std::vector<int> GetChilds(int parent, std::vector<int> pdgs) const;
    int GetChild(int parent, std::vector<int> pdgs) const;
    bool GetRecoTops();
    bool GetGenTop();
    // For JME Eff
    bool GetMuon45();
    double GetHTLep() const;
    bool BookJMEHist();
    bool FillJMEEff();
    TLorentzVector GetBestComb();
    // ====================  OPERATORS     ===============================

    STTagger& operator = ( const STTagger &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================
    std::vector<TLorentzVector> genDecayLVec       ;
    std::vector<int>            genDecayIdxVec     ;
    std::vector<int>            genDecayPdgIdVec   ;
    std::vector<int>            genDecayMomIdxVec;
    std::vector<std::string>    genDecayStrVec;

    std::vector<TLorentzVector> vRecoTops;
    std::vector<TopDecay> vGenTops;

    std::vector<TLorentzVector> vMuon45;
}; // -----  end of class STTagger  -----


#endif   // ----- #ifndef MY_STTAGGER_INC  -----
