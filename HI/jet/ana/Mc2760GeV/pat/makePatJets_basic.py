import FWCore.ParameterSet.Config as cms

process = cms.Process('MATCH')

process.source = cms.Source("PoolSource",
      fileNames =  cms.untracked.vstring(["hireco.root"])
      )

process.maxEvents = cms.untracked.PSet(
      input = cms.untracked.int32(10)
      )

process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
from PhysicsTools.PatAlgos.tools.heavyIonTools import *
configureHeavyIons(process)

from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "Summer09_7TeV_ReReco332")

# calo
#process.ak5corr = process.patJetCorrFactors.clone()
#process.ak5corr.jetSource = cms.InputTag("akFastPu5CaloJets")
#process.ak5corr.corrLevels.L2Relative = "L2Relative_AK5Calo"
#process.ak5corr.corrLevels.L3Absolute = "L3Absolute_AK5Calo"
process.akPu5corr = process.patJetCorrFactors.clone(
   jetSource = cms.InputTag("akPu5CaloJets")
)

# mc inputs
process.ak5clean = process.heavyIonCleanedGenJets.clone()
process.ak5clean.src = cms.untracked.string('ak5HiGenJets')
from PhysicsTools.JetMCAlgos.SelectPartons_cff import myPartons
process.partons = myPartons.clone(
   src = cms.InputTag("hiGenParticles")
)
process.heavyIonCleanedPartons = cms.EDProducer('HiPartonCleaner',
   src    = cms.untracked.string('partons'),
   deltaR = cms.untracked.double(0.25),
   ptCut  = cms.untracked.double(20),
   createNewCollection = cms.untracked.bool(True),
   fillDummyEntries = cms.untracked.bool(True)
)

# matching
process.akPu5match = process.patJetGenJetMatch.clone(
   src = cms.InputTag("akPu5CaloJets"),
   matched = cms.InputTag("ak5clean")
)
process.akPu5PartonMatch = process.patJetPartonMatch.clone(
   src = cms.InputTag("akPu5CaloJets"),
   matched = cms.InputTag("hiGenParticles")
)

# pat settings
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
