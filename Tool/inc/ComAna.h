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

#include "NTupleReader.h"
#include "HistTool.hh"
#include "RootTools.h"

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

    // ====================  DATA MEMBERS  ===============================
    HistTool *his;
    NTupleReader *tr;

  private:
    // ====================  METHODS       ===============================
    virtual bool CheckCut();
    int CountJets(double jetPt) const;
    bool GetLeadingJets();
    bool BookTLVHistos(std::string name);
    bool FillTLVHistos(int NCut, std::string name, TLorentzVector TLV);

    bool Book2TLVHistos(std::string name);
    bool Fill2TLVHistos(int NCut, std::string name, TLorentzVector LV1, TLorentzVector LV2);
    // ====================  DATA MEMBERS  ===============================
    std::bitset<NBITS> cutbit;
    std::vector<std::string> CutOrder;
    std::map<std::string, std::string>  CutMap;

    // Event variables
    int j30count;
    TLorentzVector Jet1;
    TLorentzVector Jet2;
    TLorentzVector Jet3;
    TLorentzVector Jet4;


}; // -----  end of class ComAna  -----

#endif   // ----- #ifndef __COMANA_INC__  -----
