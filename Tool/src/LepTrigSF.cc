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
tr(tr_), debug(false)
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

  float Lepsf = 1;
  float Trigsf = 1;
  curSpec = ana->spec;
  std::bitset<2> applybit(applybit_);

  if (applybit.test(1))
    Lepsf = GetLeptonSF(ana);
  if (applybit.test(0))
    Trigsf = GetTriggerEff(ana);

  if ( std::isnan(Lepsf) || std::isinf(Lepsf))
    Lepsf = 1.0;
  if ( std::isnan(Trigsf) || std::isinf(Trigsf))
    Trigsf = 1.0;

  return Lepsf * Trigsf;
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
  {
    if (ana->AnaName.find("Tagger") != std::string::npos)
      return GetMuonTrigEff(45);
    if (ana->AnaName.find("SUSY") != std::string::npos)
      return GetMuonTrigEff(40);
  }

  if(std::find(hlt.begin(), hlt.end(), "HLT_PFMET100_PFMHT100_IDTight_v\\d") != hlt.end())
    return GetMETTrigEff();

  return 1.;
}       // -----  end of function LepTrigSF::GetTriggerEff  -----

// ===  FUNCTION  ============================================================
//         Name:  LepTrigSF::GetElecTrigEff
//  Description:  SingleElectron
// ===========================================================================
float LepTrigSF::GetElecTrigEff()
{
  const std::vector<float> elePtbins = {0, 25, 40, 45, 50, 55, 60, 65, 75, 100, 125, 150, 175, 200, 275, 400, 500};
  const std::vector<float> elePtEffs = {
    0.003781684,
    0.5606094,
    0.7254957,
    0.7425901,
    0.7717884,
    0.7725157,
    0.7899741,
    0.7990583,
    0.8213441,
    0.9049037,
    0.9665468,
    0.9700419,
    0.9681093,
    0.9660144,
    0.9778598,
    0.969697};
  assert(elePtbins.size() - elePtEffs.size() == 1);

  const std::vector<TLorentzVector> &elesLVec   = tr->getVec<TLorentzVector>("cutEleVec"+curSpec);
  if (elesLVec.empty()) return 1.;
  float LeadingPt =  elesLVec.front().Pt();

  std::size_t idx = -1;
  for(unsigned int i=0; i < elePtbins.size()-1; ++i)
  {
    if (LeadingPt >= elePtbins.at(i) && LeadingPt < elePtbins.at(i+1) )
    {
      idx=i;
      break;
    }
  }
  if (idx == -1 && LeadingPt >= elePtbins.back())
  {
    idx = elePtbins.size()-2;
  }
  assert(idx != -1);
  return elePtEffs.at(idx);
}       // -----  end of function LepTrigSF::GetElecTrigEff  -----

// ===  FUNCTION  ============================================================
//         Name:  LepTrigSF::GetMuonTrigEff
//  Description:  Muon eff in eta bins
// ===========================================================================
float LepTrigSF::GetMuonTrigEff(int ptcut) 
{
  std::vector<float> muonEtabins = {-2.6, -2.2, -1.8, -1.4, -1.0, -0.6, -0.2, 0.2, 0.6, 1.0, 1.4, 1.8, 2.2, 2.6};
  std::vector<float> muonEta40Effs = { 
    0.8020833,
    0.8113949,
    0.8111837,
    0.8824405,
    0.9024091,
    0.8737864,
    0.9186085,
    0.8759649,
    0.894041,
    0.8848286,
    0.8293217,
    0.8263979,
    0.7605634};
  std::vector<float> muonEta45Effs = { 
    0.8014019,
    0.820324,
    0.8150242,
    0.8931234,
    0.9102737,
    0.8823417,
    0.9271233,
    0.8832173,
    0.9018908,
    0.892539,
    0.8336032,
    0.8302719,
    0.7830688};
  assert(muonEtabins.size() - muonEta40Effs.size() == 1);
  assert(muonEtabins.size() - muonEta45Effs.size() == 1);

  const std::vector<TLorentzVector> &muonsLVec   = tr->getVec<TLorentzVector>("cutMuVec"+curSpec);
  if (muonsLVec.empty()) return 1.;
  double LeadingPt = muonsLVec.front().Pt();
  if (LeadingPt < ptcut ) return 1;
  double LeadingEta =  muonsLVec.front().Eta();

  std::size_t idx = -1;
  for(unsigned int i=0; i < muonEtabins.size()-1; ++i)
  {
    if (LeadingEta >= muonEtabins.at(i) && LeadingEta < muonEtabins.at(i+1) )
    {
      idx=i;
      break;
    }
  }
  if (idx == -1 && LeadingEta >= muonEtabins.back())
  {
    idx = muonEtabins.size()-2;
  }
  if (idx == -1 && LeadingEta <= muonEtabins.front())
  {
    idx = 0;
  }

  assert(idx != -1);
  if (ptcut == 40)
    return muonEta40Effs.at(idx);
  if (ptcut == 45)
    return muonEta45Effs.at(idx);

  return 1.;
}       // -----  end of function LepTrigSF::GetMuonTrigEff  -----


// ===  FUNCTION  ============================================================
//         Name:  LepTrigSF::GetMETTrigEff
//  Description:  /* cursor */
// ===========================================================================
float LepTrigSF::GetMETTrigEff()
{

  const std::vector<float> metPtbins = {0, 25, 50, 75, 100, 125, 150, 175, 200, 250, 300, 400, 600, 1000};
  const std::vector<float> metPtEffs_LessHT = {
   0.002031716,
   0.00386317,
   0.01100592,
   0.04153341,
   0.1467812,
   0.3698591,
   0.6441574,
   0.8272891,
   0.9309105,
   0.9766839,
   0.9922869,
   0.9931271,
   1};
  const std::vector<float> metPtEffs_MoreHT = {
   0.0236715,
   0.03216302,
   0.04664076,
   0.0760191,
   0.1595577,
   0.2874818,
   0.4728682,
   0.6799037,
   0.8356643,
   0.9377845,
   0.9623431,
   0.9874055,
   0.98};

  assert(metPtbins.size() - metPtEffs_LessHT.size() == 1);
  assert(metPtbins.size() - metPtEffs_MoreHT.size() == 1);

  double met =  tr->getVar<double>("met"+curSpec);
  double ht =  tr->getVar<double>("HT"+curSpec);
  std::size_t idx = -1;
  if (met < 250) return 1;

  for(unsigned int i=0; i < metPtbins.size()-1; ++i)
  {
    if (met >= metPtbins.at(i) && met < metPtbins.at(i+1) )
    {
      idx=i;
      break;
    }
  }
  if (idx == -1 && met >= metPtbins.back())
  {
    idx = metPtbins.size()-2;
  }
  assert(idx != -1);

  if (ht > 1000)
    return metPtEffs_MoreHT.at(idx);
  else
    return metPtEffs_LessHT.at(idx);
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
  LepSFTG["mu_trkptGT10_SF"] = (TGraphAsymmErrors*) LepSFfile->Get("ratio_eff_eta3_dr030e030_corr");
  LepSFTG["mu_trkptLT10_SF"] = (TGraphAsymmErrors*) LepSFfile->Get("ratio_eff_eta3_tk0_dr030e030_corr");

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
    LepSFVec["ele"] = { "GsfElectronToCutBasedSpring15V", "ele_miniISO_SF", "ele_trkpt_SF"};
  if (muontype == "elesFlagMedium" )
    LepSFVec["ele"] = { "GsfElectronToCutBasedSpring15M", "ele_miniISO_SF", "ele_trkpt_SF"};

  float lepsf = 1.;
  lepsf *= GetMuonSF() * GetEleSF();
  if ( std::isnan(lepsf) || std::isinf(lepsf))
    lepsf = 1.0;
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
        if (LepSFTG.find(sf) == LepSFTG.end() || LepSFTG[sf] == NULL) continue;
        tempweight  = LepSFTG[sf]->Eval(eta);
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
