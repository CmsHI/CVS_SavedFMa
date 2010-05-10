import FWCore.ParameterSet.Config as cms

# Dijet Ana Modules
dijetAna_mc = cms.EDAnalyzer('DiJetAna',
    isMC = cms.untracked.bool(True),
    centFile = cms.string("CentralityTables.root"),
    centLabel = cms.string("HFhits20_MC_Hydjet2760GeV_MC_3XY_V24_v0"),
    vtxsrc = cms.untracked.InputTag("hiSelectedVertex"),
    jetsrc = cms.untracked.InputTag("akPu5patJets"),
    trksrc = cms.untracked.InputTag("allTracks"),
    jetEtaMax = cms.untracked.double(3.0),
    nVtxTrkCut = cms.untracked.int32(3),
    JECLab1 = cms.string("abs"),
    JECLab2Nr = cms.string(""),
    JECLab2Aw = cms.string(""),
    nearJetPtMin = cms.untracked.double(40.0),
    awayJetPtMin = cms.untracked.double(40.0),
    trkPtMin = cms.untracked.double(0.3),
    anaJetType = cms.untracked.int32(2),
    refJetType = cms.untracked.int32(1),
    anaTrkType = cms.untracked.int32(3),
    # HI Event Selection: top 30% centrality
    centBinBeg = cms.untracked.int32(0),
    centBinEnd = cms.untracked.int32(6)
    )

dijetAna_mc_JECRaw = dijetAna_mc.clone(
    JECLab1 = cms.string("raw")
    )

dijetAna_mc_JECL5 = dijetAna_mc.clone(
    JECLab1 = cms.string("had"),
    JECLab2Nr = cms.string("uds"),
    JECLab2Aw = cms.string("glu")
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
    dijetAna_mc_JECRaw *
    #dijetAna_mc_genjet_trk *
    dijetAna_mc_calojet_genp *
    dijetAna_mc_genjet_genp
    )
