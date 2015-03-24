import FWCore.ParameterSet.Config as cms

process = cms.Process("OWNPARTICLES")

process.load("FWCore.MessageService.MessageLogger_cfi")

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(200) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        #'file:/data/nbay04/c/benwu/PHY14/DYLL_200to400_PU20x25_PHYS14.root'
        'file:/data/nbay04/c/benwu/PHY14/TTJets_PU20bx25_PHYS14.root'
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

process.IsoTrack = trackIsolationFilter.clone( pfCandidatesTag     = cms.InputTag("packedPFCandidates"))



process.tauTag = cms.EDAnalyzer('PatTauAna',
                                PruneGenParticleInputTag = cms.InputTag("prunedGenParticles"),
                                PackedGenParticleInputTag = cms.InputTag("packedGenParticles"),
                                prodTauTag = cms.InputTag("prodTaus", "PassTau"),
                                #SkipEvent = cms.untracked.vstring('ProductNotFound')"
)
#process.myProducerLabel = cms.EDProducer('PatTauID'
#)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('myOutputFile.root'),
                               #outputCommands = cms.untracked.vstring('drop *',
                                                                      #'keep *_demo_*_*'
                                                                      #'keep *_
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('outhist.root'))



#process.p = cms.Sequence(process.prodMuons * process.prodElectrons * process.prodTaus * process.goodVertices*  process.IsoTrack )
#process.p = cms.Path(process.goodVertices *  process.prodMuons * process.prodElectrons * process.prodTaus * process.IsoTrack )
process.p = cms.Path(process.goodVertices *  process.prodMuons * process.prodElectrons * process.prodTaus * process.IsoTrack * process.tauTag)
#process.p = cms.Path(process.prodMuons * process.prodElectrons * process.prodTaus * process.goodVertices*  process.IsoTrack  * process.tauTag)
#process.p = cms.Path(process.myProducerLabel)

process.e = cms.EndPath(process.out)
