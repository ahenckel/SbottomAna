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
void RegisterVarPerEvent(NTupleReader &tr)
{
  VarPerEvent var(&tr);
  var.RunPerEvent();
}       // -----  end of function RegisterVarPerEvent  -----

//----------------------------------------------------------------------------
//       Class:  VarPerEvent
//      Method:  VarPerEvent
// Description:  constructor
//----------------------------------------------------------------------------
VarPerEvent::VarPerEvent (NTupleReader *tr_) :tr(tr_)
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
  GetRecoZ();
  PassDiMuonTrigger();
  PassDiEleTrigger();

  return true;
}       // -----  end of function VarPerEvent::RunPerEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  VarPerEvent::GetMuInfo
//  Description:  Copy from Zinvisible method for muon selection and Z reco.
//  https://github.com/susy2015/ZInvisible/blob/master/Tools/derivedTupleVariables.h#L196
// ===========================================================================
bool VarPerEvent::GetMuInfo(std::vector<TLorentzVector>* recoZVec, TypeZLepIdx *ZLepIdx) const
{

//----------------------------------------------------------------------------
//  Getting the input from the NtuplerReader
//----------------------------------------------------------------------------
  std::vector<TLorentzVector> muonsLVec;
  std::vector<double>         muonsRelIso;
  std::vector<double>         muonsMiniIso;
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
    muonsLVec                           = tr->getVec<TLorentzVector>("muonsLVec");
    muonsRelIso                         = tr->getVec<double>("muonsRelIso");
    muonsMiniIso                        = tr->getVec<double>("muonsMiniIso");
    muonsCharge                         = tr->getVec<double>("muonsCharge");
    jetsLVec                            = tr->getVec<TLorentzVector>("jetsLVec");
    recoJetschargedEmEnergyFraction     = tr->getVec<double>("recoJetschargedEmEnergyFraction");
    recoJetschargedHadronEnergyFraction = tr->getVec<double>("recoJetschargedHadronEnergyFraction");
    try
    {
      muonsFlagIDVec                      = tr->getVec<int>("muonsFlagMedium");
    }
    catch (std::string var)
    {
      muonsFlagIDVec = std::vector<int>(tr->getVec<double>("muonsMiniIso").size(), 1);
    }

    ht                                  = tr->getVar<double>("ht");
    met                                 = tr->getVar<double>("met");
    metphi                              = tr->getVar<double>("metphi");

  }
  catch (std::string var)
  {
    std::cout << "Missing inputs for VarPerEvent::GetMuInfo" << std::endl;
    return false;
  }

//----------------------------------------------------------------------------
//  Output 
//----------------------------------------------------------------------------
  std::vector<TLorentzVector>* cutMuVec = new std::vector<TLorentzVector>(); //Muons pass pt, Iso and trigger requirement
  std::vector<double>* cutMuCharge = new std::vector<double>();
  std::vector<double>* cutMuActivity = new std::vector<double>();

  std::vector<TLorentzVector> cutMuVecRecoOnly; //Muons passed pt requirement, but not Iso 
  TRandom3 *tr3 = new TRandom3();
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



  bool passDiMuTrig  = nTriggerMuons >= 2;

  const double zMassMin = 81.0;
  const double zMass    = 91.0;
  const double zMassMax = 101.0;

  double zMassCurrent = 1.0e300;
  const double minMuPt = 20.0, highMuPt = 45.0;
  //double zMassCurrent = 1.0e300, zEff = 1.0e100, zAcc = 1.0e100;
  TLorentzVector bestRecoZ(0, 0, 0, 0);
  int sumCharge = 0;
  std::pair<unsigned int, unsigned int> mupair;
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
        mupair = std::make_pair(i, j);
        zMassCurrent = zm;
        sumCharge = 0;
        sumCharge = cutMuCharge->at(i) + cutMuCharge->at(j);
      }
    }
  }
  if (bestRecoZ.Pt() != 0 && sumCharge == 0)
  {
    recoZVec->push_back(bestRecoZ);
    ZLepIdx->insert(std::make_pair( recoZVec->size(), mupair));
  }

  TLorentzVector metV, metZ;
  metV.SetPtEtaPhiM(met, 0.0, metphi, 0.0);
  metZ.SetPtEtaPhiM(bestRecoZ.Pt(), 0.0, bestRecoZ.Phi(), 0.0);
  TLorentzVector cleanMet = metV + metZ;

  bool passMuZinvSel = (cutMuVec->size() >= 2 && sumCharge == 0 && (*cutMuVec)[0].Pt() > highMuPt && (*cutMuVec)[1].Pt() > minMuPt) && (bestRecoZ.M() > zMassMin) && (bestRecoZ.M() < zMassMax);        

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
  tr->registerDerivedVar("bestRecoZPt", bestRecoZPt);
  tr->registerDerivedVar("bestRecoZM", bestRecoZ.M());
  tr->registerDerivedVar("cutMuPt1", cutMuPt1);
  tr->registerDerivedVar("cutMuPt2", cutMuPt2);


  tr->registerDerivedVec("cutMuVec", cutMuVec);
  tr->registerDerivedVec("cutMuActivity", cutMuActivity);
  tr->registerDerivedVar("passMuZinvSel", passMuZinvSel);
  tr->registerDerivedVar("passDiMuIsoTrig", passDiMuTrig);
  tr->registerDerivedVar("passSingleMu45", muTrigMu45);

  return true;
}       // -----  end of function VarPerEvent::GetMuInfo  -----

// ===  FUNCTION  ============================================================
//         Name:  VarPerEvent::GetJetsNoMu
//  Description:  Osbleted!
// ===========================================================================
bool VarPerEvent::GetJetsNoMu() const
{
  std::vector<TLorentzVector> *NewJets = new std::vector<TLorentzVector>(); //Muons pass pt, Iso and trigger requirement
  std::vector<double> *NewBJs = new std::vector<double>(); //Muons pass pt, Iso and trigger requirement
  const std::vector<TLorentzVector> cutMuVec  = tr->getVec<TLorentzVector>("cutMuVec");
  const std::vector<TLorentzVector> orgJets  = tr->getVec<TLorentzVector>("jetsLVec");

  typedef boost::bimap<boost::bimaps::set_of<unsigned int>, boost::bimaps::multiset_of<int> > JetMuType;
  JetMuType JetMuMatch; //index of jet, muon

  for(unsigned int j=0; j < cutMuVec.size(); ++j)
  {
    TLorentzVector muon = cutMuVec.at(j);
    int minJet = -1;
    double mindR = 999;
    for(unsigned int i=0; i < orgJets.size(); ++i)
    {
      TLorentzVector jet = orgJets.at(i);
      double dR = jet.DeltaR(muon);
      if (dR < 0.4 &&  dR < mindR)
      {
        minJet = i;
      }
    }
    JetMuMatch.insert({j, minJet});
  }

  typedef boost::bimap<boost::bimaps::set_of<unsigned int>, boost::bimaps::multiset_of<double> >  TLVPtType;
  TLVPtType NewJetPt; //index of jet, new Jet pt

  std::vector<TLorentzVector> tempJets;

  for(unsigned int i=0; i < orgJets.size(); ++i)
  {
    if (JetMuMatch.right.count(i) == 0)
    {
      NewJetPt.insert({i, orgJets.at(i).Pt()});
      tempJets.push_back(orgJets.at(i));
    } else{
      TLorentzVector oldJet = orgJets.at(i);
      BOOST_FOREACH(JetMuType::right_reference  rp, JetMuMatch.right.equal_range(i))
      {
        oldJet -= cutMuVec.at(rp.second);
      }
      NewJetPt.insert({i, oldJet.Pt()});
      tempJets.push_back(oldJet);
    }
  }

  // Sort and get the new Jet collection
  for(TLVPtType::right_reverse_iterator rp = NewJetPt.right.rbegin();rp != NewJetPt.right.rend(); rp++)
  {
    //std::cout <<  rp->first<<" " << rp->second <<"  " << tempJets.at(rp->second).Pt()<< std::endl;
    NewJets->push_back(tempJets.at(rp->second));
    NewBJs->push_back(tr->getVec<double>("recoJetsBtag_0").at(rp->second));
  }

  tr->registerDerivedVec("JetNoMuVec", NewJets);
  tr->registerDerivedVec("BtagNoMuVec", NewBJs);

  return true;
}       // -----  end of function VarPerEvent::GetJetsNoMu  -----


// ===  FUNCTION  ============================================================
//         Name:  VarPerEvent::PassDiMuonTrigger
//  Description:  /* cursor */
// ===========================================================================
bool VarPerEvent::PassDiMuonTrigger() const
{
  const std::vector<TLorentzVector> &cutMuVec = tr->getVec<TLorentzVector>("cutMuVec");
  assert(cutMuVec.size() == tr->getVar<int>("nMuons_Base"));
  const double minMuPt = 20.0;
  const double highMuPt = 45.0;
  bool pass = (cutMuVec.size() >= 2 && (cutMuVec)[0].Pt() > highMuPt && (cutMuVec)[1].Pt() > minMuPt);
  tr->registerDerivedVar("PassDiMuonTrigger", pass);
  return pass;
}       // -----  end of function VarPerEvent::PassDiMuonTrigger  -----

// ===  FUNCTION  ============================================================
//         Name:  VarPerEvent::PassDiEleTrigger
//  Description:  
// ===========================================================================
bool VarPerEvent::PassDiEleTrigger() const
{
  
  const std::vector<TLorentzVector> &cutEleVec = tr->getVec<TLorentzVector>("cutEleVec");
  assert(cutEleVec.size() == tr->getVar<int>("nElectrons_Base"));
  const double minElePt = 30.0;
  const double highElePt = 30.0;
  bool pass = (cutEleVec.size() >= 2 && (cutEleVec)[0].Pt() > highElePt && (cutEleVec)[1].Pt() > minElePt);
  tr->registerDerivedVar("PassDiEleTrigger", pass);
  return true;
}       // -----  end of function VarPerEvent::PassDiEleTrigger  -----

// ===  FUNCTION  ============================================================
//         Name:  VarPerEvent::GetEleZ
//  Description:  
// ===========================================================================
bool VarPerEvent::GetEleZ(std::vector<TLorentzVector>* recoZVec, TypeZLepIdx *ZLepIdx) const
{
  
  std::vector<TLorentzVector> elesLVec;
  std::vector<double>         elesRelIso;
  std::vector<double>         elesMiniIso;
  std::vector<double>         elesMtw;
  std::vector<double>         elesCharge;
  std::vector<int>            elesFlagIDVec;
  std::vector<unsigned int>            elesisEB;
  const std::string elesFlagIDLabel = "";

  try
  {
    elesLVec = tr->getVec<TLorentzVector>("elesLVec");
    elesMiniIso = tr->getVec<double>("elesMiniIso");
    elesCharge = tr->getVec<double>("elesCharge");
    elesRelIso =  tr->getVec<double>("elesRelIso");
    elesMtw =  tr->getVec<double>("elesRelIso");
    elesisEB =  tr->getVec<unsigned int>("elesRelIso");
    elesFlagIDVec = elesFlagIDLabel.empty()? 
      std::vector<int>(tr->getVec<double>("elesMiniIso").size(), 1):
      tr->getVec<int>(elesFlagIDLabel.c_str()); 
  }
  catch (std::string var)
  {
    std::cout << "Missing Var:"<< var << std::endl;
    return false;
  }

  //Muons pass pt, Iso and trigger requirement
  std::vector<TLorentzVector>* cutEleVec = new std::vector<TLorentzVector>(); 
  std::vector<double>* cutEleCharge = new std::vector<double>();
  std::vector<double>* cutEleActivity = new std::vector<double>();

  for(unsigned int i = 0; i < elesLVec.size(); ++i)
  {
    // emulates muons with pt and iso requirements.  
    if(AnaFunctions::passElectron(elesLVec[i], elesMiniIso[i], elesMtw[i], elesisEB[i], elesFlagIDVec[i], AnaConsts::elesMiniIsoArr) )
    {
      cutEleVec->push_back(elesLVec[i]);
      cutEleCharge->push_back(elesCharge[i]);
    }
  }

  const double zMassMin = 81.0;
  const double zMass    = 91.0;
  const double zMassMax = 101.0;
  const double minElePt = 30.0, highElePt = 30.0;

  double zMassCurrent = 1.0e300;
  //double zMassCurrent = 1.0e300, zEff = 1.0e100, zAcc = 1.0e100;
  TLorentzVector bestRecoZ(0, 0, 0, 0);
  int sumCharge = 0;
  std::pair<unsigned int, unsigned int> elepair;
  for(unsigned int i = 0; i < cutEleVec->size(); ++i)
  {
    if((*cutEleVec)[i].Pt() < minElePt) continue;
    for(unsigned int j = 0; j < i && j < cutEleVec->size(); ++j)
    {
      if((*cutEleVec)[j].Pt() < minElePt) continue;
      double zm = ((*cutEleVec)[i] + (*cutEleVec)[j]).M();
      //if(zm > zMassMin && zm < zMassMax && fabs(zm - zMass) < fabs(zMassCurrent - zMass))
      if(fabs(zm - zMass) < fabs(zMassCurrent - zMass))
      {
        bestRecoZ = (*cutEleVec)[i] + (*cutEleVec)[j];
        zMassCurrent = zm;
        elepair = std::make_pair(i+100, j+100);
        sumCharge = 0;
        sumCharge = cutEleCharge->at(i) + cutEleCharge->at(j);
      }
    }
  }
  if (bestRecoZ.Pt() != 0 && sumCharge == 0)
  {
    recoZVec->push_back(bestRecoZ);
    ZLepIdx->insert(std::make_pair( recoZVec->size(), elepair));
  }
  tr->registerDerivedVec("cutEleVec", cutEleVec);
  return true;
}       // -----  end of function VarPerEvent::GetEleZ  -----
// ===  FUNCTION  ============================================================
//         Name:  VarPerEvent::GetRecoZ
//  Description:  
// ===========================================================================
bool VarPerEvent::GetRecoZ() const
{
  std::vector<TLorentzVector>* recoZVec = new std::vector<TLorentzVector>();
  std::map<unsigned int, std::pair<unsigned int, unsigned int> > *ZLepIdx = 
    new std::map<unsigned int, std::pair<unsigned int, unsigned int> >();

  GetMuInfo(recoZVec, ZLepIdx);
  GetEleZ(recoZVec, ZLepIdx);

  // Setting the clean MET
  TLorentzVector metV(0, 0, 0, 0);
  metV.SetPtEtaPhiM(tr->getVar<double>("met"), 0.0, tr->getVar<double>("metphi"), 0.0);
  TLorentzVector cleanMet(metV);
  for(unsigned int i=0; i < recoZVec->size(); ++i)
  {
    TLorentzVector bestRecoZ = recoZVec->at(i);
    TLorentzVector metZ(0, 0,0,0);
    metZ.SetPtEtaPhiM(bestRecoZ.Pt(), 0.0, bestRecoZ.Phi(), 0.0);
    cleanMet += metZ;
  }

  tr->registerDerivedVar("cleanMetPt", cleanMet.Pt());
  tr->registerDerivedVar("cleanMetPhi", cleanMet.Phi());
  tr->registerDerivedVec("recoZVec", recoZVec);
  tr->registerDerivedVec("ZLepIdx", ZLepIdx);
  return true;
}       // -----  end of function VarPerEvent::GetRecoZ  -----

// ===  FUNCTION  ============================================================
//         Name:  VarPerEvent::GetGenZ
//  Description:  
// ===========================================================================
//bool VarPerEvent::GetGenZ() const
//{
  //std::vector<int>            genDecayPdgIdVec;
  //std::vector<TLorentzVector> genDecayLVec;
  //std::vector<TLorentzVector> jetsLVec;

  //try
  //{
    //genDecayPdgIdVec                    = tr->getVec<int>("genDecayPdgIdVec");
    //genDecayLVec                        = tr->getVec<TLorentzVector>("genDecayLVec");
    //jetsLVec                            = tr->getVec<TLorentzVector>("jetsLVec");
  //}
  //catch (std::string var)
  //{
    //std::cout << "Missing inputs for VarPerEvent::GetMuInfo" << std::endl;
    //return false;
  //}
  
  //std::vector<const TLorentzVector*>* genMatchIsoMuInAcc = new std::vector<const TLorentzVector*>();
  //std::vector<const TLorentzVector*>* genMatchMuInAcc = new std::vector<const TLorentzVector*>();
  //std::vector<double>* genMatchMuInAccRes = new std::vector<double>();
  //std::vector<const TLorentzVector*>* genMuInAcc = new std::vector<const TLorentzVector*>();
  //std::vector<const TLorentzVector*>* genMu = new std::vector<const TLorentzVector*>();
  //std::vector<double>* genMatchIsoMuInAccAct = new std::vector<double>();
  //std::vector<double>* genMatchMuInAccAct = new std::vector<double>();
  //std::vector<double>* genMuInAccAct = new std::vector<double>();
  //std::vector<const TLorentzVector*>* genZ = new std::vector<const TLorentzVector*>();

  ///[>************************************************************************<]/
  ////                                 Gen Study                                //
  ///[>************************************************************************<]/
  //double genHt = 0.0;
  //double nuPt1 = -999.9, nuPt2 = -999.9;
  //const double minMuPt = 20.0, highMuPt = 45.0;

  //for(unsigned int i = 0; i < genDecayPdgIdVec.size() && i < genDecayLVec.size(); ++i)
  //{
    //if((abs(genDecayPdgIdVec[i]) != 0 &&  abs(genDecayPdgIdVec[i]) < 6) ||
        //(abs(genDecayPdgIdVec[i]) > 100 && abs(genDecayPdgIdVec[i]) < 10000)) 
      //genHt += genDecayLVec[i].Pt();

    //if(genDecayPdgIdVec[i] ==  13) nuPt1 = genDecayLVec[i].Pt();
    //if(genDecayPdgIdVec[i] == -13) nuPt2 = genDecayLVec[i].Pt();

    //if(abs(genDecayPdgIdVec[i]) == 13)
    //{
      //genMu->push_back(&genDecayLVec[i]);
      //if(AnaFunctions::passMuonAccOnly(genDecayLVec[i], AnaConsts::muonsMiniIsoArr) && genDecayLVec[i].Pt() > minMuPt)
      //{
        //genMuInAcc->push_back(&genDecayLVec[i]);
        //double dRMin = 999.9;
        //double matchPt = -999.9;
        //for(unsigned int j = 0; j < cutMuVecRecoOnly.size(); ++j)
        //{
          //double dR = ROOT::Math::VectorUtil::DeltaR(genDecayLVec[i], cutMuVecRecoOnly[j]);
          //if(dR < dRMin)
          //{
            //dRMin = dR;
            //matchPt = cutMuVecRecoOnly[j].Pt();
          //}
        //}
        //if(dRMin < 0.02)
        //{
          //genMatchMuInAcc->push_back(&genDecayLVec[i]);
          //genMatchMuInAccRes->push_back((genDecayLVec[i].Pt() - matchPt)/genDecayLVec[i].Pt());
        //}

        //dRMin = 999.9;
        //for(unsigned int j = 0; j < cutMuVec->size(); ++j)
        //{
          //double dR = ROOT::Math::VectorUtil::DeltaR(genDecayLVec[i], (*cutMuVec)[j]);
          //if(dR < dRMin)
          //{
            //dRMin = dR;
          //}
        //}
        //if(dRMin < 0.02)
        //{
          //genMatchIsoMuInAcc->push_back(&genDecayLVec[i]);
        //}
      //}
    //}
  //}

  //double genZPt = -999.9, genZEta = -999.9, genZmass = -999.9, genZPhi;
  //int nZ = 0;
  //TLorentzVector genZ1;
  //for(unsigned int j = 0; j <  genDecayPdgIdVec.size(); ++j)
  //{
    //if(abs(genDecayPdgIdVec[j]) == 23)
    //{
      //nZ++;
      //genZ->push_back(&genDecayLVec[j]);
      //genZ1 = genDecayLVec[j];
      //genZPt = genDecayLVec[j].Pt();
      //genZEta = genDecayLVec[j].Eta();
      //genZPhi = genDecayLVec[j].Phi();
      //genZmass = genDecayLVec[j].M();
    //}
  //}
  //int pdgIdZDec = 0;
  //if(W_emuVec.size() == 0) pdgIdZDec = 15;
  //else if(W_emuVec.size() == 2)
  //{
    //if(abs(genDecayPdgIdVec[W_emuVec[0]]) == 11) pdgIdZDec = 11;
    //else if(abs(genDecayPdgIdVec[W_emuVec[0]]) == 13) pdgIdZDec = 13;
  //}

  //tr->registerDerivedVar("ZPtRes", (bestRecoZPt - genZPt)/genZPt);
  //tr->registerDerivedVar("genHt", genHt);
  //tr->registerDerivedVar("ZEtaRes", bestRecoZ.Eta() - genZEta);
  //tr->registerDerivedVar("ZPhiRes", bestRecoZ.Phi() - genZPhi);
  //tr->registerDerivedVar("ZMRes", (bestRecoZ.M() - genZmass)/genZmass);
  //tr->registerDerivedVec("genMu", genMu);
  //tr->registerDerivedVec("genZ", genZ);
  //tr->registerDerivedVar("ngenMu", static_cast<double>(genMu->size()));
  //tr->registerDerivedVec("genMuInAcc", genMuInAcc);
  //tr->registerDerivedVar("ngenMuInAcc", static_cast<double>(genMuInAcc->size()));
  //tr->registerDerivedVec("genMuInAccAct", genMuInAccAct);
  //tr->registerDerivedVec("genMatchMuInAcc", genMatchMuInAcc);
  //tr->registerDerivedVec("genMatchMuInAccRes", genMatchMuInAccRes);
  //tr->registerDerivedVec("genMatchIsoMuInAcc", genMatchIsoMuInAcc);
  //tr->registerDerivedVar("ngenMatchMuInAcc", static_cast<double>(genMatchMuInAcc->size()));
  //tr->registerDerivedVec("genMatchMuInAccAct", genMatchMuInAccAct);
  //tr->registerDerivedVec("genMatchIsoMuInAccAct", genMatchIsoMuInAccAct);
  //tr->registerDerivedVar("genZPt", genZPt);
  //tr->registerDerivedVar("genZEta", genZEta);
  //tr->registerDerivedVar("genZmass", genZmass);
  //tr->registerDerivedVar("pdgIdZDec", pdgIdZDec);
  //return true;
//}       // -----  end of function VarPerEvent::GetGenZ  -----
