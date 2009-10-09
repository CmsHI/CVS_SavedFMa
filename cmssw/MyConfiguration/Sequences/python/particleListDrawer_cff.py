#
# Add Particle List Drawing Utilities
# - cf
#   * https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideCandidateModules#ParticleTreeDrawer_Utility
#

def addParticleListDrawer(process):
  process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
  process.printTree = cms.EDAnalyzer("ParticleListDrawer",
   maxEventsToPrint = cms.untracked.int32(-1),
   printVertex = cms.untracked.bool(False),
   src = cms.InputTag("hiGenParticles")
  )
  process.pana = cms.Path(process.printTree)
  return(process)
addParticleListDrawer(process)
