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
#include <regex>
#include "boost/bimap.hpp"

#include "HistTool.hh"
#include "RootTools.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
#include "SusyAnaTools/Tools/SATException.h"
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
    virtual ComAna* Clone(std::string newname, std::shared_ptr<TFile> OutFile_=NULL);

    // ====================  ACCESSORS     ===============================
    bool SetEvtWeight(double weight);
    bool SetEvtWeight(std::string name);
    bool SetRateWeight(double weight);
    bool SaveCutHist(bool choice) const;
    virtual bool BookHistograms();
    virtual bool InitCutOrder(std::string ana);
    virtual bool FillCut();
    virtual bool FillCut(int NCut);
    virtual bool WriteHistogram();
    bool DrawHistogram();
    bool RunEvent();
    bool SetSysVar(std::string &Sysbit_, std::string &SysVar_, std::string SysCentral_="");
    std::vector<std::string> GetHLTsrc() const;
    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    ComAna& operator = ( const ComAna &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================
    std::string spec;
    std::string AnaName;
    std::string jetVecLabel;
    std::string CSVVecLabel;
    std::string METLabel   ;
    std::string METPhiLabel;

  protected:
    // ====================  METHODS       ===============================
    bool BookTLVHistos(std::string name);
    bool FillTLVHistos(int NCut, std::string name, TLorentzVector TLV);

    bool Book2TLVHistos(std::string name);
    bool Fill2TLVHistos(int NCut, std::string name, TLorentzVector LV1, TLorentzVector LV2);

    bool DefineLabels(std::string spec);
    bool PassTrigger( std::vector<std::string> Trigstr_ = {});
    bool IsData();
    bool CheckLeadingLeptons(int NCut);
    bool IsUpdateHLT();
    int CountJets(double jetPt) const;
    // ====================  DATA MEMBERS  ===============================
    std::vector<std::string> CutOrder;
    std::bitset<NBITS> cutbit;
    std::map<std::string, std::string>  CutMap;
    HistTool *his;
    NTupleReader *tr;
    std::vector<std::string> PretriggerName;

    TLorentzVector Jet1;
    TLorentzVector Jet2;
    TLorentzVector Jet3;
    TLorentzVector Jet4;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Input Labels ~~~~~
    bool isData;
    std::vector<std::string> HLTstr;
    std::vector<std::string> MCTrigstr;
    std::map<std::string, std::set<unsigned int> > HLTIdx;
    // Label defined in derived, which changed by spec name
    std::map<std::string, std::string> Label;
    std::shared_ptr<TFile> OutFile;

    // Systematics
    std::bitset<2> Sysbit;
    std::string SysVarName;
    std::string SysVarCentral;

  private:
    // ====================  METHODS       ===============================
    virtual bool CheckCut();
    bool GetLeadingJets();


    bool PassType3TopCrite(topTagger::type3TopTagger* type3TopTaggerPtr, std::vector<TLorentzVector>& oriJetsVec, 
        std::vector<double>& recoJetsBtagCSVS, int ic) const;
    // ====================  DATA MEMBERS  ===============================
    topTagger::type3TopTagger * type3Ptr;
    std::vector<TLorentzVector> vRecoTops;

    // Event variables
    int j30count;
    double NBaseWeight;
    double ShapeWeight;
    

}; // -----  end of class ComAna  -----

#endif   // ----- #ifndef MY__COMANA__INC  -----
