
# Hf Activity Trigger from Bryan
def customHLT_HfActivity(process):
  process.HLTHcalSimpleRecHitFilter.maskedChannels = cms.vint32(
    8137, 8141, 8146, 8149, 8150, 8153
  )
  return(process)
customHLT_HfActivity(process)
