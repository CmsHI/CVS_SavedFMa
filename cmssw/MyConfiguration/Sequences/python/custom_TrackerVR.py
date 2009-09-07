#
# dataset: /RelValHydjetQ_MinBias_4TeV/CMSSW_3_2_4-MC_31X_V3-v1/GEN-SIM-RAW
#

def custom_TrackerVR(process):
   process.setName_("DIGI2RAWTrackerVR")

   # EventSetup
   # cf /d00/icali/CMSSW_3_1_0/src/cfgFiles/SiStrip/SiStrip_SimToRaw_VR_cfg.py
   process.poolDBESSource = cms.ESSource("PoolDBESSource",
      BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
      DBParameters = cms.PSet(
	 messageLevel = cms.untracked.int32(2),
	 authenticationPath = cms.untracked.string('/afs/cern.ch/cms/DB/conddb')
	 ),
      timetype = cms.untracked.string('runnumber'),
      connect = cms.string('sqlite_file:dbfile.db'),
      toGet = cms.VPSet(cms.PSet(
	 record = cms.string('SiStripNoisesRcd'),
	 tag = cms.string('SiStripNoise_GR09_Dec')
	 ),
	 cms.PSet(
	    record = cms.string('SiStripPedestalsRcd'),
	    tag = cms.string('SiStripPedestal_GR09_Dec')
	    ),
	 cms.PSet(
	    record = cms.string('SiStripFedCablingRcd'),
	    tag = cms.string('SiStrip_GR09_Dec')
	    )
      )
   ) 

   process.es_prefer_my =cms.ESPrefer("PoolDBESSource","poolDBESSource")

   # Tracker digis: no ZS
   # cf /d00/icali/CMSSW_3_1_0/src/cfgFiles/SiStrip/SiStrip_SimToRaw_VR_cfg.py
   process.simSiStripDigis.ZeroSuppression = cms.bool(False)
   process.simSiStripDigis.Noise = cms.bool(True)
   print "=== New simSiStripDigis cfg: ==="
   print process.simSiStripDigis.dumpPython()

   # Tracker digi2raw with VR
   # - cf
   #   * EventFilter/SiStripRawToDigi/python/SiStripDigiToRaw_cfi.py
   #     + for example py
   #   * EventFilter/SiStripRawToDigi/src/SiStripFEDBufferComponents.[cc/h]
   #     + for mode_key definitions
   process.newSiStripDigiToRaw = cms.EDProducer(
      "sistrip::SiStripDigiToRawModule",
      InputModuleLabel = cms.string('simSiStripDigis'),
      InputDigiLabel = cms.string('VirginRaw'),
      FedReadoutMode = cms.untracked.string('VIRGIN_RAW'),
      UseFedKey = cms.untracked.bool(False),
      UseWrongDigiType = cms.bool(False)
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

   # compression in OM
   process.output = cms.OutputModule("PoolOutputModule",
      splitLevel = cms.untracked.int32(0),
      outputCommands = process.RAWSIMEventContent.outputCommands,
      fileName = cms.untracked.string('raw_DIGI_L1_DIGI2RAW.root'),
      dataset = cms.untracked.PSet(
	 dataTier = cms.untracked.string('DIGI-RAW'),
	 filterName = cms.untracked.string('')
	 ),
      #use_compression = cms.untracked.bool(False)
      #compression_level = cms.untracked.int32( -1 )
   )
   return(process)

custom_TrackerVR(process)
