import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:~davidlw/scratch1/HLTStudies/CMSSW_4_4_0_NewZS/src/RECO_highptInNewNotOld_NewZS_noBS.root'
    )
)

process.load("Saved.TrackerZS.trackrechitscanner_cfi")
process.trkRecHitScanner.trkSrc = "hiGlobalPrimTracks"
process.trkRecHitScanner.trkPtMin = 20

process.p = cms.Path(process.trkRecHitScanner)
