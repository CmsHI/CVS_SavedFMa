import FWCore.ParameterSet.Config as cms

highPurityTracks = cms.EDFilter("TrackSelector",
    src = cms.InputTag("generalTracks"),
    cut = cms.string(
    'quality("highPurity") && (ptError/pt)<0.05 && algo<=7 && numberOfValidHits>=5')
)

