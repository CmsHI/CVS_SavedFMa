import FWCore.ParameterSet.Config as cms

#process = cms.Process("HLTtimer")
import hlt.tsg.myp5_cmsRun_online_timingHLTV6 as myhlt
process = myhlt.process

process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32( 1000 ) )

# Bryan's timer
process.PathTimerService = cms.Service( "PathTimerService" )
process.timer = cms.EDProducer( "PathTimerInserter" )


#print process.hltDefaultOutput.fileName
process.hltDefaultOutput.fileName = "file:/tmp/HLTTimerOutput.root"
#process.hltDefaultOutput.outputCommands.append('keep HLTPerformanceInfo_*_*_*')
process.hltDefaultOutput.outputCommands = ['keep HLTPerformanceInfo_*_*_*']
print process.hltDefaultOutput.outputCommands
del process.hltDefaultOutput.SelectEvents

# Print out orginal paths
# cf
# - http://cmslxr.fnal.gov/lxr/source/L1Trigger/Configuration/python/patchToRerunL1Emulator.py
# - for listing all the attributes of a python object, use: dir([obj])
#   * cf http://docs.python.org/library/functions.html
# - for end path (python) class definition
#   * $CMSSW_RELEASE_BASE/python/FWCore/ParameterSet/SequenceTypes.py
print "All paths found in original cfg:"
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

