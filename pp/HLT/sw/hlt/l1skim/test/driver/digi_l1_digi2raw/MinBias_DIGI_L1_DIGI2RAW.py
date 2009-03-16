# Auto generated configuration file
# using: 
# Revision: 1.99.2.8 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: --filein=file:/home/frankma/data/pp/Summer08/MinBias/GEN-SIM-RAW/STARTUP_V5_STARTUP_V5_v1/00E549D0-826C-DD11-BBBF-001C23C105E3.root -s DIGI,L1,DIGI2RAW --datatier=RAW --conditions=FrontierConditions_GlobalTag,STARTUP_V9::All --eventcontent=FEVTDEBUG --writeraw -n 100 --no_exec --mc
import FWCore.ParameterSet.Config as cms

process = cms.Process('DIGI2RAW')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/Digi_cff')
process.load('L1Trigger/Configuration/SimL1Emulator_cff')
process.load('L1Trigger/Configuration/L1StartupConfig_cff')
process.load('L1TriggerConfig/L1GtConfigProducers/Luminosity/startup/L1Menu_Commissioning2009_v0_L1T_Scales_20080926_startup_Imp0_Unprescaled_cff')
process.load('Configuration/StandardSequences/DigiToRaw_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.99.2.8 $'),
    annotation = cms.untracked.string('--filein=file:/home/frankma/data/pp/Summer08/MinBias/GEN-SIM-RAW/STARTUP_V5_STARTUP_V5_v1/00E549D0-826C-DD11-BBBF-001C23C105E3.root nevts:100'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/home/frankma/data/pp/Summer08/MinBias/GEN-SIM-RAW/STARTUP_V5_STARTUP_V5_v1/00E549D0-826C-DD11-BBBF-001C23C105E3.root')
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('00E549D0-826C-DD11-BBBF-001C23C105E3_root_DIGI_L1_DIGI2RAW.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW'),
        filterName = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'STARTUP_V9::All'

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.endjob_step,process.out_step)
