#
# Add genjet producer
#
def prod_genjets(process):
   # === Produce genjets ===
   # hi jet cfg
   process.load("CmsHi.JetAnalysis.IterativeCone5HiGenJets_cff")
   # subevent map
   process.load("CmsHi.Utilities.HiGenParticles_cfi")
   # genjet configuration
   process.hiGenParticles.src = ["generator"]

   process.reco_step = cms.Path(process.hiGenParticles*process.hiGenParticlesForJets*process.iterativeCone5HiGenJets)
   process.schedule.insert(0,process.reco_step)
   return(process)

process=prod_genjets(process)


