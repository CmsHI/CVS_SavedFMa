import FWCore.ParameterSet.Config as cms

from MNguyen.InclusiveJetAnalyzer.inclusiveJetAnalyzer_cff import *
inclusiveJetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")

akPu5PFJetAnalyzer = inclusiveJetAnalyzer.clone(
    jetTag = 'akPu5PFpatJets',
    genjetTag = 'ak5HiGenJets'
    )

akPu3PFJetAnalyzer = inclusiveJetAnalyzer.clone(
    jetTag = 'akPu3PFpatJets',
    genjetTag = 'ak3HiGenJets'
    )

# For backward xcheck
from Saved.DiJetAna.dijetAna_cfi import *
djicpu5 = djcalo.clone (
  hltNames = ['HLT_HIMinBiasHfOrBSC','HLT_HIJet35U', 'HLT_HIJet50U','HLT_HIJet75U'], # for allphysics
  jetsrc = "icPu5patJets",
  refjetsrc = "icPu5patJets",
  nearJetPtMin = 1000,
  trksrc = "hiGoodTracks",
  verbosity = 1
  )
djakpu3pf_pfcand = djicpu5.clone(
  jetsrc = "akPu3PFpatJets",
  refjetsrc = "akPu3PFpatJets",
  nearJetPtMin = 70,
  anaTrkType = cms.int32(4),
  trksrc = cms.InputTag("particleFlow",""),
  trkPtMin = 0.5
  )
djgenic5 = djgen.clone(
  refjetsrc = "icPu5patJets",
  nearJetPtMin = 1000
  )
djgenak3 = djgenic5.clone(
  jetsrc = "ak3HiGenJets",
  refjetsrc = "akPu3PFpatJets"
  )

#from MitHig.PixelTrackletAnalyzer
trkAnalyzer = cms.EDAnalyzer("TrackAnalyzer",
    trackSrc = cms.InputTag("hiGoodTracks"),
    vertexSrc = cms.vstring("hiSelectedVertex"),
    trackPtMin = cms.untracked.double(0.9),
    fiducialCut = cms.untracked.bool(True)
    )

genpAnalyzer = cms.EDAnalyzer('HiGenAnalyzer',
    useHepMCProduct = cms.untracked.bool(False),
    ptMin = cms.untracked.double(0.9),
    chargedOnly = cms.untracked.bool(True),
    src = cms.untracked.InputTag("hiSignal"),
    genpSrc = cms.untracked.InputTag("hiGenParticles"),
    genHiSrc = cms.untracked.InputTag("heavyIon"),
    )

# trk eff
from edwenger.HiTrkEffAnalyzer.hitrkEffAnalyzer_cff import *
hitrkEffAnalyzer_nt = hitrkEffAnalyzer.clone(
    neededCentBins = cms.untracked.vint32(0, 1, 3, 11, 19, 35),
    tracks = "hiGoodTracks",
    trkPtMin = 0.9, # for hiGoodTracks
    fiducialCut = True,
    fillNtuples = True,
    jets = 'akPu3PFpatJets'
    )
hitrkEffAna_nt = cms.Sequence(cutsTPForFak*cutsTPForEff*hitrkEffAnalyzer_nt)

# final sequences
trkcorr_seq = cms.Sequence( (hitrkEffAna_nt) )
trkana_seq = cms.Sequence( trkAnalyzer * genpAnalyzer )
trkana_seq_data = cms.Sequence( trkAnalyzer )
jetana_seq = cms.Sequence( inclusiveJetAnalyzer * akPu5PFJetAnalyzer * akPu3PFJetAnalyzer * djicpu5 * djakpu3pf_pfcand * djgenic5 * djgenak3)
jetana_seq_data = cms.Sequence( inclusiveJetAnalyzer * akPu5PFJetAnalyzer * akPu3PFJetAnalyzer * djicpu5 * djakpu3pf_pfcand )

