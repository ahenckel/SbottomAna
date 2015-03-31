// ===========================================================================
// 
//       Filename:  PatTauAna.h
// 
//    Description:  A analizer for the PatTau, produced by the PatTauID
// 
//        Version:  1.0
//        Created:  03/09/2015 03:05:05 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  University of Illinois at Chicago, CMS@LPC
// 
// ===========================================================================



/**\class PatTauAna

Description: <+one line class summary+>

Implementation:
<+Notes on implementation+>
*/

#ifndef  __EDAnalyzer_PATTAUANA_INC__
#define  __EDAnalyzer_PATTAUANA_INC__

// system include files
#include <memory>
#include <string>
#include <vector>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "UserCode/SuperTopTagger/interface/GenParAna.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "UserCode/SuperTopTagger/interface/HistTool.h"
#include "DataFormats/PatCandidates/interface/MET.h"
//
// class declaration
//

class PatTauAna : public edm::EDAnalyzer {
  public:
    explicit PatTauAna(const edm::ParameterSet&);
    ~PatTauAna();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


  private:
    virtual void beginJob() override;
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
    virtual void endJob() override;

    virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
    virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
    virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
    virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

    bool BookHistograms();
    std::vector<pat::Tau> PassTauID();
    bool PatTauTagging(std::vector<TauDecay> &Tauidx);
    bool GetTaus();
    bool SUSYSignalEff();
    bool TauIDVar() const;
    // ----------member data ---------------------------
    //----------------------------------------------------------------------------
    //  Event Handler
    //----------------------------------------------------------------------------
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ GenParticle ~~~~~
    // Pruned particles are the one containing "important" stuff
    edm::InputTag PruneGenParticleInputTag_;
    edm::Handle<reco::GenParticleCollection> PrunedGenHdl;

    // Packed particles are all the status 1, so usable to remake jets
    // The navigation from status 1 to pruned is possible (the other direction should be made by hand)
    edm::InputTag PackedGenParticleInputTag_;
    edm::Handle<std::vector<pat::PackedGenParticle> >  PackedGenHdl;

    edm::InputTag prodTauTag_;
    edm::Handle<std::vector<pat::Tau> >  prodTauHdl;

    edm::InputTag prodMETTag_;
    edm::Handle<std::vector<pat::MET> >  prodMETHdl;

    std::vector<pat::Tau> vTaus;
    std::string DecayMode;
    std::string Isolation;
    std::string MuonReject;
    std::string EleReject;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ GenParticle ~~~~~
  
    GenParAna *AnaPtr;
    HistTool *hist;


};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//define this as a plug-in
DEFINE_FWK_MODULE(PatTauAna);
#endif   // ----- #ifndef __EDAnalyzer_PATTAUANA_INC__  -----

