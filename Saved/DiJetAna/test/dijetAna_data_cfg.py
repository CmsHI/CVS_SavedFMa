import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      'file:jetAnaSkim.root'
      )
    )

#================ DiJet Ana ==========================
process.load("Saved.DiJetAna.dijetAna_cff")
# -- sample specific configs --
process.dijetAna_data.trksrc = "allTracks"
process.dijetAna_data.anaTrkType = 3
process.dijetAna_data_seq += process.dijetAna_data_kt4
process.dijetAna_data_seq += process.dijetAna_data_ak5
print process.dijetAna_data.dumpPython()

anaOutName = "dijetAna_%s.root" % (process.dijetAna_data.jetsrc.value())
print "AnaOutput: ", anaOutName

process.TFileService = cms.Service('TFileService',
    fileName = cms.string(anaOutName)
    )

# =============== Final Paths =====================
process.ana = cms.Path(#process.chargedCandidates *
   process.dijetAna_data_seq
   )
