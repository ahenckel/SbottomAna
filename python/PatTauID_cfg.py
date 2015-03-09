import FWCore.ParameterSet.Config as cms

process = cms.Process("OWNPARTICLES")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/data/nbay04/c/benwu/PHY14/TTJets_PU20bx25_PHYS14.root'
    )
)

process.load("UserCode/SuperTopTagger/PatTauID_cfi")
#process.myProducerLabel = cms.EDProducer('PatTauID'
#)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('myOutputFile.root'),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_PassTau_*_*'
                               )

)


process.p = cms.Path(process.demo)
#process.p = cms.Path(process.myProducerLabel)

process.e = cms.EndPath(process.out)
