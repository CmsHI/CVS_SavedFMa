import FWCore.ParameterSet.Config as cms

process = cms.Process("ANALYSIS")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 
        '/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/600/22DE2330-5171-DD11-BD29-001617C3B64C.root',
        '/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/600/26E78B33-3D71-DD11-B8B8-001617DBD5AC.root',
        '/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/600/2C278B30-3C71-DD11-8AAE-000423D99AAE.root',
        '/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/600/4628105D-3D71-DD11-8634-001D09F2532F.root',
        '/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/600/74545FA9-3D71-DD11-9002-001D09F2B2CF.root',
        '/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/600/7C3A8ED3-3C71-DD11-B313-000423D98DB4.root',
        '/store/data/Commissioning08/Cosmics/RAW/CRUZET4_v1/000/058/600/82F205DA-3C71-DD11-AF2D-000423D98E6C.root'        
    )
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32( -1 )
)

process.load("Configuration.StandardSequences.GeometryPilot2_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

# Conditions: fake or frontier
# process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'IDEAL_V9::All'
process.GlobalTag.globaltag = 'GR09_E_V6::All'

process.load("Configuration.StandardSequences.L1Emulator_cff")
# Choose a menu/prescale/mask from one of the choices
# in L1TriggerConfig.L1GtConfigProducers.Luminosity
#process.load("Configuration.StandardSequences.L1TriggerDefaultMenu_cff")

# Run HLT
#process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
#process.load("HLTrigger.Configuration.HLT_2E30_cff")
#process.schedule = process.HLTSchedule

#process.hltL1gtTrigReport = cms.EDAnalyzer( "L1GtTrigReport",
#    UseL1GlobalTriggerRecord = cms.bool( False ),
#    L1GtRecordInputTag = cms.InputTag( "hltGtDigis" )
#)
#process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
#    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT' )
#)
#process.HLTAnalyzerEndpath = cms.EndPath( process.hltL1gtTrigReport + process.hltTrigReport )
#process.schedule.append(process.HLTAnalyzerEndpath)

# OpenHLT specificss
# Define the HLT reco paths
#process.load("HLTrigger.HLTanalyzers.HLTopen_cff")
#process.load("HLTrigger.HLTanalyzers.HLTopenCosmics_cff")

# Define the analyzer modules
process.load("HLTrigger.HLTanalyzers.HLTAnalyser_cfi")
process.analyzeThis = cms.Path( process.hltanalysis )

# Schedule the whole thing
process.schedule = cms.Schedule( 
    process.analyzeThis )

