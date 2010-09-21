import FWCore.ParameterSet.Config as cms

process = cms.Process('PAT')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("reco.root"),
    inputCommands = cms.untracked.vstring("keep *_*_*_*",
      "drop *_iterativeConePu5CaloJets_*_*"
      ),
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False)
    )

process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/ReconstructionHeavyIons_cff')
process.GlobalTag.globaltag = 'MC_38Y_V10::All'

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
    )

process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
from PhysicsTools.PatAlgos.tools.heavyIonTools import *
configureHeavyIons(process)

from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJECSet( process, "Spring10") # Spring10 is the new default

process.load("HeavyIonsAnalysis.Configuration.analysisProducers_cff")
process.load("HeavyIonsAnalysis.Configuration.analysisEventContent_cff")

process.output = cms.OutputModule("PoolOutputModule",
    process.jetTrkSkimContent,
    fileName = cms.untracked.string("jetAnaSkim.root")
    )
process.output.outputCommands.extend([
  "keep recoVertexs_hiSelectedVertex__RECO",
  "keep edmTriggerResults_TriggerResults__*" ,
  "keep triggerTriggerEvent_hltTriggerSummaryAOD__*",
  "keep *_hiGenParticles_*_*",
  ])
#"keep recoTracks_hiSelectedTracks__RECO",


process.prod = cms.Path(
    process.allTracks +
    process.heavyIon *
    process.iterativeConePu5CaloJets *
    process.makeHeavyIonJets
    )

from Saved.JulyExercise.customise_cfi import *
#disableMC(process)

process.out_step = cms.EndPath(process.output)

