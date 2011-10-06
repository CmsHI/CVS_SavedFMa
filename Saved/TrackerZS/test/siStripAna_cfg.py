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
        #'file:hiReco_RAW2DIGI_RECO.root'
        #'file:hiReco_RAW2DIGI_RECO_ZSHEAD10_05_13pm_ALLVR.root'
        'file:hiReco_RAW2DIGI_RECO_ZSHEAD10_05_1150pm_ALLVR.root'
    ),
    #eventsToProcess = cms.untracked.VEventRange('151088:107:646838-151088:107:646838')
)

process.GlobalTag.globaltag = 'GR_R_44_V5::All'

process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string("sstr_zshead10_05_23pm_allvr.root"))
                                  
process.load("Saved.TrackerZS.trackrechitscanner_cfi")
process.load("RecoLocalTracker.SiStripZeroSuppression.SiStripBaselineAnalyzer_cfi")

process.trkRecHitScanner.trkSrc = "hiGlobalPrimTracks"
process.trkRecHitScanner.trkPtMin = 20

process.sshis = process.SiStripBaselineAnalyzer.clone(
  srcBaseline =  cms.InputTag('siStripZeroSuppression','BADAPVBASELINE','RECO1'),
  srcAPVCM  =  cms.InputTag('siStripZeroSuppression','APVCM','RECO1'),
  srcProcessedRawDigi =  cms.InputTag('siStripZeroSuppression','VirginRaw','RECO1'),
  nModuletoDisplay = cms.uint32(100),
  plotClusters = cms.bool(True),
  plotBaseline = cms.bool(True),
  plotRawDigi	= cms.bool(True),
  plotAPVCM	= cms.bool(True)
  )

process.ssnt = cms.EDAnalyzer("StripAnalyzer",
  vr = cms.InputTag("siStripDigis","VirginRaw")
  )

process.ssntRRZS = process.ssnt.clone(vr=cms.InputTag("siStripDigis","VirginRaw","RECO1"))

process.ssntBad = process.ssnt.clone(vr=cms.InputTag("siStripZeroSuppression","VirginRaw"))

process.ssntBadRRZS = process.ssnt.clone(vr=cms.InputTag("siStripZeroSuppression","VirginRaw","RECO1"))

process.p = cms.Path(process.sshis*process.ssnt*process.ssntRRZS*process.ssntBad*process.ssntBadRRZS)
