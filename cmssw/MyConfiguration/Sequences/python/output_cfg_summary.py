#
# - iteralbles
#   * cf: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/SavedFMa/pp/HLT/sw/hlt/timing/test/bryanTimer.py?view=markup
#
# - string operations
#   * cf http://docs.python.org/library/stdtypes.html
#
def output_cfg_summary(process):
   print "================ Configuration Summary for: ",process.name_(),"==============="
   print "=== source cfg head: ==="
#   sourcelines=process.source.dumpPython().split('\n')
#   for i in sourcelines[:20]:
#      print i
   print process.source.dumpPython()[:1000]
   print "\n=== Globaltag: ==="
   print process.GlobalTag.globaltag

   print "\n=== Options ==="
   try:
      print "  # events: ", process.maxEvents.input, "\n"
   except:
      print "  no maxEvents\n"
   try:
      print "  output: ", process.output.fileName, "\n"
      print "  event content: ", process.output.outputCommands, "\n"
   except:
      print ""
   try:
      print "  options: ", process.options, "\n"
   except:
      print "no process.options defined in this cfg"

   print "\n=== verbosity ==="
   try:
      print "  categories:", process.MessageLogger.categories
      print "  destinations:", process.MessageLogger.destinations
   except:
      print "cfg has no MessageLogger definition"

   # --Using the itervalues() in dict to step through the dict container--
   print "\n===All paths found in cfg:==="
   for iterable in process.paths.itervalues():
      print "  ", iterable.label(),": ", iterable
   print "\n===All endpaths found in  cfg:==="
   for iterable in process.endpaths.itervalues():
      print "  ", iterable.label(),": ", iterable

   # final schedule
   print "\n===Final Schedule of cfg:==="
   try:
      for i in process.schedule:
	 print "  ", i.label(), ": ", i
   except:
      print "*** Attention ***: No schedule defined in cfg"

output_cfg_summary(process)
