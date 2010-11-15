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

def enableRECO(process,mode="MC",type="HI"):
  process.reco_extra = cms.Sequence()
  if type=="HI":
    # pat jet
    process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
    from PhysicsTools.PatAlgos.tools.heavyIonTools import configureHeavyIons
    configureHeavyIons(process)
    process.reco_extra *= process.heavyIon*process.makeHeavyIonJets
    if mode=="Data":
      from Saved.Skim.customise_cfi import removePatMCMatch
      removePatMCMatch(process)
      process.reco_extra.remove(process.heavyIon)
  if type=="pp":
    process.load('PhysicsTools.PatAlgos.patSequences_cff')
    process.reco_extra *= process.makePatJets
    if mode=="Data":
      from PhysicsTools.PatAlgos.tools.coreTools import removeMCMatching
      removeMCMatching(process, ['All']) # turn off MC matching for data
  # JEC Set
  from PhysicsTools.PatAlgos.tools.jetTools import switchJECSet
  #switchJECSet( process, "Spring10") # Spring10 is the new default

# If sample is pp disable HI event related variables
def enablePp(process,recoMode="PpRECO"):
  try:
    process.eventSelection.remove(process.hiEvtFilter)
  except:
    print "Notice: dijetAna will not modify process.eventSelection"
  for m in [process.djcalo,process.djcalo_tower,process.djcalo_genp,process.djgen]:
    m.sampleType = 10
    m.hltNames = ["HLT_MinBiasPixel_SingleTrack","HLT_Jet15U","HLT_Jet50U","HLT_Photon20_Cleaned_L1R"]
    if "tower" not in m.trksrc.moduleLabel.lower():
      m.trkPtMin = 0.5
    if recoMode=="HIRECO":
      continue
    m.vtxsrc = "offlinePrimaryVertices"
  if recoMode=="HIRECO":
    return
  process.highPurityTracks.src = "generalTracks"
  process.djcalo.trksrc = "highPurityTracks"
  process.djcalo.anaTrkType = 2
  process.djcalo_genp.trksrc = "genParticles"
  process.djgen.jetsrc = "ak5GenJets"
  process.djgen.trksrc = "genParticles"

def enableOpenHlt(process, seq, isData=True):
  process.load("HLTrigger.HLTanalyzers.HI_HLTAnalyser_cff")
  process.hltanalysis.RunParameters.Debug = False
  process.hltanalysis.RunParameters.UseTFileService = True
  process.hltanalysis.RunParameters.Monte = (not isData)
  process.hltanalysis.RunParameters.DoMC = (not isData)
  process.hltanalysis.RunParameters.DoJets = True
  process.hltanalysis.RunParameters.DoPhotons = True
  process.hltanalysis.RunParameters.DoSuperClusters = True
  process.hltanalysis.recjets  = "iterativeConePu5CaloJets"
  process.hltanalysis.BarrelPhoton = "correctedIslandBarrelSuperClusters"
  process.hltanalysis.EndcapPhoton = "correctedIslandEndcapSuperClusters"
  process.hltanalysis.l1GtReadoutRecord = cms.InputTag("gtDigis")
  # add to seq
  seq*=process.centralityBin
  seq*=process.hltanalysis

### If Data
def enableData(process):
  process.dijetAna_seq.remove(process.djcalo_genp)
  process.dijetAna_seq.remove(process.djgen)
  for m in [process.djcalo,process.djcalo_tower]:
    m.hltsrc = cms.InputTag("TriggerResults","","HLT")
    m.isMC = False
    m.refJetType = -1


### Output levels
djOutputThreshold = {
  'FULL': -1,
  'FF': 60,
  'LIGHT': 99999
  }
