# add Bryan's timer
def run_hlt_timer(process):
   process.PathTimerService = cms.Service( "PathTimerService" )
   process.timer = cms.EDProducer( "PathTimerInserter" )
   process.output.outputCommands = ['keep HLTPerformanceInfo_*_*_*']
   #del process.output.SelectEvents
   process.HLTAnalyzerEndpath = cms.EndPath( process.timer + process.hltL1GtTrigReport + process.hltTrigReport )
   return(process)

process=run_hlt_timer(process)
