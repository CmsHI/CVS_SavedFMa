import FWCore.ParameterSet.Config as cms

# pixel triplet tracking (HI Tracking)
from RecoLocalTracker.Configuration.RecoLocalTracker_cff import *
from RecoHI.Configuration.Reconstruction_HI_cff import *

#Track Reco
rechits = cms.Sequence(siPixelRecHits * siStripMatchedRecHits)
hiTrackReReco = cms.Sequence(rechits * heavyIonTracking)

# good track selection
from edwenger.HiTrkEffAnalyzer.TrackSelections_cff import *

hiextraTrackReco = cms.Sequence(
		hiPostGlobalPrimTracks *
    hiGoodTightTracksSelection
    )
