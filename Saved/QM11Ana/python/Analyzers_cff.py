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

jetana_seq = cms.Sequence( inclusiveJetAnalyzer * akPu5PFJetAnalyzer * akPu3PFJetAnalyzer)

#from MitHig.PixelTrackletAnalyzer
trkAnalyzer = cms.EDAnalyzer("TrackAnalyzer",
    trackSrc = cms.InputTag("hiGoodTracks"),
    vertexSrc = cms.vstring("hiSelectedVertex"),
    trackPtMin = cms.untracked.double(1.5)
    )

genpAnalyzer = cms.EDAnalyzer('HiGenAnalyzer',
    useHepMCProduct = cms.untracked.bool(False),
    ptMin = cms.untracked.double(1.5),
    chargedOnly = cms.untracked.bool(True),
    src = cms.untracked.InputTag("hiSignal"),
    genpSrc = cms.untracked.InputTag("hiGenParticles"),
    genHiSrc = cms.untracked.InputTag("heavyIon"),
    )

# trk eff
from edwenger.HiTrkEffAnalyzer.hitrkEffAnalyzer_cff import *
hitrkEffAnalyzer_nt = hitrkEffAnalyzer.clone(
    neededCentBins = cms.untracked.vint32(0, 1, 3, 11, 19, 35),
    trkPtMin = 1.5,
    fillNtuples = True,
    jets = 'akPu3PFpatJets'
    )
hitrkEffAna.replace(hitrkEffAnalyzer,hitrkEffAnalyzer_nt)

# final trk ana seq
trkana_seq = cms.Sequence( trkAnalyzer * genpAnalyzer * (hitrkEffAna))
