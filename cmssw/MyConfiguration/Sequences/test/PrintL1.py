import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
anaopt = VarParsing.VarParsing ('standard')
anaopt.register ('gbt',
    "MC_38Y_V9", # default value
    VarParsing.VarParsing.multiplicity.singleton, # singleton or list
    VarParsing.VarParsing.varType.string,          # string, int, or float
    "globaltag used for l1 menu")
anaopt.parseArguments()

process = cms.Process('L1Ana')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('HLTrigger.Configuration.HLT_1E31_cff')
process.digi2raw = cms.Path(process.HLTL1UnpackerSequence)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('RAW.root')
)

# Other statements
process.GlobalTag.globaltag = anaopt.gbt

# Path and EndPath definitions

# Schedule definition
process.schedule = cms.Schedule()
process.schedule.append(process.digi2raw)

# Automatic addition of the customisation function

def customise(process):

    process.options.wantSummary = cms.untracked.bool(True)

    process.load('L1Trigger.GlobalTriggerAnalyzer.l1GtTrigReport_cfi')
    process.l1GtTrigReport.L1GtRecordInputTag = cms.InputTag( "hltGtDigis" )

    process.L1AnalyzerEndpath = cms.EndPath( process.l1GtTrigReport )
    process.schedule.append(process.L1AnalyzerEndpath)

    process.MessageLogger.categories.append('L1GtTrigReport')

    return(process)


# End of customisation function definition

process = customise(process)
