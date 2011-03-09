import FWCore.ParameterSet.Config as cms

process = cms.Process("DJAna")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Saved.DiJetAna.eventSelection_cff")
process.load("Saved.DiJetAna.ExtraReco_cff")
process.load("Saved.DiJetAna.dijetAna_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      'file:jetAnaSkim.root'
      )
    )

# ===== Top Level =====
process.GlobalTag.globaltag = "START39_V7HI::All"
isData=False
isDataEmbed=False
hltProcess="HLT" # HLT, HISIGNAL, HLTMIX

# ===== Centrality =====
from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)
process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFhits"),
    nonDefaultGlauberModel = cms.string("Hydjet_Bass"),
    centralitySrc = cms.InputTag("hiCentrality")
    )
if (isData or isDataEmbed):
  process.HeavyIonGlobalParameters.nonDefaultGlauberModel = cms.string("")



# === Sample specific configs ===
for i,m in enumerate([process.djcalo,
  process.djcalo_tower,
  process.djcalo_pfcand,
  process.djcalo_genp,
  process.djgen,
  process.djcaloic5,
  process.djcaloak5,
  process.djpfic5,
  process.djpfak5]):
  m.hltsrc = cms.InputTag("TriggerResults","",hltProcess)
  print i, "hlt: ", m.hltsrc

anaOutName = "dj_%s.root" % (process.djcalo.jetsrc.value())
process.TFileService = cms.Service('TFileService',
    fileName = cms.string(anaOutName)
    )

# =============== Final Paths =====================
from Saved.DiJetAna.customise_cfi import *
# Event Selection
#enableTrigger(process,"Jet")
# HLT Ana
#enableOpenHlt(process,process.dijetAna_seq,isData)
#process.hltanalysis.hltresults = cms.InputTag( 'TriggerResults','',hltProcess)
#process.hltanalysis.HLTProcessName = hltProcess

enableRECO(process,"MC","HI")
#enablePp(process,"PpRECO") # options: "PpRECO", "HIRECO"

# FJ
#process.dijetAna_seq*=process.djcaloic5
#process.dijetAna_seq*=process.djcaloak5
#process.dijetAna_seq*=process.djpfic5
#process.dijetAna_seq*=process.djpfak5

# For MB
#for m in [process.djcalo,process.djcalo_tower]:
#  m.nearJetPtMin = 40

# First look at data
process.djcalo.nearJetPtMin = -1
process.djcalo.trkPtMin=1.5
process.dijetAna_seq.remove(process.djcalo_pfcand)
process.dijetAna_seq.remove(process.djcalo_tower)
#process.djcalo_pfcand.nearJetPtMin = 100
process.djcalo_genp.nearJetPtMin = -1
#process.djcalo_genp.anaGenpType = 1 # 0 for all charge, 1 for charged
process.djcalo_genp.trkPtMin=1.5
#process.djcalo_genp.trksrc = cms.InputTag("hiGenParticles","","HIJETS") # "HISIGNAL", "HIJETS"
#process.djgen.nearJetPtMin = 80
#process.djgen.anaGenpType = 1
#process.djgen.trksrc = cms.InputTag("hiGenParticles","","HIJETS")
process.djgen.trkPtMin=1.5

# trk eff ana
process.dijetAna_seq=process.hitrkEffAna
process.hitrkEffAnalyzer.fillNtuples = True
process.hitrkEffAnalyzer.trkPtMin = 2

# For Embedding
if isDataEmbed:
  enableDataMixMC(process)

process.reco = cms.Path( process.eventSelection * process.dj_reco_extra )
process.ana  = cms.Path( process.eventSelection * process.dijetAna_seq )

process.schedule = cms.Schedule(process.reco,process.ana)
