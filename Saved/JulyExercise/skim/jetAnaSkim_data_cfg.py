import FWCore.ParameterSet.Config as cms

process = cms.Process('PAT')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("reco.root")
    )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
    )

process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
from Saved.PatAlgos.tools.heavyIonTools import *
configureHeavyIons(process)

from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJECSet( process, "Spring10") # Spring10 is the new default

process.load("HeavyIonsAnalysis.Configuration.analysisProducers_cff")
process.load("HeavyIonsAnalysis.Configuration.analysisEventContent_cff")

process.output = cms.OutputModule("PoolOutputModule",
    process.jetTrkSkimContent,
    fileName = cms.untracked.string("jetAnaSkim.root")
    )

process.prod = cms.Path(process.allTracks +
    #process.heavyIon +
    process.makeHeavyIonJets
    )

from Saved.JulyExercise.customise_cfi import *
disableMC(process)

process.out_step = cms.EndPath(process.output)

