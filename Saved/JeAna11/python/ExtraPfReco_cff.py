import FWCore.ParameterSet.Config as cms

from RecoHI.Configuration.Reconstruction_hiPF_cff import *

pfTrack.TkColList = cms.VInputTag("hiGoodTightTracks")

# PF uisng a grid of pseudo-towers to emulate Russian style subtraction
from RecoHI.HiJetAlgos.ParticleTowerProducer_cff import *
# Needed to produce "HcalSeverityLevelComputerRcd" used by CaloTowersCreator/towerMakerPF
from RecoLocalCalo.Configuration.hcalLocalReco_cff import *
PFTowers = particleTowerProducer.clone()

HiParticleFlowRecoNoJets = cms.Sequence(
	particleFlowCluster
	* pfTrack
	* particleFlowReco
	* PFTowers
	)



