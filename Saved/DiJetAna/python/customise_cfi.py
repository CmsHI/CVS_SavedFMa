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
  process.CondDBCommon.connect = "sqlite_file:/net/hisrv0001/home/frankma/work/HI/jet/sw/Y1JAna_CMSSW_3_8_4/src/RecoHI/HiCentralityAlgos/data/CentralityTables.db"
  process.PoolDBESSource = cms.ESSource("PoolDBESSource",
      process.CondDBCommon,
      toGet = cms.VPSet(
	cms.PSet(record = cms.string('HeavyIonRcd'),
	  tag = cms.string(centTag)
	  )
	)
      )


def enableRECO(process,mode="MC"):
  # pat jet
  process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
  from PhysicsTools.PatAlgos.tools.heavyIonTools import configureHeavyIons
  configureHeavyIons(process)
  from PhysicsTools.PatAlgos.tools.jetTools import switchJECSet
  #switchJECSet( process, "Spring10") # Spring10 is the new default
  from Saved.Skim.customise_cfi import removePatMCMatch
  process.reco_extra = cms.Sequence(process.heavyIon*process.makeHeavyIonJets)
  if mode=="Data":
    removePatMCMatch(process)
    process.reco_extra.remove(process.heavyIon)
  return process.reco_extra

def enablePp(process):
  process.eventSelection.remove(process.hiEvtFilter)
