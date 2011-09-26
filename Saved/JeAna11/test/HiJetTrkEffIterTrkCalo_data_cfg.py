import FWCore.ParameterSet.Config as cms

process = cms.Process('JetAna')

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
       '/store/hidata/HIRun2010/HICorePhysics/RAW-RECO/v3/000/151/236/42DF91BD-DEF1-DF11-B8B3-00E08178C11F.root',
       '/store/hidata/HIRun2010/HICorePhysics/RAW-RECO/v3/000/151/353/46558173-EBF2-DF11-90FF-00E08178C169.root',
       '/store/hidata/HIRun2010/HICorePhysics/RAW-RECO/v3/000/151/237/A23DAC27-BEF2-DF11-90CA-0025B3E06514.root',
       '/store/hidata/HIRun2010/HICorePhysics/RAW-RECO/v3/000/151/353/92DDA1AA-ECF2-DF11-9B4F-0025B3E063F0.root',
       '/store/hidata/HIRun2010/HICorePhysics/RAW-RECO/v3/000/151/350/141AE965-E5F2-DF11-87F4-003048635E3C.root',
       '/store/hidata/HIRun2010/HICorePhysics/RAW-RECO/v3/000/151/353/72DB1A0E-ECF2-DF11-B9A2-00E08178C13B.root',
       '/store/hidata/HIRun2010/HICorePhysics/RAW-RECO/v3/000/151/211/AA62AB4C-6DF2-DF11-A3D7-001A64789DB8.root',
       '/store/hidata/HIRun2010/HICorePhysics/RAW-RECO/v3/000/151/426/86E5551A-AAF2-DF11-BA09-0025B3E0648A.root',
       '/store/hidata/HIRun2010/HICorePhysics/RAW-RECO/v3/000/151/428/F42E8600-E6F2-DF11-822E-002481E154EC.root',
       '/store/hidata/HIRun2010/HICorePhysics/RAW-RECO/v3/000/151/020/08F3D35C-ACF0-DF11-8131-001A64789E68.root'
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
process.GlobalTag.globaltag = 'GR_R_39X_V6B::All'

# load centrality
from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)
process.HeavyIonGlobalParameters = cms.PSet(
	centralityVariable = cms.string("HFhits"),
	nonDefaultGlauberModel = cms.string(""),
	centralitySrc = cms.InputTag("hiCentrality")
	)

# tree output
process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string("JetTrkTreeData.root"))

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
process.load('CmsHi.JetAnalysis.EventSelection_cff')
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

# Filtering
process.hltJetHI.HLTPaths = ['HLT_HIJet50U']
print "Add cleaning to analysis"
process.event_filter_seq = cms.Sequence(
  process.hltJetHI *
  process.collisionEventSelection *
  process.hbheReflagNewTimeEnv *
  process.hcalTimingFilter *
  process.HBHENoiseFilter *
  process.hiEcalRecHitSpikeFilter
  )

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
process.anaTrack.doSimTrack = cms.untracked.bool(False)
process.anaTrack.trackSrc = cms.InputTag('hiGeneralGlobalPrimTracks')
process.anaTrack.doPFMatching = cms.untracked.bool(True)

process.anaTrack_hgt = process.anaTrack.clone(trackSrc = 'hiGoodTightTracks')

process.rechitanalyzer.HBHETreePtMin = cms.untracked.double(2)
process.rechitanalyzer.HFTreePtMin = cms.untracked.double(2)
process.rechitanalyzer.EBTreePtMin = cms.untracked.double(2)
process.rechitanalyzer.EETreePtMin = cms.untracked.double(2)
process.rechitanalyzer.TowerTreePtMin = cms.untracked.double(2)

# Final Paths
process.reco_extra =      cms.Path( process.event_filter_seq * process.iterTracking_seq * process.HiParticleFlowRecoNoJets)
process.reco_extra_jet =  cms.Path( process.event_filter_seq * process.iterativeConePu5CaloJets )
process.pat_step =        cms.Path( process.event_filter_seq * process.icPu5patSequence_data)
process.ana_step =        cms.Path( process.event_filter_seq * process.icPu5JetAnalyzer * process.anaTrack * process.anaTrack_hgt * process.rechitanalyzer)

# Customization
from CmsHi.JetAnalysis.customise_cfi import *
enableDataPat(process)
enableDataAnalyzers(process)
enableOpenHlt(process,process.ana_step)

# =============== Final Schedule =====================
process.schedule = cms.Schedule(process.reco_extra,process.reco_extra_jet,process.pat_step,process.ana_step)
