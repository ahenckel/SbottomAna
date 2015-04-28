// ===========================================================================
// 
//       Filename:  PatTauAna.cc
// 
//    Description: 
// 
//        Version:  1.0
//        Created:  03/09/2015 03:07:50 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  University of Illinois at Chicago, CMS@LPC
// 
// ===========================================================================


#include  "UserCode/SuperTopTagger/interface/PatTauAna.h"

//
// constructors and destructor
//
PatTauAna::PatTauAna(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  PruneGenParticleInputTag_= iConfig.getParameter<edm::InputTag>("PruneGenParticleInputTag");
  PackedGenParticleInputTag_= iConfig.getParameter<edm::InputTag>("PackedGenParticleInputTag");
  prodTauTag_ = iConfig.getParameter<edm::InputTag>("prodTauTag");
  prodMETTag_ = iConfig.getParameter<edm::InputTag>("prodMETTag");

  DecayMode  = iConfig.getUntrackedParameter<std::string>("DecayModeTag",  "decayModeFinding");
  Isolation  = iConfig.getUntrackedParameter<std::string>("IsolationTag",  "byLooseCombinedIsolationDeltaBetaCorr3Hits");
  MuonReject = iConfig.getUntrackedParameter<std::string>("MuonRejectTag", "againstMuonTight3");
  EleReject  = iConfig.getUntrackedParameter<std::string>("EleRejectTag",  "againstElectronMediumMVA5");

  hist = new HistTool("Tau");
  AnaPtr = new GenParAna(hist);
  BookHistograms();
 
}

PatTauAna::~PatTauAna()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
  void
PatTauAna::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  iEvent.getByLabel(prodTauTag_, prodTauHdl); 
  iEvent.getByLabel(PruneGenParticleInputTag_, PrunedGenHdl); 
  iEvent.getByLabel(PackedGenParticleInputTag_, PackedGenHdl); 
  iEvent.getByLabel(prodMETTag_, prodMETHdl); 



#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif

  hist->FillTH1("evtcount", 1);
  //----------------------------------------------------------------------------
  //  Analysis Classes
  //----------------------------------------------------------------------------
  AnaPtr->GetGenParticles(PrunedGenHdl, PackedGenHdl);
  //AnaPtr->PrintGenPars(PrunedGenHdl);
  //AnaPtr->GetTopandW(PrunedGenHdl);
  //AnaPtr->PrintTopDecay();
  if (!AnaPtr->GetTau(PrunedGenHdl)) return;
  GetTaus();

  PatTauTagging(AnaPtr->Tauidx);

  TauIDVar();
  //SUSYSignalEff();

}


// ------------ method called once each job just before starting event loop  ------------
  void 
PatTauAna::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
  void 
PatTauAna::endJob() 
{
  hist->CalEfficiency("TauPT_Eff", "TauPT_Num", "TauPT_Dem");
  hist->CalEfficiency("TauEta_Eff", "TauEta_Num", "TauEta_Dem");
  hist->CalEfficiency("MisstagRate", "EventMETwithTaumisstag", "EventMET");
  hist->CalEfficiency("TauJetPT_Eff", "TauJetPT_Num", "TauJetPT_Dem");
  hist->CalEfficiency("TauJetEta_Eff", "TauJetEta_Num", "TauJetEta_Dem");
}

// ------------ method called when starting to processes a run  ------------
  void 
PatTauAna::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
  void 
PatTauAna::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
  void 
PatTauAna::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
  void 
PatTauAna::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
  void
PatTauAna::fillDescriptions(edm::ConfigurationDescriptions& descriptions) 
{
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

// ===  FUNCTION  ============================================================
//         Name:  PatTauAna::PatTauTagging
//  Description:  
// ===========================================================================
bool PatTauAna::PatTauTagging(std::vector<TauDecay> &Tauidx) 
{
  for (size_t i = 0; i < Tauidx.size(); ++i)
  {
    TauDecay gentau = Tauidx.at(i);
    if (!gentau.isLeptonic_ || gentau.Lepidx_.Pruned == NULL)  continue;
    hist->FillTH1("TauPT_Dem", gentau.tauidx_.Pruned->pt());
    hist->FillTH1("TauEta_Dem", gentau.tauidx_.Pruned->eta());

    std::vector<TLorentzVector> genTauProds = AnaPtr->ProdGenLVs(AnaPtr->FilterNeutrinos(gentau.tauidx_.Packed), 3, 0.4);
    if (genTauProds.size() < 1) continue;
    hist->FillTH1("NTauGenJet", double(genTauProds.size()));
    hist->FillTH1("TauJetPT_Dem", genTauProds.at(0).Pt());
    hist->FillTH1("TauJetEta_Dem", genTauProds.at(0).Eta());

    for(unsigned int j=0; j < genTauProds.size(); ++j)
    {
      TLorentzVector gen = genTauProds.at(j);
      TLorentzVector tau(gentau.tauidx_.Pruned->px(), gentau.tauidx_.Pruned->py(), gentau.tauidx_.Pruned->pz(), gentau.tauidx_.Pruned->energy());
      hist->FillTH1("deltaRGenJet_Tau", gen.DeltaR(tau));
    }

    bool foundmatch = false;
    for(unsigned int j=0; j < vTaus.size(); ++j)
    {
      pat::Tau tau = vTaus.at(j);

      //assert(genTauProds.size() == 1);
      for(unsigned int k=0; k < genTauProds.size(); ++k)
      {
        TLorentzVector  gentauprod = genTauProds.at(k);
        //TLorentzVector  gentauprod(gentau.tauidx_.Pruned->px(), gentau.tauidx_.Pruned->py(), gentau.tauidx_.Pruned->pz(), gentau.tauidx_.Pruned->energy());
        TLorentzVector  pattau(tau.px(), tau.py(), tau.pz(), tau.energy());
        if (gentauprod.DeltaR(pattau) < 0.4)
        {
          hist->FillTH1("TauPT_Num", gentau.tauidx_.Pruned->pt());
          hist->FillTH1("TauEta_Num", gentau.tauidx_.Pruned->eta());
          hist->FillTH1("TauJetPT_Num", genTauProds.at(0).Pt());
          hist->FillTH1("TauJetEta_Num", genTauProds.at(0).Eta());
          foundmatch = true;
          break;
        }
      }
    }

    if (! foundmatch)
    {
      hist->FillTH1("TauPT_Miss", gentau.tauidx_.Pruned->pt());
      hist->FillTH1("TauEta_Miss", gentau.tauidx_.Pruned->eta());
    }
  }

  return true;
}       // -----  end of function PatTauAna::PatTauTagging  -----

// ===  FUNCTION  ============================================================
//         Name:  PatTauAna::BookHistograms
//  Description:  
// ===========================================================================
bool PatTauAna::BookHistograms()
{
  hist->AddTH1("evtcount", "evtcount", "evtcount", "Number of Tau", 2, 0, 2);
  hist->AddTH1("TauPT", "Tau PT", "Reco P_{T}^{#tau}", "Number of Tau", 100, 0, 500);
  hist->AddTH1("TauPT_Num", "Numerator of Tau PT", 100, 0, 500);
  hist->AddTH1("TauPT_Dem", "DeNumerator of Tau PT", 100, 0, 500);
  hist->AddTH1("TauPT_Eff", "Eff of Tau PT", "Gen P_{T}^{#tau}", "Tau ID Efficiency",  100, 0, 500);

  hist->AddTH1("TauJetPT_Num", "Numerator of TauJet PT", 20, 0, 100);
  hist->AddTH1("TauJetPT_Dem", "DeNumerator of TauJet PT", 20, 0, 100);
  hist->AddTH1("TauJetPT_Eff", "Eff of TauJet PT", "Gen P_{T}^{#TauJet}", "TauJet ID Efficiency",  20, 0, 100);

  hist->AddTH1("TauEta", "Tau Eta",  "Reco #eta^{#tau}", "Number of Tau", 100, 5, -5);
  hist->AddTH1("TauEta_Num", "Numerator of Tau Eta", 100, -5, 5);
  hist->AddTH1("TauEta_Dem", "DeNumerator of Tau Eta", 100, -5, 5);
  hist->AddTH1("TauEta_Eff", "Eff of Tau Eta","Gen #eta^{#tau}", "Tau ID Efficiency",  100, -5, 5);

  hist->AddTH1("TauJetEta_Num", "Numerator of TauJet Eta", 100, -2.5, 2.5);
  hist->AddTH1("TauJetEta_Dem", "DeNumerator of TauJet Eta", 100, -2.5, 2.5);
  hist->AddTH1("TauJetEta_Eff", "Eff of TauJet Eta","Gen #eta^{#TauJet}", "TauJet ID Efficiency",  100, -2.5, 2.5);

  hist->AddTH1("TauPT_Miss", "Misserator of Tau PT", "Gen P_{T}^{#tau}", "Number of Tau not found by PatTau", 100, 0, 500);
  hist->AddTH1("TauEta_Miss", "Misserator of Tau Eta", "Gen #eta^{#tau}", "Number of Tau not found by PatTau", 100, -5, 5);
  hist->AddTH1("TauPT_Mistag", "Misserator of Tau PT", "Reco P_{T}^{#tau}", "Number of Tau in all hadronic top", 100, 0, 500);
  hist->AddTH1("TauEta_Mistag", "Misserator of Tau Eta", "Reco #eta^{#tau}", "Number of Tau in all hadronic top", 100, -5, 5);

  hist->AddTH1("NTauGenJet", "NTauGenJet", 5, 0, 5);
  hist->AddTH1("deltaRGenJet_Tau", "deltaRGenJet_Tau", 10, 0, 1);
  hist->AddTH1("EventMET", "EventMET", "#slash{E}_{T}","Number of all hadronic top",  100, 0, 500);
  hist->AddTH1("EventMETwithTaumisstag", "EventMETwithTaumisstag",  "#slash{E}_{T}","Number of events with pattau", 100, 0, 500);
  hist->AddTH1("MisstagRate", "MisstagRate",  "#slash{E}_{T}","PatTau Veto Rate in all hadronic tops", 100, 0, 500);

  return true;
}       // -----  end of function PatTauAna::BookHistograms  -----

//// ===  FUNCTION  ============================================================
////         Name:  PatTauAna::GenTauAna
////  Description:  
//// ===========================================================================
//bool PatTauAna::GenTauAna() const
//{
  

  //return true;
//}       // -----  end of function PatTauAna::GenTauAna  -----
//

// ===  FUNCTION  ============================================================
//         Name:  PatTauAna::PassTauID
//  Description:  CMS Standard Tau ID from Tau POG
//  https://twiki.cern.ch/twiki/bin/viewauth/CMS/TauIDRecommendation13TeV/
//  Recommended by Christian Veelken 
//      decayModeFindingOldDMs
//      byLooseCombinedIsolationDeltaBetaCorr3Hits
//      againstMuonTight3
//      againstElectronMediumMVA5
// ===========================================================================
std::vector<pat::Tau> PatTauAna::PassTauID()
{
  std::vector<pat::Tau> temp;

  for(unsigned int i=0; i < prodTauHdl->size(); ++i)
  {
    pat::Tau tau = prodTauHdl->at(i);
    //std::cout << "Tau ID DecayMode: " << tau.tauID(DecayMode) 
      //<< " Isolation: " << tau.tauID(Isolation)
      //<< " MuonReject: " << tau.tauID(MuonReject)
      //<< " EleReject: " << tau.tauID(EleReject)
      //<< std::endl;
    // Decay mode: 
    // decayModeFindingOldDMs
    // decayModeFindingNewDMs (nly for analysis involving high pT taus, like !SUSY searches)
    if (DecayMode != "" && tau.isTauIDAvailable(DecayMode))
    {
      if (tau.tauID(DecayMode)  < 0.5)
        break;
    }

    // Isolation:
    // byLooseCombinedIsolationDeltaBetaCorr3Hits
    // byMediumCombinedIsolationDeltaBetaCorr3Hits
    // byTightCombinedIsolationDeltaBetaCorr3Hits
    if (Isolation != "" && tau.isTauIDAvailable(Isolation))
    {
      if (tau.tauID(Isolation)  < 0.5)
        break;
    }

    // Muon Rejection:
    // againstMuonLoose3
    // againstMuonTight3
    if (MuonReject != "" && tau.isTauIDAvailable(MuonReject))
    {
      if (tau.tauID(MuonReject)  < 0.5)
        break;
    }

    // Electron Rejection:
    // againstElectronVLooseMVA5 
    // againstElectronLooseMVA5
    // againstElectronMediumMVA5

    if (EleReject!= "" && tau.isTauIDAvailable(EleReject))
    {
      if (tau.tauID(EleReject)  < 0.5)
        break;
    }

    //std::cout << "Tau ID DecayMode: " << tau.tauID(DecayMode) 
      //<< " Isolation: " << tau.tauID(Isolation)
      //<< " MuonReject: " << tau.tauID(MuonReject)
      //<< " EleReject: " << tau.tauID(EleReject)
      //<< std::endl;
    temp.push_back(tau);
  }

  return temp;
}       // -----  end of function PatTauAna::PassTauID  -----

// ===  FUNCTION  ============================================================
//         Name:  PatTauAna::GetTaus
//  Description:  
// ===========================================================================
bool PatTauAna::GetTaus()
{
  
  vTaus.clear();
  vTaus = PassTauID();
  return true;
}       // -----  end of function PatTauAna::GetTaus  -----

// ===  FUNCTION  ============================================================
//         Name:  PatTauAna::TauIDVar
//  Description:  
// ===========================================================================
bool PatTauAna::TauIDVar() const
{
  for(unsigned int j=0; j < vTaus.size(); ++j)
  {
    pat::Tau tau = vTaus.at(j);
    hist->FillTH1("TauPT", tau.pt());
    hist->FillTH1("TauEta", tau.eta());
  }

  return true;
}       // -----  end of function PatTauAna::TauIDVar  -----

// ===  FUNCTION  ============================================================
//         Name:  PatTauAna::SUSYSignalEff
//  Description:  
// ===========================================================================
bool PatTauAna::SUSYSignalEff()
{
  AnaPtr->GetTopandW(PrunedGenHdl);

  int countlep = 0;
  for(unsigned int i=0; i < AnaPtr->Topidx.size(); ++i)
  {
    TopDecay gtop = AnaPtr->Topidx.at(i);
    if (gtop.widx_.isLeptonic_) countlep++;
  }

  if (countlep > 0) return false;
  assert(prodMETHdl->size() == 1);

  const pat::MET MET = prodMETHdl->at(0);
  hist->FillTH1("EventMET", MET.pt());
  if (vTaus.size() != 0)
  hist->FillTH1("EventMETwithTaumisstag", MET.pt());

  for(unsigned int j=0; j < vTaus.size(); ++j)
  {
    pat::Tau tau = vTaus.at(j);
    hist->FillTH1("TauPT_Mistag", tau.pt());
    hist->FillTH1("TauEta_Mistag", tau.eta());
  }
  return true ;
}       // -----  end of function PatTauAna::SUSYSignalEff  -----



