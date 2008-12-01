# Auto generated configuration file
# using: 
# Revision: 1.77 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/PYTHIA6_MinBias_10TeV_cff.py --filein=file:/net/pstore01/d00/scratch/frankma/data/cmssw/217/basic_extractor/minbiasSummer08_IDEAL_V9_v1_1kSIMDIGIRAW.root -s L1 --datatier=RAW --conditions=FrontierConditions_GlobalTag,IDEAL_V9::All -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('L1')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.debugModules = ['hltLevel1GTSeed']
process.MessageLogger.categories = ['*']
process.MessageLogger.destinations = ['cout']
#process.MessageLogger.cout.threshold = cms.untracked.string('DEBUG')
#process.MessageLogger.cerr.threshold = cms.untracked.string('DEBUG')
process.MessageLogger.cout = cms.untracked.PSet(
   threshold = cms.untracked.string('DEBUG'),
   DEBUG = cms.untracked.PSet(
      limit = cms.untracked.int32(-1)
   )
)


process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/SimL1Emulator_cff')
process.load('L1TriggerConfig/L1GtConfigProducers/Luminosity/lumi1030.L1Menu2008_2E30_Unprescaled_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

# import modules for L1-skim
process.load('HLTrigger.HLTfilters.hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('L1_DoubleEG1')
process.hltLevel1GTSeed.L1GtReadoutRecordTag = cms.InputTag("hltGtDigis")
process.hltLevel1GTSeed.L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap")
process.hltLevel1GTSeed.L1MuonCollectionTag = cms.InputTag("hltL1extraParticles")
process.hltLevel1GTSeed.L1CollectionsTag = cms.InputTag("hltL1extraParticles")


process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.77 $'),
    annotation = cms.untracked.string('Configuration/GenProduction/PYTHIA6_MinBias_10TeV_cff.py nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool( True )
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/net/pstore01/d00/scratch/frankma/data/cmssw/217/basic_extractor/minbiasSummer08_IDEAL_V9_v1_1kSIMDIGIRAW.root')
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('PYTHIA6_MinBias_10TeV_cff_py_L1.root'),
    SelectEvents = cms.untracked.PSet(
       SelectEvents = cms.vstring('L1skim')
    ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW'),
        filterName = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'IDEAL_V9::All'

# Path and EndPath definitions
#process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.L1skim = cms.Path(process.hltLevel1GTSeed)
process.out_step = cms.EndPath(process.output)

# Schedule definition
#process.schedule = cms.Schedule(process.L1simulation_step,process.out_step)
process.schedule = cms.Schedule(process.L1skim,process.out_step)
