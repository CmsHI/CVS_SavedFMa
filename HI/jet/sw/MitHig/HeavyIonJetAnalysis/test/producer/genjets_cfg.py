import FWCore.ParameterSet.Config as cms

process = cms.Process("GENJET")

# Standard includes
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.Generator_cff")

# - Run trivial mixing module without secondary source
#   * to add mixing frame products needed by HiGenParticleProducer
#     + cf pwd/mixNoBackground.py
process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
del process.mix.mixObjects
process.mix.mixObjects = cms.PSet(
   mixHepMC = cms.PSet(process.mixHepMCProducts)
)

# Jet related includes
process.load("RecoJets.Configuration.RecoGenJets_cff")
process.load("RecoJets.JetProducers.IconeJetParameters_cfi")
process.load("RecoJets.Configuration.GenJetParticles_cff")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# HiGenParticleProducer
# - borrows the cross frame to produce association maps
#   * which in this case is the SubEventMap
#   * thus we need the framework of mixing
#     + runs a trivial mixing just to get the crossing frame
#       products (see above)
process.load("RecoHI.HiCandidateAlgos.hiGenParticles_cfi")

process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring('file:hydjet_edm.root')
)

process.maxEvents = cms.untracked.PSet(
      input = cms.untracked.int32(-1)
)

process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',
                                        ignoreTotal=cms.untracked.int32(0),
                                        oncePerEventMode = cms.untracked.bool(False)
                                        )

process.iterativeCone5HiGenJets = cms.EDProducer("IterativeConeHiGenJetProducer",
#                                                 process.GenJetParameters,  
                                                 process.IconeJetParameters, 
						 inputEtMin = cms.double(0.0),
						 inputEMin = cms.double(0.0),
                                                 alias = cms.untracked.string('IC5HiGenJet'),
                                                 coneRadius = cms.double(0.5)
                                                 )


process.subevent = cms.EDAnalyzer('HeavyIonJetAnalyzer',
   jetSrc = cms.vstring('iterativeCone5HiGenJets'),
   doParticles = cms.untracked.bool(False)
)

process.allevent =  cms.EDAnalyzer('HeavyIonJetAnalyzer',
   jetSrc = cms.vstring('iterativeCone5GenJets'),
   doParticles = cms.untracked.bool(False)
)

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string('genjets_tree.root')
                                   )

process.output = cms.OutputModule("PoolOutputModule",
    compressionLevel = cms.untracked.int32(2),
    commitInterval = cms.untracked.uint32(1),
    fileName = cms.untracked.string('genjets_edm.root')
)

process.runjets = cms.Sequence(process.GeneInfo*process.hiGenParticles*process.iterativeCone5HiGenJets)
process.pre = cms.Path(process.mix)
# Mixing module is needed by Philip's HiGenParticleProducer
# also,
# Run analyzer
# - We don't run all event for the moment, which needs the standard iterativeCone5GenJets
#   * we don't add this standard iCone5 jet finder, to keep things simple for now
process.p = cms.Path(process.runjets*process.subevent)
process.outpath = cms.EndPath(process.output)

process.schedule = cms.Schedule(process.pre,process.p,process.outpath)
