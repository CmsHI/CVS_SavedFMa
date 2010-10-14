import FWCore.ParameterSet.Config as cms

process = cms.Process("DJAna")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Saved.DiJetAna.eventSelection_cff")
process.load("Saved.DiJetAna.ExtraReco_cff")
process.load("Saved.DiJetAna.dijetAna_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      'file:jetAnaSkim.root'
      )
    )

# ===== Centrality =====
from Saved.DiJetAna.customise_cfi import *
#loadCentralityDB(process,'HFhits40_DataJulyExercise_AMPT2760GeV_MC_37Y_V5_ZS_v0')
#loadCentralityDB(process,'HFhits40_MC_Hydjet2760GeV_MC_3XY_V24_v0')
#loadCentralityDB(process,'CentralityTable_HFhits40_Hydjet2760GeV_v0_mc')

# === Sample specific configs ===
for i,m in enumerate([process.djcalo,
  process.djcalo_tower,
  process.djcalo_genp,
  process.djgen]):
  #m.hltsrc = cms.InputTag("TriggerResults","","HISIGNAL")
  print i, "hlt: ", m.hltsrc
process.djcalo.trksrc = "hiSelectedTracks"
process.djcalo.anaTrkType = 2
process.djgen.jetsrc = "ak5HiGenJets"

anaOutName = "dj_%s.root" % (process.djcalo.jetsrc.value())
process.TFileService = cms.Service('TFileService',
    fileName = cms.string(anaOutName)
    )

# =============== Final Paths =====================
#enableRECO(process,"MC","pp")
enablePp(process,"PpRECO") # options: "PpRECO", "HIRECO"
enableData(process)
#process.reco_extra*=process.allTracks

process.reco = cms.Path( process.eventSelection * process.reco_extra )
process.ana  = cms.Path( process.eventSelection * process.dijetAna_seq )

process.schedule = cms.Schedule(process.reco,process.ana)
