import FWCore.ParameterSet.Config as cms

# Gen
from PhysicsTools.HepMCCandAlgos.HiGenParticles_cfi import *
from RecoJets.Configuration.GenJetParticles_cff import *
from RecoHI.HiJetAlgos.HiGenJets_cff import *
from RecoHI.HiJetAlgos.HiGenCleaner_cff import *
from PhysicsTools.PatAlgos.producersHeavyIons.heavyIonProducer_cfi import heavyIon

hiGenParticles.srcVector = cms.vstring('hiSignal')

hiAnaGenJets = cms.Sequence(iterativeCone5HiGenJets + ak5HiGenJets + ak4HiGenJets + ak3HiGenJets )

hiGen = cms.Sequence(
  heavyIon * # GenHIEventProducer
  hiGenParticles *
  hiGenParticlesForJets *
  hiAnaGenJets *
  genPartons *
  hiPartons
  )