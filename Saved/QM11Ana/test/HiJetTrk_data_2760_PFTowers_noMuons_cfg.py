import FWCore.ParameterSet.Config as cms

process = cms.Process('HIJETS')

#process.Timing = cms.Service("Timing")

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    '/store/hidata/HIRun2010/HIAllPhysics/RECO/SDmaker_3SD_1CS_PDHIAllPhysicsZSv2_SD_JetHI-v1/0055/A2915765-B04F-E011-BB14-003048FEADDC.root'
    )
                            )

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(10)
    )

# Lumi Ranges from json
import PhysicsTools.PythonAnalysis.LumiList as LumiList
import FWCore.ParameterSet.Types as CfgTypes
myLumis = LumiList.LumiList(filename = 'json.txt').getCMSSWString().split(',')
process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
process.source.lumisToProcess.extend(myLumis)

#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_39X_V6B::All'

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

process.load('FWCore.MessageService.MessageLogger_cfi')

# load centrality
process.HeavyIonGlobalParameters = cms.PSet(
    centralitySrc = cms.InputTag("hiCentrality"),
    centralityVariable = cms.string("HFhits"),
    nonDefaultGlauberModel = cms.string("")
)
from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)

#  --- Track and muon reconstruction ---
# pixel triplet tracking (HI Tracking)
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
process.load("RecoHI.Configuration.Reconstruction_HI_cff")
process.load("RecoHI.HiTracking.LowPtTracking_PbPb_cff")
# Needed to produce "HcalSeverityLevelComputerRcd" used by CaloTowersCreator/towerMakerPF
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
#Track Reco
process.rechits = cms.Sequence(process.siPixelRecHits * process.siStripMatchedRecHits)
process.hiTrackReco = cms.Sequence(process.rechits * process.heavyIonTracking)

# for PF
process.load("RecoHI.Configuration.Reconstruction_hiPF_cff")
process.HiParticleFlowRecoNoJets = cms.Sequence(
    process.particleFlowCluster
    * process.trackerDrivenElectronSeeds
    * process.particleFlowReco
    )
process.trackerDrivenElectronSeeds.TkColList = cms.VInputTag("hiGoodTracks")

#for tree output
process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string("JetAnalysisTTrees_data.root"))

# ----- jet reco extra and analyzers -----
process.load("Saved.QM11Ana.ExtraReco_cff")
process.load("Saved.QM11Ana.Analyzers_cff")
process.load("MNguyen.Configuration.HI_JetSkim_cff")

# =============== customization ==================
process.hltJetHI.HLTPaths = ["HLT_HIJet35U"]
from Saved.QM11Ana.customise_cfi import *
enableData(process)
# custom extra reco
#usehiGoodMergedTracks(process)

# =============== Final Paths =====================
process.extraTrkReco = cms.Path( process.jetSkimSequence * process.hiTrackReco * process.hiextraTrackReco )
process.extraPfReco = cms.Path( process.jetSkimSequence * process.HiParticleFlowRecoNoJets )
process.extraJetReco = cms.Path( process.jetSkimSequence * process.runAllJets_data )
process.ana_step = cms.Path( process.jetSkimSequence * process.trkana_seq_data * process.jetana_seq_data )

process.schedule = cms.Schedule(process.extraTrkReco,process.extraPfReco,process.extraJetReco,process.ana_step)
