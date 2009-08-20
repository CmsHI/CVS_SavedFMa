# dataset: /RelValHydjetQ_MinBias_4TeV/CMSSW_3_2_4-MC_31X_V3-v1/GEN-SIM-RAW
def custom_TrackerVR(process):
   process.setName_("DIGI2RAWTrackerVR")

   # Tracker digis: no ZS
   process.simSiStripDigis.ZeroSuppression = cms.bool(False)
   process.simSiStripDigis.Noise = cms.bool(True)
   print "=== New simSiStripDigis cfg: ==="
   print process.simSiStripDigis.dumpPython()

   # Tracker digi2raw with VR
   process.newSiStripDigiToRaw = cms.EDProducer(
      "SiStripDigiToRawModule",
      InputModuleLabel = cms.string('simSiStripDigis'),
      InputDigiLabel = cms.string('VirginRaw'),
      FedReadoutMode = cms.untracked.string('VIRGIN_RAW'),
      UseFedKey = cms.untracked.bool(False)
      ) 

   # change DIGI and DIGI2RAW paths
   print "=== Original digi2raw_step path: ==="
   print process.digi2raw_step
   print
   print "=== Will replace process.SiStripDigiToRaw: ==="
   print process.SiStripDigiToRaw.dumpPython()
   print
   print "=== With process.newSiStripDigiToRaw: ==="
   print process.newSiStripDigiToRaw.dumpPython()
   process.digi2raw_step._replace(process.SiStripDigiToRaw,process.newSiStripDigiToRaw)

   # turn of compression in OM
   process.output = cms.OutputModule("PoolOutputModule",
      splitLevel = cms.untracked.int32(0),
      outputCommands = process.RAWSIMEventContent.outputCommands,
      fileName = cms.untracked.string('raw_DIGI_L1_DIGI2RAW.root'),
      dataset = cms.untracked.PSet(
	 dataTier = cms.untracked.string('DIGI-RAW'),
	 filterName = cms.untracked.string('')
	 ),
      #use_compression = cms.untracked.bool(False)
      compression_level = cms.untracked.int32( -1 )
   )
   return(process)

custom_TrackerVR(process)
