// ===========================================================================
// 
//       Filename:  StopBaselineFilter.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03/14/2015 07:59:54 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  University of Illinois at Chicago, CMS@LPC
// 
// ===========================================================================

#include  "UserCode/SuperTopTagger/interface/StopBaselineFilter.h"

//
// constructors and destructor
//
StopBaseline::StopBaseline(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Electron ~~~~~
  prodElectronTag_ = iConfig.getParameter<edm::InputTag>("prodElectronTag");
  prodEleRelIsoTag_ = iConfig.getParameter<edm::InputTag>("prodEleRelIsoTag");
  prodEleMtwTag_ = iConfig.getParameter<edm::InputTag>("prodEleMtwTag");
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Muon ~~~~~
  prodMuonTag_ = iConfig.getParameter<edm::InputTag>("prodMuonTag");
  prodMuonRelIsoTag_ = iConfig.getParameter<edm::InputTag>("prodMuonRelIsoTag");
  prodMuonMtwTag_ = iConfig.getParameter<edm::InputTag>("prodMuonMtwTag");
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Isotrack ~~~~~
  IsoTrackTag_ = iConfig.getParameter<edm::InputTag>("IsoTrackTag");
  IsoTrackIsoTag_ = iConfig.getParameter<edm::InputTag>("IsoTrackIsoTag");
  IsoTrackMtwTag_ = iConfig.getParameter<edm::InputTag>("IsoTrackMtwTag");
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Tau ~~~~~
  prodTauTag_ = iConfig.getParameter<edm::InputTag>("prodTauTag");
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Jet ~~~~~
  prodJetTag_ = iConfig.getParameter<edm::InputTag>("prodJetTag");
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MET ~~~~~
  prodMETTag_ = iConfig.getParameter<edm::InputTag>("prodMETTag");


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ GetBaselineTH1 ~~~~~
 cutnames.push_back("All");
 cutnames.push_back("JetCount50");
 cutnames.push_back("JetCount30");
 cutnames.push_back("MuonVeto");
 cutnames.push_back("EleVeto");
 cutnames.push_back("METCut");
 cutnames.push_back("deltaPhi");
 cutnames.push_back("bjet");
 cutflow = fs->make<TH1D>("Cutflow", "cutflow" , cutnames.size() , 0, cutnames.size());
 cutbins = fs->make<TH1D>("cutbins", "cutbins" , cutnames.size() , 0, cutnames.size());
 for(unsigned int i=0; i < cutnames.size(); ++i)
 {
   cutflow->GetXaxis()->SetBinLabel(i+1, cutnames.at(i).c_str());
   cutbins->GetXaxis()->SetBinLabel(i+1, cutnames.at(i).c_str());
 }
 dPhiJ1MET =  fs->make<TH1D>("dPhiJ1MET", "dPhiJ1MET" , 100, 0, 3.2);
 dPhiJ2MET =  fs->make<TH1D>("dPhiJ2MET", "dPhiJ2MET" , 100, 0, 3.2);
 dPhiJ3MET =  fs->make<TH1D>("dPhiJ3MET", "dPhiJ3MET" , 100, 0, 3.2);

}


StopBaseline::~StopBaseline()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
  bool
StopBaseline::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  iEvent.getByLabel(prodElectronTag_, prodElectronHdl); 
  iEvent.getByLabel(prodEleRelIsoTag_, prodEleRelIsoHdl); 
  iEvent.getByLabel(prodEleMtwTag_, prodEleMtwHdl); 
  iEvent.getByLabel(prodMuonTag_, prodMuonHdl); 
  iEvent.getByLabel(prodMuonRelIsoTag_, prodMuonRelIsoHdl); 
  iEvent.getByLabel(prodMuonMtwTag_, prodMuonMtwHdl); 
  iEvent.getByLabel(IsoTrackTag_, IsoTrackHdl); 
  iEvent.getByLabel(IsoTrackIsoTag_, IsoTrackIsoHdl); 
  iEvent.getByLabel(IsoTrackMtwTag_, IsoTrackMtwHdl); 
  iEvent.getByLabel(prodTauTag_, prodTauHdl); 
  iEvent.getByLabel(prodJetTag_, prodJetHdl); 
  iEvent.getByLabel(prodMETTag_, prodMETHdl); 

#ifdef THIS_IS_AN_EVENT_EXAMPLE
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
#endif

  return PassBaseline();
}

// ------------ method called once each job just before starting event loop  ------------
  void 
StopBaseline::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
StopBaseline::endJob() {
}

// ------------ method called when starting to processes a run  ------------
  bool 
StopBaseline::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

// ------------ method called when ending the processing of a run  ------------
  bool 
StopBaseline::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
  bool 
StopBaseline::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
  bool 
StopBaseline::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
  void
StopBaseline::fillDescriptions(edm::ConfigurationDescriptions& descriptions) 
{
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

// ===  FUNCTION  ============================================================
//         Name:  StopBaseline::CountLeps
//  Description:  Template function for counting muons, eles, and isotracks
//  Taking the threshold with minAbsEta, maxAbsEta, minPt, maxPt, maxrelIso, maxMtw
//
// ===========================================================================
    template <class T>
int StopBaseline::CountLeps(const std::vector<T>* vLeps, const std::vector<double>* vIsos, 
    const std::vector<double> *vMtws, const double Arr[]) const
{
  assert(vLeps->size() == vIsos->size());
  assert(vLeps->size() == vMtws->size());
  const double minAbsEta = Arr[0];
  const double maxAbsEta = Arr[1]; 
  const double minPt = Arr[2]; 
  const double maxPt = Arr[3]; 
  const double maxRelIso = Arr[4]; 
  const double maxMtw = Arr[5];

  int cnt = 0;

  for(unsigned int i=0; i < vLeps->size(); ++i)
  {
    T lep = vLeps->at(i);
    if(   ( minAbsEta == -1 || fabs(lep.eta()) >= minAbsEta )
        && ( maxAbsEta == -1 || fabs(lep.eta()) < maxAbsEta )
        && (     minPt == -1 || lep.pt() >= minPt )
        && (     maxPt == -1 || lep.pt() < maxPt ) ){

      if( maxRelIso != -1 && vIsos->at(i) >= maxRelIso ) continue;
      if( maxMtw != -1 && vMtws->at(i) >= maxMtw ) continue;
      cnt ++ ;
    }
  }
  return cnt;
}       // -----  end of function StopBaseline::CountLeps  -----

// ===  FUNCTION  ============================================================
//         Name:  StopBaseline::PassBaseline
//  Description:  Following the cut flow as page 8 of 
//  https://indico.cern.ch/event/365497/session/7/contribution/48/material/slides/0.pdf
// ===========================================================================
bool StopBaseline::PassBaseline()
{
  passBaseline = true;
  cutbins->Fill(0);
  cutflow->Fill(0);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Jet ~~~~~
// AK4PF CHS jets (slimmed jets)
// Njets (PT>50GeV, |eta| < 2.4) >=2
// Njets (PT>30GeV, |eta| < 2.4) >=4
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Jet ~~~~~
  int cntNJetsPt50Eta24 = PassJetCuts(AnaConsts::pt50Eta24Arr);
  int cntNJetsPt30Eta24 = PassJetCuts(AnaConsts::pt30Eta24Arr);
  //int cntNJetsPt30      = PassJetCuts(AnaConsts::pt30Arr);
   if( cntNJetsPt50Eta24 < AnaConsts::nJetsSelPt50Eta24 ){ passBaseline = false; }
   else
   {
     cutbins->Fill(1);
     if (passBaseline) cutflow->Fill(1);
   }
   if( cntNJetsPt30Eta24 < AnaConsts::nJetsSelPt30Eta24 ){ passBaseline = false; }
   else 
   {
     cutbins->Fill(2);
     if (passBaseline) cutflow->Fill(2);
   }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Lepton Veto ~~~~~
// POG Muon and Electron (prod Ele/Muon)
// Muon: pT>5GeV, |eta| < 2.4, rellso < 0.2
// Electron: pT>5GeV, |eta| < 2.5, relIso < 0.15
// Isolated Track: pT> 10GEV, relIso < 0.1, mt< 100GeV
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Lepton Veto ~~~~~
   assert(prodMuonHdl.isValid());
   assert(prodMuonRelIsoHdl.isValid());
   assert(prodMuonMtwHdl.isValid());
   int nMuons     = CountLeps<pat::Muon>(prodMuonHdl.product(),            prodMuonRelIsoHdl.product(), prodMuonMtwHdl.product(), AnaConsts::muonsArr);
   assert(prodElectronHdl.isValid());
   assert(prodEleRelIsoHdl.isValid());
   assert(prodEleMtwHdl.isValid());
   int nElectrons = CountLeps<pat::Electron>(prodElectronHdl.product(),    prodEleRelIsoHdl.product(),  prodEleMtwHdl.product(),  AnaConsts::elesArr);
   if( nMuons != AnaConsts::nMuonsSel ){ passBaseline = false; }
   else 
   {
     cutbins->Fill(3);
     if (passBaseline) cutflow->Fill(3);
   }
   if( nElectrons != AnaConsts::nElectronsSel ){ passBaseline = false;}
   else
   {
     cutbins->Fill(4);
     if (passBaseline) cutflow->Fill(4);
   }

// Isolated track veto is disabled for now
   //int nIsoTrks   = CountLeps<pat::PackedCandidate>(IsoTrackHdl.product(), IsoTrackIsoHdl.product(),    IsoTrackMtwHdl.product(), AnaConsts::isoTrksArr);
//   if( nIsoTrks != AnaConsts::nIsoTrksSel ){ passBaseline = false; passLeptVeto = false; }


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MET ~~~~~
// slimmedMETs (type-I corrected)
// MET > 200GeV (minimum trigger requirement)
// delta_phi(j1, j2, j3; MET) > 0.5, 0.5, 0.3
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MET ~~~~~
   if( GetMET() < AnaConsts::defaultMETcut ){ passBaseline = false;}
   else 
   {
     cutbins->Fill(5);
     if (passBaseline) cutflow->Fill(5);

   }
   if (!PassJetMETCuts( AnaConsts::dPhi0_CUT, AnaConsts::dPhi1_CUT, AnaConsts::dPhi2_CUT))  {passBaseline = false;}
   else 
   {
     cutbins->Fill(6);
     if (passBaseline) cutflow->Fill(6);
   }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ b-tag ~~~~~
// New CSV+IVF algorithm
// pT> 30GeV, |eta| < 2.5, medim WP > 0.814
// Nbjet >= 1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ b-tag ~~~~~
   int  cntCSVS =  countBJets( "combinedInclusiveSecondaryVertexV2BJetTags", AnaConsts::cutCSVS, AnaConsts::bTagArr);
   if( !( (AnaConsts::low_nJetsSelBtagged == -1 || cntCSVS >= AnaConsts::low_nJetsSelBtagged) && 
         (AnaConsts::high_nJetsSelBtagged == -1 || cntCSVS < AnaConsts::high_nJetsSelBtagged ) ) )
   { passBaseline = false;}
   else 
   {
     cutbins->Fill(7);
     if (passBaseline) cutflow->Fill(7);
   }

  return passBaseline;
}

// ===  FUNCTION  ============================================================
//         Name:  StopBaseline::PassJetCuts
//  Description:  
// ===========================================================================
int StopBaseline::PassJetCuts(const double Arr[]) const
{
  const double minAbsEta = Arr[0];
  const double maxAbsEta = Arr[1]; 
  const double minPt = Arr[2]; 
  const double maxPt = Arr[3]; 

  assert(prodJetHdl.isValid());

  int cnt = 0;

  for(unsigned int i=0; i < prodJetHdl->size(); ++i)
  {
    pat::Jet jet = prodJetHdl->at(i);

    //std::cout << " index " << i <<" pt " << jet.pt() <<" eta " << jet.eta() << std::endl;

    if(   ( minAbsEta == -1 || fabs(jet.eta()) >= minAbsEta )
        && ( maxAbsEta == -1 || fabs(jet.eta()) < maxAbsEta )
        && (     minPt == -1 || jet.pt() >= minPt )
        && (     maxPt == -1 || jet.pt() < maxPt ) ){
      cnt ++ ;
      //std::cout << " cnt " << cnt << std::endl;
    }
  }
  return cnt;
}       // -----  end of function StopBaseline::PassJetCuts  -----

// ===  FUNCTION  ============================================================
//         Name:  StopBaseline::GetMET
//  Description:  
// ===========================================================================
double StopBaseline::GetMET() const
{
  assert(prodMETHdl.isValid());
  pat::MET met = prodMETHdl->at(0);
  return met.pt();
}       // -----  end of function StopBaseline::GetMET  -----


// ===  FUNCTION  ============================================================
//         Name:  StopBaseline::PassMETCuts
//  Description:  
// ===========================================================================
bool StopBaseline::PassMETCuts(const double defaultMETcut) const
{
  assert(prodMETHdl.isValid());
  pat::MET met = prodMETHdl->at(0);
  
  if (met.pt() < defaultMETcut) return false;

  return true;
}       // -----  end of function StopBaseline::PassMETCuts  -----

// ===  FUNCTION  ============================================================
//         Name:  StopBaseline::PassJetMETCuts
//  Description:  
// ===========================================================================
bool StopBaseline::PassJetMETCuts(const double dPhi0cut, const double dPhi1cut, const double dPhi2cut) const
{
  
  std::vector<double> Vphis = GetdPhiJetMET(AnaConsts::dphiArr);
  if (!passBaseline && Vphis.size() < 3) return false;
  //assert(Vphis.size() >= 3);
  if (passBaseline)
  {
    dPhiJ1MET->Fill(Vphis.at(0));
    dPhiJ2MET->Fill(Vphis.at(1));
    dPhiJ3MET->Fill(Vphis.at(2));
    
  }

  if (Vphis.at(0) < AnaConsts::dPhi0_CUT)  return false;
  if (Vphis.at(1) < AnaConsts::dPhi1_CUT)  return false;
  if (Vphis.at(2) < AnaConsts::dPhi2_CUT)  return false;

  return true;
}       // -----  end of function StopBaseline::PassJetMETCuts  -----
// ===  FUNCTION  ============================================================
//         Name:  StopBaseline::GetdPhiJetMET
//  Description:  return deltaPhi of Jet and MET, ordered by jet pt
// ===========================================================================
std::vector<double> StopBaseline::GetdPhiJetMET(const double jetCutsArr[]) const
{
  const double minAbsEta = jetCutsArr[0], maxAbsEta = jetCutsArr[1], minPt = jetCutsArr[2], maxPt = jetCutsArr[3];

  //Get Pt order jet list, passing the requirement
  typedef boost::bimap<std::vector<pat::Jet>::const_iterator, double > pt_bimap;
  pt_bimap jetpt;

  for(std::vector<pat::Jet>::const_iterator it=prodJetHdl->begin();
      it!=prodJetHdl->end(); ++it)
  {
    if(   ( minAbsEta == -1 || fabs(it->eta()) >= minAbsEta )
        && ( maxAbsEta == -1 || fabs(it->eta()) < maxAbsEta )
        && (     minPt == -1 || it->pt() >= minPt )
        && (     maxPt == -1 || it->pt() < maxPt ) ){
      jetpt.insert(pt_bimap::value_type(it, it->pt()));
    }
  }

  //for(pt_bimap::left_map::const_iterator it=jetpt.left.begin();
      //it!=jetpt.left.end(); ++it)
  //{
    //std::cout << "pt" << it->second <<" order? " << it->first->pt() << std::endl;
  //}

  std::vector<double> Vphis;
  pat::MET met = prodMETHdl->at(0);
  double lastjetpt = 9999;
  for(pt_bimap::left_map::const_iterator it=jetpt.left.begin();
      it!=jetpt.left.end(); ++it)
  {
    //std::cout << "pt" << it->second <<" order? " << it->first->pt() <<" lastjet " << lastjetpt << std::endl;
    assert(it->first->pt() <= lastjetpt);
    lastjetpt = it->first->pt();
    double perDPhi = fabs(TVector2::Phi_mpi_pi( it->first->phi() - met.phi()  ));
    Vphis.push_back(perDPhi);
  }

  return Vphis;
}       // -----  end of function StopBaseline::GetdPhiJetMET  -----

// ===  FUNCTION  ============================================================
//         Name:  StopBaseline::countBJets
//  Description:  
// ===========================================================================
int StopBaseline::countBJets(const std::string bTagKeyString, const double cutCSVS, const double Arr[]) const
{
  
  const double minAbsEta = Arr[0];
  const double maxAbsEta = Arr[1]; 
  const double minPt = Arr[2]; 
  const double maxPt = Arr[3]; 

  assert(prodJetHdl.isValid());

  int cnt = 0;

  for(unsigned int i=0; i < prodJetHdl->size(); ++i)
  {
    pat::Jet jet = prodJetHdl->at(i);

    if(   ( minAbsEta == -1 || fabs(jet.eta()) >= minAbsEta )
        && ( maxAbsEta == -1 || fabs(jet.eta()) < maxAbsEta )
        && (     minPt == -1 || jet.pt() >= minPt )
        && (     maxPt == -1 || jet.pt() < maxPt ) ){
            if( jet.bDiscriminator(bTagKeyString) > cutCSVS ) 
      cnt ++ ;
    }
  }

  return cnt;

}       // -----  end of function StopBaseline::countBJets  -----
