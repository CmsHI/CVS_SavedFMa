import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
#process.load('Configuration.StandardSequences.GeometryDB_cff')
#process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        #'file:~davidlw/scratch1/HLTStudies/CMSSW_4_4_0_NewZS/src/RECO_highptInNewNotOld_NewZS_noBS.root'
        'file:hiReco_RAW2DIGI_RECO.root'
    )
)

process.GlobalTag.globaltag = 'GR_R_44_V5::All'

process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string("hists.root"))
                                  
process.load("Saved.TrackerZS.trackrechitscanner_cfi")
process.load("RecoLocalTracker.SiStripZeroSuppression.SiStripBaselineAnalyzer_cfi")

process.trkRecHitScanner.trkSrc = "hiGlobalPrimTracks"
process.trkRecHitScanner.trkPtMin = 20

process.sistripana = process.SiStripBaselineAnalyzer.clone(
  nModuletoDisplay = 100,
  plotClusters = cms.bool(True),
  plotBaseline = cms.bool(True),
  plotRawDigi	= cms.bool(True),
  plotAPVCM	= cms.bool(True)
  )

process.p = cms.Path(process.sistripana)
