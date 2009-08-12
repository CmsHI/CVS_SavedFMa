def output_cfg_summary(process):
   print "======================= Configuration Summary ===================="
   print "=== source ==="
   print "  ", process.source
   print "=== verbosity ==="
   print "  categories:", process.MessageLogger.categories
   print "  destinations:", process.MessageLogger.destinations
   # --Using the itervalues() in dict to step through the dict container--
   print "===All paths found in cfg:==="
   for iterable in process.paths.itervalues():
      print "  ", iterable.label(),": ", iterable
   print "\n===All endpaths found in  cfg:==="
   for iterable in process.endpaths.itervalues():
      print "  ", iterable.label(),": ", iterable

output_cfg_summary(process)
