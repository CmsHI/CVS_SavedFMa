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

process.TFileService = cms.Service('TFileService',
    fileName = cms.string('dijetAna.root')
    )

#================ DiJet Ana ==========================
process.load("CmsHi.DiJetAna.dijetAna_cfi")

# =============== Final Paths =====================
process.ana = cms.Path(#process.chargedCandidates *
   process.dijetAna)
