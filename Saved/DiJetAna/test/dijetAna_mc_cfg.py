import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      'file:jetAnaSkim.root'
      )
    )

from Saved.DiJetAna.customise_cfi import *
loadCentralityDB(process,'HFhits40_DataJulyExercise_AMPT2760GeV_MC_37Y_V5_ZS_v0')


#================ DiJet Ana ==========================
process.load("Saved.DiJetAna.dijetAna_cff")
# -- sample specific configs --
process.dijetAna_mc.trksrc = "allTracks"
process.dijetAna_mc.anaTrkType = 3
print process.dijetAna_mc.dumpPython()

anaOutName = "dijetAna_%s.root" % (process.dijetAna_mc.jetsrc.value())
print "AnaOutput: ", anaOutName

process.TFileService = cms.Service('TFileService',
    fileName = cms.string(anaOutName)
    )

# =============== Final Paths =====================
process.ana = cms.Path(#process.chargedCandidates *
   process.dijetAna_mc_seq
   )
