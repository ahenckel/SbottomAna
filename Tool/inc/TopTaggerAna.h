// ===========================================================================
// 
//       Filename:  TopTaggerAna.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/16/2015 04:15:08 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  University of Illinois at Chicago, CMS@LPC
// 
// ===========================================================================



#ifndef  __TOPTAGGERANA_INC__
#define  __TOPTAGGERANA_INC__

#include <cassert>

#include "HistTool.hh"
#include "NTupleReader.h"

#include "TLorentzVector.h"
#include "SusyAnaTools/TopTagger/interface/Type3TopTagger.h"

struct TopDecay 
{
  int topidx_;
  int bidx_;
  int Widx_;
  int Lepidx_;
  int neuidx_;
  int had1idx_;
  int had2idx_;
  bool isLeptonic_ = false;
};
// ===========================================================================
//        Class:  TopTaggerAna
//  Description:  
// ===========================================================================
class TopTaggerAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    TopTaggerAna (std::string &name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile);
    TopTaggerAna ( const TopTaggerAna &other );                            // copy constructor
    ~TopTaggerAna ();                                                      // destructor

    // ====================  ACCESSORS     ===============================

    // ====================  MUTATORS      ===============================
    bool EndTest();
    bool Test();

    // ====================  OPERATORS     ===============================

    TopTaggerAna& operator = ( const TopTaggerAna &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    double retval;

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================
    HistTool *his;
    NTupleReader *tr;
    // ====================  DATA MEMBERS  ===============================

    std::vector<TLorentzVector> genDecayLVec       ;
    std::vector<int>            genDecayIdxVec     ;
    std::vector<int>            genDecayPdgIdVec   ;
    std::vector<int>            genDecayMomIdxVec;
    std::vector<std::string>    genDecayStrVec;




    std::vector<TopDecay> vTops;

    bool GetGenTop();
    int GetChild(int parent, std::vector<int> pdgs) const;
    std::vector<int> GetChilds(int parent, std::vector<int> pdgs) const;

    bool BookHistograms();

    topTagger::type3TopTagger * type3Ptr;

    bool PassType3TopCrite(topTagger::type3TopTagger* type3TopTaggerPtr, std::vector<TLorentzVector>& oriJetsVec, std::vector<double>& recoJetsBtagCSVS, int ic) const;
    std::vector<int> GetT3TopTagger(double ptcut, std::string jetstr, std::string bjstr, std::string metstr ) const;
    bool CalTaggerEff(std::vector<int> toptags) const;




}; // -----  end of class TopTaggerAna  -----

#endif   // ----- #ifndef __TOPTAGGERANA_INC__  -----
