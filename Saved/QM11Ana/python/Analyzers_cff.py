import FWCore.ParameterSet.Config as cms

from MNguyen.InclusiveJetAnalyzer.inclusiveJetAnalyzer_cff import *
inclusiveJetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")

akPu3PFJetAnalyzer = inclusiveJetAnalyzer.clone(
    jetTag = 'akPu3PFpatJets',
    genjetTag = 'ak3HiGenJets'
    )

jetana_seq = cms.Sequence( inclusiveJetAnalyzer * akPu3PFJetAnalyzer)

#from MitHig.PixelTrackletAnalyzer
trkAnalyzer = cms.EDAnalyzer("TrackAnalyzer",
    trackSrc = cms.InputTag("hiGoodTracks"),
    vertexSrc = cms.vstring("hiSelectedVertex"),
    trackPtMin = cms.untracked.double(1.5)
    )

genpAnalyzer = cms.EDAnalyzer('HydjetAnalyzer',
    useHepMCProduct = cms.untracked.bool(False),
    ptMin = cms.untracked.double(1.5),
    src = cms.untracked.InputTag("hiSignal"),
    genpSrc = cms.untracked.InputTag("hiGenParticles"),
    genHiSrc = cms.untracked.InputTag("heavyIon"),
    )

trkana_seq = cms.Sequence( trkAnalyzer * genpAnalyzer)
