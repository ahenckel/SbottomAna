// ===========================================================================
// 
//       Filename:  TTZ3Lep.cc
// 
//    Description:  An analysis for TTZ background in the 3 lepton channel
// 
//        Version:  1.0
//        Created:  10/07/2015 11:02:23
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================


#include "TTZ3Lep.h"

//----------------------------------------------------------------------------
//       Class:  TTZ3Lep
//      Method:  TTZ3Lep
// Description:  constructor
//----------------------------------------------------------------------------
TTZ3Lep::TTZ3Lep (std::string name, NTupleReader* tr_, std::shared_ptr<TFile> &OutFile, std::string spec_)
: ComAna(name, tr_, OutFile, spec_)
{
  InitCutOrder(name);
  if (spec_ == "TTZ")
  {
    jetVecLabel = "prodJetsNoMu_jetsLVec";
    CSVVecLabel = "recoJetsBtag_0_MuCleaned";
  }
}  // -----  end of method TTZ3Lep::TTZ3Lep  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  TTZ3Lep
//      Method:  TTZ3Lep
// Description:  copy constructor
//----------------------------------------------------------------------------
TTZ3Lep::TTZ3Lep ( const TTZ3Lep &other ): ComAna(other)
{
}  // -----  end of method TTZ3Lep::TTZ3Lep  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  TTZ3Lep
//      Method:  ~TTZ3Lep
// Description:  destructor
//----------------------------------------------------------------------------
TTZ3Lep::~TTZ3Lep ()
{
}  // -----  end of method TTZ3Lep::-TTZ3Lep  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  TTZ3Lep
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  TTZ3Lep&
TTZ3Lep::operator = ( const TTZ3Lep &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method TTZ3Lep::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  TTZ3Lep::BookHistograms
//  Description:  
// ===========================================================================
bool TTZ3Lep::BookHistograms()
{
  ComAna::BookHistograms();
  BookTLVHistos("RecoZ");
  BookTLVHistos("3rdMuon");
  BookTLVHistos("3rdEle");
  return true;
}       // -----  end of function TTZ3Lep::BookHistograms  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZ3Lep::InitCutOrder
//  Description:  
// ===========================================================================
bool TTZ3Lep::InitCutOrder(std::string ana)
{
  //Clear the objects
  CutOrder.clear();
  CutMap.clear();

  //Add name and order of the cutflow
  CutOrder.push_back("NoCut");
  CutOrder.push_back("Filter");
  CutOrder.push_back("Trigger");
  CutOrder.push_back("HasZ");
  CutOrder.push_back("3Leps");
  CutOrder.push_back("BJets");
  CutOrder.push_back("Tagger");
  CutOrder.push_back("MET70");

  //Set the cutbit of each cut
  CutMap["NoCut"]   = "00000000000000000";
  CutMap["Filter"]  = "00000000000000001";
  CutMap["Trigger"] = "00000000001000001";
  CutMap["HasZ"]    = "00000000001000011";
  CutMap["3Leps"]   = "00000000001000111";
  CutMap["BJets"]   = "00000000001001111";
  CutMap["Tagger"]  = "00000000001011111";
  CutMap["MET70"]   = "00000000001111111";

  assert(CutOrder.size() == CutMap.size());

  his->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));
  return true;
}       // -----  end of function TTZ3Lep::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZ3Lep::CheckCut
//  Description:  
// ===========================================================================
bool TTZ3Lep::CheckCut()
{
  cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));

  // Check event has Z
  //cutbit.set(1 , HasZ());
  cutbit.set(1 , tr->getVar<bool>("passMuZinvSel"));

  cutbit.set(2 , tr->getVec<TLorentzVector>("cutMuVec").size() == 3
      || tr->getVar<int>(Label["nElectrons_Base"]) == 1);

  cutbit.set(3 , tr->getVar<int>(Label["cntCSVS"]) >= 1);

  cutbit.set(4 , tr->getVar<int>(Label["nTopCandSortedCnt"]) >= 1);

  cutbit.set(5 , tr->getVar<double>(METLabel) < 70);

  cutbit.set(6 , tr->getVar<bool>("PassDiMuonTrigger"));


  return true;
}       // -----  end of function TTZ3Lep::CheckCut  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZ3Lep::FillCut
//  Description:  
// ===========================================================================
bool TTZ3Lep::FillCut()
{

  //std::cout << "nMuons "<< tr->getVar<int>("nMuons")
    //<< " nMuons_CUT "<< tr->getVar<int>("nMuons_CUT")
    //<< " nMuons_Base "<< tr->getVar<int>("nMuons_Base")
    //<< " nMuonsVec "<< tr->getVec<TLorentzVector>("muonsLVec").size()
    //<< " cutMuVec "<< tr->getVec<TLorentzVector>("cutMuVec").size()
    //<< std::endl;

//----------------------------------------------------------------------------
//  Check cut and fill cut-based plots
//----------------------------------------------------------------------------
  CheckCut();
  ComAna::RunEvent();
  bool passcuts = false;

  for (unsigned int i = 0; i < CutOrder.size(); ++i)
  {
    std::bitset<NBITS> locbit(CutMap[CutOrder.at(i)]);
    if ( (cutbit & locbit) != locbit) continue;

    his->FillTH1("CutFlow", int(i)); 

    if (tr->getVec<TLorentzVector>("recoZVec").at(0).Pt() != 0)
      FillTLVHistos(i, "RecoZ", tr->getVec<TLorentzVector>("recoZVec").at(0));

    ComAna::FillCut(i);
    Check3rdLep(i);
    if (i+1 == CutOrder.size()) 
    {
      passcuts = true;
    }
  }

  return passcuts;
}       // -----  end of function TTZ3Lep::FillCut  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZ3Lep::HasZ
//  Description:  
// ===========================================================================
bool TTZ3Lep::HasZ() const
{
  const std::vector<TLorentzVector> &cutMuVec = tr->getVec<TLorentzVector>("cutMuVec");
  const std::vector<int> &cutMuCharge = tr->getVec<int>("cutMuCharge");
  const double minMuPt = 20.0;
  const double highMuPt = 45.0;
  const double zMassMin = 71.0;
  const double zMass    = 91.0;
  const double zMassMax = 111.0;
  int sumCharge =0;

  double zMassCurrent = 1.0e300;
  //double zMassCurrent = 1.0e300, zEff = 1.0e100, zAcc = 1.0e100;
  TLorentzVector bestRecoZ;
  for(unsigned int i = 0; i < cutMuVec.size(); ++i)
  {
    if(cutMuVec.at(i).Pt() < minMuPt) continue;
    for(unsigned int j = 0; j < i && j < cutMuVec.size(); ++j)
    {
      if(cutMuVec.at(j).Pt() < minMuPt) continue;
      double zm = (cutMuVec.at(i) + cutMuVec.at(j)).M();
      //if(zm > zMassMin && zm < zMassMax && fabs(zm - zMass) < fabs(zMassCurrent - zMass))
      if(fabs(zm - zMass) < fabs(zMassCurrent - zMass))
      {
        sumCharge = cutMuCharge.at(i) + cutMuCharge.at(j);
        bestRecoZ = (cutMuVec)[i] + (cutMuVec)[j];
        zMassCurrent = zm;
      }
    }
  }

  return cutMuVec.size() >= 2 && sumCharge == 0 && 
      (cutMuVec)[0].Pt() > highMuPt && (cutMuVec)[1].Pt() > minMuPt &&
    (bestRecoZ.M() > zMassMin) && (bestRecoZ.M() < zMassMax);        
}       // -----  end of function TTZ3Lep::HasZ  -----


// ===  FUNCTION  ============================================================
//         Name:  TTZ3Lep::Check3rdLep
//  Description:  
// ===========================================================================
bool TTZ3Lep::Check3rdLep(int NCut)
{
  //----------------------------------------------------------------------------
  //  Z from diMuon
  //----------------------------------------------------------------------------
  // 3rd Ele
  std::string elesFlagIDLabel = "";
  const std::vector<int> & elesFlagIDVec = elesFlagIDLabel.empty()? std::vector<int>(tr->getVec<double>("elesMiniIso").size(), 1)
    :tr->getVec<int>(elesFlagIDLabel.c_str()); // Fake electrons since we don't have different ID for electrons now, but maybe later
  TLorentzVector thirdEle(0, 0, 0, 0);
  int cntNElectrons = 0;
  for(unsigned int ie=0; ie<tr->getVec<TLorentzVector>("elesLVec").size(); ie++)
  {
    if(AnaFunctions::passElectron(tr->getVec<TLorentzVector>("elesLVec").at(ie), 
          tr->getVec<double>("elesMiniIso").at(ie), tr->getVec<double>("elesMtw").at(ie), 
          tr->getVec<unsigned int>("elesisEB").at(ie), elesFlagIDVec.at(ie), AnaConsts::elesMiniIsoArr))
    {
      cntNElectrons ++;
      thirdEle = tr->getVec<TLorentzVector>("elesLVec").at(ie);
    }
  }
  if (cntNElectrons == 1)
  {
    FillTLVHistos(NCut, "3rdEle", thirdEle);
  }

  if (tr->getVec<TLorentzVector>("recoZVec").size() == 0 || 
      tr->getVec<TLorentzVector>("recoZVec").at(0).Pt() == 0 ||
      tr->getVec<TLorentzVector>("cutMuVec").size() != 3)
    return false;

  //3rd Muon
  TLorentzVector thirdMuon(0, 0, 0, 0);
  const std::vector<TLorentzVector> &cutMuVec = tr->getVec<TLorentzVector>("cutMuVec");
  TLorentzVector recoZ = tr->getVec<TLorentzVector>("recoZVec").at(0);
  std::map<unsigned, bool> MuonIdx;
  const double minMuPt = 20.0;
  const double highMuPt = 45.0;

  for(unsigned int i = 0; i < cutMuVec.size(); ++i)
  {
    if(cutMuVec.at(i).Pt() < minMuPt) continue;
    for(unsigned int j = 0; j < i && j < cutMuVec.size(); ++j)
    {
      if(cutMuVec.at(j).Pt() < minMuPt) continue;
      TLorentzVector localZ  = (cutMuVec.at(i) + cutMuVec.at(j));
      if (localZ == recoZ)
      {
        MuonIdx[i] = true;
        MuonIdx[j] = true;
      }
    }
  }

  for(unsigned int i = 0; i < cutMuVec.size(); ++i)
  {
    if (MuonIdx.find(i) != MuonIdx.end())
      continue;
    thirdMuon = cutMuVec.at(i);
  }

  if (thirdMuon.Pt() != 0)
  {
    FillTLVHistos(NCut, "3rdMuon", thirdMuon);
  }

  return true;
}       // -----  end of function TTZ3Lep::Check3rdLep  -----
