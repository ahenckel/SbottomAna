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
#include "Type3Bhad.h"

#include "TLorentzVector.h"
#include "SusyAnaTools/TopTagger/interface/Type3TopTagger.h"
#include "boost/bimap.hpp"

struct TopDecay 
{
  TopDecay(): topidx_(-1), bidx_(-1), Widx_(-1), Lepidx_(-1), neuidx_(-1),had1idx_(-1), had2idx_(-1), isLeptonic_(false) {}
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
    TopTaggerAna ();
    TopTaggerAna (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile);
    TopTaggerAna ( const TopTaggerAna &other );                            // copy constructor
    ~TopTaggerAna ();                                                      // destructor

    // ====================  ACCESSORS     ===============================
    int GetNTops() const;
    std::vector<TLorentzVector> RecoTops;
    std::vector<TopDecay> vTops;
    std::vector<int> Type3Jets;

    // ====================  MUTATORS      ===============================
    bool EndTest();
    bool RunTagger();
    bool GetT3TopTagger(double ptcut, std::string jetstr, std::string bjstr, std::string metstr );
    bool GetFatTopTagger(std::string jetstr);
    bool CheckRecoEvent();
    // ====================  OPERATORS     ===============================

    TopTaggerAna& operator = ( const TopTaggerAna &other ); // assignment operator
    bool GetBoverlapHad() const;
    bool SetType3BHad(bool run);
    bool SetType3BhadWindow(int low, int high) const;

    // ====================  DATA MEMBERS  ===============================
    bool goodreco; 

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
    double HT;




    std::vector<int> vToptagged;
    std::vector<TLorentzVector> jetsforTT;
    std::vector<double> bjsforTT;

    int GetGenTop();
    int GetChild(int parent, std::vector<int> pdgs) const;
    std::vector<int> GetChilds(int parent, std::vector<int> pdgs) const;
    bool FillGenTop();
    bool CalHT();

    bool BookHistograms();
    bool ZeroTopTagStudy() const;

    topTagger::type3TopTagger * type3Ptr;
    Type3Bhad * t3bhadPtr;
    bool SortToptager( boost::bimap<int, double > dm_bimap);
    bool PassType3TopCrite(topTagger::type3TopTagger* type3TopTaggerPtr, std::vector<TLorentzVector>& oriJetsVec, std::vector<double>& recoJetsBtagCSVS, int ic) const;

    bool CalTaggerEff() const;
    bool CalMisTag();
    bool RunType3BHad;

    std::vector<int> SortType3Tagger( boost::bimap<int, double > & dm_bimap);

}; // -----  end of class TopTaggerAna  -----

#endif   // ----- #ifndef __TOPTAGGERANA_INC__  -----
