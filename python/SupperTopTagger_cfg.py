import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
options = VarParsing ('analysis')
options.parseArguments()

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    #fileNames = cms.untracked.vstring('file:/Users/benwu/Data/Dataset/PHY14/TTJets_PU20bx25_PHYS14.root')
    fileNames = cms.untracked.vstring(options.inputFiles),
)

process.demo = cms.EDAnalyzer('SuperTopTagger',
                              PruneGenParticleInputTag = cms.InputTag("prunedGenParticles"),
                              PackedGenParticleInputTag = cms.InputTag("packedGenParticles"),
                              METTag = cms.InputTag("slimmedMETs"),
                              JetTag = cms.InputTag("slimmedJets"),
)


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(options.outputFile))

process.p = cms.Path(process.demo)
    
