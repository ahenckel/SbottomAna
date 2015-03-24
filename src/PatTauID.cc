// ===========================================================================
// 
//       Filename:  PatTauID.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  03/09/2015 01:48:40 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  University of Illinois at Chicago, CMS@LPC
// 
// ===========================================================================

#include  "UserCode/SuperTopTagger/interface/PatTauID.h"

//
// constructors and destructor
//
PatTauID::PatTauID(const edm::ParameterSet& iConfig)
{
  //register your products
  /* Examples
     produces<ExampleData2>();

  //if do put with a label
  produces<ExampleData2>("label");

  //if you want to put into the Run
  produces<ExampleData2,InRun>();
  */
  produces<std::vector<pat::Tau> >("PassTau");
  //now do what ever other initialization is needed
  PatTauTag_ = iConfig.getParameter<edm::InputTag>("PatTauTag");
  PatTauTag_ = iConfig.getParameter<edm::InputTag>("PatTauTag");

  DecayMode = iConfig.getUntrackedParameter<std::string>("DecayModeTag",  "decayModeFindingOldDMs");
  Isolation = iConfig.getUntrackedParameter<std::string>("IsolationTag", "byLooseCombinedIsolationDeltaBetaCorr3Hits");
  MuonReject = iConfig.getUntrackedParameter<std::string>("MuonRejectTag", "againstMuonTight3");
  EleReject = iConfig.getUntrackedParameter<std::string>("EleRejectTag", "againstElectronMediumMVA5");


}


PatTauID::~PatTauID()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
  void
PatTauID::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  iEvent.getByLabel(PatTauTag_, PatTauHdl); 
  assert(PatTauHdl.isValid());
  /* This is an event example
  //Read 'ExampleData' from the Event
  Handle<ExampleData> pIn;
  iEvent.getByLabel("example",pIn);

  //Use the ExampleData to create an ExampleData2 which 
  // is put into the Event
  std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
  iEvent.put(pOut);
  */

  /* this is an EventSetup example
  //Read SetupData from the SetupRecord in the EventSetup
  ESHandle<SetupData> pSetup;
  iSetup.get<SetupRecord>().get(pSetup);
  */
  std::auto_ptr<std::vector<pat::Tau> > pOut(new  std::vector<pat::Tau> );
  std::vector<pat::Tau>  temp =  PassTauID();

  std::cout << " Output Pat Tau size " << temp.size() << std::endl;
  *pOut.get() = temp;
  iEvent.put(pOut, "PassTau");
}

// ------------ method called once each job just before starting event loop  ------------
  void 
PatTauID::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PatTauID::endJob() {
}

// ------------ method called when starting to processes a run  ------------
  void 
PatTauID::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
  void 
PatTauID::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
  void 
PatTauID::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
  void 
PatTauID::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
  void
PatTauID::fillDescriptions(edm::ConfigurationDescriptions& descriptions) 
{
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


// ===  FUNCTION  ============================================================
//         Name:  PatTauID::PassTauID
//  Description:  CMS Standard Tau ID from Tau POG
//  https://twiki.cern.ch/twiki/bin/viewauth/CMS/TauIDRecommendation13TeV/
//  Recommended by Christian Veelken 
//      decayModeFindingOldDMs
//      byLooseCombinedIsolationDeltaBetaCorr3Hits
//      againstMuonTight3
//      againstElectronMediumMVA5
// ===========================================================================
std::vector<pat::Tau> PatTauID::PassTauID()
{
  std::vector<pat::Tau> temp;

  for(unsigned int i=0; i < PatTauHdl->size(); ++i)
  {
    pat::Tau tau = PatTauHdl->at(i);
    std::cout << "Tau ID DecayMode: " << tau.tauID(DecayMode) 
      << " Isolation: " << tau.tauID(Isolation)
      << " MuonReject: " << tau.tauID(MuonReject)
      << " EleReject: " << tau.tauID(EleReject)
      << std::endl;
    // Decay mode: 
    // decayModeFindingOldDMs
    // decayModeFindingNewDMs (nly for analysis involving high pT taus, like !SUSY searches)
    if (tau.isTauIDAvailable(DecayMode))
    {
      if (tau.tauID(DecayMode)  < 0.5)
        break;
    }

    // Isolation:
    // byLooseCombinedIsolationDeltaBetaCorr3Hits
    // byMediumCombinedIsolationDeltaBetaCorr3Hits
    // byTightCombinedIsolationDeltaBetaCorr3Hits
    if (tau.isTauIDAvailable(Isolation))
    {
      if (tau.tauID(Isolation)  < 0.5)
        break;
    }

    // Muon Rejection:
    // againstMuonLoose3
    // againstMuonTight3
    if (tau.isTauIDAvailable(MuonReject))
    {
      if (tau.tauID(MuonReject)  < 0.5)
        break;
    }

    // Electron Rejection:
    // againstElectronVLooseMVA5 
    // againstElectronLooseMVA5
    // againstElectronMediumMVA5

    if (tau.isTauIDAvailable(EleReject))
    {
      if (tau.tauID(EleReject)  < 0.5)
        break;
    }

    std::cout << "Tau ID DecayMode: " << tau.tauID(DecayMode) 
      << " Isolation: " << tau.tauID(Isolation)
      << " MuonReject: " << tau.tauID(MuonReject)
      << " EleReject: " << tau.tauID(EleReject)
      << std::endl;
    temp.push_back(tau);
  }

  return temp;
}       // -----  end of function PatTauID::PassTauID  -----
