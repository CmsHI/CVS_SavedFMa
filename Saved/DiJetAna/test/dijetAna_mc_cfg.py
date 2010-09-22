import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      'file:jetAnaSkim.root'
      )
    )

# ===== Centrality =====
from Saved.DiJetAna.customise_cfi import *
#loadCentralityDB(process,'HFhits40_DataJulyExercise_AMPT2760GeV_MC_37Y_V5_ZS_v0')
loadCentralityDB(process,'HFhits40_MC_Hydjet2760GeV_MC_3XY_V24_v0')

process.load("RecoHI.HiCentralityAlgos.CentralityFilter_cfi")
process.centralityFilter.centralityBase = "HF"
process.centralityFilter.selectedBins = range(40)

#================ DiJet Ana ==========================
process.load("HeavyIonsAnalysis.Configuration.analysisProducers_cff")
process.load("Saved.DiJetAna.dijetAna_cff")
# -- sample specific configs --
for i,m in enumerate([process.dijetAna_mc,
    process.dijetAna_mc_calojet_tower,
    process.dijetAna_mc_calojet_genp,
    process.dijetAna_mc_genjet_genp]):
  m.hltsrc = cms.InputTag("TriggerResults","","DIGI2RAW")
  print i, "hlt: ", m.hltsrc
process.dijetAna_mc.trksrc = "hiSelectedTracks"
process.dijetAna_mc.anaTrkType = 2
print process.dijetAna_mc.dumpPython()

anaOutName = "dijetAna_%s.root" % (process.dijetAna_mc.jetsrc.value())
print "AnaOutput: ", anaOutName

process.TFileService = cms.Service('TFileService',
    fileName = cms.string(anaOutName)
    )

# =============== Final Paths =====================
process.reco = cms.Path( process.centralityFilter * enableRECO(process) )
process.ana = cms.Path(
   process.centralityFilter *
   #process.allTracks *
   process.dijetAna_mc_seq
   )

process.schedule = cms.Schedule(
  process.reco,
  process.ana
  )
