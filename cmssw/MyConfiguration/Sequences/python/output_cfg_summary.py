#
# - iteralbles
#   * cf: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/SavedFMa/pp/HLT/sw/hlt/timing/test/bryanTimer.py?view=markup
#
def output_cfg_summary(process):
   print "======================= Configuration Summary ===================="
   print "=== source ==="
   print "  ", process.source
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
   print "  options: ", process.options, "\n"

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
