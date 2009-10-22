######################################################################################
#
# PAT Sequence Script for Heavy Ion Testing
#

import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

################################################################################
# Input Variable Parsing
#
# This script uses input variable parsing, which means that you
# can change the input and output files, as well as max events
# from the command line rather than having to edit the file
#
# Usage:
#
# cmsRun pat_hi_sequence_cfg.py files=/path/to/input1.root output=file:output.root maxEvents=10
#
# You can use multiple files= options to read in multiple inputs
#


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

##################################################################################
#
# hiGenParticles - (Temporary)
#
# Currently not in production sequence, needed for pat::Jets
#

process.load("PhysicsTools.HepMCCandAlgos.HiGenParticles_cfi")

#process.hiGenParticles.src = cms.vstring("generator")
process.hiGenParticles.src = cms.vstring("signal","generator")

##################################################################################
#
#  RECO - re-run jets for 3_2_4
#

process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'MC_31X_V8::All'

process.load("RecoLocalCalo.Configuration.ecalLocalRecoSequence_cff")
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
process.load("RecoHI.HiEgammaAlgos.HiEgamma_cff")
process.load("RecoHI.HiJetAlgos.IterativeConePu5Jets_PbPb_cff")

# PAT

process.load("CmsHi.HiPatAlgos.HiPatSequences_cff")
process.heavyIon.doReco = False

process.patHIProductionSequence = cms.Sequence(
    process.heavyIon *
    process.hiPatJetSequence *
    process.selectedLayer1Jets
)


process.iterativeCone5HiGenJets.ignoreHydro = cms.untracked.bool(True)


#####################################################################################
# Jet Response Analyzer

process.caloJetToRef = cms.EDFilter("MatchRecToGen",
                            srcGen = cms.InputTag("hiGenJetCleaner"),
                            srcRec = cms.InputTag("iterativeConePu5CaloJets")
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
                             srcRef = cms.InputTag("hiGenJetCleaner"),
                             doHeavyIon = cms.untracked.bool(True)
                             )

process.jetAna = cms.Sequence(process.caloJetToRef * process.ic5calo)

process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string(ivars.output + ".hist")
                                   )


#######################################################################################
#
# VALIDATION
#


process.load("Validation.RecoJets.producers.GenJetClosure_cfi")

process.genJetClosure.recs = cms.InputTag('iterativeConePu5CaloJets')
process.genJetClosure.refs = cms.InputTag('hiGenJetCleaner')
process.genJetClosure.hist = cms.string(ivars.output + '.closure')

process.caloTest =  cms.EDAnalyzer('CaloJetTester',
                                   src = cms.InputTag('iterativeConePu5CaloJets'),
                                   srcGen = cms.InputTag('hiGenJetCleaner'),
                                   outputFile = cms.untracked.string(ivars.output + ".test"),
                                   genPtThreshold = cms.double(20),
                                   genEnergyFractionThreshold = cms.double(0.1),
                                   reverseEnergyFractionThreshold = cms.double(0.1),
                                   RThreshold = cms.double(0.3)
                                   )

DQMStore = cms.Service("DQMStore",
                       referenceFileName = cms.untracked.string(''),
                       verbose = cms.untracked.int32(0),
                       verboseQT = cms.untracked.int32(0),
                       collateHistograms = cms.untracked.bool(False)
                       )

#######################################################################################
# My Efficiency analysis

process.recoevent = cms.EDAnalyzer('HeavyIonJetAnalyzer',
                           jetSrc = cms.InputTag('iterativeConePu5CaloJets'),
                           doParticles = cms.untracked.bool(True),
                           doVertices = cms.untracked.bool(False)
                           )

process.genevent = cms.EDAnalyzer('HeavyIonJetAnalyzer',
                          jetSrc = cms.InputTag('iterativeCone5HiGenJets'),
                          doParticles = cms.untracked.bool(False)
                          )

process.myAna = cms.Sequence(process.recoevent+process.genevent)

######################################################################################
# Output file
#

process.output = cms.OutputModule("PoolOutputModule",
    process.hiPatEventContent,
    compressionLevel = cms.untracked.int32(2),
    commitInterval = cms.untracked.uint32(1),
    fileName = cms.untracked.string(ivars.output + ".pat")
)

savejets = cms.untracked.vstring('keep *Jet*_*_*_PAT')
process.output.outputCommands.extend(savejets)

process.pdigi = cms.Path(process.RawToDigi)

process.preco = cms.Path(
    process.ecalLocalRecoSequence *
    process.hcalLocalRecoSequence *
    process.hiEcalClusters *
    process.runjets
    )

process.pana = cms.Path(
    process.hiGenParticles *
    process.hiGenParticlesForJets *
    process.iterativeCone5HiGenJets *
    process.hiGenJetCleaner *
    process.myAna +
    process.jetAna +
    process.caloTest +
    process.genJetClosure + 
    process.patHIProductionSequence +
    process.fileSaver
)

process.out = cms.EndPath( process.output )
