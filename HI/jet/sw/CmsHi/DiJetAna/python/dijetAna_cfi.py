import FWCore.ParameterSet.Config as cms

dijetAna = cms.EDAnalyzer('DiJetAna',
    vtxsrc = cms.untracked.InputTag("hiSelectedVertex"),
    jetsrc = cms.untracked.InputTag("akPu5patJets"),
    trksrc = cms.untracked.InputTag("hiSelectedTracks"),
    isMC = cms.untracked.bool(True),
    jetEtaMax = cms.untracked.double(5.0),
    nVtxTrkCut = cms.untracked.int32(3),
    doJEC = cms.untracked.int32(3)
)
