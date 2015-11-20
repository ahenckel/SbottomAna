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

#ifndef  MY_COMANA_INC
#define  MY_COMANA_INC


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
    ComAna (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile, std::string spec="");
    ~ComAna ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool SetEvtWeight(double weight) const;
    bool SaveCutHist(bool choice) const;
    virtual bool BookHistograms();
    virtual bool InitCutOrder(std::string ana);
    virtual bool FillCut();
    virtual bool FillCut(int NCut);
    bool WriteHistogram();
    bool DrawHistogram();
    bool RunEvent();
    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    ComAna& operator = ( const ComAna &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    bool BookTLVHistos(std::string name);
    bool FillTLVHistos(int NCut, std::string name, TLorentzVector TLV);

    bool Book2TLVHistos(std::string name);
    bool Fill2TLVHistos(int NCut, std::string name, TLorentzVector LV1, TLorentzVector LV2);

    bool DefineLabels(std::string spec);
    bool PassTrigger();
    bool IsData();
    // ====================  DATA MEMBERS  ===============================
    std::vector<std::string> CutOrder;
    std::bitset<NBITS> cutbit;
    std::map<std::string, std::string>  CutMap;
    HistTool *his;
    NTupleReader *tr;

    TLorentzVector Jet1;
    TLorentzVector Jet2;
    TLorentzVector Jet3;
    TLorentzVector Jet4;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Input Labels ~~~~~
    bool isData;
    std::string spec;
    std::string jetVecLabel;
    std::string CSVVecLabel;
    std::string METLabel   ;
    std::string METPhiLabel;
    std::vector<std::string> HLTstr;
    std::vector<std::string> MCTrigstr;
    // Label defined in derived, which changed by spec name
    std::map<std::string, std::string> Label;

  private:
    // ====================  METHODS       ===============================
    virtual bool CheckCut();
    int CountJets(double jetPt) const;
    bool GetLeadingJets();


    bool PassType3TopCrite(topTagger::type3TopTagger* type3TopTaggerPtr, std::vector<TLorentzVector>& oriJetsVec, 
        std::vector<double>& recoJetsBtagCSVS, int ic) const;
    // ====================  DATA MEMBERS  ===============================
    topTagger::type3TopTagger * type3Ptr;
    std::vector<TLorentzVector> vRecoTops;

    // Event variables
    int j30count;
    

}; // -----  end of class ComAna  -----

#endif   // ----- #ifndef MY__COMANA__INC  -----
