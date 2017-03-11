// ===========================================================================
// 
//       Filename:  LepTrigSF.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/10/2017 02:14:49 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================

#ifndef  MY_LEPTRIGSF_INC
#define  MY_LEPTRIGSF_INC

#include <algorithm>

#include "TGraphAsymmErrors.h"
#include "ComAna.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
// ===========================================================================
//        Class:  LepTrigSF
//  Description:  Provide scale factor for trigger and leptons
// ===========================================================================
class LepTrigSF
{
  public:

    // ====================  LIFECYCLE     ===============================
    LepTrigSF (NTupleReader* tr_, std::string LepSFFileName="allINone_leptonSF_Moriond17.root");                             
    // constructor
    LepTrigSF ( const LepTrigSF &other );   // copy constructor
    ~LepTrigSF ();                            // destructor

    // ====================  ACCESSORS     ===============================
    float GetEventSF(ComAna *&ana, std::string applybit_="11");

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    LepTrigSF& operator = ( const LepTrigSF &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================
    float GetEleSF();
    float GetMuonSF();
    bool InitLeptonSF(std::string LepSFFileName);
    float GetMETTrigEff();
    float GetMuonTrigEff(int ptcut);
    float GetLeptonSF(ComAna* ana);
    float GetElecTrigEff();
    float GetTriggerEff(ComAna *ana);

    // ====================  DATA MEMBERS  ===============================
    bool debug;
    NTupleReader *tr;
    TFile *LepSFfile;
    std::string curSpec;
    std::map<std::string, TGraphAsymmErrors*> LepSFTG;
    std::map<std::string, TH1D*> LepSF1D;
    std::map<std::string, TH2D*> LepSF2D;
    std::map<std::string, std::vector<std::string> > LepSFVec;

}; // -----  end of class LepTrigSF  -----


#endif   // ----- #ifndef MY_LEPTRIGSF_INC  -----
