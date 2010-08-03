import FWCore.ParameterSet.Config as cms

# === MC Reco ===
dijetAna_mc = cms.EDAnalyzer('DiJetAna',
    # Event source
    isMC = cms.bool(True),
    # Event Selection
    centFile = cms.string("CentralityTables.root"),
    centLabel = cms.string("HFhits20_MC_Hydjet2760GeV_MC_3XY_V24_NoZS_v0"),
    centBinBeg = cms.int32(0),
    centBinEnd = cms.int32(20), # Take all centrality at the moment
    vtxsrc = cms.InputTag("hiSelectedVertex"),
    nVtxTrkCut = cms.int32(3),
    # jet reco
    jetsrc = cms.InputTag("patJets"),
    anaJetType = cms.int32(2),
    jetEtaMax = cms.double(3.0),
    # jet energy correction
    JECLab1 = cms.string("abs"),
    applyAnaJEC = cms.int32(0), # this is ana level JEC on top of the pp default JEC
    # jet mc matching
    refjetsrc = cms.InputTag("patJets"), # careful: this should be same collection as anajets, if anajets is patjet
    refJetType = cms.int32(21),
    # di-jet reco
    djDPhiMin = cms.double(2.5),
    # trk selection
    nearJetPtMin = cms.double(-1),
    trksrc = cms.InputTag("allTracks"),
    anaTrkType = cms.int32(3),
    trkPtMin = cms.double(0.7),
    # debug
    verbosity = cms.untracked.int32(2)
    )

# === Data Reco ===
dijetAna_data = dijetAna_mc.clone(
    # Event source
    isMC = cms.bool(False),
    # Event Selection
    centLabel = cms.string("HFhits20_DataJulyExercise_AMPT2760GeV_MC_37Y_V5_ZS_v0"),
    centBinBeg = cms.int32(0),
    centBinEnd = cms.int32(20),
    # no jet mc matching
    refJetType = cms.int32(-1),
    # trk selection
    trksrc = cms.InputTag("hiSelectedTracks"),
    anaTrkType = cms.int32(2)
    )


# === MC Gen+Reco ===
dijetAna_mc_genjet_trk = dijetAna_mc.clone(
    # jet reco
    jetsrc = cms.InputTag("iterativeCone5HiGenJets"),
    anaJetType = cms.int32(1),
    # jet mc matching
    refJetType = cms.int32(12)
    )

dijetAna_mc_calojet_genp = dijetAna_mc.clone(
    # trk selection
    trksrc = cms.InputTag("hiGenParticles"),
    anaTrkType = cms.int32(0)
    )

dijetAna_mc_genjet_genp = dijetAna_mc.clone(
    # jet reco
    jetsrc = cms.InputTag("iterativeCone5HiGenJets"),
    anaJetType = cms.int32(1),
    # jet mc matching
    refJetType = cms.int32(12),
    # trk selection
    trksrc = cms.InputTag("hiGenParticles"),
    anaTrkType = cms.int32(0)
    )
