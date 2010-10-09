import FWCore.ParameterSet.Config as cms

from Saved.DiJetAna.TrackSelection_cff import *
from HeavyIonsAnalysis.Configuration.analysisProducers_cff import *

reco_extra = cms.Sequence(highPurityTracks)
