import FWCore.ParameterSet.Config as cms

#
# cf https://twiki.cern.ch/twiki/bin/view/CMS/HeavyIonEventEmbedding
#

process = cms.Process("MIX")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("CmsHi.Utilities.HiGenParticles_cfi")


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1)
                                       )

process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',
                                        ignoreTotal=cms.untracked.int32(0),
                                        oncePerEventMode = cms.untracked.bool(False)
                                        )


##################################################################################
# Pb+Pb Background Source
process.source = cms.Source('PoolSource',
      #fileNames = cms.untracked.vstring('file:///d00/yjlee/sample/hydjet_mb_2_2_4/1EA7C31D-83FB-DD11-8218-001C23BED6CA.root')
      fileNames = cms.untracked.vstring('dcache:/pnfs/cmsaf.mit.edu/hibat/cms/users/davidlw/HYDJET_Minbias_4TeV_31X/sim/HYDJET_Minbias_4TeV_seq11_31X.root')
      )
#
# Note:
#   We cannot do gen and mix together at one step b/c pyquen cannot have 2 instances
#   in one job
#   cf Yetkin
# Thus, we start from a hydjet file
process.source.fileNames = ['file:/home/frankma/scratch/HI/jet/cmssw310pre10/hydjet_gen.root']


#------------------------------- Signal: "signal" -------------------------------
# Make sure the random number generator types are consistent with standard
process.RandomNumberGeneratorService.signal = cms.PSet(process.RandomNumberGeneratorService.generator) # For 3_1_X
#process.RandomNumberGeneratorService.signal = cms.PSet(process.RandomNumberGeneratorService.theSource)
#process.RandomNumberGeneratorService.signalSIM = cms.PSet(process.RandomNumberGeneratorService.g4SimHits)
process.RandomNumberGeneratorService.signal.initialSeed = 4
#process.RandomNumberGeneratorService.signalSIM.initialSeed = 5


##################################################################################
# Generate Pyquen Signal
from GeneratorInterface.PyquenInterface.pyquenPythiaDefault_cff import *
import GeneratorInterface.PyquenInterface.pyquenDefault_cfi
process.signal = GeneratorInterface.PyquenInterface.pyquenDefault_cfi.generator.clone()
process.signal.embeddingMode = cms.bool(True)

# modifications to the default pyquen source (unquenched 100 GeV dijets)
process.signal.PythiaParameters.parameterSets = cms.vstring('pythiaDefault','pythiaJets','kinematics')
process.signal.PythiaParameters.kinematics = cms.vstring (
      "CKIN(3)=100",  #min pthat
      "CKIN(4)=9999", #max pthat
      "CKIN(7)=-2.",  #min rapidity
      "CKIN(8)=2."    #max rapidity
      )   
process.signal.doQuench = False

# ----------------------------- Do Embedding -----------------------------------
##################################################################################
# Embed Pyquen signal into Background source at SIM level
from CmsHi.Utilities.EventEmbedding_cff import *
process.mix=cms.EDProducer('HiEventEmbedder',
      genEventEmbeddingMixParameters,
      signalTag = cms.vstring("signal","signalSIM")
      )

# ---
process.ana = cms.EDAnalyzer('SubEventAnalyzer'
                             )

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string('31X.root')
                                   )

#------------------------------  Save and run -------------------------------
process.output = cms.OutputModule("PoolOutputModule",
            fileName = cms.untracked.string('hydjet_gen_mixed.root')
	          )

process.save = cms.EndPath(process.output)

#process.p = cms.Path(process.generator*process.signal*process.mix*process.hiGenParticles*process.ana)
process.p = cms.Path(process.signal*process.mix*process.hiGenParticles*process.ana)
process.save = cms.EndPath(process.output)



