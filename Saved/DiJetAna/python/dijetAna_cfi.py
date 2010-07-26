import FWCore.ParameterSet.Config as cms

# === MC Reco ===
dijetAna_mc = cms.EDAnalyzer('DiJetAna',
    # Event source
    isMC = cms.untracked.bool(True),
    genOnly = cms.untracked.bool(False),
    # Event Selection
    centFile = cms.string("CentralityTables.root"),
    centLabel = cms.string("HFhits20_DataJulyExercise_AMPT2760GeV_MC_37Y_V5_ZS_v0"),
    centBinBeg = cms.untracked.int32(0),
    centBinEnd = cms.untracked.int32(20), # Take all centrality at the moment
    vtxsrc = cms.untracked.InputTag("hiSelectedVertex"),
    nVtxTrkCut = cms.untracked.int32(3),
    # jet reco
    jetsrc = cms.untracked.InputTag("patJets"),
    anaJetType = cms.untracked.int32(2),
    fillL1Corr = cms.untracked.bool(False),                         
    jetEtaMax = cms.untracked.double(3.0),
    # jet energy correction
    JECLab1 = cms.string("abs"),
    JECLab2Nr = cms.string(""),
    JECLab2Aw = cms.string(""),
    # jet mc matching
    refjetsrc = cms.untracked.InputTag("patJets"),
    refJetType = cms.untracked.int32(11),
    # di-jet reco
    nearJetPtMin = cms.untracked.double(20.0),
    awayJetPtMin = cms.untracked.double(20.0),
    djDPhiMin = cms.double(2.5),
    # trk selection
    trksrc = cms.untracked.InputTag("allTracks"),
    anaTrkType = cms.untracked.int32(3),
    trkPtMin = cms.untracked.double(0.7),
    # debug
    verbosity = cms.untracked.int32(1)
    )

# === Data Reco ===
dijetAna_data = dijetAna_mc.clone(
    # Event source
    isMC = cms.untracked.bool(False),
    # Event Selection
    centLabel = cms.string("HFhits20_DataJulyExercise_AMPT2760GeV_MC_37Y_V5_ZS_v0"),
    centBinBeg = cms.untracked.int32(0),
    centBinEnd = cms.untracked.int32(20),
    # no jet mc matching
    refJetType = cms.untracked.int32(-1),
    # trk selection
    trksrc = cms.untracked.InputTag("hiSelectedTracks"),
    anaTrkType = cms.untracked.int32(2)
    )


# === MC Gen+Reco ===
dijetAna_mc_genjet_trk = dijetAna_mc.clone(
    # jet reco
    jetsrc = cms.untracked.InputTag("iterativeCone5HiGenJets"),
    anaJetType = cms.untracked.int32(1),
    # jet mc matching
    refJetType = cms.untracked.int32(12)
    )

dijetAna_mc_calojet_genp = dijetAna_mc.clone(
    # trk selection
    trksrc = cms.untracked.InputTag("hiGenParticles"),
    anaTrkType = cms.untracked.int32(0)
    )

dijetAna_mc_genjet_genp = dijetAna_mc.clone(
    # jet reco
    jetsrc = cms.untracked.InputTag("iterativeCone5HiGenJets"),
    anaJetType = cms.untracked.int32(1),
    # jet mc matching
    refJetType = cms.untracked.int32(12),
    # trk selection
    trksrc = cms.untracked.InputTag("hiGenParticles"),
    anaTrkType = cms.untracked.int32(0)
    )
