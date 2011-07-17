import FWCore.ParameterSet.Config as cms

process = cms.Process('JetAna')

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    '/store/user/davidlw/Hydjet_Bass_MinBias_2760GeV/Pyquen_UnquenchedDiJet_Pt170_GEN-SIM-RECO_393_set1/fae6fe9048513d9ac8f476dd10ba6ba7/hiReco_RAW2DIGI_RECO_99_1_Zza.root'
    ))

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(1))

#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

# pair-seeded step
process.load("RecoHI.HiTracking.secondStep_cff")                            # pair-seeded step

#process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.GlobalTag.globaltag = 'START39_V7::All'

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
process.load('Saved.JeAna11.ExtraGenReco_cff')
process.load('Saved.JeAna11.ExtraTrackReco_cff')
process.load('Saved.JeAna11.ExtraPfReco_cff')
process.load('Saved.JeAna11.ExtraJetReco_cff')
process.load('Saved.JeAna11.PatAna_cff')
process.load('Saved.JeAna11.JetAnalyzers_cff')
process.load('Saved.JeAna11.TrkAnalyzers_cff')

process.hiPostGlobalFirstStepTracks = cms.EDProducer("HiHackedAnalyticalTrackSelector",
    max_d0 = cms.double(1000),
    minNumber3DLayers = cms.uint32(0),
    qualityBit = cms.string(''),
    minNumberLayers = cms.uint32(0),
    chi2n_par = cms.double(0.4),
    dz_par1 = cms.vdouble(99999.0, 0.0),
    dz_par2 = cms.vdouble(999.0, 0.0),
    applyAdaptedPVCuts = cms.bool(True),
    vtxTracks = cms.uint32(0),
    vtxChi2Prob = cms.double(0.0),
    copyTrajectories = cms.untracked.bool(True),
    vtxNumber = cms.int32(0),
    keepAllTracks = cms.bool(False),
    maxNumberLostLayers = cms.uint32(99999),
    min_relpterr = cms.double(0.06),
    copyExtras = cms.untracked.bool(False),
    beamspot = cms.InputTag("offlineBeamSpot"),
    max_z0 = cms.double(1000),
    min_nhits = cms.uint32(12),
    src = cms.InputTag("hiGlobalPrimTracks"),
    vertices = cms.InputTag("hiSelectedVertex"),
    d0_par2 = cms.vdouble(999.0, 0.0),
    d0_par1 = cms.vdouble(99999.0, 0.0),
    res_par = cms.vdouble(99999.0, 99999.0)
)
process.hiGoodTightFirstStepTracks = cms.EDProducer("HiHackedAnalyticalTrackSelector",
    max_d0 = cms.double(1000),
    minNumber3DLayers = cms.uint32(0),
    qualityBit = cms.string('highPurity'),
    minNumberLayers = cms.uint32(0),
    chi2n_par = cms.double(0.15),
    dz_par1 = cms.vdouble(99999.0, 0.0),
    dz_par2 = cms.vdouble(3, 0.0),
    applyAdaptedPVCuts = cms.bool(True),
    vtxTracks = cms.uint32(0),
    vtxChi2Prob = cms.double(0.0),
    copyTrajectories = cms.untracked.bool(True),
    vtxNumber = cms.int32(0),
    keepAllTracks = cms.bool(False),
    maxNumberLostLayers = cms.uint32(99999),
    min_relpterr = cms.double(0.05),
    copyExtras = cms.untracked.bool(False),
    beamspot = cms.InputTag("offlineBeamSpot"),
    max_z0 = cms.double(1000),
    min_nhits = cms.uint32(13),
    src = cms.InputTag("hiPostGlobalFirstStepTracks"),
    vertices = cms.InputTag("hiSelectedVertex"),
    d0_par2 = cms.vdouble(3, 0.0),
    d0_par1 = cms.vdouble(99999.0, 0.0),
    res_par = cms.vdouble(99999.0, 99999.0)
)

process.hiNewClusters.Strip.maxSize = cms.uint32(2)

process.anaTrack = cms.EDAnalyzer('TrackAnalyzer',
                                  trackPtMin = cms.untracked.double(4),
                                  simTrackPtMin = cms.untracked.double(4),
                                  doSimTrack = cms.untracked.bool(True),
                                  vertexSrc = cms.vstring('hiSelectedVertex'),
                                  trackSrc = cms.InputTag('hiMergedTracks'),
                                  tpFakeSrc = cms.untracked.InputTag('cutsTPForFak'),
                                  tpEffSrc = cms.untracked.InputTag('cutsTPForEff')
                                 )


# fill tree
process.hitrkEffAnalyzer_akpu3pf.fillNtuples = cms.bool(True)
process.hitrkEffAnalyzer_akpu3pf.trkPtMin = cms.double(5)
process.hitrkEffAnalyzer_akpu3pf.tracks = cms.untracked.InputTag("hiMergedTracks")

# use calo jet
process.hitrkEffAnalyzer_akpu3pf.jets = cms.untracked.InputTag("icPu5patJets")

process.hiMergedTracks.TrackProducer1 = cms.string("hiGlobalPrimTracks")
process.hiMergedTracks.TrackProducer2 = cms.string("hiNewGlobalTracks")

#process.hiPostGlobalPrimTracks.src = cms.InputTag("hiMergedTracks")
process.hiPostGlobalPrimTracks.src = cms.InputTag("hiMergedTracks")
process.firstStepFilter.recTracks = cms.InputTag("hiGoodTightFirstStepTracks")
#process.firstStepFilter.recTracks = cms.InputTag("hiGlobalPrimTracks")
# use 

process.reco_extra = cms.Path( process.hiGen * process.hiTrackReReco * process.hiPostGlobalFirstStepTracks * process.hiGoodTightFirstStepTracks * process.secondStep * process.hiextraTrackReco * process.HiParticleFlowRecoNoJets)
process.reco_extra_jet = cms.Path( process.iterativeConePu5CaloJets )
process.pat_step = cms.Path(process.icPu5patSequence)
process.ana_step = cms.Path(process.hitrkEffAna_akpu3pf * process.icPu5JetAnalyzer * process.anaTrack)

# Customization
from Saved.JeAna11.customise_cfi import *
#enableDataPat(process)
#enableDataAnalyzers(process)
#enableOpenHlt(process,process.ana_step)

# =============== Final Schedule =====================
process.schedule = cms.Schedule(process.reco_extra,process.reco_extra_jet,process.pat_step,process.ana_step)

