import FWCore.ParameterSet.Config as cms

process = cms.Process('JetAna')

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    #'file:/d101/frankma/ana/qm11jet/hiReco_RAW2DIGI_RECO_9_1_QNo.root'
    'file:/d101/frankma/mc/413/RelValHydjetQ_MinBias_2760GeV.root'
    ))

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(10))

#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
#process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.GlobalTag.globaltag = 'START311_V2A::All'

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
                                  fileName=cms.string("JetAnaTrees_mc.root"))

# Define Jet energy correction
process.jec = cms.ESSource("PoolDBESSource",
                           DBParameters = cms.PSet(
    messageLevel = cms.untracked.int32(0)
    ),
                           timetype = cms.string('runnumber'),
                           toGet = cms.VPSet(
    cms.PSet(record = cms.string("JetCorrectionsRecord"),
             tag = cms.string("JetCorrectorParametersCollection_HIReco_413patch2_IC5Calo"),
             label = cms.untracked.string("IC5Calo")
             ),
    cms.PSet(record = cms.string("JetCorrectionsRecord"),
             tag = cms.string("JetCorrectorParametersCollection_HIReco_413patch2_PFTower_AK3PF"),
             label = cms.untracked.string("AK3PF")
             ),
    cms.PSet(record = cms.string("JetCorrectionsRecord"),
             tag = cms.string("JetCorrectorParametersCollection_HIReco_413patch2_PFTower_AK4PF"),
             label = cms.untracked.string("AK4PF")
             ),
    cms.PSet(record = cms.string("JetCorrectionsRecord"),
             tag = cms.string("JetCorrectorParametersCollection_HIReco_413patch2_PFTower_AK5PF"),
             label = cms.untracked.string("AK5PF")
             ),
    ),
                           connect = cms.string("sqlite_file:JEC_HIReco_PFTower_413patch2_2011_v2.db"),
                           )

process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

# Define Analysis sequencues
process.load('Saved.JeAna11.ExtraGenReco_cff')
process.load('Saved.JeAna11.ExtraTrackReco_cff')
process.load('Saved.JeAna11.ExtraPfReco_cff')
process.load('Saved.JeAna11.ExtraJetReco_cff')
process.load('Saved.JeAna11.PatAna_cff')
process.load('Saved.JeAna11.JetAnalyzers_cff')

#process.reco_extra = cms.Path( process.hiGen * process.hiTrackReReco * process.hiextraTrackReco * process.HiParticleFlowRecoNoJets)
process.reco_extra = cms.Path( process.hiGen * process.hiextraTrackReco * process.HiParticleFlowRecoNoJets)
process.reco_extra_jet = cms.Path( process.iterativeConePu5CaloJets * process.akPu3PFJets )
process.pat_step = cms.Path(process.icPu5patSequence * process.akPu3PFpatSequence)
process.ana_step = cms.Path(process.icPu5JetAnalyzer * process.akPu3PFJetAnalyzer)

# Customization
from Saved.JeAna11.customise_cfi import *
#enableDataPat(process)
enableDataAnalyzers(process)
#enableOpenHlt(process,process.ana_step)

# =============== Final Schedule =====================
process.schedule = cms.Schedule(process.reco_extra,process.reco_extra_jet,process.pat_step,process.ana_step)
