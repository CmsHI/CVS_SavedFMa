import FWCore.ParameterSet.Config as cms
process = cms.Process("bestjet")

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

# Skimmed AOD input
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    'file:/net/hibat0003/d00/scratch/frankma/HI/jet/JExCMSSW_3_7_0_patch4/skim/outputs/jetAnaSkim_Hard_try4_14k.root'))

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

# Select pat jets above some ET
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.selectedPatJets.cut = cms.string('pt > 170')

# Filter on events with jets above some ET
process.jetCount = cms.EDFilter("CandViewCountFilter",
                                src=cms.InputTag("selectedPatJets"),
                                minNumber = cms.uint32(1)
                                )

process.filter_step = cms.Path(process.selectedPatJets*process.jetCount)

# Save events that pass filter 
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('bestjet.root'),
                               SelectEvents=cms.untracked.PSet(
                                   SelectEvents=cms.vstring('filter_step')
                                   )
                               )

process.end = cms.EndPath(process.out)
