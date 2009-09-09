#
# Use a local es source db file
#
def custom_essource(process):
   # EventSetup
   process.GlobalTag.globaltag = 'MC_31X_V3::All'

   # For tracker local dbfile
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
   return(process)

custom_essource(process)

