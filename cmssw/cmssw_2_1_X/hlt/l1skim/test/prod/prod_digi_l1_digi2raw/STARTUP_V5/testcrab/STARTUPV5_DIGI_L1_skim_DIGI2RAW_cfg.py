# Auto generated configuration file
# using: 
# Revision: 1.77 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/PYTHIA6_MinBias_10TeV_cff.py --filein=file:/net/pstore01/d00/scratch/frankma/data/cmssw/217/sim_skim/minbiasSummer08_IDEAL_V9_v1_SIM.root -s DIGI,L1 --datatier=DIGI --conditions=FrontierConditions_GlobalTag,IDEAL_V9::All -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('L1SKIM')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/Digi_cff')
process.load('Configuration/StandardSequences/SimL1Emulator_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

# Message Logger
process.load('FWCore/MessageService/MessageLogger_cfi')

# L1 configuration
#process.load('L1TriggerConfig/L1GtConfigProducers/Luminosity/lumi1030.L1Menu2008_2E30_Unprescaled_cff')
#process.load('L1TriggerConfig/L1GtConfigProducers/Luminosity/lumi1030.myL1Menu2008_2E30_PrescaleFactorsAlgoTrig_cff')
process.load('L1TriggerConfig/L1GtConfigProducers/Luminosity/lumi1030.L1MenuTsg21X_3E30_PrescaleFactorsAlgoTrig_cff')

# L1Extra needed by l1 skim filter
process.load('L1Trigger/L1ExtraFromDigis/l1extraParticles_cfi')
process.l1extraParticles.muonSource = cms.InputTag("simGmtDigis")
process.l1extraParticles.etTotalSource = cms.InputTag("simGctDigis")
process.l1extraParticles.nonIsolatedEmSource = cms.InputTag("simGctDigis","nonIsoEm")
process.l1extraParticles.etMissSource = cms.InputTag("simGctDigis")
process.l1extraParticles.forwardJetSource = cms.InputTag("simGctDigis","forJets")
process.l1extraParticles.centralJetSource = cms.InputTag("simGctDigis","cenJets")
process.l1extraParticles.tauJetSource = cms.InputTag("simGctDigis","tauJets")
process.l1extraParticles.isolatedEmSource = cms.InputTag("simGctDigis","isoEm")
process.l1extraParticles.etHadSource = cms.InputTag("simGctDigis")

# L1 skim using HLTLevell1GTSeed filter
import HLTrigger.HLTfilters.hltLevel1GTSeed_cfi as l1GtSeedFilter
process.l1filter = l1GtSeedFilter.hltLevel1GTSeed
process.l1filter.L1GtReadoutRecordTag = cms.InputTag("simGtDigis")
process.l1filter.L1GtObjectMapTag = cms.InputTag("simGtDigis")
process.l1filter.L1SeedsLogicalExpression = \
   'L1_SingleJet15 OR L1_SingleJet30 OR L1_SingleJet50 OR L1_SingleJet70 OR L1_IsoEG10_Jet15_ForJet10 OR L1_SingleJet150  OR L1_DoubleJet70 OR L1_TripleJet50 OR L1_QuadJet15 OR L1_QuadJet30 OR L1_ETT60 OR L1_ETM20 OR L1_ETM30 OR L1_ETM40 OR L1_ETM50 OR L1_HTT300 OR L1_SingleIsoEG12 OR L1_SingleEG15 OR L1_SingleEG12 OR L1_SingleEG8 OR L1_SingleEG10 OR L1_DoubleIsoEG8 OR L1_DoubleEG10 OR L1_DoubleEG5 OR L1_ExclusiveDoubleIsoEG6 OR L1_SingleMu7 OR L1_DoubleMu3 OR L1_SingleMuOpen  OR L1_SingleMu3 OR L1_SingleMu5 OR L1_SingleMu10 OR L1_DoubleJet100  OR L1_SingleJet100  OR L1_Mu5_Jet15 OR L1_HTT200 OR L1_SingleTauJet80 OR L1_TauJet30_ETM30 OR L1_DoubleTauJet40 OR L1_DoubleTauJet20 OR L1_Mu3_IsoEG5 OR L1_Mu3_EG12 OR L1_IsoEG10_TauJet20 OR L1_IsoEG10_Jet20 OR L1_IsoEG10_Jet30 OR L1_EG5_TripleJet15 OR L1_Mu5_TauJet20 OR L1_Mu5_Jet15 OR L1_Mu3_TripleJet15 OR L1_ZeroBias OR L1_MinBias_HTT10 OR L1_SingleJetCountsHFTow  OR L1_DoubleJetCountsHFTow  OR L1_SingleJetCountsHFRing0Sum3 OR L1_DoubleJetCountsHFRing0Sum3  OR L1_SingleJetCountsHFRing0Sum6  OR L1_DoubleJetCountsHFRing0Sum6 OR L1_SingleEG2  OR L1_DoubleEG1 OR L1_SingleMuBeamHalo OR L1_SingleTauJet30  OR L1_SingleTauJet40  OR L1_SingleTauJet60 OR L1_SingleIsoEG5  OR L1_SingleIsoEG8  OR L1_SingleIsoEG10 OR L1_SingleIsoEG15  OR L1_SingleIsoEG20  OR L1_SingleIsoEG25  OR L1_SingleEG5  OR L1_SingleEG20  OR L1_SingleEG25'

# Digi to Raw
process.load('Configuration/StandardSequences/DigiToRaw_cff')
process.rawDataCollector.currentProcessOnly = True

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    annotation = cms.untracked.string('Configuration/GenProduction/python/PYTHIA6_MinBias_10TeV_cff.py nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True)
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       #'file:/net/pstore01/d00/scratch/frankma/data/cmssw/217/sim_skim/minbiasSummer08_IDEAL_V9_v1_SIM.root'
       #'dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/mc/Summer08/MinBias/GEN-SIM-RAW/IDEAL_V9_v1/0029/00C2D656-7282-DD11-AC51-001C23C0F1F9.root'
       'file:/export/04a/frankma/data/217/basic_extractor/minbiasSummer08_STARTUP_V5.root'
    )
)
#print process.source.fileNames
#import hlt.l1skim.STARTUP_V5_files as STARTUP_V5_files
#process.source.fileNames = STARTUP_V5_files.fileNames
#print process.source.fileNames[:2]

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    #outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    outputCommands = cms.untracked.vstring( 'keep *'),
    fileName = cms.untracked.string('Summer08_MinBias_STARTUPV5_L1skimmed_Raw.root'),
    # Select on the events that passed path L1skim_digi2raw_step
    SelectEvents = cms.untracked.PSet(
       SelectEvents = cms.vstring(
	  "L1skim_digi2raw_step"
	  )
    ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW'),
        filterName = cms.untracked.string('')
    )
)
process.output.outputCommands = [
   'drop *',
   'keep *_rawDataCollector_*_L1SKIM'
]
print "process.output.outputCommands: ", process.output.outputCommands

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'STARTUP_V5::All'

# L1 Gt trigger report
process.load("L1Trigger.GlobalTriggerAnalyzer.l1GtTrigReport_cfi")
process.l1GtTrigReport.L1GtRecordInputTag = "simGtDigis"

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.L1skim_digi2raw_step = cms.Path(process.l1extraParticles*process.l1filter*process.DigiToRaw)
process.report = cms.EndPath(process.l1GtTrigReport)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.L1skim_digi2raw_step,process.report,process.out_step)
