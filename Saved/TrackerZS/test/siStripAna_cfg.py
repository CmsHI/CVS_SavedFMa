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
        #'file:hiReco_RAW2DIGI_RECO_ZSHEAD10_05_1150pm_ALLVR.root'
        #'file:hiReco_RAW2DIGI_RECO_ZSHEAD10_05_1150pm_ALLVR_DistrotionThresh0.root'
        #'file:hiReco_RAW2DIGI_RECO_ZSHEAD10_07_1148am_ALLVR_DistrotionThresh0MeanCM158GTV4_hi2010Pedestal.root'
        'file:hiReco_RAW2DIGI_RECO_ZSHEAD10_07_1148am_ALLVR_GTV4_hi2010Pedestal.root'
        # wei's jet 50u
        #Events in new ZS but not old ZS:
        #'file:/net/hisrv0001/home/davidlw/scratch1/HLTStudies/CMSSW_4_4_0_OldZS/src/RECO_highptInNewNotOld_OldZS_Jet50U.root'
        #'file:/net/hisrv0001/home/davidlw/scratch1/HLTStudies/CMSSW_4_4_0_NewZS/src/RECO_highptInNewNotOld_NewZS_Jet50U.root'
        #Events in old ZS but not new ZS:
        #'file:/net/hisrv0001/home/davidlw/scratch1/HLTStudies/CMSSW_4_4_0_NewZS/src/RECO_highptInOldNotNew_NewZS_Jet50U.root'
        #'file:/net/hisrv0001/home/davidlw/scratch1/HLTStudies/CMSSW_4_4_0_OldZS/src/RECO_highptInOldNotNew_OldZS_Jet50U.root'

    ),
    #eventsToProcess = cms.untracked.VEventRange('151088:107:646838-151088:107:646838')
)

process.GlobalTag.globaltag = 'GR_R_44_V5::All'

proc = "RECOZSHEAD"
tag="10_07_1148am"
#proc = "RECO1"
#tag=""
#zscfg="restoreThresh0MeanCM128GTV4_hi2010ped"
zscfg="GTV4_hi2010ped"
process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string("sstr_"+proc+tag+"_"+zscfg+".root"))
                                  
process.load("Saved.TrackerZS.trackrechitscanner_cfi")
process.load("RecoLocalTracker.SiStripZeroSuppression.SiStripBaselineAnalyzer_cfi")

process.trkRecHitScanner.trkSrc = "hiGlobalPrimTracks"
process.trkRecHitScanner.trkPtMin = 20

process.sshis = process.SiStripBaselineAnalyzer.clone(
  srcBaseline =  cms.InputTag('siStripZeroSuppression','BADAPVBASELINE'),
  srcAPVCM  =  cms.InputTag('siStripZeroSuppression','APVCM'),
  srcProcessedRawDigi =  cms.InputTag('siStripZeroSuppression','VirginRaw'),
  nModuletoDisplay = cms.uint32(500),
  plotClusters = cms.bool(True),
  plotBaseline = cms.bool(True),
  plotRawDigi	= cms.bool(True),
  plotAPVCM	= cms.bool(True)
  )

from RecoLocalTracker.SiStripZeroSuppression.DefaultAlgorithms_cff import *

process.ssnt = cms.EDAnalyzer("StripAnalyzer",
  Algorithms = DefaultAlgorithms,
  vr = cms.InputTag("siStripDigis","VirginRaw",proc),
  zs = cms.InputTag("siStripZeroSuppression","VirginRaw",proc),
  pr = cms.InputTag("siStripZeroSuppression","BADAPVBASELINE",proc),
  cl = cms.InputTag("siStripClusters","",proc),
  doVR = cms.bool(True),
  doZS = cms.bool(True),
  doPR = cms.bool(True),
  doCL = cms.bool(True)
  )

process.ssntBad = process.ssnt.clone(
  vr = cms.InputTag("siStripZeroSuppression","VirginRaw",proc),
  doZS = False,
  doPR = False,
  doCL = False
  )

process.ssntBadBLPts = process.ssnt.clone(
  zs = cms.InputTag("siStripZeroSuppression","BADAPVBASELINEPOINTS",proc),
  pr = cms.InputTag("siStripZeroSuppression","BADAPVBASELINEPOINTS",proc),
  doVR = False,
  doPR = False,
  doCL = False
  )
if proc=="RECO1":
  process.ssntBadBLPts.doPR=True
  process.ssntBadBLPts.doZS=False

#process.ssntBadRRZS = process.ssntRRZS.clone(
#  vr = cms.InputTag("siStripZeroSuppression","VirginRaw",proc)
#  )

#process.ssntRRZS = process.ssnt.clone(
#  vr=cms.InputTag("siStripDigis","VirginRaw",proc),
#  zs = cms.InputTag("siStripZeroSuppression","VirginRaw",proc),
#  pr = cms.InputTag("siStripZeroSuppression","BADAPVBASELINE",proc),
#  cl = cms.InputTag("siStripClusters","",proc),
#)

#process.p = cms.Path(process.sshis*process.ssnt*process.ssntRRZS*process.ssntBad*process.ssntBadRRZS)
process.p = cms.Path(process.sshis*process.ssnt*process.ssntBad*process.ssntBadBLPts)
