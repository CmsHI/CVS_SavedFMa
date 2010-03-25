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
from PhysicsTools.PatAlgos.tools.coreTools import *
removeMCMatching(process, ['All'])

# get the 900 GeV jet corrections
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJECSet( process, "900GeV")


process.out = cms.OutputModule("PoolOutputModule",
       outputCommands = cms.untracked.vstring('drop *_*_*_*','keep recoGenParticles_genParticles_*_*',
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
