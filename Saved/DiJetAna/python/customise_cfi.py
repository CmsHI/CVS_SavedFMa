import FWCore.ParameterSet.Config as cms

# Setup centrality info from db
def loadCentralityDB(process,centTag):
  process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
  process.GlobalTag.globaltag = 'MC_38Y_V8::All'

  process.GlobalTag.toGet = cms.VPSet(
      cms.PSet(record = cms.string("HeavyIonRcd"),
	tag = cms.string(centTag),
	connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_PHYSICSTOOLS")
	)
      )

  #  process.load("CondCore.DBCommon.CondDBCommon_cfi")
#  process.CondDBCommon.connect = "sqlite_file:/net/hisrv0001/home/frankma/work/HI/jet/sw/Y1JAna_CMSSW_3_8_4/src/RecoHI/HiCentralityAlgos/data/CentralityTables.db"
#  process.PoolDBESSource = cms.ESSource("PoolDBESSource",
#      process.CondDBCommon,
#      toGet = cms.VPSet(
#	cms.PSet(record = cms.string('HeavyIonRcd'),
#	  tag = cms.string(centTag)
#	  )
#	)
#      )


def enableRECO(process,mode="MC",type="HI"):
  process.load('Saved.DiJetAna.TrackSelection_cff')
  if type=="HI":
    # pat jet
    process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
    from PhysicsTools.PatAlgos.tools.heavyIonTools import configureHeavyIons
    configureHeavyIons(process)
    process.reco_extra = cms.Sequence(process.heavyIon*process.makeHeavyIonJets)
    if mode=="Data":
      from Saved.Skim.customise_cfi import removePatMCMatch
      removePatMCMatch(process)
      process.reco_extra.remove(process.heavyIon)
  if type=="pp":
    process.load('PhysicsTools.PatAlgos.patSequences_cff')
    process.reco_extra = cms.Sequence(process.highPurityTracks*process.makePatJets)
    if mode=="Data":
      from PhysicsTools.PatAlgos.tools.coreTools import removeMCMatching
      removeMCMatching(process, ['All']) # turn off MC matching for data
  # JEC Set
  from PhysicsTools.PatAlgos.tools.jetTools import switchJECSet
  #switchJECSet( process, "Spring10") # Spring10 is the new default
  return process.reco_extra

# If sample is pp disable HI event related variables
def enablePp(process):
  process.eventSelection.remove(process.hiEvtFilter)
  for m in [process.djcalo,
      process.djcalo_tower,
      process.djcalo_genp,
      process.djgen]:
    m.sampleType = 10
    m.hltsrc = cms.InputTag("TriggerResults","","REDIGI36X")
    m.hltNames = ["HLT_Jet15U","HLT_Jet50U","HLT_Photon20_L1R"]
    m.vtxsrc = "offlinePrimaryVertices"
  process.djcalo.trksrc = "highPurityTracks"
  process.djcalo.anaTrkType = 2
  process.djcalo_genp.trksrc = "genParticles"
  process.djgen.jetsrc = "ak5GenJets"
  process.djgen.trksrc = "genParticles"

### If Data
def enableData(process):
  process.dijetAna_seq.remove(process.djcalo_genp)
  process.dijetAna_seq.remove(process.djgen)
  for m in [process.djcalo,process.djcalo_tower]:
    m.isMC = False
    m.refJetType = -1
