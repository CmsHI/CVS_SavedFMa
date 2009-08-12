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
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('standard')
# set default values
options.files = ''
options.maxEvents = -1
options.output = 'output.root'
# get and parse the command line arguments
options.parseArguments()

# now define the customization
def mycustomise(process):
   process.options.wantSummary = cms.untracked.bool(True)
   if (options.files == ''):
      process.source.fileNames = options.files
   process.maxEvents.input = options.maxEvents
   try:
      process.output.fileName = options.output
   except:
      print "no output module with the given name"
   return(process)

# redefine process
process=mycustomise(process)

