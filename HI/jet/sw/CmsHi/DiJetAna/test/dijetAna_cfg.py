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
process.load("CmsHi.DiJetAna.dijetAna_cfi")
process.dijetAna.jetsrc = "ak5HiGenJets"
process.dijetAna.anaJetType = 1
print process.dijetAna.dumpPython()

anaOutName = "dijetAna_anaJet%d_refJet%d.root" % (process.dijetAna.anaJetType.value(),process.dijetAna.refJetType.value())
print "AnaOutput: ", anaOutName

process.TFileService = cms.Service('TFileService',
    fileName = cms.string(anaOutName)
    )

# =============== Final Paths =====================
process.ana = cms.Path(#process.chargedCandidates *
   process.dijetAna)
