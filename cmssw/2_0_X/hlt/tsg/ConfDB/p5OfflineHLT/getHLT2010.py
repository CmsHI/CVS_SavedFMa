#!/usr/bin/env python

import sys
import os
import commands
import getopt
import fileinput

def usage():
    print sys.argv[0]
    print "   A script to get 20X (2_0_10+) p5 timing cfg's from ConfDB and"
    print "   modify it with appropriate options and scripts to "
    print "   make it cmsRun out of box"
    print "Usage:"
    print "  ", sys.argv[0], " <ConfDBPath> <out name> <file:inputfile.root> [modification script]"
    print "Note:"
    print "   You need to modify the arguments to the modification script"
    print "   if you want non default arguments"
    sys.exit(1)

argc = len(sys.argv)
if argc == 1:
    usage()

dbName = sys.argv[1]
outName = sys.argv[2]
inputfile = sys.argv[3]
mod = sys.argv[4]

if os.path.exists(outName):
    print outName, "already exists - abort!"
else:
    # Initialize everything
    essources = "  " 
    esmodules = "  "
    modules   = "  "
#    services  = "--services -PrescaleService"
    services  = "  "
#    paths     = "--paths -AlCaOutput"
    paths     = "  "
    psets     = "  "

    myGet = "edmConfigFromDB       --orcoff --format Ascii --configName " + dbName + " " + essources + " " + esmodules + " " + modules + " " + services + " " + paths + " " + psets + " --input " + inputfile + " > " + outName
    print "command: ", myGet
    os.system(myGet)

    myMod = mod + " " + outName + " my" + outName + " 2"
    print "command: ", myMod
    os.system(myMod)

