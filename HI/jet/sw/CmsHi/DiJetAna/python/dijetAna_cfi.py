import FWCore.ParameterSet.Config as cms

dijetAna = cms.EDAnalyzer('DiJetAna',
    isMC = cms.untracked.bool(True),
    vtxsrc = cms.untracked.InputTag("hiSelectedVertex"),
    jetsrc = cms.untracked.InputTag("akPu5patJets"),
    trksrc = cms.untracked.InputTag("hiSelectedTracks"),
    jetEtaMax = cms.untracked.double(5.0),
    nVtxTrkCut = cms.untracked.int32(3),
    doJEC = cms.untracked.int32(3),
    nearJetPtMin = cms.untracked.double(40.0),
    awayJetPtMin = cms.untracked.double(40.0),
    anaJetType = cms.untracked.int32(2),
    refJetType = cms.untracked.int32(1)
)
