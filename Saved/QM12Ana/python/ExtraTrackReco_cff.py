import FWCore.ParameterSet.Config as cms

from RecoLocalTracker.Configuration.RecoLocalTracker_cff import *
from RecoHI.HiTracking.LowPtTracking_PbPb_cff import *
from RecoHI.HiTracking.hiIterTracking_cff import *
from RecoHI.HiTracking.HICaloCompatibleTracks_cff import *

hiCaloCompatibleGeneralTracks = hiCaloCompatibleTracks.clone(srcTracks = 'hiGeneralTracks')

hiCaloCompatibleGeneralTracksQuality = cms.EDFilter("TrackSelector",
   src = cms.InputTag("hiCaloCompatibleGeneralTracks"),
   cut = cms.string(
   'quality("highPuritySetWithPV")')
)

hiGeneralTracksQuality = cms.EDFilter("TrackSelector",
   src = cms.InputTag("hiCaloCompatibleGeneralTracks"),
   cut = cms.string(
   'quality("highPurity")')
)

hiSelectedTrackQuality = cms.EDFilter("TrackSelector",
   src = cms.InputTag("hiCaloCompatibleGeneralTracks"),
   cut = cms.string(
   'quality("highPurity")&&algo()==4')
)

# Conformal Pixel Tracks
from Appeltel.PixelTracksRun2010.HiLowPtPixelTracksFromReco_cff import *
from Appeltel.PixelTracksRun2010.HiMultipleMergedTracks_cff import *

# Track Mergeing
hiMergedTracksSelcted = hiGoodMergedTracks.clone(
    TrackProducer1  = "hiSelectedTrackQuality",
    TrackProducer2  = "hiConformalPixelTracks")

hiMergedTracksGeneral = hiGoodMergedTracks.clone(
    TrackProducer1  = "hiGeneralTracksQuality",
    TrackProducer2  = "hiConformalPixelTracks")

hiMergedTracksGeneralCalo = hiGoodMergedTracks.clone(
    TrackProducer1  = "hiCaloCompatibleGeneralTracksQuality",
    TrackProducer2  = "hiConformalPixelTracks")

hiextraTrackReco = cms.Sequence( conformalPixelTrackReco *
                                 hiCaloCompatibleGeneralTracks * hiCaloCompatibleGeneralTracksQuality * hiMergedTracksGeneralCalo *
                                 hiSelectedTrackQuality * hiMergedTracksSelcted * 
                                 hiGeneralTracksQuality * hiMergedTracksGeneral
                               )
                                 
