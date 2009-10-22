######################################################################################
#
# PAT Sequence Script for Heavy Ion Testing
#

import FWCore.ParameterSet.Config as cms

process = cms.Process("ANA")

import FWCore.ParameterSet.VarParsing as VarParsing

ivars = VarParsing.VarParsing('standard')
ivars.files = ''
ivars.output = 'HIPAT_output_full.root'
ivars.maxEvents = -1

ivars.parseArguments()

########################################################################################
# Input Files and max events
#
#
# Although MC data sets should not have duplicate event numbers, checking
# for duplicate events is turned off here for testing purposes.
#

process.source = cms.Source("PoolSource",
    inputCommands = cms.untracked.vstring('drop *',
      'keep *HepMCProduct*_*_*_*',
      'keep *_raw*_*_*'
      ),
    fileNames = cms.untracked.vstring( ivars.files ),
    duplicateCheckMode = cms.untracked.string( 'noDuplicateCheck' )
    )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(ivars.maxEvents)
)

########################################################################################
# Timing and memory services
#

process.Timing = cms.Service("Timing")

process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
                                        oncePerEventMode = cms.untracked.bool(True),
                                        ignoreTotal = cms.untracked.int32(0)
                                        )

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.load("Configuration.StandardSequences.Services_cff")

#####################################################################################
# Jet Response Analyzer

process.caloJetToRef = cms.EDFilter("MatchRecToGen",
    srcGen = cms.InputTag("iterativeCone5GenJets"),
    srcRec = cms.InputTag("iterativeCone5CaloJets")
    )

process.fileSaver = cms.EDFilter("JetFileSaver",
    OutputFile = cms.untracked.string(ivars.output + '.file')
    )

import JetMETAnalysis.JetAnalyzers.JRA_Defaults_cff as Defaults
import JetMETAnalysis.JetAnalyzers.JRA_TreeDefaults_cff as Tree

Defaults.JetResponseParameters = Tree.JetResponseParameters

process.ic5calo = cms.EDAnalyzer("JetResponseAnalyzer",
                             Defaults.JetResponseParameters,
                             srcRefToJetMap = cms.InputTag("caloJetToRef","gen2rec"),
                             srcRef = cms.InputTag("iterativeCone5GenJets"),
                             doHeavyIon = cms.untracked.bool(False)
                             )

process.jetAna = cms.Sequence(process.caloJetToRef * process.ic5calo)

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string(ivars.output + ".hist")
                                   )


# My Efficiency analysis

process.recoevent = cms.EDAnalyzer('HeavyIonJetAnalyzer',
                           jetSrc = cms.InputTag('iterativeCone5CaloJets'),
                           doParticles = cms.untracked.bool(False),
                           doVertices = cms.untracked.bool(False)
                           )

process.genevent = cms.EDAnalyzer('HeavyIonJetAnalyzer',
                          jetSrc = cms.InputTag('iterativeCone5GenJets'),
                          doParticles = cms.untracked.bool(True)
                          )

process.myAna = cms.Sequence(process.recoevent+process.genevent)

######################################################################################
# Output file
#
process.pana = cms.Path(
    #process.myAna +
    process.jetAna
)
