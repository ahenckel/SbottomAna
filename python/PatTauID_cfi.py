import FWCore.ParameterSet.Config as cms

prodTaus = cms.EDProducer('PatTauID',
                      PatTauTag = cms.InputTag("slimmedTaus"),
                      DecayModeTag = cms.untracked.string ("decayModeFinding"),
                      #DecayModeTau = cms.untracked.string ("decayModeFindingOldDMs"),
                      IsolationTag = cms.untracked.string("byLooseCombinedIsolationDeltaBetaCorr3Hits"),
                      MuonRejectTag=cms.untracked.string("againstMuonTight3"),
                      EleRejectTag = cms.untracked.string("againstElectronMediumMVA5"),
                      )

##t::Tau: the ID decayModeFindingOldDMs can't be found in this pat::Tau.  #The
#available IDs are: 'againstElectronLoose' 'againstElectronLooseMVA5'
#'againstElectronMVA5category' 'againstElectronMVA5raw' 'againstElectronMedium'
#'againstElectronMediumMVA5' 'againstElectronTight' 'againstElectronTightMVA5'
#'against #ElectronVLooseMVA5' 'againstElectronVTightMVA5' 'againstMuonLoose'
#'againstMuonLoose2' 'againstMuonLoose3' 'againstMuonLooseMVA'
#'againstMuonMVAraw' 'againstMuonMedium' 'againstMuonMedium2'
#'againstMuonMediumMVA' 'againstMuonTight' 'again #stMuonTight2'
#'againstMuonTight3' 'againstMuonTightMVA'
#'byCombinedIsolationDeltaBetaCorrRaw3Hits' 'byIsolationMVA3newDMwLTraw'
#'byIsolationMVA3newDMwoLTraw' 'byIsolationMVA3oldDMwLTraw'
#'byIsolationMVA3oldDMwoLTraw' 'byLooseCombinedIsolationDeltaBetaCorr3Hits'
#'byLooseIsolationMVA3newDMwLT' 'byLooseIsolationMVA3newDMwoLT'
#'byLooseIsolationMVA3oldDMwLT' 'byLooseIsolationMVA3oldDMwoLT'
#'byMediumCombinedIsolationDeltaBetaCorr3Hits' 'byMediumIsolationMVA3newDMwLT'
#'byMediumIsolationMVA3newDMwoLT' 'byMediumIsolationMVA3oldDMwLT'
#'byMediumIsolationMVA3oldDMwoLT' 'byTightCombinedIsolationDeltaBetaCorr3Hits'
#'byTightIsolationMVA3newDMwLT' 'byTightIsolationMVA3newDMwoLT'
#'byTightIsolationMVA3oldDMwLT' 'byTightIsolationMVA3oldDMwoLT'
#'byVLooseIsolationMVA3newDMwLT' 'byVLooseIsolationMVA3newDMwoLT'
#'byVLooseIsolationMVA3oldDMwLT' 'byVLooseIsolationMVA3oldDMwoLT'
#'byVTightIsolationMVA3newDMwLT' 'byVTightIsolationMVA3newDMwoLT'
#'byVTightIsolationMVA3oldDMwLT' 'byVTightIsolationMVA3oldDMwoLT'
#'byVVTightIsolationMVA3newDMwLT' 'byVVTightIsolationMVA3newDMwoLT'
#'byVVTightIsolationMVA3oldDMwLT' 'byVVTightIsolationMVA3oldDMwoLT'
#'chargedIsoPtSum' 'decayModeFinding' 'decayModeFindingNewDMs' 'neutralIsoPtSum'
#'puCorrPtSum' .
#----- End Fatal Exception ---------------------------
