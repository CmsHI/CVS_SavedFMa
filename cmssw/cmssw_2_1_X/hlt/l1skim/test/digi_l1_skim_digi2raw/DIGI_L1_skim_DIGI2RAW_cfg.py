# Auto generated configuration file
# using: 
# Revision: 1.77 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/PYTHIA6_MinBias_10TeV_cff.py --filein=file:/net/pstore01/d00/scratch/frankma/data/cmssw/217/sim_skim/minbiasSummer08_IDEAL_V9_v1_SIM.root -s DIGI,L1 --datatier=DIGI --conditions=FrontierConditions_GlobalTag,IDEAL_V9::All -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('L1')

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
process.load('L1TriggerConfig/L1GtConfigProducers/Luminosity/lumi1030.L1Menu2008_2E30_Unprescaled_cff')
#process.load('L1TriggerConfig/L1GtConfigProducers/Luminosity/lumi1030.L1Menu2008_2E30_PrescaleFactorsAlgoTrig_cff')

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
   'L1_DoubleEG1 OR L1_MinBias_HTT10'

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.77 $'),
    annotation = cms.untracked.string('Configuration/GenProduction/python/PYTHIA6_MinBias_10TeV_cff.py nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True)
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/net/pstore01/d00/scratch/frankma/data/cmssw/217/sim_skim/minbiasSummer08_IDEAL_V9_v1_SIM.root')
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('DIGI_L1_L1Extra_skim.root'),
    # Select on the events that passed path L1skim
    SelectEvents = cms.untracked.PSet(
       SelectEvents = cms.vstring(
	  "L1skim"
	  )
    ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('DIGI'),
        filterName = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'IDEAL_V9::All'

# L1 Gt trigger report
process.load("L1Trigger.GlobalTriggerAnalyzer.l1GtTrigReport_cfi")
process.l1GtTrigReport.L1GtRecordInputTag = "simGtDigis"

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.L1skim = cms.Path(process.l1extraParticles*process.l1filter)
process.report = cms.EndPath(process.l1GtTrigReport)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.L1skim,process.report,process.out_step)
