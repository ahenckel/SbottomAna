// ===========================================================================
// 
//       Filename:  GenParAna.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/30/2015 12:19:54 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  University of Illinois at Chicago, CMS@LPC
// 
// ===========================================================================

#ifndef  __GENPARANA_INC__
#define  __GENPARANA_INC__

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "UserCode/SuperTopTagger/interface/HistTool.h"

// Boost
#include <boost/foreach.hpp>

#include "TLorentzVector.h"
// fastjet
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/GhostedAreaSpec.hh"

struct GenPrunePacked
{
  const reco::Candidate* Pruned = NULL;
  std::vector<const reco::Candidate*> Packed;
};

struct WDecay
{
  GenPrunePacked Widx_;
  GenPrunePacked Lepidx_;
  GenPrunePacked neuidx_;
  GenPrunePacked had1idx_;
  GenPrunePacked had2idx_;
  bool isLeptonic_ = false;
};

struct TopDecay 
{
  GenPrunePacked topidx_;
  GenPrunePacked bidx_;
  WDecay widx_;
};

struct TauDecay
{
  GenPrunePacked tauidx_;
  GenPrunePacked Lepidx_;
  std::vector<const reco::Candidate*> PrunedNeus;
  bool isLeptonic_ = false;
};

// ===========================================================================
//        Class:  GenParAna
//  Description:  
// ===========================================================================
class GenParAna
{
  public:

    // ====================  LIFECYCLE     ===============================
    GenParAna (HistTool* hist_);                             // constructor
    GenParAna ( const GenParAna &other );   // copy constructor
    ~GenParAna ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool GetGenParticles(edm::Handle<reco::GenParticleCollection> PrunedGenHdl, 
        edm::Handle<std::vector<pat::PackedGenParticle> >  PackedGenHdl);

    bool PrintGenPars(edm::Handle<reco::GenParticleCollection> PrunedGenHdl) const;
    bool GetTopandW(edm::Handle<reco::GenParticleCollection> PrunedGenHdl);
    bool Test() const;

    WDecay GetWDecay(const reco::Candidate*  WPar);
    TopDecay GetTopDecay(const reco::Candidate* TopPar);
    // ====================  MUTATORS      ===============================
    bool PrintTopDecay() const;
    bool PrintWDecay(WDecay wboson) const;
    bool AnaWdiJets() const;
    bool AnaTopandW() const;
    bool BookHistogram();
    bool GetGenCount();
    int GetTopCount() const;
    int GetLepCount() const;
    bool GetTop(edm::Handle<reco::GenParticleCollection> PrunedGenHdl);

    bool GetW(edm::Handle<reco::GenParticleCollection> PrunedGenHdl);
    bool GetTau(edm::Handle<reco::GenParticleCollection> PrunedGenHdl);
    bool GetTauDecay(const reco::Candidate* TauPar);
    std::vector<fastjet::PseudoJet> ProdGenJets(const std::vector<const reco::Candidate*> &PackedGens, const double jetPtMin = 0, const double rParam_ = 0.4);
    std::vector<TLorentzVector> ProdGenLVs(const std::vector<const reco::Candidate*> &PackedGens, const double jetPtMin = 0, const double rParam = 0.4);
    // ====================  OPERATORS     ===============================

    GenParAna& operator = ( const GenParAna &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================
    std::map<std::string, int> GenCounts;
    std::vector<TopDecay> Topidx;
    std::vector<WDecay> Widx;
    std::vector<TauDecay> Tauidx;


  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================
    bool CleanPerEvent();
    GenPrunePacked GetPackedPars(const reco::Candidate* GenPar);
    bool isAncestor(const reco::Candidate* ancestor, const reco::Candidate * particle) const;

    // ====================  DATA MEMBERS  ===============================
    edm::Handle<std::vector<pat::PackedGenParticle> >  LocalPackedGenHdl;
    /// Get the vector of GenParticles per event
    HistTool* hist;

}; // -----  end of class GenParAna  -----
#endif   // ----- #ifndef __GENPARANA_INC__  -----
