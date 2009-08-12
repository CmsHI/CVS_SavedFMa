#
# - iteralbles
#   * cf: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/SavedFMa/pp/HLT/sw/hlt/timing/test/bryanTimer.py?view=markup
#
def output_cfg_summary(process):
   print "======================= Configuration Summary ===================="
   print "=== source ==="
   print "  ", process.source
   try:
      print "# events: ", process.maxEvents.input, "\n"
   except:
      print "no maxEvents"
   try:
      print "output: \n  ", process.output.fileName
      print "event content: \n  ", process.output.outputCommands
   except:
      print ""

   print "\n=== verbosity ==="
   print "  categories:", process.MessageLogger.categories
   print "  destinations:", process.MessageLogger.destinations

   # --Using the itervalues() in dict to step through the dict container--
   print "\n===All paths found in cfg:==="
   for iterable in process.paths.itervalues():
      print "  ", iterable.label(),": ", iterable
   print "\n===All endpaths found in  cfg:==="
   for iterable in process.endpaths.itervalues():
      print "  ", iterable.label(),": ", iterable

output_cfg_summary(process)
