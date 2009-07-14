import FWCore.ParameterSet.Config as cms

process = cms.Process("GEN")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("CmsHi.Utilities.HiGenParticles_cfi")

process.load("GeneratorInterface.HydjetInterface.hydjetDefault_cfi")
process.generator.cFlag = 0
process.generator.bFixed = 0

process.source = cms.Source("EmptySource")

process.RandomNumberGeneratorService.generator.initialSeed = 1

from CmsHi.Utilities.EventEmbedding_cff import *
process.mix = mixGenNoPU.clone()

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(50)
                                       )

process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',
                                        ignoreTotal=cms.untracked.int32(0),
                                        oncePerEventMode = cms.untracked.bool(False)
                                        )

process.ana = cms.EDAnalyzer('SubEventAnalyzer'
                             )

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string('31X.root')
                                   )

process.output = cms.OutputModule("PoolOutputModule",
      #      process.HITrackAnalysisObjects,
      compressionLevel = cms.untracked.int32(2),
      commitInterval = cms.untracked.uint32(1),
      fileName = cms.untracked.string('hydjet_gen.root')
      )

process.p = cms.Path(process.generator*process.mix*process.hiGenParticles*process.ana)
process.save = cms.EndPath(process.output)






