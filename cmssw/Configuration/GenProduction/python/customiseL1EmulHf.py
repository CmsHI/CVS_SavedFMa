process.load("SimCalorimetry.HcalTrigPrimProducers.hcalTTPDigis_cff")
process.simGtDigis.TechnicalTriggersInputTags.extend([cms.InputTag('simHcalTTPRecord')])
process.SimL1TechnicalTriggers += process.hcalTTPSequence
