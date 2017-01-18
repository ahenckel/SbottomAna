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
  if (AnaName.find("TTZ") != std::string::npos)
  {
    jetVecLabel = "jetsLVecLepCleaned";
    CSVVecLabel = "recoJetsBtag_0_LepCleaned";
    if (strcmp(&(spec.back()), "M") == 0)
    {
      HLTstr.push_back("HLT_Mu45_eta2p1_v\\d");
      MCTrigstr.push_back(Label["PassDiMuonTrigger"]);
    }
    if (strcmp(&(spec.back()), "E") == 0)
    {
      HLTstr.push_back("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v\\d");
      MCTrigstr.push_back(Label["PassDiEleTrigger"]);
    }
  }
  if (AnaName.find("SUSY") != std::string::npos)
  {
    jetVecLabel = "jetsLVecLepCleaned";
    CSVVecLabel = "recoJetsBtag_0_LepCleaned";
    if (strcmp(&(AnaName.back()), "E") == 0)
    {
      HLTstr.push_back("HLT_Ele27_WPTight_Gsf_v\\d");
      //MCTrigstr.push_back(Label["PassDiEleTrigger"]);
    }
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

// ===  FUNCTION  ============================================================
//         Name:  TTZ3Lep::Clone
//  Description:  
// ===========================================================================
TTZ3Lep* TTZ3Lep::Clone(std::string newname, std::shared_ptr<TFile> OutFile_)
{
  if (OutFile_ == NULL)
    return new TTZ3Lep(newname, tr, OutFile, spec);
  else
    return new TTZ3Lep(newname, tr, OutFile_, spec);

}       // -----  end of function TTZ3Lep::Clone  -----

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
  //ComAna::BookHistograms();
  BookTLVHistos("RecoZ");
  BookTLVHistos("3rdMuon");
  BookTLVHistos("3rdEle");
  his->AddTH1C("JBT", "JBT", "JBT", "Events", 400, 0, 400);
  his->AddTH2C("JBTVsZPT", "JBTVsZPT", "ZPT", "JBT", 20, 0, 1000, 400, 0, 400);
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

  if (AnaName.find("TTZ") != std::string::npos)
  {
    //Add name and order of the cutflow
    CutOrder.push_back("NoCut");
    CutOrder.push_back("Filter");
    CutOrder.push_back("Trigger");
    CutOrder.push_back("nJets");
    CutOrder.push_back("3Leps");
    CutOrder.push_back("HasZ");
    CutOrder.push_back("BJets");
    CutOrder.push_back("Tagger");
    CutOrder.push_back("MET40");
    CutOrder.push_back("2B1T");

    //Set the cutbit of each cut
    CutMap["NoCut"]   = "00000000000000000";
    CutMap["Filter"]  = "00000000000000001";
    CutMap["Trigger"] = "00000000000000011";
    CutMap["nJets"]   = "00000000000000111";
    CutMap["3Leps"]   = "00000000000001111";
    CutMap["HasZ"]    = "00000000000011111";
    CutMap["BJets"]   = "00000000000111111";
    CutMap["Tagger"]  = "00000000001111111";
    CutMap["MET40"]   = "00000000011111111";
    CutMap["2B1T"]    = "00000000111111111";
  }
  if (AnaName.find("SUSY") != std::string::npos)
  {
    //Add name and order of the cutflow
    CutOrder.push_back("NoCut");
    CutOrder.push_back("Filter");
    CutOrder.push_back("Trigger");
    CutOrder.push_back("3Leps");
    CutOrder.push_back("HasZ");
    CutOrder.push_back("nJets40");
    CutOrder.push_back("nBs30");
    CutOrder.push_back("MET30");
    CutOrder.push_back("Tagger");

    //Set the cutbit of each cut
    CutMap["NoCut"]   = "00000000000000000";
    CutMap["Filter"]  = "00000000000000001";
    CutMap["Trigger"] = "00000000000000011";
    CutMap["3Leps"]   = "00000000000000111";
    CutMap["HasZ"]    = "00000000000001111";
    CutMap["nJets40"] = "00000000000011111";
    CutMap["nBs30"]   = "00000000000111111";
    CutMap["MET30"]   = "00000000001111111";
    CutMap["Tagger"]  = "00000000011111111";
  }
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
  if (AnaName.find("TTZ") != std::string::npos)
  {
    cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));

    // Check event has Z
    //cutbit.set(1 , HasZ());
    cutbit.set(1 , ComAna::PassTrigger());

    cutbit.set(2 , tr->getVar<bool>(Label["passnJets"]));

    cutbit.set(3 ,  tr->getVar<int>(Label["nMuons_Base"]) + tr->getVar<int>(Label["nElectrons_Base"]) == 3 );

    cutbit.set(4 , tr->getVec<TLorentzVector>(Label["recoZVec"]).size() == 1);

    cutbit.set(5 , tr->getVar<int>(Label["cntCSVS"]) >= 1);

    cutbit.set(6 , tr->getVar<int>(Label["nTopCandSortedCnt"]) >= 1);

    cutbit.set(7 , tr->getVar<double>(METLabel) > 40);

    cutbit.set(8 , tr->getVar<int>(Label["cntCSVS"]) == 2);
  }

  if (AnaName.find("SUSY") != std::string::npos)
  {
    cutbit.set(0 , tr->getVar<bool>(Label["passNoiseEventFilter"]));
    cutbit.set(1 , ComAna::PassTrigger());
    cutbit.set(2 , Pass3Leps(40, 20, 20, true));
    cutbit.set(3 , tr->getVec<TLorentzVector>(Label["recoZVec"]).size() == 1);
    cutbit.set(4 , ComAna::CountJets(40) >= 4);
    cutbit.set(5 , tr->getVar<int>(Label["cntCSVS"]) >= 2);
    cutbit.set(6 , tr->getVar<double>(METLabel) > 30);
    cutbit.set(7 , tr->getVar<int>(Label["nTopCandSortedCnt"]) >= 1);
  }
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


    int JBTcount = tr->getVar<int>(Label["nTopCandSortedCnt"]) * 100 + tr->getVar<int>(Label["cntCSVS"]) * 10 + tr->getVec<TLorentzVector>(Label["jetsLVec_forTagger"]).size();
    his->FillTH1(i, "JBT", JBTcount);
    if (tr->getVec<TLorentzVector>(Label["recoZVec"]).size() > 0)
    {
      FillTLVHistos(i, "RecoZ", tr->getVec<TLorentzVector>(Label["recoZVec"]).at(0));
      his->FillTH2(i, "JBTVsZPT", tr->getVec<TLorentzVector>(Label["recoZVec"]).at(0).Pt(), JBTcount);
    }

    //ComAna::FillCut(i);
    ComAna::CheckLeadingLeptons(i);

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
bool TTZ3Lep::HasZ()
{
  const std::vector<TLorentzVector> &cutMuVec = tr->getVec<TLorentzVector>(Label["cutMuVec"]);
  const std::vector<int> &cutMuCharge = tr->getVec<int>(Label["cutMuCharge"]);
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
  if (tr->getVec<TLorentzVector>(Label["recoZVec"]).size() == 0) 
    return false;

  if (tr->getVar<int>(Label["nMuons_Base"]) + tr->getVar<int>(Label["nElectrons_Base"]) != 3 ) 
    return false;

  const std::map<unsigned int, std::pair<unsigned int, unsigned int> > &ZLepIdx = 
    tr->getMap<unsigned int, std::pair<unsigned int, unsigned int> >(Label["ZLepIdx"]);

  std::vector<unsigned int> EleinZ;
  std::vector<unsigned int> MuinZ;
  for(auto &it : ZLepIdx)
  {
    if (it.second.first >= 100)
    {
      assert(it.second.second >= 100 );
      EleinZ.push_back(it.second.first - 100);
      EleinZ.push_back(it.second.second - 100);
    } else {
      assert(it.second.second < 100 );
      MuinZ.push_back(it.second.first);
      MuinZ.push_back(it.second.second);
    }
  }

  std::vector<TLorentzVector> cutMuVec;
  std::vector<TLorentzVector> cutEleVec;

  if (tr->hasVar(Label["cutMuVec"]))
    cutMuVec = tr->getVec<TLorentzVector>(Label["cutMuVec"]);

  if (tr->hasVar(Label["cutEleVec"]))
    cutEleVec = tr->getVec<TLorentzVector>(Label["cutEleVec"]);


  if((cutEleVec.size() + cutMuVec.size())  - (EleinZ.size() + MuinZ.size() ) != 1)
    return false;
  assert((cutEleVec.size() + cutMuVec.size())  - (EleinZ.size() + MuinZ.size() ) == 1);

  TLorentzVector thirdEle(0, 0, 0, 0);
  TLorentzVector thirdMuon(0, 0, 0, 0);
  for(unsigned int i=0; i < cutMuVec.size(); ++i)
  {
    if(find(MuinZ.begin(), MuinZ.end(), i) != MuinZ.end())
      continue;
    thirdMuon = cutMuVec.at(i);
  }

  for(unsigned int i=0; i < cutEleVec.size(); ++i)
  {
    if(find(EleinZ.begin(), EleinZ.end(), i) != EleinZ.end())
      continue;
    thirdEle = cutEleVec.at(i);
  }

  assert(thirdMuon.Pt() == 0 || thirdEle.Pt() == 0);

  if (thirdEle.Pt() != 0)
  {
    FillTLVHistos(NCut, "3rdEle", thirdEle);
  }

  if (thirdMuon.Pt() != 0)
  {
    FillTLVHistos(NCut, "3rdMuon", thirdMuon);
  }

  return true;
}       // -----  end of function TTZ3Lep::Check3rdLep  -----

// ===  FUNCTION  ============================================================
//         Name:  TTZ3Lep::Pass3Leps
//  Description:  
// ===========================================================================
bool TTZ3Lep::Pass3Leps(int pt1, int pt2, int pt3, bool isEle)
{
  
  const std::vector<TLorentzVector> &cutMuVec = tr->getVec<TLorentzVector>( Label["cutMuVec"] );
  const std::vector<TLorentzVector> &cutEleVec = tr->getVec<TLorentzVector>( Label["cutEleVec"] );

  if ((cutMuVec.size() + cutEleVec.size()) != 3) return false;
  if (isEle)
  {
    if (cutEleVec.size() == 0 || cutEleVec.front().Pt() < pt1)
      return false;
  } else{
    if (cutMuVec.size() == 0 || cutMuVec.front().Pt() < pt1)
      return false;
  }

  std::vector<TLorentzVector>  temp = cutMuVec;
  temp.insert(temp.end(), cutEleVec.begin(), cutEleVec.end());
  std::sort(temp.begin(), temp.end(), [](const TLorentzVector &a, const TLorentzVector &b){return a.Pt() > b.Pt();});
  assert(temp.front().Pt() > temp.back().Pt());
  assert(temp.size() == 3);
  std::vector<TLorentzVector>::iterator tit = temp.begin();
  if (tit->Pt() < pt1) return false;
  tit ++;
  if (tit->Pt() < pt2) return false;
  tit ++;
  if (tit->Pt() < pt3) return false;
  return true;
}       // -----  end of function TTZ3Lep::Pass3Leps  -----


