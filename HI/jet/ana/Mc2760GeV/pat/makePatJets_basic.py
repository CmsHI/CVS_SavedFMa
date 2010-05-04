import FWCore.ParameterSet.Config as cms

process = cms.Process('AnaSkim')
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')


process.source = cms.Source("PoolSource",
    fileNames =  cms.untracked.vstring([
      "/store/relval/CMSSW_3_7_0_pre1/RelValPyquen_DiJet_pt80to120_2760GeV/GEN-SIM-RECO/MC_37Y_V0-v1/0015/EE6CFF9B-3A4D-DF11-A326-002618943868.root"])
    )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
    )

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.GlobalTag.globaltag = 'MC_37Y_V0::All'

# define ana modules
process.load('ana.Mc2760GeV.patAna_cff')

# define pat producers
process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
from PhysicsTools.PatAlgos.tools.heavyIonTools import *
configureHeavyIons(process)
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "Summer09_7TeV_ReReco332")

process.patJets.addJetCorrFactors   = True
process.patJets.addGenPartonMatch   = True
process.patJets.addJetID            = False
process.patJets.addGenJetMatch      = True
process.patJets.embedGenJetMatch    = True
process.akPu5patJets = process.patJets.clone(
   jetSource  = cms.InputTag("akPu5CaloJets"),
   genJetMatch = cms.InputTag("akPu5match"),
   genPartonMatch =  cms.InputTag("akPu5PartonMatch"),
   jetCorrFactorsSource = cms.VInputTag(cms.InputTag("akPu5corr"))
)

# define track candidates
process.load("ana.Mc2760GeV.anaTrkCandidates_cff")

# === Output ===
process.load("ana.Mc2760GeV.anaSkimContent_cff")
process.output = cms.OutputModule("PoolOutputModule",
    process.analysisSkimContent,
    fileName = cms.untracked.string("jetAnaSkim.root")
    )

# === Final Paths ===
process.ak5pu_patseq = cms.Sequence(
   process.akPu5match *
   process.akPu5PartonMatch *
   process.akPu5corr *
   process.akPu5patJets
)

process.ana = cms.Path(
   process.genana *
   process.ak5pu_patseq *
   process.allTracks
)

process.out_step = cms.EndPath(process.output)
