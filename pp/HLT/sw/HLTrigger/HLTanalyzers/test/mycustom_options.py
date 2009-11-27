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
options.files = 'input0.root'
options.maxEvents = -1
options.output = 'default_output.root'
# get and parse the command line arguments
options.parseArguments()

# now define the customization
def mycustomise(process):
  # === event setup ===
  # GR09_E_V6 for runs 121324+
  process.GlobalTag.globaltag = "GR09_E_V6::All"
  if 'Level1MenuOverride' in process.__dict__:
    process.Level1MenuOverride.connect  = 'frontier://FrontierProd/CMS_COND_31X_L1T'
  # === report ===
  from L1Trigger.GlobalTriggerAnalyzer.l1GtTrigReport_cfi import l1GtTrigReport
  process.hltL1GtTrigReport = l1GtTrigReport
  process.hltL1GtTrigReport.L1GtRecordInputTag = cms.InputTag( 'hltGtDigis' )
  process.L1AnalyzerEndpath = cms.EndPath( process.hltL1GtTrigReport )

  # Define the analyzer modules
  process.load("HLTrigger.HLTanalyzers.HLTAnalyser_cfi")
  process.analyzeThis = cms.EndPath( process.hltanalysis )
  process.hltanalysis.l1GtObjectMapRecord="hltL1GtObjectMap::"+process.name_()
  process.hltanalysis.l1GtReadoutRecord="hltGtDigis::"+process.name_()
  process.hltanalysis.hltresults="TriggerResults::"+process.name_()
  process.hltanalysis.RunParameters.Debug = False

  # delete unwanted parts
  del process.PrescaleService
  del process.DQMOutput
  del process.HLTOutputBnotT0
  del process.HLTOutputB
  del process.HLTOutput
  del process.ESOutput
  del process.AlCaOutput
  del process.MONOutput

  # === custom job options ===
  # process.options
  process.options = cms.untracked.PSet(
      wantSummary = cms.untracked.bool(True)
      )
  # process.source
  if (options.files[0] != 'input0.root'):
    process.source.fileNames = cms.untracked.vstring(options.files)
  # process.maxEvents
  process.maxEvents = cms.untracked.PSet(
      input = cms.untracked.int32(options.maxEvents)
      )
  # process.output
  try:
    if ('default_output' not in options.output):
      process.hltanalysis.RunParameters.HistogramFile = cms.untracked.string(options.output)
  except:
    print "no output module with the given name"
  # done
  return(process)

# redefine process
process=mycustomise(process)

