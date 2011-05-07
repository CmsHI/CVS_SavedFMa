# Auto generated configuration file
# using: 
# Revision: 1.77 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/PYTHIA6_MinBias_10TeV_cff.py --filein=file:/net/pstore01/d00/scratch/frankma/data/cmssw/217/sim_skim/minbiasSummer08_IDEAL_V9_v1_SIM.root -s DIGI,L1 --datatier=DIGI --conditions=FrontierConditions_GlobalTag,IDEAL_V9::All -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('L1')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/Digi_cff')
process.load('Configuration/StandardSequences/SimL1Emulator_cff')
process.load('L1TriggerConfig/L1GtConfigProducers/Luminosity/lumi1030.L1Menu2008_2E30_Unprescaled_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.77 $'),
    annotation = cms.untracked.string('Configuration/GenProduction/python/PYTHIA6_MinBias_10TeV_cff.py nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True)
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/net/pstore01/d00/scratch/frankma/data/cmssw/217/sim_skim/minbiasSummer08_IDEAL_V9_v1_SIM.root')
)
#process.source.fileNames = ["file:testL1HLTAllPath.root"]
process.source.fileNames = ["rfio:/castor/cern.ch/user/f/frankma/data/cmssw/217/driver/genhlt/PYTHIA6_MinBias_10TeV_cff_GEN_SIM_DIGI_L1_DIGI2RAW_HLT.root"]

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('PYTHIA6_MinBias_10TeV_cff_py_DIGI_L1.root'),
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
process.l1GtTrigReport.L1GtRecordInputTag = cms.InputTag('simGtDigis','','L1')

# Restore Random seed
if hasattr(process, "RandomNumberGeneratorService"):
    del process.RandomNumberGeneratorService.theSource
else:
    process.load("IOMC/RandomEngine/IOMC_cff")
    del process.RandomNumberGeneratorService.theSource

process.RandomNumberGeneratorService.restoreStateLabel = cms.untracked.string("randomEngineStateProducer")
process.mix.playback = True

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.out_step = cms.EndPath(process.l1GtTrigReport+process.output)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.out_step)