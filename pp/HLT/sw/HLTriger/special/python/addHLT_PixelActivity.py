# Pixel activity trigger from Krisztian
def addHLT_PixelActivity(process):
  # copy HLT_PixelActivity path
  processPix = cms.Process('Pix')
  processPix.load("HLTrigger.special.HLT_MinBiasPixel_cff")
  process.hltSiPixelDigis = processPix.hltSiPixelDigis
  process.hltSiPixelClusters = processPix.hltSiPixelClusters
  process.hltPixelActivityFilter = processPix.hltPixelActivityFilter
  # remove l1 filters
  processPix.HLT_PixelActivity.remove(processPix.hltPreMinBiasPixel)
  processPix.HLT_PixelActivity.remove(processPix.hltL1sMinBiasPixel)
  process.HLT_PixelActivity = processPix.HLT_PixelActivity
  # reset HLTriggerFinalPath
  HLTriggerFinalPath = process.HLTriggerFinalPath
  del process.HLTriggerFinalPath
  process.HLTriggerFinalPath = HLTriggerFinalPath
  return(process)
addHLT_PixelActivity(process)
