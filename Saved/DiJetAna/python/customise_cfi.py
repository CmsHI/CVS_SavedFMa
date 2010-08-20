import FWCore.ParameterSet.Config as cms

# Setup centrality info from db
def loadCentralityDB(process,centTag):
  #process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
  #process.GlobalTag.globaltag = 'MC_38Y_V8::All'

  #process.GlobalTag.toGet = cms.VPSet(
  #    cms.PSet(record = cms.string("HeavyIonRcd"),
  #      tag = cms.string("CentralityTable_HFhits40_AMPT2760GeV_v0_mc"),
  #      connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_PHYSICSTOOLS")
  #      )
  #    )

  process.load("CondCore.DBCommon.CondDBCommon_cfi")
  process.CondDBCommon.connect = "sqlite_file:CentralityTables.db"
  process.PoolDBESSource = cms.ESSource("PoolDBESSource",
      process.CondDBCommon,
      toGet = cms.VPSet(
	cms.PSet(record = cms.string('HeavyIonRcd'),
	  tag = cms.string(centTag)
	  )
	)
      )
