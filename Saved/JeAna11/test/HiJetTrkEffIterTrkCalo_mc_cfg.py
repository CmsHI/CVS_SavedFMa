import FWCore.ParameterSet.Config as cms

process = cms.Process('JetAna')

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    '/store/user/davidlw/Hydjet1_8v1_MinBias_2760GeV_GEN_SIM_RAW_399p1_v1/Hydjet1_8v1_MinBias_2760GeV_GEN_SIM_RECO_399p1_v1/3a59201d76ed5b3e8f0cfb7f019327cc/hiReco_RAW2DIGI_RECO_9_1_Bk0.root'
    ))

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(5))

#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

#process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.GlobalTag.globaltag = 'START39_V7::All'

process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
 ignoreTotal=cms.untracked.int32(1),
 oncePerEventMode=cms.untracked.bool(False)
)

# load centrality
from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)
process.HeavyIonGlobalParameters = cms.PSet(
	centralityVariable = cms.string("HFhits"),
	nonDefaultGlauberModel = cms.string("Hydjet_Bass"),
	centralitySrc = cms.InputTag("hiCentrality")
	)

# tree output
process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string("JetTrkTree.root"))

# Define Jet energy correction
process.jec = cms.ESSource("PoolDBESSource",
	DBParameters = cms.PSet(messageLevel = cms.untracked.int32(0)),
	timetype = cms.string('runnumber'),
	toGet = cms.VPSet(
		cms.PSet(record = cms.string("JetCorrectionsRecord"),
		tag = cms.string("JetCorrectorParametersCollection_HI_hiGoodTightTracks_D6T_399_IC5Calo"),
		label = cms.untracked.string("IC5Calo")),
		cms.PSet(record = cms.string("JetCorrectionsRecord"),
		tag = cms.string("JetCorrectorParametersCollection_HI_PFTowers_hiGoodTightTracks_D6T_399_AK3PF"),
		label = cms.untracked.string("AK3PF")),
		cms.PSet(record = cms.string("JetCorrectionsRecord"),
		tag = cms.string("JetCorrectorParametersCollection_HI_PFTowers_hiGoodTightTracks_D6T_399_AK4PF"),
		label = cms.untracked.string("AK4PF")),
		cms.PSet(record = cms.string("JetCorrectionsRecord"),
		tag = cms.string("JetCorrectorParametersCollection_HI_PFTowers_hiGoodTightTracks_D6T_399_AK5PF"),
		label = cms.untracked.string("AK5PF")
		),
	),
	connect = cms.string("sqlite_file:JEC_HI_PFTowers_hiGoodTightTracks_D6T_399.db"),
)
process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

# Define Analysis sequencues
process.load('CmsHi.JetAnalysis.ExtraGenReco_cff')
process.load('CmsHi.JetAnalysis.ExtraTrackReco_cff')
process.load('CmsHi.JetAnalysis.ExtraPfReco_cff')
process.load('CmsHi.JetAnalysis.ExtraJetReco_cff')
process.load('CmsHi.JetAnalysis.PatAna_cff')
process.load('CmsHi.JetAnalysis.JetAnalyzers_cff')
process.load('CmsHi.JetAnalysis.TrkAnalyzers_cff')
process.load('MitHig.PixelTrackletAnalyzer.trackAnalyzer_cff')
process.load('CmsHi.JetAnalysis.rechitanalyzer_cfi')
process.hiPixelAdaptiveVertex.useBeamConstraint = cms.bool(False) # better data vs mc comparison

# matt's iterative tracking
process.load("MNguyen.iterTracking.TrackSelections_cff")
process.hiGoodTightTracks.keepAllTracks = False
process.load("MNguyen.iterTracking.secondStep_triplet_cff")
process.load("MNguyen.iterTracking.thirdStep_pairs_cff")
process.load("MNguyen.iterTracking.MergeTrackCollectionsHI_cff")

process.iterGlobalPrimTracks = process.iterTracks.clone(
    TrackProducer1 = 'hiScndGlobalPrimTracks',
    TrackProducer2 = 'hiThrdGlobalPrimTracks',
    )


process.hiGeneralGlobalPrimTracks = process.hiGeneralTracks.clone(
    TrackProducer1 = 'hiGlobalPrimTracks',
    TrackProducer2 = 'iterGlobalPrimTracks',
    )

process.globalPrimTrackCollectionMerging = cms.Sequence(
    process.iterGlobalPrimTracks*
    process.hiGeneralGlobalPrimTracks
    )

process.iterTracking_seq = cms.Sequence(
    process.hiTrackReReco *
    process.hiGoodTightTracks *
    process.secondStep *
    process.thirdStep *
    process.globalPrimTrackCollectionMerging
    )

# pf
process.trackerDrivenElectronSeeds.TkColList = cms.VInputTag("hiGeneralGlobalPrimTracks")
process.trackerDrivenElectronSeeds.UseQuality = cms.bool(False)

# analyzer
process.anaTrack.trackPtMin = cms.untracked.double(4)
process.anaTrack.simTrackPtMin = cms.untracked.double(4)
process.anaTrack.doSimTrack = cms.untracked.bool(True)
process.anaTrack.trackSrc = cms.InputTag('hiGeneralGlobalPrimTracks')
process.anaTrack.tpFakeSrc = cms.untracked.InputTag('cutsTPForFak')
process.anaTrack.tpEffSrc = cms.untracked.InputTag('cutsTPForEff')
process.anaTrack.doPFMatching = cms.untracked.bool(True)

process.anaTrack_hgt = process.anaTrack.clone(trackSrc = 'hiGoodTightTracks')

process.rechitanalyzer.HBHETreePtMin = cms.untracked.double(2)
process.rechitanalyzer.HFTreePtMin = cms.untracked.double(2)
process.rechitanalyzer.EBTreePtMin = cms.untracked.double(2)
process.rechitanalyzer.EETreePtMin = cms.untracked.double(2)
process.rechitanalyzer.TowerTreePtMin = cms.untracked.double(2)

# fill tree
process.hitrkEffAnalyzer_akpu3pf.fillNtuples = cms.bool(True)
process.hitrkEffAnalyzer_akpu3pf.trkPtMin = cms.double(4)
process.hitrkEffAnalyzer_akpu3pf.tracks = cms.untracked.InputTag("hiGeneralGlobalPrimTracks")
# use calo jet
process.hitrkEffAnalyzer_akpu3pf.jets = cms.untracked.InputTag("icPu5patJets")


process.reco_extra = cms.Path( process.hiGen * process.iterTracking_seq * process.HiParticleFlowRecoNoJets)
process.reco_extra_jet = cms.Path( process.iterativeConePu5CaloJets )
process.pat_step = cms.Path(process.icPu5patSequence)
process.ana_step = cms.Path(process.hitrkEffAna_akpu3pf * process.icPu5JetAnalyzer * process.anaTrack * process.anaTrack_hgt * process.rechitanalyzer)

# Customization
from CmsHi.JetAnalysis.customise_cfi import *
#enableDataPat(process)
#enableDataAnalyzers(process)
enableOpenHlt(process,process.ana_step)
# to run on hydjet only
process.hiGenParticles.srcVector = cms.vstring('generator')
process.icPu5JetAnalyzer.eventInfoTag = cms.InputTag("generator")

# =============== Final Schedule =====================
process.schedule = cms.Schedule(process.reco_extra,process.reco_extra_jet,process.pat_step,process.ana_step)

