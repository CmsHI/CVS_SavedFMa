# Auto generated configuration file
# using: 
# Revision: 1.120 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: reco -s RAW2DIGI,RECO --conditions FrontierConditions_GlobalTag,IDEAL_31X::All --eventcontent RECOSIM
import FWCore.ParameterSet.Config as cms

process = cms.Process('SKIM')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('reco nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)

process.load("MyConfiguration.Sequences.python.Local_source_cff")

# options
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True)
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string('raw320_MC_31X_V3.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    )
)
process.output.outputCommands = ['drop *','keep FEDRawDataCollection_rawDataCollector_*_*']

# Additional output definition

# Path and EndPath definitions
process.out_step = cms.EndPath(process.output)

