import FWCore.ParameterSet.Config as cms

# === MC Reco ===
dijetAna_mc = cms.EDAnalyzer('DiJetAna',
    isMC = cms.untracked.bool(True),
    fillL1Corr = cms.untracked.bool(False),                         
    genOnly = cms.untracked.bool(False),
    centFile = cms.string("CentralityTables.root"),
    centLabel = cms.string("HFhits20_DataJulyExercise_AMPT2760GeV_MC_37Y_V5_ZS_v0"),
    vtxsrc = cms.untracked.InputTag("hiSelectedVertex"),
    jetsrc = cms.untracked.InputTag("patJets"),
    refjetsrc = cms.untracked.InputTag("patJets"),
    trksrc = cms.untracked.InputTag("allTracks"),
    jetEtaMax = cms.untracked.double(3.0),
    nVtxTrkCut = cms.untracked.int32(3),
    JECLab1 = cms.string("abs"),
    JECLab2Nr = cms.string(""),
    JECLab2Aw = cms.string(""),
    # dphi cut
    nearJetPtMin = cms.untracked.double(20.0),
    awayJetPtMin = cms.untracked.double(20.0),
    djDPhiMin = cms.double(2.5),
    # trk cut
    trkPtMin = cms.untracked.double(0.7),
    anaJetType = cms.untracked.int32(2),
    refJetType = cms.untracked.int32(11),
    anaTrkType = cms.untracked.int32(3),
    # Take all centrality at the moment
    centBinBeg = cms.untracked.int32(0),
    centBinEnd = cms.untracked.int32(20)
    )

# === Data Reco ===
dijetAna_data = dijetAna_mc.clone(
    isMC = cms.untracked.bool(False),
    centLabel = cms.string("HFhits20_DataJulyExercise_AMPT2760GeV_MC_37Y_V5_ZS_v0"),
    trksrc = cms.untracked.InputTag("hiSelectedTracks"),
    anaTrkType = cms.untracked.int32(2),
    refJetType = cms.untracked.int32(-1),
    # Select all centralities for Data to first look before cut
    centBinBeg = cms.untracked.int32(0),
    centBinEnd = cms.untracked.int32(20)
    )


# === MC Gen+Reco ===
dijetAna_mc_genjet_trk = dijetAna_mc.clone(
    jetsrc = cms.untracked.InputTag("iterativeCone5HiGenJets"),
    anaJetType = cms.untracked.int32(1),
    refJetType = cms.untracked.int32(12)
    )

dijetAna_mc_calojet_genp = dijetAna_mc.clone(
    trksrc = cms.untracked.InputTag("hiGenParticles"),
    anaTrkType = cms.untracked.int32(0)
    )

dijetAna_mc_genjet_genp = dijetAna_mc.clone(
    jetsrc = cms.untracked.InputTag("iterativeCone5HiGenJets"),
    anaJetType = cms.untracked.int32(1),
    trksrc = cms.untracked.InputTag("hiGenParticles"),
    refJetType = cms.untracked.int32(12),
    anaTrkType = cms.untracked.int32(0)
    )
