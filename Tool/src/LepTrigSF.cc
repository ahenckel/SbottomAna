// ===========================================================================
// 
//       Filename:  LepTrigSF.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  02/10/2017 02:14:14 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
// 
// ===========================================================================


#include "LepTrigSF.h"

//----------------------------------------------------------------------------
//       Class:  LepTrigSF
//      Method:  LepTrigSF
// Description:  constructor
//----------------------------------------------------------------------------
LepTrigSF::LepTrigSF (NTupleReader* tr_, std::string LepSFFileName):
tr(tr_)
{
  InitLeptonSF(LepSFFileName);
}  // -----  end of method LepTrigSF::LepTrigSF  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  LepTrigSF
//      Method:  LepTrigSF
// Description:  copy constructor
//----------------------------------------------------------------------------
LepTrigSF::LepTrigSF ( const LepTrigSF &other )
{
}  // -----  end of method LepTrigSF::LepTrigSF  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  LepTrigSF
//      Method:  ~LepTrigSF
// Description:  destructor
//----------------------------------------------------------------------------
LepTrigSF::~LepTrigSF ()
{
}  // -----  end of method LepTrigSF::-LepTrigSF  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  LepTrigSF
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  LepTrigSF&
LepTrigSF::operator = ( const LepTrigSF &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method LepTrigSF::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  LepTrigSF::GetEventSF
//  Description:  
// ===========================================================================
float LepTrigSF::GetEventSF(ComAna *&ana, std::string applybit_)
{
  if (tr->getVar<int>("run") != 1) // Set weight to 1 for Data
    return 1;

  float LTsf = 1;
  curSpec = ana->spec;
  std::bitset<2> applybit(applybit_);

  if (applybit.test(0))
    LTsf *= GetLeptonSF(ana);
  if (applybit.test(1))
    LTsf *= GetTriggerEff(ana);
  return LTsf;
}       // -----  end of function LepTrigSF::GetEventSF  -----


// ===  FUNCTION  ============================================================
//         Name:  LepTrigSF::GetTriggerEff
//  Description:  
// ===========================================================================
float LepTrigSF::GetTriggerEff(ComAna *ana)
{
  // No Trigger SF for trigger study
  if (ana->AnaName.find("Trig") != std::string::npos)
    return 1.;

  std::vector<std::string> hlt = ana->GetHLTsrc();

  if(std::find(hlt.begin(), hlt.end(), "HLT_Ele27_WPTight_Gsf_v\\d") != hlt.end())
    return GetElecTrigEff();
  
  if(std::find(hlt.begin(), hlt.end(), "HLT_Mu50_v\\d") != hlt.end())
    return GetMuonTrigEff();

  if(std::find(hlt.begin(), hlt.end(), "HLT_PFMET100_PFMHT100_IDTight_v\\d") != hlt.end())
    return GetMETTrigEff();

  return 1.;
}       // -----  end of function LepTrigSF::GetTriggerEff  -----

// ===  FUNCTION  ============================================================
//         Name:  LepTrigSF::GetElecTrigEff
//  Description:  
// ===========================================================================
float LepTrigSF::GetElecTrigEff()
{
  const std::vector<float> eleEtabins = {};
  const std::vector<float> eleEtaPts = {};
  const float eleTrigPTcut = 40;
  const std::vector<TLorentzVector> &elesLVec   = tr->getVec<TLorentzVector>("cutEleVec"+curSpec);

  if (elesLVec.empty()) return 0.;

  double LeadingPt =  -1;
  for(auto i : elesLVec)
  {
    if (i.Pt() > LeadingPt)
    {
      LeadingPt = i.Pt();
    }
  }
  if (LeadingPt < eleTrigPTcut) return 0.;

  return true;
}       // -----  end of function LepTrigSF::GetElecTrigEff  -----

// ===  FUNCTION  ============================================================
//         Name:  LepTrigSF::GetMuonTrigEff
//  Description: 
// ===========================================================================
float LepTrigSF::GetMuonTrigEff() 
{
  std::vector<float> muonEtabins = {};
  std::vector<float> muonEtaPts = {};


  return true;
}       // -----  end of function LepTrigSF::GetMuonTrigEff  -----


// ===  FUNCTION  ============================================================
//         Name:  LepTrigSF::GetMETTrigEff
//  Description:  /* cursor */
// ===========================================================================
float LepTrigSF::GetMETTrigEff()
{
  return true;
}       // -----  end of function LepTrigSF::GetMETTrigEff  -----


// ===  FUNCTION  ============================================================
//         Name:  LepTrigSF::InitLeptonSF
//  Description:  
// ===========================================================================
bool LepTrigSF::InitLeptonSF(std::string LepSFFileName)
{
  TFile * LepSFfile = new TFile(LepSFFileName.c_str());
  assert( !LepSFfile->IsZombie());
  LepSF2D["mu_mediumID_SF"]  = (TH2D*) LepSFfile->Get("pt_abseta_PLOT_pair_probeMultiplicity_bin0");
  LepSF2D["mu_miniISO_SF"]   = (TH2D*) LepSFfile->Get("pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_Medium2016_pass");
  LepSF2D["ele_VetoID_SF"]   = (TH2D*) LepSFfile->Get("GsfElectronToVeto");
  LepSF2D["ele_miniISO_SF"]  = (TH2D*) LepSFfile->Get("MVAVLooseElectronToMini");
  LepSF2D["ele_trkpt_SF"]    = (TH2D*) LepSFfile->Get("EGamma_SF2D");
  LepSF1D["mu_trkptGT10_SF"] = (TH1D*) LepSFfile->Get("mutrksfptg10");
  LepSF1D["mu_trkptLT10_SF"] = (TH1D*) LepSFfile->Get("mutrksfptl10");
  return true;
}       // -----  end of function LepTrigSF::InitLeptonSF  -----

// ===  FUNCTION  ============================================================
//         Name:  LepTrigSF::GetLeptonSF
//  Description:  
// ===========================================================================
float LepTrigSF::GetLeptonSF(ComAna* ana)
{
  LepSFVec.clear();
  const std::string muontype =  tr->getVar<std::string>("cutMuID"+ curSpec);
  const std::string eletype =  tr->getVar<std::string>("cutEleID"+ curSpec);

  if (muontype == "muonsFlagMedium" )
    LepSFVec["muon"] = { "mu_mediumID_SF", "mu_miniISO_SF", "mu_trkptLT10_SF", "mu_trkptGT10_SF"};
  if (muontype == "elesFlagVeto" )
    LepSFVec["ele"] = { "ele_VetoID_SF", "ele_miniISO_SF", "ele_trkpt_SF"};

  float lepsf = 1.;
  lepsf *= GetMuonSF() * GetEleSF();
  return lepsf;
}       // -----  end of function LepTrigSF::GetLeptonSF  -----

// ===  FUNCTION  ============================================================
//         Name:  LepTrigSF::GetMuonSF
//  Description:  
// ===========================================================================
float LepTrigSF::GetMuonSF()
{
  const std::vector<TLorentzVector> &muonsLVec   = tr->getVec<TLorentzVector>("cutMuVec"+curSpec);
  float returnweight = 1.0;
  for(auto mu : muonsLVec)
  {
    float thismuweight = 1.0;
    const float eta = mu.Eta();
    const float pt = mu.Pt();
    const float abseta = std::abs(eta);

    for(auto sf : LepSFVec["muon"])
    {
      float tempweight = 1.0;
      if (sf.find("ID") != std::string::npos || sf.find("ISO") != std::string::npos )
      {
        if (LepSF2D.find(sf) == LepSF2D.end() || LepSF2D[sf] == NULL) continue;
        tempweight  = LepSF2D[sf]->GetBinContent(LepSF2D[sf]->FindBin(pt, abseta));
        if( tempweight == 0 ) tempweight = 1.0; // very simple way dealing with out of range issue of the TH2D
      }
      if (sf.find("trkpt") != std::string::npos)
      {
        if (sf == "mu_trkptGT10_SF" && pt < 10) continue;
        if (sf == "mu_trkptLT10_SF" && pt > 10) continue;
        if (LepSF1D.find(sf) == LepSF1D.end() || LepSF1D[sf] == NULL) continue;
        tempweight  = LepSF1D[sf]->GetBinContent(LepSF1D[sf]->FindBin(eta));
        if( tempweight == 0 ) tempweight = 1.0; // very simple way dealing with out of range issue of the TH2D
      }
      thismuweight *= tempweight;
    }
    returnweight *= thismuweight;
  }

  return returnweight;
}       // -----  end of function LepTrigSF::GetMuonSF  -----

// ===  FUNCTION  ============================================================
//         Name:  LepTrigSF::GetEleSF
//  Description:  
// ===========================================================================
float LepTrigSF::GetEleSF()
{
  const std::vector<TLorentzVector> &ElesLVec   = tr->getVec<TLorentzVector>("cutEleVec"+curSpec);
  float returnweight = 1.0;
  for(auto ele : ElesLVec)
  {
    float thiseleweight = 1.0;
    const float eta = ele.Eta();
    const float pt = ele.Pt();
    const float abseta = std::abs(eta);

    for(auto sf : LepSFVec["ele"])
    {
      float tempweight = 1.0;
      if (sf.find("ID") != std::string::npos || sf.find("ISO") != std::string::npos )
      {
        if (LepSF2D.find(sf) == LepSF2D.end() || LepSF2D[sf] == NULL) continue;
        tempweight  = LepSF2D[sf]->GetBinContent(LepSF2D[sf]->FindBin(pt, abseta));
        if( tempweight == 0 ) tempweight = 1.0; // very simple way dealing with out of range issue of the TH2D
      }
      if (sf.find("trkpt") != std::string::npos)
      {
        if (LepSF2D.find(sf) == LepSF2D.end() || LepSF2D[sf] == NULL) continue;
        tempweight  = LepSF2D[sf]->GetBinContent(LepSF2D[sf]->FindBin(eta, pt));
        if( tempweight == 0 ) tempweight = 1.0; // very simple way dealing with out of range issue of the TH2D
      }
      thiseleweight *= tempweight;
    }
    returnweight *= thiseleweight;
  }

  return returnweight;
}       // -----  end of function LepTrigSF::GetEleSF  -----
