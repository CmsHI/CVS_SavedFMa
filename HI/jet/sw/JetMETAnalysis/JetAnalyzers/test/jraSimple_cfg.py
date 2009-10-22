#
# Based on
# - http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/JetMETAnalysis/JetAnalyzers/python/JRA_Modules_cff.py?view=markup
#

######################################################################################
#
# PAT Sequence Script for Heavy Ion Testing
#

import FWCore.ParameterSet.Config as cms

process = cms.Process("ANA")

import FWCore.ParameterSet.VarParsing as VarParsing

ivars = VarParsing.VarParsing('standard')
ivars.files = ''
ivars.output = 'jratree.root'
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

process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff")
process.prefer("L2L3JetCorrectorIC5Calo")

#####################################################################################
# Jet Response Analyzer

process.caloJetToRef = cms.EDFilter("MatchRecToGen",
    srcGen = cms.InputTag("iterativeCone5GenJets"),
    srcRec = cms.InputTag("iterativeCone5CaloJets")
    )

process.corrCaloJetToRef = process.caloJetToRef.clone(
    srcRec = cms.InputTag("L2L3CorJetIC5Calo")
    )

import JetMETAnalysis.JetAnalyzers.JRA_Defaults_cff as Defaults
import JetMETAnalysis.JetAnalyzers.JRA_TreeDefaults_cff as Tree

Defaults.JetResponseParameters = Tree.JetResponseParameters

process.ic5caloUncorrected = cms.EDAnalyzer("JetResponseAnalyzer",
                             Defaults.JetResponseParameters,
                             srcRefToJetMap = cms.InputTag("caloJetToRef","gen2rec"),
                             srcRef = cms.InputTag("iterativeCone5GenJets"),
                             doHeavyIon = cms.untracked.bool(False)
                             )

process.ic5caloCorrected = cms.EDAnalyzer("JetResponseAnalyzer",
                                          Defaults.JetResponseParameters,
                                          srcRefToJetMap = cms.InputTag("corrCaloJetToRef","gen2rec"),
                                          srcRef = cms.InputTag("iterativeCone5GenJets"),
                                          doHeavyIon = cms.untracked.bool(False)
                                          )

process.jetAna = cms.Sequence(process.caloJetToRef*process.ic5caloUncorrected + process.corrCaloJetToRef*process.ic5caloCorrected)

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string(ivars.output)
                                   )


# My Efficiency analysis

process.uncorsignal = cms.EDAnalyzer('HeavyIonJetAnalyzer',
                           jetSrc = cms.InputTag('iterativeCone5CaloJets'),
                           doParticles = cms.untracked.bool(False),
                           doVertices = cms.untracked.bool(False)
                           )

process.gensignal = cms.EDAnalyzer('HeavyIonJetAnalyzer',
                          jetSrc = cms.InputTag('iterativeCone5GenJets'),
                          doParticles = cms.untracked.bool(True)
                          )

process.corsignal = cms.EDAnalyzer('HeavyIonJetAnalyzer',
                                   jetSrc = cms.InputTag('L2L3CorJetIC5Calo'),
                                   doParticles = cms.untracked.bool(False),
                                   doVertices = cms.untracked.bool(False)
                                   )

process.myAna = cms.Sequence(process.uncorsignal+process.gensignal+process.corsignal)

######################################################################################
# Output file
#

process.pcorr= cms.Path(process.L2L3CorJetIC5Calo)

process.pana = cms.Path(
    process.myAna +
    process.jetAna
)
