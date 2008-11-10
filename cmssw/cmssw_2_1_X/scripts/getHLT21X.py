#!/usr/bin/env python

import sys
import os
import commands
import getopt
import fileinput

def usage():
    print sys.argv[0]
    print "   A script to get 21X ~ HLT Dev cfg's from ConfDB and"
    print "   modify it with appropriate options and scripts to "
    print "   make it cmsRun out of box"
    print "Usage:"
    print "  ", sys.argv[0], " <ConfDBPath> <out name> <file:inputfile.root> [modification script]"
    print "Note:"
    print "   1. You need to modify the arguments to the modification script"
    print "   if you want non default arguments"
    print "   2. If you are using DAQ ConfDB you need to modify this script"
    print "      (to be automated in later improvements)"
    print "   3. If you are running on a local root file, don't forget to"
    print "      *prefix* 'file:' to inputfile.root"
    sys.exit(1)

argc = len(sys.argv)
if argc == 1:
    usage()

dbName = sys.argv[1]
outName = sys.argv[2]
inputfile = sys.argv[3]
myMod = ""
if argc == 5:
    mod = sys.argv[4]
    myMod = mod + " " + outName + " my" + outName + " 1000"

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

    myGet = "edmConfigFromDB       --format Ascii --configName " + dbName + " " + essources + " " + esmodules + " " + modules + " " + services + " " + paths + " " + psets + " --input " + inputfile + " > " + outName
    # if from DAQ ConfDB
    #myGet = "edmConfigFromDB       --orcoff --format Ascii --configName " + dbName + " " + essources + " " + esmodules + " " + modules + " " + services + " " + paths + " " + psets + " --input " + inputfile + " > " + outName
    print "getHLT command: ", myGet
    os.system(myGet)

    print "modification command: ", myMod
    os.system(myMod)

