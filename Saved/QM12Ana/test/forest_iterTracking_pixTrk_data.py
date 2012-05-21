import FWCore.ParameterSet.Config as cms

process = cms.Process('hiForestAna2012')

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

#####################################################################################
# High Level Setup
#####################################################################################
isData=True
saveAllGenParticles = True

import FWCore.ParameterSet.VarParsing as VarParsing
ivars = VarParsing.VarParsing('python')

ivars.register ('randomNumber',
                1,
                ivars.multiplicity.singleton,
                ivars.varType.int,
                "Random Seed")

ivars.randomNumber = 1
if isData:
   ivars.inputFiles = 'file:/mnt/hadoop/cms/store/user/velicanu/reco/96A31CE6-1921-E111-B606-00A0D1E953AA.root'
else:
   ivars.inputFiles = "file:/mnt/hadoop/cms/store/user/yetkin/MC_Production/Pythia200_HydjetDrum_mix01/RECO/set1_random10000_HydjetDrum_11.root"
ivars.outputFile = 'output.root'
ivars.maxEvents = -1

if not isData:
   ivars.parseArguments()

#####################################################################################
# Input source
#####################################################################################

process.source = cms.Source("PoolSource",
 duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring(ivars.inputFiles))

# Number of events we want to process, -1 = all events
process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(ivars.maxEvents))


#####################################################################################
# Event Filtering
#####################################################################################
process.load('CmsHi.JetAnalysis.EventSelection_cff')
if isData:
   process.ic5CaloL2Relative.algorithm = 'IC5Calo_2760GeV'
   process.ic5CaloL3Absolute.algorithm = 'IC5Calo_2760GeV'
   
   process.hltJet80 = process.hltJetHI.clone(HLTPaths = ["HLT_HIJet80_v*"])
   process.singleJetPt115Filter = cms.EDFilter("EtaEtMinCaloJetCountFilter",
                                               src = cms.InputTag("icPu5CaloJetsL2L3"),
                                               etMin = cms.double(115),
                                               etaMin = cms.double(-3),
                                               etaMax = cms.double(3),
                                               minNumber = cms.uint32(1)
                                               )
   
   process.superFilterSequence = cms.Sequence(process.hltJet80*process.collisionEventSelection*process.icPu5CaloJetsL2L3*process.singleJetPt115Filter)
   process.superFilterPath = cms.Path(process.superFilterSequence)

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
if isData:
   # Data Global Tag 44x 
   process.GlobalTag.globaltag = 'GR_P_V27A::All'
else:
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
	centralitySrc = cms.InputTag("hiCentrality")
	)
if not isData:
   process.hiCentrality.pixelBarrelOnly = False
   process.HeavyIonGlobalParameters.nonDefaultGlauberModel = cms.string("Hydjet_Drum")

#####################################################################################
# Define tree output
#####################################################################################
process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string(ivars.outputFile))

#####################################################################################
# Additional Reconstruction and Analysis
#####################################################################################
process.load("CmsHi.JetAnalysis.hcalNoise_cff")
process.load('CmsHi.JetAnalysis.ExtraGenReco_cff')
#process.load('CmsHi.JetAnalysis.ExtraTrackReco_cff')
process.load('CmsHi.JetAnalysis.ExtraPfReco_cff')
process.load('CmsHi.JetAnalysis.ExtraJetReco_cff')
process.load('CmsHi.JetAnalysis.ExtraEGammaReco_cff')
process.load('CmsHi.JetAnalysis.PatAna_cff')
process.load('CmsHi.JetAnalysis.JetAnalyzers_cff')
process.load('CmsHi.JetAnalysis.TrkAnalyzers_cff')

##################### Jet Related
process.akPu5PFJets.doAreaFastjet = False
process.akPu3PFJets.doAreaFastjet = False
process.iterativeConePu5CaloJets.doAreaFastjet = False
process.akPu5PFJets.doRhoFastjet = False
process.akPu3PFJets.doRhoFastjet = False

##################### Photon Related
# EcalSeverityLevel ES Producer
process.load("RecoLocalCalo/EcalRecAlgos/EcalSeverityLevelESProducer_cfi")
process.load("RecoEcal.EgammaCoreTools.EcalNextToDeadChannelESProducer_cff")

# for photon isolation
process.hiSelectedTrackHighPurity = cms.EDFilter("TrackSelector",
   src = cms.InputTag("hiSelectedTracks"),
   cut = cms.string(
   'quality("highPurity")')
)

##################### Tracking Related
# Iterative Tracking
# https://twiki.cern.ch/twiki/bin/view/CMS/HiTracking
process.load("RecoHI.HiTracking.hiIterTracking_cff")
# Calo Matched
process.load("RecoHI.HiTracking.HICaloCompatibleTracks_cff")
process.hiCaloCompatibleGeneralTracks = process.hiCaloCompatibleTracks.clone(srcTracks = 'hiGeneralTracks')
process.hiCaloCompatibleGeneralTracksQuality = cms.EDFilter("TrackSelector",
   src = cms.InputTag("hiCaloCompatibleGeneralTracks"),
   cut = cms.string(
   'quality("highPuritySetWithPV")')
)
process.hiGeneralTracksQuality = cms.EDFilter("TrackSelector",
   src = cms.InputTag("hiGeneralTracks"),
   cut = cms.string(
   'quality("highPurity")')
)

# Conformal Pixel Tracks
process.load('Appeltel.PixelTracksRun2010.HiLowPtPixelTracksFromReco_cff')
process.load('Appeltel.PixelTracksRun2010.HiMultipleMergedTracks_cff')
process.hiIterCaloPixelMergedTracks = process.hiGoodMergedTracks.clone(
    TrackProducer1  = "hiCaloCompatibleGeneralTracksQuality",
    TrackProducer2  = "hiConformalPixelTracks")

##################### Particle Flow
process.particleFlowTmp.postMuonCleaning = False
process.particleFlowClusterPS.thresh_Pt_Seed_Endcap = cms.double(99999.)

process.pfTrack.TrackQuality = cms.string('loose') # To recover tracks before calo matching
process.pfTrack.TkColList = cms.VInputTag("hiGeneralTracks") # needed to run the calo matching on hiGeneralTracks


##################### Pat
process.interestingTrackEcalDetIds.TrackCollection = cms.InputTag("hiSelectedTracks")
process.patPhotons.addPhotonID = cms.bool(False)
if (not isData):
   process.photonMatch.matched = cms.InputTag("hiGenParticles")

################################################################
# Analyzers
################################################################

##################### Event Tree Analyzers
process.load("CmsHi/HiHLTAlgos.hievtanalyzer_cfi")
#process.hiEvtAnalyzer.doMC = cms.bool(True) # Not working for the moment..
process.hiEvtAnalyzer.doEvtPlane = cms.bool(True)

##################### Jet Tree Analyzers
process.icPu5JetAnalyzer.isMC = cms.untracked.bool(False)
process.akPu3PFJetAnalyzer.isMC = cms.untracked.bool(False)
process.akPu5PFJetAnalyzer.isMC = cms.untracked.bool(False)
process.akPu3CaloJetAnalyzer.isMC = cms.untracked.bool(False)
process.akPu5CaloJetAnalyzer.isMC = cms.untracked.bool(False)

##################### Photon Tree Analyzers
process.load('CmsHi.JetAnalysis.EGammaAnalyzers_cff')
process.multiPhotonAnalyzer.GammaEtaMax = cms.untracked.double(100)
process.multiPhotonAnalyzer.GammaPtMin = cms.untracked.double(0)
process.multiPhotonAnalyzer.gsfElectronCollection = cms.untracked.InputTag("ecalDrivenGsfElectrons")

##################### Track Analyzers
process.load("MitHig.PixelTrackletAnalyzer.trackAnalyzer_cff")
process.anaTrack.trackPtMin = 0
process.anaTrack.useQuality = False
process.anaTrack.doSimTrack = False
process.anaTrack.simTrackPtMin = 0.4
process.anaTrack.doPFMatching = False
process.anaTrack.pfCandSrc = cms.InputTag("particleFlowTmp")
# process.anaTrack.qualityString = cms.untracked.string("highPuritySetWithPV")
# process.anaTrack.trackSrc = cms.InputTag("hiCaloCompatibleGeneralTracks")
process.anaTrack.qualityString = cms.untracked.string("highPurity")
process.anaTrack.trackSrc = cms.InputTag("hiGeneralTracks")
#pixel tracks
process.anaPixTrack = process.anaTrack.clone(useQuality = False,
                                             doPFMatching = False,
                                             trackSrc = cms.InputTag("hiConformalPixelTracks")
                                             )
#final merged tracks
process.anaMergeTrack = process.anaTrack.clone(useQuality = False,
                                             trackSrc = cms.InputTag("hiIterCaloPixelMergedTracks")
                                             )

######## track efficiency calculator
process.load("edwenger.HiTrkEffAnalyzer.hitrkEffAnalyzer_cfi")
process.hitrkEffAnalyzer_Selected = process.hitrkEffAnalyzer.clone(
   tracks = cms.untracked.InputTag("hiSelectedTrackHighPurity"),
   qualityString = "highPurity",
   hasSimInfo = True,
   ptBinScheme = 3,
   fiducialCut = False,
   usePxlPair = True
   )
process.hitrkEffAnalyzer_General = process.hitrkEffAnalyzer.clone(
   tracks = cms.untracked.InputTag("hiGeneralTracksQuality"),
   qualityString = process.anaTrack.qualityString,
   hasSimInfo = True,
   ptBinScheme = 3,
   fiducialCut = False,
   usePxlPair = True
   )
process.hitrkEffAnalyzer_GeneralCalo = process.hitrkEffAnalyzer.clone(
   tracks = cms.untracked.InputTag("hiCaloCompatibleGeneralTracks"),
   qualityString = "highPuritySetWithPV",
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
process.hitrkEffAnalyzer_MergeTrk = process.hitrkEffAnalyzer_GeneralCalo.clone(
   tracks = "hiIterCaloPixelMergedTracks",
   label_tp_fake = "cutsTPForFakPxl",
   label_tp_effic = "cutsTPForEffPxl",
   useQaulityStr = False
   )
############################################ Pf Analyzers
process.load("CmsHi.JetAnalysis.pfcandAnalyzer_cfi")
process.pfcandAnalyzer.skipCharged = False
process.pfcandAnalyzer.pfPtMin = 0.5

############################################ MC Info
if (not isData):
   process.icPu5JetAnalyzer.isMC = cms.untracked.bool(True)
   process.akPu3PFJetAnalyzer.isMC = cms.untracked.bool(True)
   process.akPu5PFJetAnalyzer.isMC = cms.untracked.bool(True)
   process.akPu3CaloJetAnalyzer.isMC = cms.untracked.bool(True)
   process.akPu5CaloJetAnalyzer.isMC = cms.untracked.bool(True)
   process.icPu5JetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")
   process.akPu3PFJetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")
   process.akPu5PFJetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")
   process.akPu3CaloJetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")
   process.akPu5CaloJetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")
   process.multiPhotonAnalyzer.GenEventScale = cms.InputTag("hiSignal")
   
   process.hiGenParticles.srcVector = cms.vstring('hiSignal')
   process.HiGenParticleAna = cms.EDAnalyzer('HiGenAnalyzer',
       useHepMCProduct = cms.untracked.bool(False),
       ptMin = cms.untracked.double(0.4),
       chargedOnly = cms.untracked.bool(True),
       src = cms.untracked.InputTag("hiSignal"),
       genpSrc = cms.untracked.InputTag("hiGenParticles"), # save just the signal genp
       genHiSrc = cms.untracked.InputTag("heavyIon"),
       )
   if saveAllGenParticles:
       process.HiGenParticleAna.genpSrc = cms.untracked.InputTag("hiGenParticles","","HISIGNAL")
   
   process.genpana = cms.EDAnalyzer("GenParticleCounter",
                                 src = cms.untracked.string("hiGenParticles"),
                                 doCentrality = cms.untracked.bool(False),
                                 VertexProducer = cms.untracked.string("hiSelectedVertex")
                                 )

##################### Final Paths
if (not isData):
   process.gen_step          = cms.Path( process.hiGenParticles * process.hiGenParticlesForJets * process.genPartons * process.hiPartons * process.hiRecoGenJets)
process.reco_extra        = cms.Path( process.siPixelRecHits * process.siStripMatchedRecHits *
                                      process.heavyIonTracking *
                                      process.hiSelectedTrackHighPurity *
                                      process.hiIterTracking * process.hiGeneralTracksQuality *
                                      process.electronGsfTrackingHi *
                                      process.hiElectronSequence *
                                      process.HiParticleFlowReco *
                                      ( process.hiCaloCompatibleGeneralTracks * process.hiCaloCompatibleGeneralTracksQuality ) *
                                      process.PFTowers *
                                      process.conformalPixelTrackReco * process.hiIterCaloPixelMergedTracks
                                      )
process.reco_extra_jet    = cms.Path( process.iterativeConePu5CaloJets *
                                      process.akPu3PFJets * process.akPu5PFJets *
                                      process.akPu3CaloJets * process.akPu5CaloJets *
                                      process.photon_extra_reco)
if (isData):
   process.pat_step          = cms.Path(process.icPu5patSequence_data +
                                        process.akPu3PFpatSequence_data + process.akPu5PFpatSequence_data +
                                        process.akPu3patSequence_data + process.akPu5patSequence_data +
                                        process.makeHeavyIonPhotons)
else:
   process.pat_step          = cms.Path(process.icPu5patSequence +
                                        process.akPu3PFpatSequence + process.akPu5PFpatSequence +
                                        process.akPu3patSequence + process.akPu5patSequence +
                                        process.makeHeavyIonPhotons)
process.extrapatstep = cms.Path(process.selectedPatPhotons)
process.ana_step          = cms.Path(
                                      process.hcalNoise +
                                      process.jetana_seq +                                      
                                      process.multiPhotonAnalyzer +
                                      ( process.anaTrack ) +
                                      ( process.anaPixTrack ) +
#                                       ( process.anaMergeTrack ) +
                                      process.pfcandAnalyzer +
                                      process.hiEvtAnalyzer
                                      )
##################### Customization
process.pat_step.remove(process.interestingTrackEcalDetIds)
from CmsHi.JetAnalysis.customise_cfi import *
setPhotonObject(process,"cleanPhotons")
if (isData):
   enableDataPat(process)
   enableDataAnalyzers(process)
   process.pat_step.remove(process.photonMatch)
else:
   process.ana_step.replace(process.anaTrack, (process.cutsTPForFak * process.cutsTPForEff * process.hitrkEffAnalyzer_Selected *  process.hitrkEffAnalyzer_General * process.hitrkEffAnalyzer_GeneralCalo * process.anaTrack))
   process.ana_step.replace(process.anaPixTrack, (process.cutsTPForFakPxl * process.cutsTPForEffPxl * process.hitrkEffAnalyzer_PixTrk * process.anaPixTrack))
#    process.ana_step.Replace(process.anaMergeTrack, (process.cutsTPForFakPxl * process.cutsTPForEffPxl * process.hitrkEffAnalyzer_MergeTrk * process.anaMergeTrack))
   process.ana_step+=(process.genpana + process.HiGenParticleAna)


process.pcollisionEventSelection = cms.Path(process.collisionEventSelection)
process.pHBHENoiseFilter = cms.Path( process.HBHENoiseFilter )
if (isData):
   process.phltJetHI = cms.Path( process.hltJetHI )
   process.phiEcalRecHitSpikeFilter = cms.Path(process.hiEcalRecHitSpikeFilter )

process.load('CmsHi.HiHLTAlgos.hltanalysis_cff')
if (not isData):
   process.hltanalysis.HLTProcessName = cms.string('RECO')
   process.hltanalysis.hltresults = cms.InputTag("TriggerResults","","RECO")
 else:
   process.hltAna = cms.Path(process.hltanalysis)
process.skimanalysis.hltresults = cms.InputTag("TriggerResults","",process.name_())
process.pAna = cms.EndPath(process.skimanalysis)

########### random number seed
if (not isData):
   process.RandomNumberGeneratorService.generator.initialSeed = ivars.randomNumber 
process.RandomNumberGeneratorService.multiPhotonAnalyzer = process.RandomNumberGeneratorService.generator.clone()

#####################################################################################
# Edm Output
#####################################################################################

#process.out = cms.OutputModule("PoolOutputModule",
#                               fileName = cms.untracked.string("output.root")
#                               )
#process.save = cms.EndPath(process.out)

if (isData):
   process.skimanalysis.superFilters = cms.vstring("superFilterPath")
   for path in process.paths:
          getattr(process,path)._seq = process.superFilterSequence*getattr(process,path)._seq
