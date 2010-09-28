# === HLT Ana Customization ===
from HLTrigger.HLTanalyzers.customise_cfi import *
defineExtraHlt(process)
defineExtraAna(process)
process.load("GeneratorInterface.HiGenCommon.HeavyIon_cff")
process.higen += process.heavyIon

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
    outputCommands = cms.untracked.vstring('drop *','keep *CaloTower*_*_*_*'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('HLT_HIActivityHF_Coincidence3')),
    fileName = cms.untracked.string('RelVal_HLT_HIon.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW-HLT'),
        filterName = cms.untracked.string('HI-Activity')
    )
)
# Path and EndPath definitions
process.out_step = cms.EndPath(process.output)
#
# Redefine schedule
process.schedule.remove(process.analyzeThis)
process.schedule.remove(process.HLTHIOutput)
process.schedule.remove(process.HLTAnalyzerEndpath)
process.schedule.extend([process.HLT_HIActivityHF_Coincidence3_2Hit])
#defineReco(process)
#process.schedule.extend([process.raw2digi_step])
process.schedule.extend([process.extra_reco_step])
process.schedule.extend([process.HLTAnalyzerEndpath])
process.schedule.extend([process.analyzeThis])
process.schedule.extend([process.extra_ana_step])
#process.schedule.extend([process.out_step])

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
      process.hltanalysis.RunParameters.HistogramFile=options.output.replace(".root","_ohtree.root")
      process.TFileService.fileName = options.output.replace(".root","_djana.root")
  except:
    print "no output module with the given name"
  # done
  return(process)

# redefine process
process=mycustomise(process)

