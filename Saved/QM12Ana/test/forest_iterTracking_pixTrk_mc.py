import FWCore.ParameterSet.VarParsing as VarParsing

ivars = VarParsing.VarParsing('python')

ivars.register ('randomNumber',
                1,
                ivars.multiplicity.singleton,
                ivars.varType.int,
                "Random Seed")

ivars.randomNumber = 1
ivars.inputFiles = "file:/mnt/hadoop/cms/store/user/yetkin/MC_Production/Pythia200_HydjetDrum_mix01/RECO/set1_random10000_HydjetDrum_11.root"
ivars.outputFile = 'output.root'

ivars.parseArguments()

import FWCore.ParameterSet.Config as cms

process = cms.Process('hiForestAna2011')

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

#####################################################################################
# Input source
#####################################################################################

process.source = cms.Source("PoolSource",
 duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(ivars.inputFiles))

# Number of events we want to process, -1 = all events
process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(10))


#####################################################################################
# Load some general stuff
#####################################################################################

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff')
# Data Global Tag 44x 
#process.GlobalTag.globaltag = 'GR_P_V27::All'
# MC Global Tag 44x 
process.GlobalTag.globaltag = 'STARTHI44_V7::All'

process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',
                                        ignoreTotal=cms.untracked.int32(0),
                                        oncePerEventMode =
                                        cms.untracked.bool(False)
                                        )

# load centrality
from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)
process.HeavyIonGlobalParameters = cms.PSet(
	centralityVariable = cms.string("HFtowers"),
	nonDefaultGlauberModel = cms.string("Hydjet_Drum"),
	centralitySrc = cms.InputTag("hiCentrality")
	)
process.hiCentrality.pixelBarrelOnly = False

########### random number seed
process.RandomNumberGeneratorService.generator.initialSeed = ivars.randomNumber 
process.RandomNumberGeneratorService.multiPhotonAnalyzer = process.RandomNumberGeneratorService.generator.clone()

# EcalSeverityLevel ES Producer
process.load("RecoLocalCalo/EcalRecAlgos/EcalSeverityLevelESProducer_cfi")
process.load("RecoEcal.EgammaCoreTools.EcalNextToDeadChannelESProducer_cff")


#####################################################################################
# Define tree output
#####################################################################################
process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string(ivars.outputFile))

#####################################################################################
# Additional Reconstruction and Analysis
#####################################################################################
process.load("CmsHi.JetAnalysis.hcalNoise_cff")
# Define Analysis sequencues
process.load('CmsHi.JetAnalysis.EventSelection_cff')
process.load('CmsHi.JetAnalysis.ExtraGenReco_cff')
#process.load('CmsHi.JetAnalysis.ExtraTrackReco_cff')
process.load('CmsHi.JetAnalysis.ExtraPfReco_cff')
process.load('CmsHi.JetAnalysis.ExtraJetReco_cff')
process.load('CmsHi.JetAnalysis.ExtraEGammaReco_cff')
process.load('CmsHi.JetAnalysis.PatAna_cff')
process.load('CmsHi.JetAnalysis.JetAnalyzers_cff')
process.load('CmsHi.JetAnalysis.TrkAnalyzers_cff')
process.load('CmsHi.JetAnalysis.EGammaAnalyzers_cff')
process.load("MitHig.PixelTrackletAnalyzer.trackAnalyzer_cff")
process.anaTrack.trackPtMin = 0
process.anaTrack.useQuality = True
process.anaTrack.doSimTrack = False
process.anaTrack.simTrackPtMin = 0.4
process.anaTrack.doPFMatching = False
process.anaTrack.pfCandSrc = cms.InputTag("particleFlowTmp")
process.anaTrack.qualityString = cms.untracked.string("highPuritySetWithPV")
process.anaTrack.trackSrc = cms.InputTag("hiGeneralCaloMatchedTracks")
#pixel tracks
process.anaPixTrack = process.anaTrack.clone(useQuality = False,
                                             doPFMatching = False,
                                             trackSrc = cms.InputTag("hiConformalPixelTracks")
                                             )

######## track efficiency calculator
process.load("edwenger.HiTrkEffAnalyzer.hitrkEffAnalyzer_cfi")
process.hitrkEffAnalyzer_GeneralCalo = process.hitrkEffAnalyzer.clone(
   tracks = cms.untracked.InputTag("hiGeneralCaloMatchedTracks"),
   qualityString = process.anaTrack.qualityString,
   hasSimInfo = True,
   ptBinScheme = 3,
   fiducialCut = False,
   usePxlPair = True
   )
process.hitrkEffAnalyzer_PixTrk = process.hitrkEffAnalyzer_GeneralCalo.clone(
   tracks = "hiConformalPixelTracks",
   label_tp_fake = "cutsTPForFakPxl",
   label_tp_effic = "cutsTPForEffPxl",
   useQaulityStr = False,
   usePxlPair = False
   )
##########################
process.load("CmsHi.JetAnalysis.pfcandAnalyzer_cfi")
process.pfcandAnalyzer.skipCharged = False
process.pfcandAnalyzer.pfPtMin = 0.5
process.interestingTrackEcalDetIds.TrackCollection = cms.InputTag("hiSelectedTracks")

process.genpana = cms.EDAnalyzer("GenParticleCounter",
                                 src = cms.untracked.string("hiGenParticles"),
                                 doCentrality = cms.untracked.bool(False),
                                 VertexProducer = cms.untracked.string("hiSelectedVertex")
                                 )

##################### Tracking Related
# for photon isolation
# Are we really using the highPurity tracks?
process.hiSelectedTrackHighPurity = cms.EDFilter("TrackSelector",
   src = cms.InputTag("hiSelectedTracks"),
   cut = cms.string(
   'quality("highPurity")')
)
# Iterative Tracking
# https://twiki.cern.ch/twiki/bin/view/CMS/HiTracking
process.load("RecoHI.HiTracking.hiIterTracking_cff")
# Calo Matched
process.load("RecoHI.HiTracking.HICaloCompatibleTracks_cff")
process.hiGeneralCaloMatchedTracks = process.hiCaloCompatibleTracks.clone(
    srcTracks = 'hiGeneralTracks',
    keepAllTracks = False
    )
# Conformal Pixel Tracks
process.load('Appeltel.PixelTracksRun2010.HiLowPtPixelTracksFromReco_cff')
process.load('Appeltel.PixelTracksRun2010.HiMultipleMergedTracks_cff')

##################### Particle Flow
process.particleFlowTmp.postMuonCleaning = False
process.particleFlowClusterPS.thresh_Pt_Seed_Endcap = cms.double(99999.)

process.pfTrack.TrackQuality = cms.string('loose') # To recover tracks before calo matching
process.pfTrack.TkColList = cms.VInputTag("hiGeneralTracks") # needed to run the calo matching on hiGeneralTracks


##################### Event Tree Analyzers
process.load("CmsHi/HiHLTAlgos.hievtanalyzer_cfi")
#process.hiEvtAnalyzer.doMC = cms.bool(True) # Not working for the moment..
process.hiEvtAnalyzer.doEvtPlane = cms.bool(True)

process.hiGenParticles.srcVector = cms.vstring('hiSignal')
process.icPu5JetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")
process.akPu3PFJetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")
process.akPu5PFJetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")
process.akPu3CaloJetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")
process.akPu5CaloJetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")
process.multiPhotonAnalyzer.GenEventScale = cms.InputTag("hiSignal")

process.icPu5JetAnalyzer.hltTrgResults = cms.untracked.string('TriggerResults::RECO')
process.akPu3PFJetAnalyzer.hltTrgResults = cms.untracked.string('TriggerResults::RECO')
process.icPu5JetAnalyzer.isMC   = cms.untracked.bool(True)
process.akPu3PFJetAnalyzer.isMC = cms.untracked.bool(True)

process.HiGenParticleAna = cms.EDAnalyzer('HiGenAnalyzer',
    useHepMCProduct = cms.untracked.bool(False),
    ptMin = cms.untracked.double(0),
    chargedOnly = cms.untracked.bool(True),
    src = cms.untracked.InputTag("hiSignal"),
    genpSrc = cms.untracked.InputTag("hiGenParticles"),
    genHiSrc = cms.untracked.InputTag("heavyIon"),
    )
    
##################### Final Paths
process.reco_extra        = cms.Path( process.siPixelRecHits * process.siStripMatchedRecHits *
                                      process.heavyIonTracking *
                                      process.hiSelectedTrackHighPurity *
                                      process.hiIterTracking *
                                      process.electronGsfTrackingHi *
                                      process.hiElectronSequence *
                                      process.HiParticleFlowReco *
                                      process.hiGeneralCaloMatchedTracks *
                                      process.PFTowers *
                                      process.conformalPixelTrackReco
                                      )
process.reco_extra_jet    = cms.Path( process.iterativeConePu5CaloJets *
                                      process.akPu3PFJets * process.akPu5PFJets *
                                      process.akPu3CaloJets * process.akPu5CaloJets *
                                      process.photon_extra_reco)
process.gen_step          = cms.Path( process.hiGenParticles * process.hiGenParticlesForJets * process.genPartons * process.hiPartons * process.hiRecoGenJets)
process.pat_step          = cms.Path(process.icPu5patSequence +
                                     process.akPu3PFpatSequence + process.akPu5PFpatSequence +
                                     process.akPu3patSequence + process.akPu5patSequence +
                                     process.makeHeavyIonPhotons)
process.pat_step.remove(process.interestingTrackEcalDetIds)
process.photonMatch.matched = cms.InputTag("hiGenParticles")
#process.pat_step.remove(process.photonMatch)
#+ process.patPhotons)

process.patPhotons.addPhotonID = cms.bool(False)
#process.makeHeavyIonPhotons)
process.extrapatstep = cms.Path(process.selectedPatPhotons)

process.multiPhotonAnalyzer.GammaEtaMax = cms.untracked.double(100)
process.multiPhotonAnalyzer.GammaPtMin = cms.untracked.double(0)
process.ana_step          = cms.Path( process.genpana +
                                      process.hcalNoise +
                                      process.jetana_seq +                                      
                                      process.multiPhotonAnalyzer +
                                      ( process.cutsTPForFak * process.cutsTPForEff * process.hitrkEffAnalyzer_GeneralCalo * process.anaTrack) +
                                      ( process.cutsTPForFakPxl * process.cutsTPForEffPxl * process.hitrkEffAnalyzer_PixTrk * process.anaPixTrack ) +
                                      process.pfcandAnalyzer +
                                      process.hiEvtAnalyzer +
                                      process.HiGenParticleAna
                                      )


process.pcollisionEventSelection = cms.Path(process.collisionEventSelection)
process.pHBHENoiseFilter = cms.Path( process.HBHENoiseFilter )

# Customization
from CmsHi.JetAnalysis.customise_cfi import *
setPhotonObject(process,"cleanPhotons")

process.load('CmsHi.HiHLTAlgos.hltanalysis_cff')

#process.hltanalysis.hltresults = cms.InputTag("TriggerResults","","RECO")
#process.hltAna = cms.Path(process.hltanalysis)
process.pAna = cms.EndPath(process.skimanalysis)

#####################################################################################
# Edm Output
#####################################################################################

#process.out = cms.OutputModule("PoolOutputModule",
#                               fileName = cms.untracked.string("output.root")
#                               )
#process.save = cms.EndPath(process.out)
