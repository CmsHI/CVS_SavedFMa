#
# Add Particle List Drawing Utilities
# - cf
#   * http://www.cmsaf.mit.edu/twiki/bin/view/CmsHi/SavedFrankCMSSWAna#Gen_Particles
#

def addParticleListDrawer(process):
  process.printList = cms.EDAnalyzer("ParticleListDrawer",
   maxEventsToPrint = cms.untracked.int32(-1),
   printVertex = cms.untracked.bool(True),
   src = cms.InputTag("hiGenParticles")
  )
  return(process)
addParticleListDrawer(process)

def addParticleTreeDrawer(process):
  process.printTree = cms.EDFilter("ParticleTreeDrawer",
   src = cms.InputTag("hiGenParticles"),
   printP4 = cms.untracked.bool(True),
   printPtEtaPhi = cms.untracked.bool(True),
   printVertex = cms.untracked.bool(True),
   printStatus = cms.untracked.bool(True),
   printIndex = cms.untracked.bool(False),
   status = cms.untracked.vint32(1, 2, 3)
   )
  return(process)
addParticleTreeDrawer(process)

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.pana = cms.Path(process.printList*process.printTree)

