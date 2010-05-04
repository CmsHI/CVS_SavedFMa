import FWCore.ParameterSet.Config as cms

# Main pat sequences
from PhysicsTools.PatAlgos.patHeavyIonSequences_cff import *

# calo
akPu5corr = patJetCorrFactors.clone(
   jetSource = cms.InputTag("akPu5CaloJets")
)

# mc inputs
ak5clean = heavyIonCleanedGenJets.clone()
ak5clean.src = cms.untracked.string('ak5HiGenJets')

from PhysicsTools.JetMCAlgos.SelectPartons_cff import myPartons
partons = myPartons.clone(
   src = cms.InputTag("hiGenParticles")
)

# matching
akPu5match = patJetGenJetMatch.clone(
   src = cms.InputTag("akPu5CaloJets"),
   matched = cms.InputTag("ak5clean")
)
akPu5PartonMatch = patJetPartonMatch.clone(
   src = cms.InputTag("akPu5CaloJets"),
   matched = cms.InputTag("hiGenParticles")
)

# sequences
genana = cms.Sequence(
   partons *
   #heavyIonCleanedPartons *
   ak5clean
)

