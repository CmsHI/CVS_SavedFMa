import FWCore.ParameterSet.Config as cms

process = cms.Process('HIJETS')

process.Timing = cms.Service("Timing")

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    #'/store/relval/CMSSW_3_10_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START310_V3-v1/0050/54ED7C66-680E-E011-BD9F-001A92810ADE.root'
    #'/store/relval/CMSSW_3_9_9/RelValHydjetQ_B0_2760GeV/GEN-SIM-RECO/START39_V7HI-v1/0001/0618A00D-E23D-E011-A7FE-001A9281173E.root'                                                              
    '/store/user/davidlw/Hydjet_Bass_MinBias_2760GeV/Pyquen_UnquenchedDiJet_Pt50_GEN-SIM-RECO_393_v2/1243c1b8707a4e7eb28eae64e1474920/hiReco_RAW2DIGI_RECO_9_1_ZVb.root'
    )
                            )

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(-1)
    )

#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START39_V9::All'

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

process.load('FWCore.MessageService.MessageLogger_cfi')

# load centrality

process.load("CommonTools.UtilAlgos.TFileService_cfi")

process.HeavyIonGlobalParameters = cms.PSet(
           centralityVariable = cms.string("HFhits"),
           nonDefaultGlauberModel = cms.string("Hydjet_2760GeV"),
           centralitySrc = cms.InputTag("hiCentrality")
                     )

process.makeCentralityTableTFile = cms.EDAnalyzer('CentralityTableProducer',
                                                  isMC = cms.untracked.bool(True),
                                                  makeDBFromTFile = cms.untracked.bool(False),
                                                  makeTFileFromDB = cms.untracked.bool(True)
                                                  )


#  --- Track and muon reconstruction ---
# pixel triplet tracking (HI Tracking)
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
process.load("RecoHI.Configuration.Reconstruction_HI_cff")
process.load("RecoHI.HiTracking.LowPtTracking_PbPb_cff")
# Needed to produce "HcalSeverityLevelComputerRcd" used by CaloTowersCreator/towerMakerPF
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
# keep all the tracks for muon reco, then set high purity flag
process.hiTracksWithLooseQuality.keepAllTracks = True
process.hiTracksWithTightQuality.keepAllTracks = True
process.hiTracksWithTightQuality.qualityBit = 'loose'
process.hiSelectedTracks.keepAllTracks = True
process.hiSelectedTracks.qualityBit = 'tight'

# Muon Reco
from RecoHI.HiMuonAlgos.HiRecoMuon_cff import * 
process.globalMuons.TrackerCollectionLabel = cms.InputTag("hiGoodTracks")
muons.JetExtractorPSet.JetCollectionLabel = cms.InputTag("iterativeConePu5CaloJets")
#muons.JetExtractorPSet.JetCollectionLabel = cms.InputTag("iterativeCone5CaloJets")
muons.TrackExtractorPSet.inputTrackCollection = cms.InputTag("hiGoodTracks")
muons.inputCollectionLabels = cms.VInputTag("hiGoodTracks", "globalMuons", "standAloneMuons:UpdatedAtVtx")

#Track Reco
process.rechits = cms.Sequence(process.siPixelRecHits * process.siStripMatchedRecHits)
process.hiTrackReco = cms.Sequence(process.rechits * process.heavyIonTracking)

# good track selection
process.load("edwenger.HiTrkEffAnalyzer.TrackSelections_cff")
# merge with pixel tracks
#process.load('Appeltel.PixelTracksRun2010.HiLowPtPixelTracksFromReco_cff')
#process.load('Appeltel.PixelTracksRun2010.HiMultipleMergedTracks_cff')

# for PF
process.load("RecoHI.Configuration.Reconstruction_hiPF_cff")


# particle-flow stuff
process.HiParticleFlowRecoNoJets = cms.Sequence(
    process.particleFlowCluster
    * process.trackerDrivenElectronSeeds
    * process.particleFlowReco
    )
process.trackerDrivenElectronSeeds.TkColList = cms.VInputTag("hiGoodTracks")

#for tree output
process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string("JetAnalysisTTrees_hiGoodTracks_v1.root"))

# ----- jet reco extra and analyzers -----
process.load("Saved.QM11Ana.ExtraReco_cff")
process.load("Saved.QM11Ana.Analyzers_cff")


# =============== Final Paths =====================
process.extraReco = cms.Path(
    process.makeCentralityTableTFile*
    process.hiTrackReco*
    process.hiGoodTracksSelection*
    #process.conformalPixelTrackReco *
    #process.hiGoodMergedTracks *
    muonRecoPbPb*
    process.HiParticleFlowRecoNoJets *
    process.hiGen *
    process.runAllJets
    )

process.ana_step = cms.Path( process.jetana_seq * process.trkana_seq )

process.schedule = cms.Schedule(process.extraReco,process.ana_step)
