import FWCore.ParameterSet.Config as cms

from Saved.DiJetAna.TrackSelection_cff import *
from HeavyIonsAnalysis.Configuration.analysisProducers_cff import *
from RecoHI.HiCentralityAlgos.CentralityBin_cfi import *

dj_reco_extra = cms.Sequence(centralityBin #*
    #highPurityTracks
    #allTracks
    )
