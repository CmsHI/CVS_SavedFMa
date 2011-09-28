import FWCore.ParameterSet.Config as cms

trkRecHitScanner = cms.EDAnalyzer('TrackRecHitScanner',
  trkSrc = cms.InputTag("hiGlobalPrimTracks"),
  trkPtMin = cms.double(20)
)
