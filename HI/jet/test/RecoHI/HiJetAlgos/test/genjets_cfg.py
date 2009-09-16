import FWCore.ParameterSet.Config as cms

process = cms.Process('GENJETRECO')

# === import configurations ===
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
# hi jet cfg
process.load("CmsHi.JetAnalysis.IterativeCone5HiGenJets_cff")
# subevent map
process.load("CmsHi.Utilities.HiGenParticles_cfi")
# quick check analyzer
process.load("CmsHi.JetAnalysis.HiJetAnalysisModules_cff")


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
    #outputCommands = process.RecoHiJetsFEVT.outputCommands,
    fileName = cms.untracked.string('edm_output.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'MC_31X_V3::All'


# === Genjet specific configuration ===
# for now only one HepMC collection, no mixing yet
process.hiGenParticles.src = ["generator"]

process.genevent.jetSrc = cms.InputTag("iterativeCone5HiGenJets")

process.TFileService = cms.Service('TFileService',
   fileName = cms.string('treefile.root')
)


# === Executable ===
process.reco_step = cms.Path(process.hiGenParticles*process.hiGenParticlesForJets*process.iterativeCone5HiGenJets*process.genevent)
process.out_step = cms.EndPath(process.output)
