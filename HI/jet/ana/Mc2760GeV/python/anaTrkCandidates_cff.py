import FWCore.ParameterSet.Config as cms

# analysis level light trk collection, once trk selection has been done
allTracks = cms.EDProducer("ConcreteChargedCandidateProducer",
    src = cms.InputTag("hiSelectedTracks"),
    particleType = cms.string('pi+')
    )

