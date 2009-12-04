import FWCore.ParameterSet.Config as cms


process = cms.Process("ANALYSIS")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      #'/store/express/BeamCommissioning09/OfflineMonitor/FEVTHLTALL/v2/000/122/314/F8F49C1B-60D8-DE11-AB34-001D09F28F0C.root' # file from run 122314    
    #'/store/relval/CMSSW_3_3_2/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG/STARTUP31X_V8-v2/0000/CC203608-59C8-DE11-B1E6-0018F3D096EA.root' # MC TTBAR
    #'/store/data/BeamCommissioning09/MinimumBias/RECO/v2/000/123/151/FA9D48E2-14DE-DE11-BB11-001D09F291D2.root'
    '/store/data/BeamCommissioning09/MinimumBias/RAW/v1/000/122/314/CC89C4BC-60D8-DE11-B365-0030487D0D3A.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32( 100 ),
    skipBadFiles = cms.bool(True)
    )

process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# Which AlCa condition for what. Available from pre11
# * DESIGN_31X_V1 - no smearing, alignment and calibration constants = 1.  No bad channels.
# * MC_31X_V1 (was IDEAL_31X) - conditions intended for 31X physics MC production: no smearing,
#   alignment and calibration constants = 1.  Bad channels are masked.
# * STARTUP_31X_V1 (was STARTUP_31X) - conditions needed for HLT 8E29 menu studies: As MC_31X_V1 (including bad channels),
#   but with alignment and calibration constants smeared according to knowledge from CRAFT.
# * CRAFT08_31X_V1 (was CRAFT_31X) - conditions for CRAFT08 reprocessing.
# * CRAFT_31X_V1P, CRAFT_31X_V1H - initial conditions for 2009 cosmic data taking - as CRAFT08_31X_V1 but with different
#   tag names to allow append IOV, and DT cabling map corresponding to 2009 configuration (10 FEDs).
# Meanwhile...:
#process.GlobalTag.globaltag = 'MC_31X_V9::All'
process.GlobalTag.globaltag = 'GR09_H_V6OFF::All'

# Define the HLT reco paths
# Note
# * PrescaleService is automatically stripped in cff mode of getHLT.py
#process.load("HLTrigger.HLTanalyzers.HLT_Startup09_data_cff")
# To load the HLT menu, somehow cmsRun asks for DQM service ...

# Define the analyzer modules
process.load("HLTrigger.HLTanalyzers.HLTBitAnalyser_cfi")
# Settings
isRaw=True # * =1 use existing gtDigis on the input file, =0 extract gtDigis from the RAW data collection
isMC=False
from HLTrigger.HLTanalyzers.HLTBitAnalyser_cfi import update_cfg_cases
update_cfg_cases(process,isRaw,isMC)
#process.hltbitanalysis.RunParameters.Monte= cms.bool(False)
#process.hltbitanalysis.hltresults = cms.InputTag( 'TriggerResults','','HLT' )

process.schedule = cms.Schedule( process.analyzeHLT_step )

# === Some useful customization ===
# Make L1 reports
process.load('L1Trigger.GlobalTriggerAnalyzer.l1GtTrigReport_cfi')
if (isRaw):
  process.l1GtTrigReport.L1GtRecordInputTag = cms.InputTag( 'hltGtDigis','',process.name_() )
else:
  process.l1GtTrigReport.L1GtRecordInputTag = cms.InputTag( "gtDigis" )
process.MessageLogger.categories.append('L1GtTrigReport')
# Make HLT reports
process.hltTrigReport = cms.EDAnalyzer( 'HLTrigReport',
  HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT' )
)
process.MessageLogger.categories.append('TriggerSummaryProducerAOD')
process.MessageLogger.categories.append('HLTrigReport')
# Add to Schedule
process.L1AnalyzerEndpath = cms.EndPath( process.l1GtTrigReport*process.hltTrigReport)
process.schedule.append(process.L1AnalyzerEndpath)

