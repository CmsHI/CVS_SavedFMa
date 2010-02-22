import FWCore.ParameterSet.Config as cms

process = cms.Process("ANASKIM")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
# activate the following lines to get some output
#process.MessageLogger.destinations = cms.untracked.vstring("cout")
#process.MessageLogger.cout = cms.untracked.PSet(threshold = cms.untracked.string("INFO"))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      # data
      "/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Jan29Skim-v2/0022/FEF6AAC7-760E-DF11-8AA3-002618943861.root"
      # mc
      #"/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8P_900GeV_Jan29ReReco-v1/0018/FEE5AED4-540D-DF11-819B-00E08178C129.root"
      ),
    secondaryFileNames = cms.untracked.vstring(
      )
    #        '/store/data/Commissioning08/Cosmics/RAW/v1/000/069/578/085EFED4-E5AB-DD11-9ACA-001617C3B6FE.root')
    )

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.4 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/DPGAnalysis/Skims/python/GoodVertex_cfg.py,v $'),
    annotation = cms.untracked.string('At least two general track or one pixel track or one pixelLess track')
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))


# ================ Physics Declared ================
process.physDeclFilter = cms.EDFilter("PhysDecl",
    applyfilter = cms.untracked.bool(True)
    )


# ================ bptx =======================
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed
process.bptxAnd = hltLevel1GTSeed.clone(
    L1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('0')
    )


# ================ BSC, beam halo veto =======================
# no work here, running on bsc+nohalo skimmed sample
#process.L1T1=process.hltLevel1GTSeed.clone()
#process.L1T1.L1TechTriggerSeeding = cms.bool(True)
#process.L1T1.L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')


# ================ monster event rejection =======================
process.monsterVeto = cms.EDFilter("FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(True),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.2)
    )


# ================ Vertex Filter ===================
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
    vertexCollection = cms.InputTag('offlinePrimaryVertices'),
    minimumNDOF = cms.uint32(4) ,
    maxAbsZ = cms.double(15),	
    maxd0 = cms.double(2)	
    )

# =============== Final Filter Path ==================
process.jetAnaSkim_step = cms.Path(process.physDeclFilter*process.bptxAnd*process.monsterVeto)


# ================= Output ======================
process.output = cms.OutputModule("PoolOutputModule",
    #outputCommands = cms.untracked.vstring('keep *','drop *_MEtoEDMConverter_*_*'),
    outputCommands = cms.untracked.vstring('drop *',
      # event
      'keep *_offlinePrimaryVertices_*_*',
      'keep *_offlineBeamSpot_*_*',
      'keep *_TriggerResults_*_HLT',
      'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*RECO',
      # jet
      'keep *Jet*_*CaloJet*_*_*',
      'keep *_*JetID*_*_*',
      'keep *Jet*_*JetExtender*_*_*',
      'keep *_towerMaker_*_*',
      'keep *_hcalnoise_*_*',
      # tracks
      'keep recoTracks_generalTracks_*_*RECO',
      # mc (if present)
      'keep *_*GenJet*_*_*',
      'keep reco*_genParticles_*_*',
      # clean up
      'drop *_*BJetTags*_*_*',
      ),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('jetAnaSkim_step')),
    dataset = cms.untracked.PSet(
      dataTier = cms.untracked.string('AOD'),
      filterName = cms.untracked.string('JetAnaFilter')),
    fileName = cms.untracked.string('jetAnaSkimAOD.root')
    )

process.outpath = cms.EndPath(process.output)
