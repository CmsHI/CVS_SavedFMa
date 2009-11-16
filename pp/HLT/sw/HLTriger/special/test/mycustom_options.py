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
# for calo threshold
options.register ('HfEMin',
   2.0, # default value
   VarParsing.VarParsing.multiplicity.singleton, # singleton or list
   VarParsing.VarParsing.varType.float,          # string, int, or float
   "HF Energy ith threshold for HLTHcalSimpleRecHitFilter"
)
options.register ('EcalEMin',
   5., # default value
   VarParsing.VarParsing.multiplicity.singleton, # singleton or list
   VarParsing.VarParsing.varType.float,          # string, int, or float
   "Ecal Energy ith threshold for Ecal Crystal"
)
options.register ('PixClustMin',
   1, # default value
   VarParsing.VarParsing.multiplicity.singleton, # singleton or list
   VarParsing.VarParsing.varType.int,          # string, int, or float
   "Min # of pixel clusters"
)

# get and parse the command line arguments
options.parseArguments()

# now define the customization
def mycustomise(process):
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
      process.output.fileName = cms.untracked.string(options.output)
  except:
    print "no output module with the given name"
  # HLTHcalSimpleRecHitFilter
  process.HLTHcalSimpleRecHitFilter.threshold = options.HfEMin
  process.hltHI1jet35U.MinPt = options.EcalEMin
  process.hltPixelActivityFilter.minClusters = options.PixClustMin
  print "===== Activity trigger thresholds ====="
  print "HLTHcalSimpleRecHitFilter.threshold: ", process.HLTHcalSimpleRecHitFilter.threshold
  print "Ecal Activity crystal threshold: ", process.hltHI1jet35U.MinPt
  print "Pixel Cluster min: ", process.hltPixelActivityFilter.minClusters
  # done
  return(process)

# redefine process
process=mycustomise(process)

