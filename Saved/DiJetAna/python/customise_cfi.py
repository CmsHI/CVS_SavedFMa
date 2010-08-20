import FWCore.ParameterSet.Config as cms

# Setup centrality info from db
def loadCentralityDB(process):
  #process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
  #process.GlobalTag.globaltag = 'MC_38Y_V8::All'

  #process.GlobalTag.toGet = cms.VPSet(
  #    cms.PSet(record = cms.string("HeavyIonRcd"),
  #      tag = cms.string("CentralityTable_HFhits40_AMPT2760GeV_v0_mc"),
  #      connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_PHYSICSTOOLS")
  #      )
  #    )

  process.load("CondCore.DBCommon.CondDBCommon_cfi")
  process.CondDBCommon.connect = "sqlite_file:/net/hisrv0001/home/frankma/work/HI/jet/sw/Y1JAna_CMSSW_3_8_1_patch2/src/CmsHi/JulyExercise/data/CentralityTables.db"
  process.PoolDBESSource = cms.ESSource("PoolDBESSource",
      process.CondDBCommon,
      toGet = cms.VPSet(
	cms.PSet(record = cms.string('HeavyIonRcd'),
	  tag = cms.string('HFhits40_DataJulyExercise_AMPT2760GeV_MC_37Y_V5_ZS_v0')
	  )
	)
      )
  return process
