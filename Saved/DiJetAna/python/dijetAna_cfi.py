import FWCore.ParameterSet.Config as cms

# === Full Reco ===
# Dijet Ana Modules
dijetAna_mc = cms.EDAnalyzer('DiJetAna',
    isMC = cms.untracked.bool(True),
    centFile = cms.string("CentralityTables.root"),
    centLabel = cms.string("HFhits20_MC_Hydjet2760GeV_MC_3XY_V24_v0"),
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
    djDPhiMin = cms.double(1.),
    # trk cut
    trkPtMin = cms.untracked.double(0.3),
    anaJetType = cms.untracked.int32(2),
    refJetType = cms.untracked.int32(11),
    anaTrkType = cms.untracked.int32(3),
    # HI Event Selection: top 30% centrality
    centBinBeg = cms.untracked.int32(0),
    centBinEnd = cms.untracked.int32(20)
    )

# === Data ===
dijetAna_data = dijetAna_mc.clone(
    isMC = cms.untracked.bool(False),
    centLabel = cms.string("HFhits20_DataJulyExercise_Hydjet2760GeV_MC_37Y_V5_NZS_v0"),
    trksrc = cms.untracked.InputTag("hiSelectedTracks"),
    anaTrkType = cms.untracked.int32(2),
    refJetType = cms.untracked.int32(-1),
    # Select all centralities for Data to first look before cut
    centBinBeg = cms.untracked.int32(0),
    centBinEnd = cms.untracked.int32(20)
    )


# === Systematic Studies ===

dijetAna_mc_genjet_trk = dijetAna_mc.clone(
    jetsrc = cms.untracked.InputTag("iterativeCone5HiGenJets"),
    anaJetType = cms.untracked.int32(1)
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
