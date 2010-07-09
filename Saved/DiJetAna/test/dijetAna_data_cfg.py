import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
      'file:jetAnaSkim.root'
      )
    )

#================ DiJet Ana ==========================
process.load("Saved.DiJetAna.dijetAna_cfi")
process.dijetAna_data.centLabel = cms.string("HFhits20_DataJulyExercise_Hydjet2760GeV_MC_37Y_V5_v0")
#process.dijetAna_data.djDPhiMin = 2.5
print process.dijetAna_data.dumpPython()

anaOutName = "dijetAna_anaJet_Data%d.root" % (process.dijetAna_mc.isMC.value())
print "AnaOutput: ", anaOutName

process.TFileService = cms.Service('TFileService',
    fileName = cms.string(anaOutName)
    )

# =============== Final Paths =====================
process.ana = cms.Path(#process.chargedCandidates *
   process.dijetAna_data_seq
   )
