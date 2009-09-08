import FWCore.ParameterSet.Config as cms

process = cms.Process('GENJETRECO')

# === import of 33X configurations ===
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load("RecoHI.HiJetAlgos.IterativeCone5HiGenJets_cff")
process.load("RecoHI.HiJetAlgos.RecoHiJets_EventContent_cff")


# === standard configurations ===
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_3_2_4/RelValHydjetQ_MinBias_4TeV/GEN-SIM-RAW/MC_31X_V3-v1/0010/D62F586C-4E84-DE11-80D3-000423D98E54.root')
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.RecoHiJetsFEVT.outputCommands,
    fileName = cms.untracked.string('genjets330pre2.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'MC_31X_V3::All'


# === Genjet specific configuration ===
# First produce a GenParticle collection with subevent record
hiGenParticles = cms.EDProducer('GenParticleProducer',
   doSubEvent = cms.untracked.bool(True),
   #srcVector = cms.vstring("hiSignal","generator")
   srcVector = cms.vstring("generator") # for now no mixing
)
process.hiGenParticles = hiGenParticles


# === Executable ===
#process.reco_step = cms.Path(process.hiGenParticles*process.hiGenParticlesForJets*process.iterativeCone5HiGenJets)
process.reco_step = cms.Path(process.hiGenParticles*process.hiGenParticlesForJets)
process.out_step = cms.EndPath(process.output)
