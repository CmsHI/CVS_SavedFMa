import FWCore.ParameterSet.Config as cms

process = cms.Process('HIJETS')

#process.Timing = cms.Service("Timing")

process.SimpleMemoryCheck=cms.Service("SimpleMemoryCheck",
                                      ignoreTotal=cms.untracked.int32(1),
                                      oncePerEventMode=cms.untracked.bool(True))

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
process.GlobalTag.globaltag = 'START39_V7::All'

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

process.load('FWCore.MessageService.MessageLogger_cfi')

# load centrality
from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)
process.HeavyIonGlobalParameters = cms.PSet(
           centralityVariable = cms.string("HFhits"),
           nonDefaultGlauberModel = cms.string("Hydjet_Bass"),
           centralitySrc = cms.InputTag("hiCentrality")
                     )

#for tree output
process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string("JetAnalysisTTrees_hiGoodTracks_v1.root"))

# =============== Extra Sequences ==================
process.load("Saved.QM11Ana.ExtraReco_cff")
process.load("Saved.QM11Ana.Analyzers_cff")

# =============== customization ==================
from Saved.QM11Ana.customise_cfi import *
# custom extra reco
#usehiGoodMergedTracks(process)
usehiHighPtTracks(process)

# =============== Final Paths =====================
#process.extraHiReco = cms.Path( process.makeCentralityTableTFile )
process.extraTrkReco = cms.Path( process.hiTrackReReco * process.hiextraTrackReco )
process.extraPfReco = cms.Path( process.HiParticleFlowRecoNoJets )
process.extraJetReco = cms.Path( process.hiGen * process.runAllJets )
#process.ana_step = cms.Path( process.trkcorr_seq * process.trkval_seq * process.trkana_seq * process.jetana_seq )
process.ana_step = cms.Path( process.trkcorr_seq * process.trkval_seq * process.akPu3PFJetAnalyzer)

process.schedule = cms.Schedule(process.extraTrkReco,process.extraPfReco,process.extraJetReco,process.ana_step)

# =============== Output ================================
#process.output = cms.OutputModule("PoolOutputModule",
#    #process.analysisSkimContent,
#    fileName = cms.untracked.string('jetAnaSkim.root')
#    )
#process.output_step = cms.EndPath(process.output)
#process.schedule.extend([process.output_step])
