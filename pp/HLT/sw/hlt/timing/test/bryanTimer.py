import FWCore.ParameterSet.Config as cms

#process = cms.Process("HLTtimer")
import hlt.hi.HIHLT_2_2_6_HLT as myhlt
process = myhlt.process

import os
l = os.listdir("/tmp/frankma/data/mb/")
l.sort()
#l[8:] = []
fileNames = []
for i in l:
   fileNames.append("file:/tmp/frankma/data/mb/" + i)
process.source.fileNames = fileNames
process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32( -1 ) )
print process.source.fileNames[:20]

# Bryan's timer
process.PathTimerService = cms.Service( "PathTimerService" )
process.timer = cms.EDProducer( "PathTimerInserter" )


#print process.hltDefaultOutput.fileName
process.out4A.fileName = "file:/tmp/HLTTimerOutput.root"
#process.hltDefaultOutput.outputCommands.append('keep HLTPerformanceInfo_*_*_*')
process.out4A.outputCommands = ['keep HLTPerformanceInfo_*_*_*']
print process.out4A.outputCommands
del process.out4A.SelectEvents

# Print out orginal paths
# cf
# - http://cmslxr.fnal.gov/lxr/source/L1Trigger/Configuration/python/patchToRerunL1Emulator.py
# - for listing all the attributes of a python object, use: dir([obj])
#   * cf http://docs.python.org/library/functions.html
# - cmssw end path in python
#   * is a dict
#     + cf help(process.outpath)
#   * class definition
#     + $CMSSW_RELEASE_BASE/python/FWCore/ParameterSet/SequenceTypes.py

# Using the itervalues() in dict to step through the dict container.
print "All paths found in original cfg:"
for iterable in process.paths.itervalues():
   print "  ", iterable.label(),": ", iterable
print "All endpaths found in original cfg:"
for iterable in process.endpaths.itervalues():
   print "  ", iterable.label(),": ", iterable
   #del iterable

#process.HLTAnalyzerEndpath = cms.EndPath( process.timer + process.hltL1gtTrigReport + process.hltTrigReport )
# appearantly Bryan's timer executable would not work if the order of path definition 
# is switched ...
process.HLTAnalyzerEndpath.replace(process.hltL1gtTrigReport,process.timer)

# Print out some useful info
myep=process.HLTAnalyzerEndpath
print "Modified path:"
print "  ", myep.label(),": ", myep

