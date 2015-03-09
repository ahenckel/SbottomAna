import FWCore.ParameterSet.Config as cms

demo = cms.EDProducer('PatTauID',
                      PatTauTag = cms.InputTag("slimmedTaus"),
                      DecayModeTau = cms.untracked.string ("decayModeFindingOldDMs"),
                      IsolationTag = cms.untracked.string("byLooseCombinedIsolationDeltaBetaCorr3Hits"),
                      MuonRejectTag=cms.untracked.string("againstMuonTight3"),
                      EleRejectTag = cms.untracked.string("againstElectronMediumMVA5"),
                      )
