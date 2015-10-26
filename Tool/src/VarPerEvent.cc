// ===========================================================================
//
//       Filename:  VarPerEvent.cc
//
//    Description:
//
//        Version:  1.0
//        Created:  10/13/2015 16:21:55
//       Compiler:  g++ -std=c++11
//
//         Author:  Zhenbin Wu (benwu)
//          Email:  zhenbin.wu@gmail.com
//        Company:  UIC, CMS@LPC, CDF@FNAL
//
// ===========================================================================

#include "VarPerEvent.h"

// ===  FUNCTION  ============================================================
//         Name:  RegisterVarPerEvent
//  Description:  
// ===========================================================================
void RegisterVarPerEvent(NTupleReader &tr, topTagger::type3TopTagger * type3Ptr)
{
  VarPerEvent var(&tr, type3Ptr);
  var.RunPerEvent();
}       // -----  end of function RegisterVarPerEvent  -----

//----------------------------------------------------------------------------
//       Class:  VarPerEvent
//      Method:  VarPerEvent
// Description:  constructor
//----------------------------------------------------------------------------
VarPerEvent::VarPerEvent (NTupleReader *tr_, topTagger::type3TopTagger * type3Ptr_)
:tr(tr_),type3Ptr(type3Ptr_)
{
}  // -----  end of method VarPerEvent::VarPerEvent  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  VarPerEvent
//      Method:  ~VarPerEvent
// Description:  destructor
//----------------------------------------------------------------------------
VarPerEvent::~VarPerEvent ()
{
}  // -----  end of method VarPerEvent::-VarPerEvent  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  VarPerEvent
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  VarPerEvent&
VarPerEvent::operator = ( const VarPerEvent &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method VarPerEvent::operator =  (assignment operator)  ---

// ===  FUNCTION  ============================================================
//         Name:  VarPerEvent::RunPerEvent
//  Description:  
// ===========================================================================
bool VarPerEvent::RunPerEvent() const
{
  GetnTops();
  GetMuInfo();

  return true;
}       // -----  end of function VarPerEvent::RunPerEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  VarPerEvent::GetnTops
//  Description:  
// ===========================================================================
bool VarPerEvent::GetnTops() const
{
  int nTops = tr->getVar<int>("nTopCandSortedCnt");
  std::vector<TLorentzVector> *vTops = new std::vector<TLorentzVector>();
  std::map<int, std::vector<TLorentzVector> > *vTopJets = new std::map<int, std::vector<TLorentzVector> >();

  for(int it=0; it<nTops; it++)
  {
    TLorentzVector topLVec = type3Ptr->buildLVec(tr->getVec<TLorentzVector>("jetsLVec_forTagger"), 
        type3Ptr->finalCombfatJets[type3Ptr->ori_pickedTopCandSortedVec[it]]);
    vTops->push_back(topLVec);

    std::vector<TLorentzVector> temp;
    std::vector<int> indexCombVec = type3Ptr->finalCombfatJets[type3Ptr->ori_pickedTopCandSortedVec[it]];
    for (size_t k = 0; k < indexCombVec.size(); ++k)
    {
      temp.push_back( (tr->getVec<TLorentzVector>("jetsLVec_forTagger")).at(indexCombVec.at(k)));
    }
    vTopJets->insert(std::make_pair(it, temp));
  }

  tr->registerDerivedVec("vTops", vTops);
  tr->registerDerivedVec("mTopJets", vTopJets);

  return true;
}       // -----  end of function VarPerEvent::GetnTops  -----

// ===  FUNCTION  ============================================================
//         Name:  VarPerEvent::GetMuInfo
//  Description:  Copy from Zinvisible method for muon selection and Z reco.
//  https://github.com/susy2015/ZInvisible/blob/master/Tools/derivedTupleVariables.h#L196
// ===========================================================================
bool VarPerEvent::GetMuInfo() const
{

//----------------------------------------------------------------------------
//  Getting the input from the NtuplerReader
//----------------------------------------------------------------------------
  std::vector<int>            genDecayPdgIdVec;
  std::vector<TLorentzVector> genDecayLVec;
  std::vector<TLorentzVector> muonsLVec;
  std::vector<double>         muonsRelIso;
  std::vector<double>         muonsMiniIso;
  std::vector<int>            W_emuVec;
  std::vector<double>         muonsCharge;
  std::vector<TLorentzVector> jetsLVec;
  std::vector<double>         recoJetschargedEmEnergyFraction;
  std::vector<double>         recoJetschargedHadronEnergyFraction;
  std::vector<int>            muonsFlagIDVec;
  double                      ht;
  double                      met;
  double                      metphi;

  try
  {
    genDecayPdgIdVec                    = tr->getVec<int>("genDecayPdgIdVec");
    genDecayLVec                        = tr->getVec<TLorentzVector>("genDecayLVec");
    muonsLVec                           = tr->getVec<TLorentzVector>("muonsLVec");
    muonsRelIso                         = tr->getVec<double>("muonsRelIso");
    muonsMiniIso                        = tr->getVec<double>("muonsMiniIso");
    W_emuVec                            = tr->getVec<int>("W_emuVec");
    muonsCharge                         = tr->getVec<double>("muonsCharge");
    jetsLVec                            = tr->getVec<TLorentzVector>("jetsLVec");
    recoJetschargedEmEnergyFraction     = tr->getVec<double>("recoJetschargedEmEnergyFraction");
    recoJetschargedHadronEnergyFraction = tr->getVec<double>("recoJetschargedHadronEnergyFraction");
    muonsFlagIDVec                      = tr->getVec<int>("muonsFlagMedium");
    ht                                  = tr->getVar<double>("ht");
    met                                 = tr->getVar<double>("met");
    metphi                              = tr->getVar<double>("metphi");

  }
  catch (std::string var)
  {
    return false;
  }

//----------------------------------------------------------------------------
//  Output 
//----------------------------------------------------------------------------

  std::vector<const TLorentzVector*>* genMatchIsoMuInAcc = new std::vector<const TLorentzVector*>();
  std::vector<const TLorentzVector*>* genMatchMuInAcc = new std::vector<const TLorentzVector*>();
  std::vector<double>* genMatchMuInAccRes = new std::vector<double>();
  std::vector<const TLorentzVector*>* genMuInAcc = new std::vector<const TLorentzVector*>();
  std::vector<const TLorentzVector*>* genMu = new std::vector<const TLorentzVector*>();
  std::vector<double>* genMatchIsoMuInAccAct = new std::vector<double>();
  std::vector<double>* genMatchMuInAccAct = new std::vector<double>();
  std::vector<double>* genMuInAccAct = new std::vector<double>();
  std::vector<double>* genMuAct = new std::vector<double>();
  std::vector<const TLorentzVector*>* genZ = new std::vector<const TLorentzVector*>();

  std::vector<TLorentzVector>* cutMuVec = new std::vector<TLorentzVector>(); //Muons pass pt, Iso and trigger requirement
  std::vector<double>* cutMuCharge = new std::vector<double>();
  std::vector<double>* cutMuActivity = new std::vector<double>();
  std::vector<TLorentzVector>* recoZVec = new std::vector<TLorentzVector>();

  std::vector<TLorentzVector> cutMuVecRecoOnly; //Muons passed pt requirement, but not Iso 
  TRandom3 *tr3 = new TRandom3();
  int sumCharge = 0;
  int nTriggerMuons = 0;


  for(unsigned int i = 0; i < muonsLVec.size(); ++i)
  {
    // emulates muons with pt but no iso requirements.  
    if(AnaFunctions::passMuon( muonsLVec[i], 0.0, 0.0, muonsFlagIDVec[i], AnaConsts::muonsArr)) 
    {
      cutMuVecRecoOnly.push_back(muonsLVec[i]);
    }

    // emulates muons with pt and iso requirements.  
    if(AnaFunctions::passMuon( muonsLVec[i], muonsMiniIso[i], 0.0, muonsFlagIDVec[i], AnaConsts::muonsArr))
    {
      if(AnaFunctions::passMuon( muonsLVec[i], muonsRelIso[i], 0.0, muonsFlagIDVec[i], AnaConsts::muonsTrigArr)) 
      {
        if(nTriggerMuons == 0 && muonsLVec[i].Pt() > 17)  nTriggerMuons++;
        else if(muonsLVec[i].Pt() > 8)  nTriggerMuons++;
      }
      cutMuVec->push_back(muonsLVec[i]);
      cutMuCharge->push_back(muonsCharge[i]);
      cutMuActivity->push_back(AnaFunctions::getMuonActivity(muonsLVec[i], jetsLVec, recoJetschargedHadronEnergyFraction, recoJetschargedEmEnergyFraction, AnaConsts::muonsAct));
      if(muonsCharge[i] > 0) sumCharge++;
      else                   sumCharge--;
    }
  }

  //mu45 non-iso trigger emulation 
  const double effsnom2012ABC[] = {0.928,0.8302,0.8018};
  const double upedge2012ABC[] = { 0.9, 1.2, 2.1};
  bool muTrigMu45 = false;
  for(TLorentzVector& mu : *cutMuVec)
  {
    if(mu.Pt() > 45)
    {
      for(unsigned int iBin = 0; iBin < sizeof(effsnom2012ABC)/sizeof(double); ++iBin)
      {
        if(mu.Eta() < upedge2012ABC[iBin] && tr3 && tr3->Uniform() < effsnom2012ABC[iBin])
        {
          muTrigMu45 = true;
          break;
        }
      }
      if(muTrigMu45) break;
    }
  }

  //**************************************************************************//
  //                                 Gen Study                                //
  //**************************************************************************//
  double genHt = 0.0;
  const double minMuPt = 20.0, highMuPt = 45.0;
  double nuPt1 = -999.9, nuPt2 = -999.9;

  for(unsigned int i = 0; i < genDecayPdgIdVec.size() && i < genDecayLVec.size(); ++i)
  {
    if((abs(genDecayPdgIdVec[i]) != 0 &&  abs(genDecayPdgIdVec[i]) < 6) ||
        (abs(genDecayPdgIdVec[i]) > 100 && abs(genDecayPdgIdVec[i]) < 10000)) 
      genHt += genDecayLVec[i].Pt();

    if(genDecayPdgIdVec[i] ==  13) nuPt1 = genDecayLVec[i].Pt();
    if(genDecayPdgIdVec[i] == -13) nuPt2 = genDecayLVec[i].Pt();

    if(abs(genDecayPdgIdVec[i]) == 13)
    {
      genMu->push_back(&genDecayLVec[i]);
      genMuAct->push_back(AnaFunctions::getMuonActivity(genDecayLVec[i], jetsLVec, recoJetschargedHadronEnergyFraction, recoJetschargedEmEnergyFraction, AnaConsts::muonsAct));
      if(AnaFunctions::passMuonAccOnly(genDecayLVec[i], AnaConsts::muonsMiniIsoArr) && genDecayLVec[i].Pt() > minMuPt)
      {
        genMuInAcc->push_back(&genDecayLVec[i]);
        genMuInAccAct->push_back(genMuAct->back());
        double dRMin = 999.9;
        double matchPt = -999.9;
        for(unsigned int j = 0; j < cutMuVecRecoOnly.size(); ++j)
        {
          double dR = ROOT::Math::VectorUtil::DeltaR(genDecayLVec[i], cutMuVecRecoOnly[j]);
          if(dR < dRMin)
          {
            dRMin = dR;
            matchPt = cutMuVecRecoOnly[j].Pt();
          }
        }
        if(dRMin < 0.02)
        {
          genMatchMuInAcc->push_back(&genDecayLVec[i]);
          genMatchMuInAccAct->push_back(genMuAct->back());
          genMatchMuInAccRes->push_back((genDecayLVec[i].Pt() - matchPt)/genDecayLVec[i].Pt());
        }

        dRMin = 999.9;
        for(unsigned int j = 0; j < cutMuVec->size(); ++j)
        {
          double dR = ROOT::Math::VectorUtil::DeltaR(genDecayLVec[i], (*cutMuVec)[j]);
          if(dR < dRMin)
          {
            dRMin = dR;
          }
        }
        if(dRMin < 0.02)
        {
          genMatchIsoMuInAcc->push_back(&genDecayLVec[i]);
          genMatchIsoMuInAccAct->push_back(genMuAct->back());
        }
      }
    }
  }

  double genZPt = -999.9, genZEta = -999.9, genZmass = -999.9, genZPhi;
  int nZ = 0;
  TLorentzVector genZ1;
  for(unsigned int j = 0; j <  genDecayPdgIdVec.size(); ++j)
  {
    if(abs(genDecayPdgIdVec[j]) == 23)
    {
      nZ++;
      genZ->push_back(&genDecayLVec[j]);
      genZ1 = genDecayLVec[j];
      genZPt = genDecayLVec[j].Pt();
      genZEta = genDecayLVec[j].Eta();
      genZPhi = genDecayLVec[j].Phi();
      genZmass = genDecayLVec[j].M();
    }
  }


  int pdgIdZDec = 0;
  if(W_emuVec.size() == 0) pdgIdZDec = 15;
  else if(W_emuVec.size() == 2)
  {
    if(abs(genDecayPdgIdVec[W_emuVec[0]]) == 11) pdgIdZDec = 11;
    else if(abs(genDecayPdgIdVec[W_emuVec[0]]) == 13) pdgIdZDec = 13;
  }
  bool passDiMuTrig  = nTriggerMuons >= 2;

  const double zMassMin = 71.0;
  const double zMass    = 91.0;
  const double zMassMax = 111.0;

  double zMassCurrent = 1.0e300;
  //double zMassCurrent = 1.0e300, zEff = 1.0e100, zAcc = 1.0e100;
  TLorentzVector bestRecoZ;
  for(unsigned int i = 0; i < cutMuVec->size(); ++i)
  {
    if((*cutMuVec)[i].Pt() < minMuPt) continue;
    for(unsigned int j = 0; j < i && j < cutMuVec->size(); ++j)
    {
      if((*cutMuVec)[j].Pt() < minMuPt) continue;
      double zm = ((*cutMuVec)[i] + (*cutMuVec)[j]).M();
      //if(zm > zMassMin && zm < zMassMax && fabs(zm - zMass) < fabs(zMassCurrent - zMass))
      if(fabs(zm - zMass) < fabs(zMassCurrent - zMass))
      {
        bestRecoZ = (*cutMuVec)[i] + (*cutMuVec)[j];
        zMassCurrent = zm;
      }
    }
  }
  recoZVec->push_back(bestRecoZ);

  TLorentzVector metV, metZ;
  metV.SetPtEtaPhiM(met, 0.0, metphi, 0.0);
  metZ.SetPtEtaPhiM(bestRecoZ.Pt(), 0.0, bestRecoZ.Phi(), 0.0);
  TLorentzVector cleanMet = metV + metZ;

  bool passMuZinvSel = (cutMuVec->size() == 2 && sumCharge == 0 && (*cutMuVec)[0].Pt() > highMuPt && (*cutMuVec)[1].Pt() > minMuPt) && (bestRecoZ.M() > zMassMin) && (bestRecoZ.M() < zMassMax);        

  double cutMuPt1 = -999.9;
  double cutMuPt2 = -999.9;
  if(cutMuVec->size() >= 1) cutMuPt1 = cutMuVec->at(0).Pt();
  if(cutMuVec->size() >= 2) cutMuPt2 = cutMuVec->at(1).Pt();

  //const unsigned int& run   = tr->getVar<unsigned int>("run");
  //const unsigned int& lumi  = tr->getVar<unsigned int>("lumi");
  //const unsigned int& event = tr->getVar<unsigned int>("event");
  //if(passMuZinvSel && genZPt > 400 && fabs(nuPt1 - nuPt2) > 300) std::cout << "BONJOUR!!! \t" << genZPt << "\t" << fabs(nuPt1 - nuPt2) << "\t" << run << "\t" << lumi << "\t" << event << std::endl;

  //double mindPhiMetJ = 999.9;
  //int jc = 0;
  //for(const TLorentzVector& jet : jetsLVec)
  //{
    //if(jc >= 3) break;
    //jc++;
    //mindPhiMetJ = std::min(mindPhiMetJ, fabs(ROOT::Math::VectorUtil::DeltaPhi(genZ, jet)));
  //}
  //if(genZPt > 600) std::cout << "HELLO THERE!!!!" << std::endl;
  //if(genZPt > 600 && mindPhiMetJ < 0.5) std::cout << "BONJOUR!!! \t" << genZPt << "\t" << mindPhiMetJ << "\t" << run << "\t" << lumi << "\t" << event << std::endl;

  double bestRecoZPt = bestRecoZ.Pt();
  double cleanMetPt = cleanMet.Pt();
  //double cleanMet2Pt = cleanMet2.Pt();
  tr->registerDerivedVar("bestRecoZPt", bestRecoZPt);
  tr->registerDerivedVar("bestRecoZM", bestRecoZ.M());
  tr->registerDerivedVar("cleanMetPt", cleanMetPt);
  tr->registerDerivedVar("cleanMetPhi", cleanMet.Phi());
  //tr->registerDerivedVar("cleanMet2Pt", cleanMet2Pt);
  tr->registerDerivedVar("genHt", genHt);
  tr->registerDerivedVar("cutMuPt1", cutMuPt1);
  tr->registerDerivedVar("cutMuPt2", cutMuPt2);
  //tr->registerDerivedVar("mindPhiMetJ", mindPhiMetJ);

  tr->registerDerivedVar("ZPtRes", (bestRecoZPt - genZPt)/genZPt);
  tr->registerDerivedVar("ZEtaRes", bestRecoZ.Eta() - genZEta);
  tr->registerDerivedVar("ZPhiRes", bestRecoZ.Phi() - genZPhi);
  tr->registerDerivedVar("ZMRes", (bestRecoZ.M() - genZmass)/genZmass);

  tr->registerDerivedVec("cutMuVec", cutMuVec);
  tr->registerDerivedVec("cutMuActivity", cutMuActivity);
  tr->registerDerivedVec("genMu", genMu);
  tr->registerDerivedVec("genZ", genZ);
  tr->registerDerivedVec("recoZVec", recoZVec);
  tr->registerDerivedVar("ngenMu", static_cast<double>(genMu->size()));
  tr->registerDerivedVec("genMuInAcc", genMuInAcc);
  tr->registerDerivedVec("genMuAct", genMuAct);
  tr->registerDerivedVar("ngenMuInAcc", static_cast<double>(genMuInAcc->size()));
  tr->registerDerivedVec("genMuInAccAct", genMuInAccAct);
  tr->registerDerivedVec("genMatchMuInAcc", genMatchMuInAcc);
  tr->registerDerivedVec("genMatchMuInAccRes", genMatchMuInAccRes);
  tr->registerDerivedVec("genMatchIsoMuInAcc", genMatchIsoMuInAcc);
  tr->registerDerivedVar("ngenMatchMuInAcc", static_cast<double>(genMatchMuInAcc->size()));
  tr->registerDerivedVec("genMatchMuInAccAct", genMatchMuInAccAct);
  tr->registerDerivedVec("genMatchIsoMuInAccAct", genMatchIsoMuInAccAct);
  tr->registerDerivedVar("genZPt", genZPt);
  tr->registerDerivedVar("genZEta", genZEta);
  tr->registerDerivedVar("genZmass", genZmass);
  tr->registerDerivedVar("pdgIdZDec", pdgIdZDec);
  tr->registerDerivedVar("passMuZinvSel", passMuZinvSel);
  tr->registerDerivedVar("passDiMuIsoTrig", passDiMuTrig);
  tr->registerDerivedVar("passSingleMu45", muTrigMu45);

  return true;
}       // -----  end of function VarPerEvent::GetMuInfo  -----
