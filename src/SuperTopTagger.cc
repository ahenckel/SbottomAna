// -*- C++ -*-
//
// Package:    UserCode/SuperTopTagger
// Class:      SuperTopTagger
// 
/**\class SuperTopTagger SuperTopTagger.cc UserCode/SuperTopTagger/plugins/SuperTopTagger.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  
//         Created:  Fri, 30 Jan 2015 17:47:07 GMT
//
//


#include "UserCode/SuperTopTagger/interface/SuperTopTagger.h"
#include "UserCode/SuperTopTagger/interface/GenParAna.h"

#include "boost/bimap.hpp"
//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
SuperTopTagger::SuperTopTagger(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
  PruneGenParticleInputTag_= iConfig.getParameter<edm::InputTag>("PruneGenParticleInputTag");
  PackedGenParticleInputTag_= iConfig.getParameter<edm::InputTag>("PackedGenParticleInputTag");
  METTag_ = iConfig.getParameter<edm::InputTag>("METTag");
  JetTag_ = iConfig.getParameter<edm::InputTag>("JetTag");
  hist = new HistTool("Top");
  AnaPtr = new GenParAna(hist);
  type3TopTaggerPtr = new topTagger::type3TopTagger();
  MatchingPtr = new MatchingTaggerGen(hist, &metLVec, &oriJetsVec, &recoJetsBtagCSVS );
  //SetType3TopTagger();
}


SuperTopTagger::~SuperTopTagger()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
SuperTopTagger::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  GetHandleByLabel(iEvent);


#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif


   //----------------------------------------------------------------------------
   //  Analysis Classes
   //----------------------------------------------------------------------------
  AnaPtr->GetGenParticles(PrunedGenHdl, PackedGenHdl);
  //AnaPtr->PrintGenPars(PrunedGenHdl);
  AnaPtr->GetTopandW(PrunedGenHdl);
  //AnaPtr->PrintTopDecay();

  if (AnaPtr->GetLepCount() == 0) return;

  AnaPtr->AnaWdiJets();
  //----------------------------------------------------------------------------
  //  Type3 Top Tagger
  //----------------------------------------------------------------------------
  RunType3TopTagger();
  MatchingPtr->AnaType3Gen(type3TopTaggerPtr, AnaPtr);

}


// ------------ method called once each job just before starting event loop  ------------
void SuperTopTagger::beginJob()
{
  AnaPtr->BookHistogram();
  MatchingPtr->BookHistogram();
  BookHistogram();
}


// ------------ method called once each job just after ending the event loop  ------------
void 
SuperTopTagger::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
SuperTopTagger::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
SuperTopTagger::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
SuperTopTagger::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
SuperTopTagger::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SuperTopTagger::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

// ===  FUNCTION  ============================================================
//         Name:  SuperTopTagger::GetHandleByLabel
//  Description:  
// ===========================================================================
int SuperTopTagger::GetHandleByLabel(const edm::Event& iEvent)
{

  iEvent.getByLabel(PruneGenParticleInputTag_, PrunedGenHdl); 
  iEvent.getByLabel(PackedGenParticleInputTag_, PackedGenHdl); 
  iEvent.getByLabel(METTag_, METHdl); 
  iEvent.getByLabel(JetTag_, JetHdl); 

  return true;
}       // -----  end of function SuperTopTagger::GetHandleByLabel  -----


// ===  FUNCTION  ============================================================
//         Name:  SuperTopTagger::SetType3TopTagger
//  Description:  
// ===========================================================================
bool SuperTopTagger::SetType3TopTagger() const
{
  type3TopTaggerPtr->setdoExtraCuts(true);
  type3TopTaggerPtr->setmTbcut(500);
  type3TopTaggerPtr->setmTtcut(365);
  type3TopTaggerPtr->setMT2cut(300);
  type3TopTaggerPtr->setmTWcut(600);
  type3TopTaggerPtr->setdoMTMT2cuts(true);
  type3TopTaggerPtr->setPDGmWmTop(80.385, 173.5);
  type3TopTaggerPtr->setlowRatioWoverTop(0.85);
  type3TopTaggerPtr->sethighRatioWoverTop(1.25);
  type3TopTaggerPtr->setlowArcTanm13overm12(0.2);
  type3TopTaggerPtr->sethighArcTanm13overm12(1.3);
  type3TopTaggerPtr->setlowm23overm123(0.35);
  type3TopTaggerPtr->setdefaultJetCone(2.0);
  type3TopTaggerPtr->setsimuCAdeltaR(1.5);
  type3TopTaggerPtr->setsimuCALargerdeltaR(-1);
  type3TopTaggerPtr->setlowTopCut(80);
  type3TopTaggerPtr->sethighTopCut(270);
  type3TopTaggerPtr->setlowWCut(50);
  type3TopTaggerPtr->sethighWCut(120);
  type3TopTaggerPtr->setCSVS(0.679);
  type3TopTaggerPtr->setnSubJetsDiv(3);
  type3TopTaggerPtr->setnJetsSel(5);
  type3TopTaggerPtr->setmaxEtaForbJets(2.4);
  //type3TopTaggerPtr->setorderingOptArr(orderingOptArr_);
  //type3TopTaggerPtr->setmaxIndexForOrderingArr(maxIndexForOrderingArr_);
  type3TopTaggerPtr->setdebug(true);
  type3TopTaggerPtr->settaggingMode(true);
  type3TopTaggerPtr->setdobVetoCS(false);


  return true;
}       // -----  end of function SuperTopTagger::SetType3TopTagger  -----

// ===  FUNCTION  ============================================================
//         Name:  SuperTopTagger::RunType3TopTagger
//  Description:   
// ===========================================================================
bool SuperTopTagger::RunType3TopTagger() 
{
  loadMETMHT();
  PickJetFromCombining();
  bool pass = type3TopTaggerPtr->processEvent(oriJetsVec, recoJetsBtagCSVS, metLVec);
  hist->FillTH1("NType3TopTagger", type3TopTaggerPtr->nTopCandSortedCnt);
  type3TopTaggerPtr->prepareFindingBestTopCandidate(oriJetsVec, recoJetsBtagCSVS);

  //std::cout << " T3TopTag " << pass << std::endl;
  return pass;
}       // -----  end of function SuperTopTagger::RunType3TopTagger  -----

// ===  FUNCTION  ============================================================
//         Name:  SuperTopTagger::loadMETMHT
//  Description:  
// ===========================================================================
bool SuperTopTagger::loadMETMHT()
{
  metLVec.SetPxPyPzE(0, 0, 0, 0);
  double met = (*METHdl)[0].pt(); 
  double metphi = (*METHdl)[0].phi();
  metLVec.SetPtEtaPhiM(met, 0, metphi, 0);
  return true;
}       // -----  end of function SuperTopTagger::loadMETMHT  -----


// ===  FUNCTION  ============================================================
//         Name:  SuperTopTagger::PickJetFromCombining
//  Description:  
// ===========================================================================
bool SuperTopTagger::PickJetFromCombining()
{
  
  oriJetsVec.clear(); 
  recoJetsBtagCSVS.clear();

  for(size_t ij=0; ij<JetHdl->size(); ij++){
    const pat::Jet & jet = (*JetHdl)[ij];
    if( !JetSelection(jet) ) continue;
    TLorentzVector jetLVec;
    jetLVec.SetPtEtaPhiE(jet.pt(), jet.eta(), jet.phi(), jet.energy());
    oriJetsVec.push_back(jetLVec);
    double btag = jet.bDiscriminator("combinedSecondaryVertexBJetTags");
    recoJetsBtagCSVS.push_back(btag);
  }

  return true;
}       // -----  end of function SuperTopTagger::PickJetFromCombining  -----


// ===  FUNCTION  ============================================================
//         Name:  SuperTopTagger::JetSelection
//  Description:  
// ===========================================================================
bool SuperTopTagger::JetSelection(const pat::Jet& jets) const
{
  return true;
}       // -----  end of function SuperTopTagger::JetSelection  -----


// ===  FUNCTION  ============================================================
//         Name:  SuperTopTagger::BookHistogram
//  Description:  
// ===========================================================================
bool SuperTopTagger::BookHistogram()
{
  hist->AddTH1("NType3TopTagger", "Number of Type 3 Top Tagger for Hadronic Tops", 5, 0, 5);
  return true;
}       // -----  end of function SuperTopTagger::BookHistogram  -----

//define this as a plug-in
DEFINE_FWK_MODULE(SuperTopTagger);

