import FWCore.ParameterSet.Config as cms

from Saved.QM11Ana.ExtraJetReco_cff import *
from Saved.QM11Ana.PatAna_cff import *

# Gen
from PhysicsTools.HepMCCandAlgos.HiGenParticles_cfi import *
from RecoJets.Configuration.GenJetParticles_cff import *
from RecoHI.HiJetAlgos.HiGenJets_cff import *
from RecoHI.HiJetAlgos.HiGenCleaner_cff import *

hiGenParticles.srcVector = cms.vstring('hiSignal')

hiGen = cms.Sequence(
  heavyIon * # GenHIEventProducer
  hiGenParticles *
  hiGenParticlesForJets *
  hiRecoGenJets *
  genPartons *
  hiPartons
  )

# good track selection
from edwenger.HiTrkEffAnalyzer.TrackSelections_cff import *
# merge with pixel tracks
from Appeltel.PixelTracksRun2010.HiLowPtPixelTracksFromReco_cff import *
from Appeltel.PixelTracksRun2010.HiMultipleMergedTracks_cff import *

# hiHighPtTracks
hiTightGlobalPrimTracks = cms.EDFilter("TrackSelector",
    src = cms.InputTag("hiGlobalPrimTracks"),
    cut = cms.string('(ptError/pt)<0.06&&numberOfValidHits>=12')
    )

from RecoHI.HiTracking.HISelectedTracks_cfi import hiSelectedTracks
hiHighPtTracksNoTraj = hiSelectedTracks.clone(
    src="hiTightGlobalPrimTracks",
    copyTrajectories = False,
    d0_par2 = [9999,1],
    dz_par2 = [9999,1]
    )
hiHighPtTracks = cms.EDProducer("HiHackedAnalyticalTrackSelector",
                                src = cms.InputTag("hiGlobalPrimTracks"),
                                keepAllTracks = cms.bool(False), ## if set to true tracks failing this filter are kept in the output
                                beamspot = cms.InputTag("offlineBeamSpot"),
                                vertices = cms.InputTag("hiSelectedVertex"),
                                vtxNumber = cms.int32(0),
                                vtxTracks = cms.uint32(2),
                                vtxChi2Prob = cms.double(0),
                                copyTrajectories = cms.untracked.bool(True),
                                copyExtras = cms.untracked.bool(False),
                                qualityBit = cms.string("highPurity"),
                                min_relpterr = cms.double(0.06),
                                min_nhits = cms.uint32(12),
                                # adapted optimal cuts on chi2 and PV vertex compatibility
                                chi2n_par = cms.double(0.4),
                                res_par = cms.vdouble(0.003,0.001),
                                d0_par1 = cms.vdouble(99999.,0),
                                dz_par1 = cms.vdouble(99999.,0),
                                d0_par2 = cms.vdouble(99999.,1),
                                dz_par2 = cms.vdouble(99999.,1),
                                # if the adapted pv compatibiilty cuts are to be applied
                                applyAdaptedPVCuts = cms.bool(True),
                                # Impact parameter absolute cuts.
                                max_d0 = cms.double(100),
                                max_z0 = cms.double(100),
                                # Cuts on number of layers with hits/3D hists/lost hists.
                                minNumberLayers = cms.uint32(7),
                                minNumber3DLayers = cms.uint32(3),
                                maxNumberLostLayers = cms.uint32(999)
                                )


hiextraTrackReco = cms.Sequence(
    hiTightGlobalPrimTracks * hiHighPtTracksNoTraj
    * hiHighPtTracks
    * hiGoodTracksSelection
    )

# particle flow
from RecoHI.Configuration.Reconstruction_hiPF_cff import *
trackerDrivenElectronSeeds.TkColList = cms.VInputTag("hiGoodTracks")
HiParticleFlowRecoNoJets = cms.Sequence(
    particleFlowCluster
    * trackerDrivenElectronSeeds
    * particleFlowReco
    )

#  --- Track and muon reconstruction (needed to run particle fllow) ---
# pixel triplet tracking (HI Tracking)
from RecoLocalTracker.Configuration.RecoLocalTracker_cff import *
from RecoHI.Configuration.Reconstruction_HI_cff import *
from RecoHI.HiTracking.LowPtTracking_PbPb_cff import *
# Needed to produce "HcalSeverityLevelComputerRcd" used by CaloTowersCreator/towerMakerPF
from RecoLocalCalo.Configuration.hcalLocalReco_cff import *
#Track Reco
rechits = cms.Sequence(siPixelRecHits * siStripMatchedRecHits)
hiTrackReReco = cms.Sequence(rechits * heavyIonTracking)
