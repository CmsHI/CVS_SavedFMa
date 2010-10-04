import FWCore.ParameterSet.Config as cms

from Saved.DiJetAna.eventSelection_cfi import *

eventSelection = cms.Sequence(
    triggerSelection *
    hiEvtFilter
    )
