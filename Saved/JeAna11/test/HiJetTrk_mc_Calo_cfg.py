import FWCore.ParameterSet.Config as cms

process = cms.Process('JetAna')

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

isEmbedded = False

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_3_9_9/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RECO/START39_V7HI-v1/0000/40B4255D-5A3D-E011-9D80-001BFCDBD100.root'
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
                                  fileName=cms.string("JetAnaTrees_mc.root"))

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
#process.load('Saved.JeAna11.ExtraTrackReco_cff')
#process.load('Saved.JeAna11.ExtraPfReco_cff')
process.load('Saved.JeAna11.ExtraJetReco_cff')
process.load('Saved.JeAna11.PatAna_cff')
process.load('Saved.JeAna11.JetAnalyzers_cff')



if isEmbedded:
    process.hiGenParticles.srcVector = cms.vstring('hiSignal')
    process.icPu5JetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")
else:
    process.hiGenParticles.srcVector = cms.vstring('generator')
    process.icPu5JetAnalyzer.eventInfoTag = cms.InputTag("generator")



process.reco_extra = cms.Path( process.hiGen )
process.reco_extra_jet = cms.Path( process.iterativeConePu5CaloJets )
process.pat_step = cms.Path(process.icPu5patSequence )
process.ana_step = cms.Path(process.icPu5JetAnalyzer )

# Customization
from Saved.JeAna11.customise_cfi import *
#enableDataPat(process)
#enableDataAnalyzers(process)
enableOpenHlt(process,process.ana_step)

# =============== Final Schedule =====================
process.schedule = cms.Schedule(process.reco_extra,process.reco_extra_jet,process.pat_step,process.ana_step)
