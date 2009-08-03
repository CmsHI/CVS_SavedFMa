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
options.files = 'file:input.root'
options.maxEvents = 100
options.output = 'output.root'
# get and parse the command line arguments
options.parseArguments()

# now define the customization
def mycustomise(process):
#   process.options.wantSummary = cms.untracked.bool(True)
   process.source.fileNames = options.files
   process.maxEvents.input = options.maxEvents
   process.output.fileName = options.output
   return(process)

# redefine process
process=mycustomise(process)

# === print out summary of cfg ===
print "===Summary of Final Configuration==="
print "input: \n  ", process.source.fileNames
print "# events: \n  ", process.maxEvents.input
print "output: \n  ", process.output.fileName
print "event content: \n  ", process.output.outputCommands
# Using the itervalues() in dict to step through the dict container.
print "All paths found in cfg:"
for iterable in process.paths.itervalues():
   print "  ", iterable.label(),": ", iterable

print "\nAll endpaths found in  cfg:"
for iterable in process.endpaths.itervalues():
   print "  ", iterable.label(),": ", iterable

