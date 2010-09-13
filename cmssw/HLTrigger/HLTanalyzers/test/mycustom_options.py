# === HLT Customization ===
process.HLT_L1Tech_BSC_minBias.remove(process.hltL1sZeroBias)
process.HLT_L1Tech_HCAL_HF.remove(process.hltL1sZeroBias)
process.HLT_ZeroBiasPixel_SingleTrack.remove(process.hltL1sZeroBias)
# - Activity Path tests -
process.load("HLTrigger.Configuration.HLT_HIon_Activity_cff")
process.HLT_HIActivityPixels.remove(process.hltL1sZeroBias)
process.HLTHcalSimpleRecHitFilter = cms.EDFilter("HLTHcalSimpleRecHitFilter",
    threshold = cms.double(3.0),
    minNHitsNeg = cms.int32(1),
    minNHitsPos = cms.int32(1),
    doCoincidence = cms.bool(False),
    maskedChannels = cms.vint32(),
    HFRecHitCollection = cms.InputTag("hltHfreco")
    )
process.HLTHcalSimpleRecHitFilter_Coinc1 = process.HLTHcalSimpleRecHitFilter.clone(
    doCoincidence = cms.bool(True)
    )
process.HLTHcalSimpleRecHitFilter_Coinc2 = process.HLTHcalSimpleRecHitFilter.clone(
    minNHitsNeg = cms.int32(2),
    minNHitsPos = cms.int32(2),
    doCoincidence= cms.bool(True)
    )
process.HLT_ActivityHF3_Coinc1.replace(process.HLTHcalSimpleRecHitFilter,process.HLTHcalSimpleRecHitFilter_Coinc1)
process.HLT_ActivityHF3_Coinc2.replace(process.HLTHcalSimpleRecHitFilter,process.HLTHcalSimpleRecHitFilter_Coinc2)


# analyzer customization
process.hltanalysis.hltresults = cms.InputTag("TriggerResults::"+process.process)
process.hltanalysis.HLTProcessName = process.process

#process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
#from PhysicsTools.PatAlgos.tools.heavyIonTools import *
#configureHeavyIons(process)
#from Saved.JulyExercise.customise_cfi import *
#removePatMCMatch(process)
#process.load("Saved.DiJetAna.dijetAna_cff")
#process.dijetAna_data.hltsrc = cms.InputTag("TriggerResults::"+process.process)
#process.dijetAna_data.hltNames = cms.untracked.vstring("HLT_ActivityHF3_Coinc1")
#process.dijetAna_data.jetEtaMax = cms.double(5.)
#process.dijetAna_data.trksrc = cms.InputTag("towerMaker")
#process.dijetAna_data.anaTrkType = cms.int32(3)
#process.jetana = cms.Path(process.makeHeavyIonJets*process.dijetAna_data)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    #outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    outputCommands = cms.untracked.vstring('drop *','keep *_towerMaker_*_*'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('HLT_AcitivityHF3_Coinc1')),
    fileName = cms.untracked.string('RelVal_HLT_HIon.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW-HLT'),
        filterName = cms.untracked.string('HLT_AcitivityHF3_Coinc1')
    )
)
# Path and EndPath definitions
process.out_step = cms.EndPath(process.output)

# Redefine schedule
process.schedule = cms.Schedule( 
    process.higen,
    process.HLTriggerFirstPath,
    process.HLT_HIActivityPixels,
    process.HLT_ActivityHF3,
    process.HLT_ActivityHF3_Coinc1,
    process.HLT_ActivityHF3_Coinc2,
    process.HLT_L1DoubleMuOpen,
    process.HLT_ZeroBiasPixel_SingleTrack,
    process.HLT_L1Tech_BSC_minBias,
    process.HLT_L1Tech_HCAL_HF,
    process.HLTriggerFinalPath,
    process.HLTAnalyzerEndpath,
    process.analyzeThis#,
    #process.jetana
    )
process.schedule.extend([process.out_step])

# print some config info
print "=== HLTHcalSimpleRecHitFilter cfg: ===\n"
print process.HLTHcalSimpleRecHitFilter.dumpPython()
print "=== HLTHcalSimpleRecHitFilter_Coinc1 cfg: ===\n"
print process.HLTHcalSimpleRecHitFilter_Coinc1.dumpPython()
print "=== HLTHcalSimpleRecHitFilter_Coinc2 cfg: ===\n"
print process.HLTHcalSimpleRecHitFilter_Coinc2.dumpPython()

#
# Python cfg Customization
# - cf: https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideAboutPythonConfigFile#Passing_Command_Line_Arguments_T
# - cf: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HLTrigger/Configuration/python/customL1T_Options.py?view=markup
# - iteralbles
#   * cf: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/SavedFMa/pp/HLT/sw/hlt/timing/test/bryanTimer.py?view=markup
#

# === Addition of Customization ===
print "\nBegin My Customization\n"
# command line inputs
import sys
print "cmssw command line arguements:",sys.argv
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('standard')
# set default values
options.maxEvents = -1
options.output = 'default_output.root'
# get and parse the command line arguments
options.parseArguments()

# now define the customization
def mycustomise(process):
  # process.options
  process.options = cms.untracked.PSet(
      wantSummary = cms.untracked.bool(True)
      )
  # process.source
  runInput=False
  for i in sys.argv:
    if ("files" in i) or("files_load" in i):
      runInput=True
      break
  if (runInput):
    process.source.fileNames = cms.untracked.vstring(options.files)
  # process.maxEvents
  process.maxEvents = cms.untracked.PSet(
      input = cms.untracked.int32(options.maxEvents)
      )
  # process.output
  try:
    if ('default_output' not in options.output):
      process.output.fileName = cms.untracked.string(options.output)
  except:
    print "no output module with the given name"
  # done
  return(process)

# redefine process
process=mycustomise(process)

