// ===========================================================================
// 
//       Filename:  StopBaselineFilter.h
// 
//    Description:  A emfilter for the stop baseline, defined as in 
//    https://github.com/susy2015/SusyAnaTools/blob/baseline_def_Jan31_2015/Tools/baselineDef.C
// 
//        Version:  1.0
//        Created:  03/14/2015 07:55:02 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  University of Illinois at Chicago, CMS@LPC
// 
// ===========================================================================

/**\class StopBaseline

Description: <+one line class summary+>

Implementation:
<+Notes on implementation+>
*/

#ifndef  __EDFilter_STOPBASELINE_INC__
#define  __EDFilter_STOPBASELINE_INC__

// system include files
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "boost/bimap.hpp"
#include "boost/dynamic_bitset.hpp"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "SusyAnaTools/Tools/customize.h"
//
// class declaration
//

class StopBaseline : public edm::EDFilter {
  public:
    explicit StopBaseline(const edm::ParameterSet&);
    ~StopBaseline();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob() ;
    virtual bool filter(edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    virtual bool beginRun(edm::Run&, edm::EventSetup const&);
    virtual bool endRun(edm::Run&, edm::EventSetup const&);
    virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
    virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

    bool PassBaseline();
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Jet Cut ~~~~~
    int PassJetCuts(const double Arr[]) const;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MET Cut ~~~~~
    std::vector<double> GetdPhiJetMET(const double Arr[]) const;
    bool PassJetMETCuts(const double dPhi0cut, const double dPhi1cut, const double dPhi2cut) const;
    bool PassMETCuts(const double defaultMETcut) const;
    double GetMET() const;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Lepton Cut ~~~~~
    template <class T> int CountLeps(const std::vector<T> *vLeps, const std::vector<double> *vIsos, 
        const std::vector<double> *vMtws, const double Arr[]) const;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ b-tag ~~~~~
    int countBJets(const std::string bTagKeyString, const double cutCSVS, const double Arr[]) const;



    // ----------member data ---------------------------
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Begin Handler ~~~~~
    edm::InputTag prodElectronTag_;
    edm::Handle<std::vector<pat::Electron> >  prodElectronHdl;

    edm::InputTag prodEleRelIsoTag_;
    edm::Handle<std::vector<double> >  prodEleRelIsoHdl;

    edm::InputTag prodEleMtwTag_;
    edm::Handle<std::vector<double> >  prodEleMtwHdl;
    
    edm::InputTag prodMuonTag_;
    edm::Handle<std::vector<pat::Muon> >  prodMuonHdl;
    
    edm::InputTag prodMuonRelIsoTag_;
    edm::Handle<std::vector<double> >  prodMuonRelIsoHdl;

    edm::InputTag prodMuonMtwTag_;
    edm::Handle<std::vector<double> >  prodMuonMtwHdl;

    edm::InputTag IsoTrackTag_;
    edm::Handle<std::vector<pat::PackedCandidate> >  IsoTrackHdl;

    edm::InputTag IsoTrackIsoTag_;
    edm::Handle<std::vector<double> >  IsoTrackIsoHdl;

    edm::InputTag IsoTrackMtwTag_;
    edm::Handle<std::vector<double> >  IsoTrackMtwHdl;
    
    edm::InputTag prodTauTag_;
    edm::Handle<std::vector<pat::Tau> >  prodTauHdl;
    
    edm::InputTag prodJetTag_;
    edm::Handle<std::vector<pat::Jet> >  prodJetHdl;
    
    edm::InputTag prodMETTag_;
    edm::Handle<std::vector<pat::MET> >  prodMETHdl;
    
    edm::Service<TFileService> fs;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of Handler ~~~~~
    TH1D* cutflow;
    TH1D* cutbins;
    std::vector<std::string> cutnames;
};
//
// constants, enums and typedefs
//

//
// static data member definitions
//

//define this as a plug-in
DEFINE_FWK_MODULE(StopBaseline);
#endif   // ----- #ifndef __EDFilter_STOPBASELINE_INC__  -----

