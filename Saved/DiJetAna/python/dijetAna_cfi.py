import FWCore.ParameterSet.Config as cms

# === MC Reco ===
dijetAna_mc = cms.EDAnalyzer('DiJetAna',
    # Event source
    isMC = cms.bool(True),
    # Event Info
    vtxsrc = cms.InputTag("hiSelectedVertex"),
    hltsrc = cms.InputTag("TriggerResults","","HLT"),
    hltNames = cms.untracked.vstring(
      "HLT_HIMinBiasCalo",
      "HLT_HIJet35U",
      "HLT_HIPhoton15"
      ),
    # jet reco
    jetsrc = cms.InputTag("patJets"),
    anaJetType = cms.int32(2),
    jetEtaMax = cms.double(3.0),
    # jet energy correction
    JECLab1 = cms.string("abs"),
    doFJL1Corr = cms.bool(False), # currently on top of the pp default JEC
    # jet mc matching
    refjetsrc = cms.InputTag("patJets"), # careful: this should be same collection as anajets, if anajets is patjet
    refJetType = cms.int32(21),
    # di-jet reco
    djDPhiMin = cms.double(2.5),
    # trk selection
    nearJetPtMin = cms.double(-1), # Note: this is only for deciding whether to save the tracks in event
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
