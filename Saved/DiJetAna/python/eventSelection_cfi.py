import FWCore.ParameterSet.Config as cms

# Beam Basic
physDeclFilter = cms.EDFilter("PhysDecl",
    applyfilter = cms.untracked.bool(False),
    HLTriggerResults = cms.InputTag("TriggerResults","","HLT")
    )

# Basic L1 Filters
from L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff import *
from L1Trigger.Skimmer.l1Filter_cfi import l1Filter
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed

bscNoHalo = hltLevel1GTSeed.clone(
    l1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('NOT (36 OR 37 OR 38 OR 39)')
    )

L1HfOrBscCoinc = l1Filter.clone(
    algorithms = cms.vstring("L1_BscMinBiasInnerThreshold1","L1_HcalHfCoincidencePm")
    )

# HLT Selection
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
hltMB = hltHighLevel.clone( HLTPaths = ["HLT_HIMinBiasHfOrBSC_Core"] )
hltJet = hltHighLevel.clone( HLTPaths = ["HLT_HIJet35U_Core","HLT_HIJet50U_Core","HLT_HIJet75U_Core","HLT_HIJet90U_Core"] )

# HI event Filters
from RecoHI.HiCentralityAlgos.CentralityFilter_cfi import *
centralityFilter.centralityBase = "HF"
centralityFilter.selectedBins = range(40)

# HI Data
from HeavyIonsAnalysis.Configuration.collisionEventSelection_cff import *

# Noise Filters
from CmsHi.PhotonAnalysis.hiEcalRecHitSpikeFilter_cfi import *
hiEcalRecHitSpikeFilter.minEt = 50.0

# Final Filter Sequences
triggerSelection = cms.Sequence(physDeclFilter)
hiEvtFilter = cms.Sequence(centralityFilter)
