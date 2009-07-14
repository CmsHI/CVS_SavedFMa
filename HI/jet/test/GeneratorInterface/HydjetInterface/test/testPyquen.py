import FWCore.ParameterSet.Config as cms

process = cms.Process("PYQUENCHECK")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("CmsHi.Utilities.HiGenParticles_cfi")


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000)
                                       )

process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',
                                        ignoreTotal=cms.untracked.int32(0),
                                        oncePerEventMode = cms.untracked.bool(False)
                                        )


process.source = cms.Source("EmptySource")

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
process.signal.embeddingMode = cms.bool(False)

# modifications to the default pyquen source (unquenched 100 GeV dijets)
process.signal.PythiaParameters.parameterSets = cms.vstring('pythiaDefault','pythiaJets','kinematics')
process.signal.PythiaParameters.kinematics = cms.vstring (
      "CKIN(3)=100",  #min pthat
      "CKIN(4)=9999", #max pthat
      "CKIN(7)=-2.",  #min rapidity
      "CKIN(8)=2."    #max rapidity
      )   
process.signal.doQuench = False

# ===Convert signal to GenParticles===
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
process.genParticles.src = cms.InputTag("signal")


process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string('31X.root')
                                   )

#------------------------------- Now do jets ----------------------------------
process.load("CmsHi.JetAnalysis.HiJetAnalysisModules_cff")

#------------------------------  Save and run -------------------------------
process.output = cms.OutputModule("PoolOutputModule",
      fileName = cms.untracked.string('pyquen_jets.root')
      )

process.save = cms.EndPath(process.output)

#process.p = cms.Path(process.generator*process.signal*process.mix*process.hiGenParticles*process.ana)
process.p = cms.Path(process.signal*process.genParticles)
process.genjets = cms.Path(process.signalJets)
process.save = cms.EndPath(process.output)

process.s = cms.Schedule(process.p,process.genjets,process.save)


