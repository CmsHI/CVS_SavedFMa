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
process.GlobalTag.globaltag = "GR10_P_V12::All"
isData=True

# ===== Centrality =====
from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)
process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFhits"),
    nonDefaultGlauberModel = cms.string("AMPT_organ_2760GeV"),
    centralitySrc = cms.InputTag("hiCentrality")
    )
if (isData):
  process.HeavyIonGlobalParameters.nonDefaultGlauberModel = cms.string("")



# === Sample specific configs ===
for i,m in enumerate([process.djcalo,
  process.djcalo_tower,
  process.djcalo_genp,
  process.djgen]):
  #m.hltsrc = cms.InputTag("TriggerResults","","HISIGNAL")
  print i, "hlt: ", m.hltsrc

anaOutName = "dj_%s.root" % (process.djcalo.jetsrc.value())
process.TFileService = cms.Service('TFileService',
    fileName = cms.string(anaOutName)
    )

# =============== Final Paths =====================
from Saved.DiJetAna.customise_cfi import *
#enableRECO(process,"MC","pp")
#enablePp(process,"PpRECO") # options: "PpRECO", "HIRECO"
enableData(process)
#process.reco_extra*=process.allTracks
enableOpenHlt(process,process.dijetAna_seq,isData)

#process.reco = cms.Path( process.eventSelection * process.reco_extra )
process.ana  = cms.Path( process.eventSelection * process.dijetAna_seq )

#process.schedule = cms.Schedule(process.reco,process.ana)
process.schedule = cms.Schedule(process.ana)
