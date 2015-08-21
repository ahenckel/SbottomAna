// ===========================================================================
// 
//       Filename:  SuperTopTagger.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/30/2015 02:36:15 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  University of Illinois at Chicago, CMS@LPC
// 
// ===========================================================================


#ifndef  __SUPERTOPTAGGER_INC__
#define  __SUPERTOPTAGGER_INC__

// system include files
#include <memory>
#include <utility>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// DataFormat
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

// UserCode 
#include "UserCode/SuperTopTagger/interface/GenParAna.h"
#include "UserCode/SuperTopTagger/interface/HistTool.h"
#include "UserCode/SuperTopTagger/interface/MatchingTaggerGen.h"
#include "SusyAnaTools/TopTagger/interface/Type3TopTagger.h"


//
// class declaration
//

class SuperTopTagger : public edm::EDAnalyzer {
   public:
      explicit SuperTopTagger(const edm::ParameterSet&);
      ~SuperTopTagger();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      HistTool *hist;

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      int GetHandleByLabel(const edm::Event& iEvent);
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      bool JetSelection(const pat::Jet& jets) const;
      bool PickJetFromCombining();
      bool loadMETMHT();
      bool RunType3TopTagger();
      bool BookHistogram();
      bool SetType3TopTagger() const;
      bool AnaType3WTag(topTagger::type3TopTagger* type3TopTaggerPtr, std::vector<WDecay> &Widx);
      std::vector<TLorentzVector> GetType3WDijet(std::vector<TLorentzVector> pp, std::vector<int > T3TopComb);

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
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ GenParticle ~~~~~


      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Begin Handler ~~~~~
      edm::InputTag METTag_;
      edm::Handle<std::vector<pat::MET> >  METHdl;
      
      edm::InputTag JetTag_;
      edm::Handle<std::vector<pat::Jet> >  JetHdl;
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of Handler ~~~~~
      

      //----------------------------------------------------------------------------
      //  Analysis Class
      //----------------------------------------------------------------------------
      GenParAna *AnaPtr;
      topTagger::type3TopTagger *type3TopTaggerPtr;
      MatchingTaggerGen *MatchingPtr;

      TLorentzVector metLVec; 
      std::vector<TLorentzVector> oriJetsVec; 
      std::vector<double> recoJetsBtagCSVS;
};


#endif   // ----- #ifndef __SUPERTOPTAGGER_INC__  -----

