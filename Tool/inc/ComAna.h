// ===========================================================================
// 
//       Filename:  ComAna.h
// 
//    Description:  A common analysis class, aiming for cocmparison between
//    different cutflow and optimization
// 
//        Version:  1.0
//        Created:  08/24/2015 14:57:29
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

#ifndef  __COMANA_INC__
#define  __COMANA_INC__

#include <sstream>
#include <bitset>
#include <memory>
#include "boost/bimap.hpp"

#include "HistTool.hh"
#include "RootTools.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/TopTagger/interface/Type3TopTagger.h"

#define NBITS 20

// ===========================================================================
//        Class:  ComAna
//  Description:  
// ===========================================================================
class ComAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    ComAna (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile);
    ComAna ( const ComAna &other );   // copy constructor
    ~ComAna ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool SetEvtWeight(double weight) const;
    virtual bool BookHistograms();
    virtual bool InitCutOrder(std::string ana) const;
    virtual bool FillCut();
    virtual bool FillCut(int NCut);
    bool WriteHistogram();
    bool DrawHistogram();
    bool RunEvent();
    // ====================  MUTATORS      ===============================
    // Move HistTool to public so that the CMSSW can book and fill some
    // histogram apart from the DelCut.
    // Warning: This is not healthy for the code! But we need to scarify for
    // the analysis
    HistTool *His; 

    // ====================  OPERATORS     ===============================

    ComAna& operator = ( const ComAna &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    int GetType3TopTagger();

    // ====================  DATA MEMBERS  ===============================
    std::vector<std::string> CutOrder;
    std::bitset<NBITS> cutbit;
    std::map<std::string, std::string>  CutMap;
    HistTool *his;
    NTupleReader *tr;

    bool BookTLVHistos(std::string name);
    bool FillTLVHistos(int NCut, std::string name, TLorentzVector TLV);

    bool Book2TLVHistos(std::string name);
    bool Fill2TLVHistos(int NCut, std::string name, TLorentzVector LV1, TLorentzVector LV2);

    TLorentzVector Jet1;
    TLorentzVector Jet2;
    TLorentzVector Jet3;
    TLorentzVector Jet4;

  private:
    // ====================  METHODS       ===============================
    virtual bool CheckCut();
    int CountJets(double jetPt) const;
    bool GetLeadingJets();


    bool PassType3TopCrite(topTagger::type3TopTagger* type3TopTaggerPtr, std::vector<TLorentzVector>& oriJetsVec, 
        std::vector<double>& recoJetsBtagCSVS, int ic) const;
    std::vector<int> SortToptager( boost::bimap<int, double > dm_bimap);
    // ====================  DATA MEMBERS  ===============================

    topTagger::type3TopTagger * type3Ptr;
    std::vector<TLorentzVector> vRecoTops;

    // Event variables
    int j30count;


}; // -----  end of class ComAna  -----
#endif   // ----- #ifndef __COMANA_INC__  -----
