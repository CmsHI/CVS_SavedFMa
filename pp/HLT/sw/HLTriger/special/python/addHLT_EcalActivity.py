
# Ecal Activity Trigger from Christof
def addHLT_EcalActivity(process):
  from HLTrigger.special.OnLine_HLT_TestEcal import process as processEcal
  process.hltTowerMakerForEcal = processEcal.hltTowerMakerForEcal
  process.hltHI1jet35U = processEcal.hltHI1jet35U
  process.HLT_EcalActivity1 = processEcal.HLT_EcalActivity1
  # reset HLTriggerFinalPath
  HLTriggerFinalPath = process.HLTriggerFinalPath
  del process.HLTriggerFinalPath
  process.HLTriggerFinalPath = HLTriggerFinalPath
  return(process)
addHLT_EcalActivity(process)
