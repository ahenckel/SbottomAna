import FWCore.ParameterSet.Config as cms

process = cms.Process("OWNPARTICLES")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

#import FWCore.ParameterSet.VarParsing as VarParsing
#options = VarParsing.VarParsing ('analysis')
## get and parse the command line arguments
#options.parseArguments()

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        '/store/mc/Phys14DR/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/563CD412-C16B-E411-ACE1-C4346BC8E730.root',
        '/store/mc/Phys14DR/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/10000/D83EC823-A66B-E411-80CA-00266CFFC4C4.root',
        '/store/mc/Phys14DR/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/10000/F414852E-A66B-E411-B879-1CC1DE046F78.root'
        #'file:/data/nbay04/c/benwu/PHY14/TTJets_PU20bx25_PHYS14.root'
        #options.inputFiles
    )
)

process.goodVertices = cms.EDFilter(
  "VertexSelector",
  filter = cms.bool(False),
  src = cms.InputTag("offlineSlimmedPrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.rho < 2")
)

process.load("UserCode/SuperTopTagger/PatTauID_cfi")
process.load("SusyAnaTools/SkimsAUX/prodElectrons_cfi")
process.load("SusyAnaTools/SkimsAUX/prodMuons_cfi")
from SusyAnaTools.SkimsAUX.trackIsolationMaker_cfi import trackIsolationFilter

process.IsoTrack = trackIsolationFilter.clone(
    doTrkIsoVeto        = cms.bool(False),
    pfCandidatesTag     = cms.InputTag("packedPFCandidates"))

process.StopBaseline = cms.EDFilter( "StopBaseline",
    prodElectronTag = cms.InputTag("prodElectrons"),
    prodEleRelIsoTag = cms.InputTag("prodElectrons", "elesRelIso" ),
    prodEleMtwTag = cms.InputTag("prodElectrons", "elesMtw" ),
    prodMuonTag = cms.InputTag("prodMuons"),
    prodMuonRelIsoTag = cms.InputTag("prodMuons", "muonsRelIso"),
    prodMuonMtwTag = cms.InputTag("prodMuons", "muonsMtw"),
    IsoTrackTag = cms.InputTag("IsoTrack"),
    IsoTrackIsoTag = cms.InputTag("IsoTrack", "pfcandstrkiso"),
    IsoTrackMtwTag = cms.InputTag("IsoTrack", "pfcandstrkiso"),
    prodTauTag = cms.InputTag("prodTaus"),
    prodJetTag = cms.InputTag("slimmedJets"),
    prodMETTag = cms.InputTag("slimmedMETs"),
)

#process.myProducerLabel = cms.EDProducer('PatTauID'
#)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('myOutputFile.root'),
                               #outputCommands = cms.untracked.vstring('drop *',
                                                                      #'keep *_demo_*_*'
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
                                MuonRejectTag=cms.untracked.string("againstMuonLoose3"),
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
                                MuonRejectTag=cms.untracked.string("againstMuonLoose3"),
                                prodMETTag = cms.InputTag("slimmedMETs"),
                                EleRejectTag = cms.untracked.string("againstElectronVLooseMVA5"),
                                #SkipEvent = cms.untracked.vstring('ProductNotFound')"
)
process.TFileService = cms.Service("TFileService",
                                   #fileName = cms.string(options.outputFile))
                                   fileName = cms.string('outhist.root'))

process.prod = cms.Sequence(process.goodVertices*  process.prodMuons * process.prodElectrons * process.prodTaus * process.IsoTrack  * process.StopBaseline)
process.tauAna = cms.Sequence( process.OldDecay * process.Isolation * process.againstMuon * process.againstEle)
process.p = cms.Path(process.prod * process.tauAna)

#process.p = cms.Path(process.myProducerLabel)

#process.e = cms.EndPath(process.out)
