# === HLT Ana Customization ===
process.GlobalTag.globaltag = 'START38_V9::All'
from HLTrigger.HLTanalyzers.customise_cfi import *
defineExtraHlt(process)
#defineReco(process)
defineExtraAna(process)

# analyzer customization
process.hltanalysis.hltresults = cms.InputTag("TriggerResults::"+process.process)
process.hltanalysis.HLTProcessName = process.process

process.TFileService = cms.Service('TFileService',
    fileName = cms.string("dijetAna.root")
    )

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    #outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    outputCommands = cms.untracked.vstring('drop *','keep *_towerMaker_*_*'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('HLT_HIActivityHF_Coincidence3')),
    fileName = cms.untracked.string('RelVal_HLT_HIon.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW-HLT'),
        filterName = cms.untracked.string('HI-Activity')
    )
)
# Path and EndPath definitions
process.out_step = cms.EndPath(process.output)

# Redefine schedule
process.schedule = cms.Schedule( 
    process.higen,
    process.HLTriggerFirstPath,
    process.HLT_ZeroBiasPixel_SingleTrack,
    process.HLT_HIMinBiasBSC,
    process.HLT_HIMinBiasCalo,
    process.HLT_MinBiasPixel_SingleTrack,
    process.HLT_HIJet35U,
    process.HLT_HIPhoton15,
    process.HLT_L1DoubleMuOpen,
    process.HLT_HIUpcEcal,
    process.HLT_HIUpcMu,
    process.HLT_L1Tech_HCAL_HF,
    process.HLT_L1Tech_BSC_minBias,
    process.HLT_L1Tech_BSC_highmult,
    process.HLT_HIActivityPixels,
    process.HLT_HIActivityPixel_SingleTrack,
    process.HLT_HIActivityHF_Single3,
    process.HLT_HIActivityHF_Coincidence3,
    process.HLT_HIActivityHF_Coincidence3_2Hit,
    process.AlCa_HICentralityVeto,
    process.HLTriggerFinalPath,
    process.HLTAnalyzerEndpath
    )
process.schedule.extend([process.extra_reco_step,process.analyzeThis,process.extra_ana_step,process.out_step])

# print some config info
print "=== HLTHcalSimpleRecHitFilter cfg: ===\n"
print process.HLTHcalSimpleRecHitFilter.dumpPython()
print "=== HLTHcalSimpleRecHitFilter_Coinc1 cfg: ===\n"
print process.hltHcalSimpleRecHitFilterCoincidence.dumpPython()
print "=== HLTHcalSimpleRecHitFilter_Coinc2 cfg: ===\n"
print process.hltHcalSimpleRecHitFilterCoincidence2.dumpPython()

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

