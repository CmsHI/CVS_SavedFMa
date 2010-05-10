import FWCore.ParameterSet.Config as cms

# Dijet Ana Modules
dijetAna_mc = cms.EDAnalyzer('DiJetAna',
    isMC = cms.untracked.bool(True),
    centFile = cms.string("CentralityTables.root"),
    vtxsrc = cms.untracked.InputTag("hiSelectedVertex"),
    jetsrc = cms.untracked.InputTag("akPu5patJets"),
    trksrc = cms.untracked.InputTag("allTracks"),
    jetEtaMax = cms.untracked.double(3.0),
    nVtxTrkCut = cms.untracked.int32(3),
    doJEC = cms.untracked.int32(3),
    nearJetPtMin = cms.untracked.double(40.0),
    awayJetPtMin = cms.untracked.double(40.0),
    trkPtMin = cms.untracked.double(0.3),
    anaJetType = cms.untracked.int32(2),
    refJetType = cms.untracked.int32(1),
    anaTrkType = cms.untracked.int32(3)
    )

dijetAna_mc_genjet_trk = dijetAna_mc.clone(
    jetsrc = cms.untracked.InputTag("ak5HiGenJets"),
    anaJetType = cms.untracked.int32(1)
    )

dijetAna_mc_calojet_genp = dijetAna_mc.clone(
    trksrc = cms.untracked.InputTag("hiGenParticles"),
    anaTrkType = cms.untracked.int32(0)
    )

dijetAna_mc_genjet_genp = dijetAna_mc.clone(
    jetsrc = cms.untracked.InputTag("ak5HiGenJets"),
    anaJetType = cms.untracked.int32(1),
    trksrc = cms.untracked.InputTag("hiGenParticles"),
    anaTrkType = cms.untracked.int32(0)
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
    #dijetAna_mc_genjet_trk *
    dijetAna_mc_calojet_genp *
    dijetAna_mc_genjet_genp
    )
