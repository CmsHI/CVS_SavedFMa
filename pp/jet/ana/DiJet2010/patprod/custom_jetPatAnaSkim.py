#cmdline: cat tmp.py /home/frankma/UserCode/SavedFMa/cmssw/MyConfiguration/Sequences/python/output_cfg_summary.py >> custom_jetPatAnaSkim.py
#cmdline: cat jetPatAnaSkim.py mySeq/mycustom_options.py >> custom_jetPatAnaSkim.py
# Run on RECO
#
# References
# - https://twiki.cern.ch/twiki/bin/viewauth/CMS/WorkBookPATExampleQCD
#   * UserCode/QCDPATExample
# - PhysicsTools/PatAlgos/python/patSequences_cff.py
# - And physTools hypernews!
# - https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePATFirstData
# - https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuidePATFAQs
# 

import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
process.source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( [
  # 342p1
  #'/store/data/BeamCommissioning09/MinimumBias/RAW-RECO/BSCNOBEAMHALO-Feb9Skim_v1/0030/FC575D7B-3918-DF11-A28F-00304867924E.root'
  # 355
  '/store/data/BeamCommissioning09/MinimumBias/RECO/Mar24thReReco_PreProduction_v2/0101/FEE4E809-DF37-DF11-B49D-002354EF3BDA.root'
       ] );


secFiles.extend( [
               ] )


# Starting with a skeleton process which gets imported with the following line
#from PhysicsTools.PatAlgos.patTemplate_cfg import *

# load the standard PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")


# note that you can use a bunch of core tools of PAT 
# to taylor your PAT configuration; for a few examples
# uncomment the following lines

#from PhysicsTools.PatAlgos.tools.coreTools import *
# extraction of jet sequences
process.load("PhysicsTools.PatAlgos.producersLayer1.jetProducer_cff")
process.load("PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi")

#addJetCollection(process,'iterativeCone5CaloJets','IC05CaloJetsl2l3',runCleaner=None,doJTA=False,doBTagging=False,jetCorrLabel=('IC5','Calo'),doType1MET=False,doL1Counters=False, doJetID = True, jetIdLabel = "ic5")

from PhysicsTools.PatAlgos.tools.jetTools import *

#addJetCollection(process,cms.InputTag('kt4CaloJets'),'KT4',
#                        doJTA=False,doBTagging=False,jetCorrLabel=('KT4','Calo'),doType1MET=False,doL1Cleaning=True,doL1Counters=True,
#                        genJetCollection=cms.InputTag("kt4GenJets"), doJetID = True, jetIdLabel = "kt4")

# cleaning up sequence - jet track association is set to false
# so here we remove modules that are dependent on it.
# also removing B-tagging modules as that has been set to false in the previous
# addJetCollection command

process.makePatJets.remove(process.patJetCharge)
#process.makePatJets.remove(process.secondaryVertexNegativeTagInfos)
#process.makePatJets.remove(process.simpleSecondaryVertexNegativeBJetTags)
process.patJets.addAssociatedTracks = cms.bool(False)
process.patJets.addJetCharge = cms.bool(False)
process.patJets.addBTagInfo = cms.bool(False)
process.patJets.addDiscriminators = cms.bool(False)

# turn off MC matching for the process
#from PhysicsTools.PatAlgos.tools.coreTools import *
#removeMCMatching(process, ['All'])

#get the 7 TeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "Summer09_7TeV")


process.out = cms.OutputModule("PoolOutputModule",
       outputCommands = cms.untracked.vstring('drop *_*_*_*','keep recoGenParticles_genParticles_*_*',
			        'keep *_ak5GenJets_*_*',
                                'keep *_genEventScale_*_*',
                                'keep *_genEventWeight_*_*',
                                'keep *_genEventPdfInfo_*_*',
                                'keep edmTriggerResults_TriggerResults_*_HLT',
                                'keep *_hltTriggerSummaryAOD_*_*',
                                'keep *_offlineBeamSpot_*_*',
                                'keep *_offlinePrimaryVertices_*_*',
                                'keep *_towerMaker_*_*',
                                'keep *_selectedPatJets*_*_*',
                                'keep *_patJets*_*_*'),
    
    fileName = cms.untracked.string('jetSkim.root')
)


# let it run
process.p = cms.Path(
  process.makePatJets *
  process.selectedPatJets
)

process.outpath = cms.EndPath(process.out)
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
import sys
print "cmssw command line arguements:",sys.argv
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('standard')
# set default values
options.maxEvents = -1
options.output = 'default_output.root'
# get and parse the command line arguments
options.parseArguments()

# now define the customization
def mycustomise(process):
  # process.options
  process.options = cms.untracked.PSet(
      wantSummary = cms.untracked.bool(True)
      )
  # process.source
  runInput=False
  for i in sys.argv:
    if ("files" in i) or("files_load" in i):
      runInput=True
      break
  if (runInput):
    process.source.fileNames = cms.untracked.vstring(options.files)
  # process.maxEvents
  process.maxEvents = cms.untracked.PSet(
      input = cms.untracked.int32(options.maxEvents)
      )
  # process.output
  try:
    if ('default_output' not in options.output):
      process.output.fileName = cms.untracked.string(options.output)
  except:
    print "no output module with the given name"
  # done
  return(process)

# redefine process
process=mycustomise(process)

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
   try:
     print process.GlobalTag.globaltag
   except:
     print "no Global tag used in cfg"

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
