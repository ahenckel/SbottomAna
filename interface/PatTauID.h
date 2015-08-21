// ===========================================================================
// 
//       Filename:  PatTauID.h
// 
//    Description:  A producer to select Tau ID that is best for the stop
//    analysis
// 
//        Version:  1.0
//        Created:  03/09/2015 01:48:48 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  University of Illinois at Chicago, CMS@LPC
// 
// ===========================================================================


/**\class PatTauID

Description: <+one line class summary+>

Implementation:
<+Notes on implementation+>
*/

#ifndef  __EDProducer_PATTAUID_INC__
#define  __EDProducer_PATTAUID_INC__

// system include files
#include <memory>
#include <string>
#include <vector>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Tau.h"
//
// class declaration
//

class PatTauID : public edm::EDProducer {
  public:
    explicit PatTauID(const edm::ParameterSet&);
    ~PatTauID();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void beginJob() ;
    virtual void produce(edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    virtual void beginRun(edm::Run&, edm::EventSetup const&);
    virtual void endRun(edm::Run&, edm::EventSetup const&);
    virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
    virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

    std::vector<pat::Tau> PassTauID();
    // ----------member data ---------------------------
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Begin Handler ~~~~~
    edm::InputTag PatTauTag_;
    edm::Handle<std::vector<pat::Tau> >  PatTauHdl;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of Handler ~~~~~
  
    std::string DecayMode;
    std::string Isolation;
    std::string MuonReject;
    std::string EleReject;
};
//
// constants, enums and typedefs
//


//
// static data member definitions
//

//define this as a plug-in
DEFINE_FWK_MODULE(PatTauID);
#endif   // ----- #ifndef __EDProducer_PATTAUID_INC__  -----




