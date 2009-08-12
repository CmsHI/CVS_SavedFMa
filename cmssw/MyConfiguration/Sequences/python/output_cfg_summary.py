def output_cfg_summary(process):
   print "======================= Configuration Summary ===================="
   print "  === source ==="
   print process.source
   print "  === verbosity ==="
   print "  categories:", process.MessageLogger.categories
   print "  destinations:", process.MessageLogger.destinations
   print "  === paths: ==="
   print process.paths
   print "  === endpaths: ==="
   print process.endpaths

output_cfg_summary(process)
