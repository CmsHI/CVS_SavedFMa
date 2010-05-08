import FWCore.ParameterSet.Config as cms

# Dijet Ana Modules
dijetAna_mc = cms.EDAnalyzer('DiJetAna',
    isMC = cms.untracked.bool(True),
    vtxsrc = cms.untracked.InputTag("hiSelectedVertex"),
    jetsrc = cms.untracked.InputTag("akPu5patJets"),
    trksrc = cms.untracked.InputTag("hiSelectedTracks"),
    jetEtaMax = cms.untracked.double(3.0),
    nVtxTrkCut = cms.untracked.int32(3),
    doJEC = cms.untracked.int32(3),
    nearJetPtMin = cms.untracked.double(40.0),
    awayJetPtMin = cms.untracked.double(40.0),
    anaJetType = cms.untracked.int32(2),
    refJetType = cms.untracked.int32(1),
    anaTrkType = cms.untracked.int32(2)
    )

genDijetAna_mc = dijetAna_mc.clone(
    jetsrc = cms.untracked.InputTag("ak5HiGenJets"),
    anaJetType = cms.untracked.int32(1)
    )

dijetAna_data = dijetAna_mc.clone(
    isMC = cms.untracked.bool(False)
    )

# Analysis Sequences
dijetAna_data_seq = cms.Sequence(
    dijetAna_data
    )

dijetAna_mc_seq = cms.Sequence(
    dijetAna_mc *
    genDijetAna_mc
    )
