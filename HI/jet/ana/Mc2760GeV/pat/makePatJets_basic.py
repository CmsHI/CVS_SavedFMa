import FWCore.ParameterSet.Config as cms

process = cms.Process('MATCH')

process.source = cms.Source("PoolSource",
      fileNames =  cms.untracked.vstring(["hireco.root"])
      )

process.maxEvents = cms.untracked.PSet(
      input = cms.untracked.int32(10)
      )

# define ana modules
process.load('ana.Mc2760GeV.patAna_cff')

# define pat producers
process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
from PhysicsTools.PatAlgos.tools.heavyIonTools import *
configureHeavyIons(process)
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "Summer09_7TeV_ReReco332")

process.patJets.addJetCorrFactors = True
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

process.output = cms.OutputModule("PoolOutputModule",
                                  fileName = cms.untracked.string("jetAnaSkim.root"),
                                  outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_*FastPu*_*_JETS',
    'drop recoCaloJets_*_*_*',
    'keep recoGenJets_*_*_HISIGNAL',
    'keep *_heavyIon_*_*',
    'keep *_hiCentrality_*_*',
    'keep *_towerMaker_*_JETS',
    'keep patJets_*_*_*',
    'keep *_partons_*_*',
    )
)

process.cleaning = cms.Sequence(
   (process.partons *
   #process.heavyIonCleanedPartons *
   process.ak5clean)
)

process.ak5pu_patseq = cms.Sequence(
   process.akPu5match *
   process.akPu5PartonMatch *
   process.akPu5corr *
   process.akPu5patJets
)

process.ana = cms.Path(
   (process.cleaning) *
   (process.ak5pu_patseq)
)

process.out_step = cms.EndPath(process.output)
