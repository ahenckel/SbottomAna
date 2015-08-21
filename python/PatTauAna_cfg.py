import FWCore.ParameterSet.Config as cms

process = cms.Process("OWNPARTICLES")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(200) )
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')
# get and parse the command line arguments
options.parseArguments()

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        #'file:/data/nbay04/c/benwu/PHY14/TTJets_PU20bx25_PHYS14.root',
        #'file:/data/nbay04/c/benwu/PHY14/TTJets_PU20bx25_PHYS14_v2.root'
        options.inputFiles
    )
)


process.All = cms.EDAnalyzer('PatTauAna',
                                PruneGenParticleInputTag = cms.InputTag("prunedGenParticles"),
                                PackedGenParticleInputTag = cms.InputTag("packedGenParticles"),
                                prodTauTag = cms.InputTag("slimmedTaus"),
                                DecayModeTag = cms.untracked.string (""),
                                prodMETTag = cms.InputTag("slimmedMETs"),
                                #DecayModeTau = cms.untracked.string ("decayModeFindingOldDMs"),
                                IsolationTag = cms.untracked.string(""),
                                MuonRejectTag=cms.untracked.string(""),
                                EleRejectTag = cms.untracked.string(""),
                                #SkipEvent = cms.untracked.vstring('ProductNotFound')"
)

process.OldDecay = cms.EDAnalyzer('PatTauAna',
                                PruneGenParticleInputTag = cms.InputTag("prunedGenParticles"),
                                PackedGenParticleInputTag = cms.InputTag("packedGenParticles"),
                                prodTauTag = cms.InputTag("slimmedTaus"),
                                prodMETTag = cms.InputTag("slimmedMETs"),
                                DecayModeTag = cms.untracked.string ("decayModeFinding"),
                                #DecayModeTau = cms.untracked.string ("decayModeFindingOldDMs"),
                                IsolationTag = cms.untracked.string(""),
                                MuonRejectTag=cms.untracked.string(""),
                                EleRejectTag = cms.untracked.string(""),
                                #SkipEvent = cms.untracked.vstring('ProductNotFound')"
)


process.NewDecay = cms.EDAnalyzer('PatTauAna',
                                PruneGenParticleInputTag = cms.InputTag("prunedGenParticles"),
                                PackedGenParticleInputTag = cms.InputTag("packedGenParticles"),
                                prodTauTag = cms.InputTag("slimmedTaus"),
                                prodMETTag = cms.InputTag("slimmedMETs"),
                                DecayModeTag = cms.untracked.string ("decayModeFindingNewDMs"),
                                #DecayModeTau = cms.untracked.string ("decayModeFindingOldDMs"),
                                IsolationTag = cms.untracked.string(""),
                                MuonRejectTag=cms.untracked.string(""),
                                EleRejectTag = cms.untracked.string(""),
                                #SkipEvent = cms.untracked.vstring('ProductNotFound')"
)

process.Isolation = cms.EDAnalyzer('PatTauAna',
                                PruneGenParticleInputTag = cms.InputTag("prunedGenParticles"),
                                PackedGenParticleInputTag = cms.InputTag("packedGenParticles"),
                                prodMETTag = cms.InputTag("slimmedMETs"),
                                prodTauTag = cms.InputTag("slimmedTaus"),
                                DecayModeTag = cms.untracked.string ("decayModeFinding"),
                                #DecayModeTau = cms.untracked.string ("decayModeFindingOldDMs"),
                                IsolationTag = cms.untracked.string("byLooseCombinedIsolationDeltaBetaCorr3Hits"),
                                MuonRejectTag=cms.untracked.string(""),
                                EleRejectTag = cms.untracked.string(""),
                                #SkipEvent = cms.untracked.vstring('ProductNotFound')"
)


process.againstMuon = cms.EDAnalyzer('PatTauAna',
                                PruneGenParticleInputTag = cms.InputTag("prunedGenParticles"),
                                PackedGenParticleInputTag = cms.InputTag("packedGenParticles"),
                                prodTauTag = cms.InputTag("slimmedTaus"),
                                DecayModeTag = cms.untracked.string ("decayModeFinding"),
                                #DecayModeTau = cms.untracked.string ("decayModeFindingOldDMs"),
                                prodMETTag = cms.InputTag("slimmedMETs"),
                                IsolationTag = cms.untracked.string("byLooseCombinedIsolationDeltaBetaCorr3Hits"),
                                MuonRejectTag=cms.untracked.string("againstMuonTight3"),
                                EleRejectTag = cms.untracked.string(""),
                                #SkipEvent = cms.untracked.vstring('ProductNotFound')"
)

process.againstEle = cms.EDAnalyzer('PatTauAna',
                                PruneGenParticleInputTag = cms.InputTag("prunedGenParticles"),
                                PackedGenParticleInputTag = cms.InputTag("packedGenParticles"),
                                prodTauTag = cms.InputTag("slimmedTaus"),
                                DecayModeTag = cms.untracked.string ("decayModeFinding"),
                                #DecayModeTau = cms.untracked.string ("decayModeFindingOldDMs"),
                                IsolationTag = cms.untracked.string("byLooseCombinedIsolationDeltaBetaCorr3Hits"),
                                MuonRejectTag=cms.untracked.string("againstMuonTight3"),
                                prodMETTag = cms.InputTag("slimmedMETs"),
                                EleRejectTag = cms.untracked.string("againstElectronMediumMVA5"),
                                #SkipEvent = cms.untracked.vstring('ProductNotFound')"
)
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(options.outputFile))
                                   #fileName = cms.string('outhist.root'))



#process.p = cms.Path(process.OldDecay * process.Isolation * process.againstMuon * process.againstEle)
process.p = cms.Path(process.All * process.OldDecay * process.NewDecay * process.Isolation * process.againstMuon * process.againstEle)
#process.p = cms.Path(process.tauTag * process.test)

