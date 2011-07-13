import FWCore.ParameterSet.Config as cms

process = cms.Process('JetAna')

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    'file:/d101/frankma/data/HIAllPhysics/ZS-v2/A2AD1439-F242-E011-A425-003048F00412.root'
    ))

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(1000))

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
                                  fileName=cms.string("JetAnaTrees.root"))

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
process.load('CmsHi.JetAnalysis.ExtraJetReco_cff')
process.load('CmsHi.JetAnalysis.PatAna_cff')
process.load('CmsHi.JetAnalysis.JetAnalyzers_cff')

process.reco_extra_jet = cms.Path( process.iterativeConePu5CaloJets)
process.pat_step = cms.Path(process.icPu5patSequence_data )
process.ana_step = cms.Path(process.icPu5JetAnalyzer )

# Customization
from CmsHi.JetAnalysis.customise_cfi import *
enableDataPat(process)
enableDataAnalyzers(process)
enableOpenHlt(process,process.ana_step)

# release jet reco
print "Add jet from release"
process.icPu5corrRelVal = process.icPu5corr.clone(src = cms.InputTag("iterativeConePu5CaloJets","","RECO"))
process.icPu5patJetsRelVal = process.icPu5patJets.clone(
  jetSource = cms.InputTag("iterativeConePu5CaloJets","","RECO"),
  jetCorrFactorsSource = cms.VInputTag(cms.InputTag("icPu5corrRelVal"))
  )
process.icPu5patSequence_dataRelVal = cms.Sequence( process.icPu5corrRelVal * process.icPu5patJetsRelVal )
process.icPu5JetAnalyzerRelVal = process.icPu5JetAnalyzer.clone(jetTag = 'icPu5patJetsRelVal')
process.pat_step *= process.icPu5patSequence_dataRelVal
process.ana_step *= process.icPu5JetAnalyzerRelVal

# Cleaning
print "Add cleaning analysis"
process.event_filter_seq = cms.Sequence(
  #process.collisionEventSelection
  process.hiEcalRecHitSpikeFilter
  * process.hbheReflagNewTimeEnv
  * process.hcalTimingFilter
  * process.HBHENoiseFilter
  )
process.icPu5JetAnalyzer_clean = process.icPu5JetAnalyzer.clone()
process.hltanalysis_clean = process.hltanalysis.clone()

process.ana_step_wCleaning = cms.Path( process.event_filter_seq * process.icPu5JetAnalyzer_clean * process.hltanalysis_clean )

# =============== Final Schedule =====================
process.schedule = cms.Schedule(process.reco_extra_jet,process.pat_step,process.ana_step,process.ana_step_wCleaning)
