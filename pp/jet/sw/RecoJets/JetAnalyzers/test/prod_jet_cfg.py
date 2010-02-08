import FWCore.ParameterSet.Config as cms

process = cms.Process('ANA')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.Generator_cff')

process.maxEvents = cms.untracked.PSet(
          input = cms.untracked.int32(1)
	  )

# Input source
process.source = cms.Source("PoolSource",
          fileNames = cms.untracked.vstring('file:input.root')
	  )

process.TFileService = cms.Service("TFileService",fileName = cms.string('jet_ana.root'))

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
      splitLevel = cms.untracked.int32(0),
      #outputCommands = process.RAWDEBUGEventContent.outputCommands,
      fileName = cms.untracked.string('Hydjet_Quenched_MinBias_4TeV_cfi_py_ANA.root'),
      dataset = cms.untracked.PSet(
	 dataTier = cms.untracked.string('GEN'),
	 filterName = cms.untracked.string('')
	 )
      )


#############   Gen Jets   ###########################
process.ana = cms.EDAnalyzer("GenJetPlotsExample",
    JetAlgorithm  = cms.string('iterativeCone5HiGenJets'),
    HistoFileName = cms.string('GenJetPlotsExample.root'),
    NJets         = cms.int32(2)
)

# Path and EndPath definitions
process.generation_step = cms.Path(process.hiGenParticles*process.hiGenJets*process.ana)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step)

