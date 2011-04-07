import FWCore.ParameterSet.Config as cms

from Saved.QM11Ana.ExtraJetReco_cff import *
from Saved.QM11Ana.PatAna_cff import *

# Gen
from PhysicsTools.HepMCCandAlgos.HiGenParticles_cfi import *
from RecoJets.Configuration.GenJetParticles_cff import *
from RecoHI.HiJetAlgos.HiGenJets_cff import *
from RecoHI.HiJetAlgos.HiGenCleaner_cff import *

#hiGenParticles.srcVector = cms.vstring('hiSignal')

hiGen = cms.Sequence(
  heavyIon * # GenHIEventProducer
  #hiGenParticles *
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

hiextraTrackReco = cms.Sequence(
    hiGoodTracksSelection
    )
