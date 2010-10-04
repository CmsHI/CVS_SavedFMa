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

# HI event Filters
from RecoHI.HiCentralityAlgos.CentralityFilter_cfi import *

# Final Filter Sequences
triggerSelection = cms.Sequence(physDeclFilter)
hiEvtFilter = cms.Sequence(centralityFilter)
