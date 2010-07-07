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
print process.dijetAna_data_allcbin.dumpPython()

anaOutName = "dijetAna_anaJet_Data%d.root" % (process.dijetAna_mc.isMC.value())
print "AnaOutput: ", anaOutName

process.TFileService = cms.Service('TFileService',
    fileName = cms.string(anaOutName)
    )

# =============== Final Paths =====================
process.ana = cms.Path(#process.chargedCandidates *
   process.dijetAna_data_seq
   )
