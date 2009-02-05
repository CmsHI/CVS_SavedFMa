import FWCore.ParameterSet.Config as cms

import hlt.tsg.myp5_cmsRun_online_timingHLTV6 as myhlt
process = myhlt.process

process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32( 1000 ) )

# Bryan's timer
process.PathTimerService = cms.Service( "PathTimerService" )
process.timer = cms.EDProducer( "PathTimerInserter" )


#print process.hltDefaultOutput.fileName
process.hltDefaultOutput.fileName = "file:/tmp/HLTTimerOutput.root"
#process.hltDefaultOutput.outputCommands.append('keep HLTPerformanceInfo_*_*_*')
process.hltDefaultOutput.outputCommands = ['keep HLTPerformanceInfo_*_*_*']
print process.hltDefaultOutput.outputCommands
del process.hltDefaultOutput.SelectEvents

process.HLTAnalyzerEndpath = cms.EndPath( process.timer + process.hltL1gtTrigReport + process.hltTrigReport )

