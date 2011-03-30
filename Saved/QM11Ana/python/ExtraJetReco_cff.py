import FWCore.ParameterSet.Config as cms

# PF uisng a grid of pseudo-towers to emulate Russian style subtraction
from RecoHI.HiJetAlgos.ParticleTowerProducer_cff import *
PFTowers = particleTowerProducer.clone()

# reco jet with russian pileup subtraction
from RecoHI.HiJetAlgos.HiRecoJets_cff import *
from RecoHI.HiJetAlgos.HiRecoPFJets_cff import *

iterativeConePu5CaloJets.puPtMin = cms.double(15.0)
iterativeConePu5CaloJets.doPVCorrection = cms.bool(True)
iterativeConePu5CaloJets.srcPVs = 'hiSelectedVertex'

akPu5PFJets = ak5PFJets.clone()
akPu5PFJets.src = 'PFTowers'
akPu5PFJets.jetType = 'BasicJet'
akPu5PFJets.doPUOffsetCorr = True
akPu5PFJets.sumRecHits = False
akPu5PFJets.puPtMin = cms.double(30.0)

akPu3PFJets = akPu5PFJets.clone()
akPu3PFJets.rParam = cms.double(0.3)
akPu3PFJets.puPtMin = cms.double(20.0)
