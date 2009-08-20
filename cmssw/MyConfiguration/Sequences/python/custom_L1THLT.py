#
# L1T and HLT reports
# - cf HLTrigger/Configuration/python
#
def custom_L1THLT(process):
   # define schedule
   # -- HLT specific schedule --
   #
   # Note: Mu path removed
   #
   process.HLTSchedule = cms.Schedule( process.HLTriggerFirstPath, process.HLT_HIJet50U, process.HLT_HIJet75U, process.HLT_HIJet90U, process.HLT_HIPhoton10, process.HLT_HIPhoton20, process.HLT_HIPhoton30, process.HLTriggerFinalPath)
   
   process.schedule = cms.Schedule()
   process.schedule.extend(process.HLTSchedule)

   # nice summaries
   process.options.wantSummary = cms.untracked.bool(True)

   # L1 reports
   from L1Trigger.GlobalTriggerAnalyzer.l1GtTrigReport_cfi import l1GtTrigReport
   process.hltL1GtTrigReport = l1GtTrigReport
   process.hltL1GtTrigReport.L1GtRecordInputTag = cms.InputTag( 'hltGtDigis' )

   process.MessageLogger.categories.append('L1GtTrigReport')

   # HLT reports
#   from HLTrigger.HLTanalyzers.hlTrigReport_cfi import hlTrigReport
#   process.hltTrigReport = hlTrigReport
#   process.hltTrigReport.HLTriggerResults = cms.InputTag( 'TriggerResults','',process.name_() )
   # Fix me: can't find hlt report ...
   process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
      HLTriggerResults = cms.InputTag( 'TriggerResults','',process.name_() )
   )
   process.MessageLogger.categories.append('TriggerSummaryProducerAOD')
   process.MessageLogger.categories.append('HLTrigReport')

   # define reporting paths
   process.HLTAnalyzerPath = cms.Path( process.hltL1GtTrigReport + process.hltTrigReport )
   process.schedule.extend([process.HLTAnalyzerPath,process.HLTOutput])
   return(process)

custom_L1THLT(process)
