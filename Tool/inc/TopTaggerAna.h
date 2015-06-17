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



#include "HistTool.hh"
#include "NTupleReader.h"

#include "TLorentzVector.h"

struct TopDecay 
{
  TLorentzVector topidx_;
  TLorentzVector bidx_;
  TLorentzVector Widx_;
  TLorentzVector Lepidx_;
  TLorentzVector neuidx_;
  TLorentzVector had1idx_;
  TLorentzVector had2idx_;
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

    std::vector<TLorentzVector> genDecayLVec     ;
    std::vector<int>            genDecayIdxVec   ;
    std::vector<int>            genDecayPdgIdVec ;
    std::vector<int>            genDecayMomIdxVec;











}; // -----  end of class TopTaggerAna  -----

