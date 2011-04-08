import FWCore.ParameterSet.Config as cms

djcalo = cms.EDAnalyzer('DiJetAna',
    # Event source
    isMC = cms.bool(True),
    sampleType = cms.int32(0), # HI: 0, pp: 10
    # Event Info
    vtxsrc = cms.InputTag("hiSelectedVertex"),
    hltsrc = cms.InputTag("TriggerResults","","HLT"),
    hltNames = cms.untracked.vstring(
      "HLT_HIMinBiasHfOrBSC_Core",
      "HLT_HIJet35U_Core",
      "HLT_HIJet50U_Core",
      "HLT_HIJet75U_Core",
      "HLT_HIJet90U_Core",
      "HLT_HIPhoton20_Core"
      ),
    # jet reco
    jetsrc = cms.InputTag("icPu5patJets"),
    anaJetType = cms.int32(2),
    jetEtaMax = cms.double(2.0),
    # jet energy correction
    JECLab1 = cms.string("L3Absolute"),
    doFJL1Corr = cms.bool(False), # currently on top of the pp default JEC
    # jet mc matching
    refjetsrc = cms.InputTag("icPu5patJets"), # careful: this should be same collection as anajets, if anajets is patjet
    refJetType = cms.int32(21),
    # di-jet reco
    djDPhiMin = cms.double(3.14159/30.),
    # trk selection
    nearJetPtMin = cms.double(200), # Note: this is only for deciding whether to save the tracks in event
    trksrc = cms.InputTag("hiGoodMergedTracks"), # 0 genp, 2 recoTrk, 3 chcandidate, 4 pfcand
    anaTrkType = cms.int32(2),
    trkPtMin = cms.double(0.15),
    anaGenpType = cms.int32(1),
    # debug
    verbosity = cms.untracked.int32(0) # 2 for debug
    )

djgen_trk = djcalo.clone(
    # jet reco
    jetsrc = cms.InputTag("iterativeCone5HiGenJets"),
    anaJetType = cms.int32(1),
    # jet mc matching
    refJetType = cms.int32(12)
    )

djcalo_genp = djcalo.clone(
    # trk selection
    trksrc = cms.InputTag("hiGenParticles"),
    anaTrkType = cms.int32(0)
    )

djgen = djcalo.clone(
    # jet reco
    jetsrc = cms.InputTag("iterativeCone5HiGenJets"),
    anaJetType = cms.int32(1),
    # jet mc matching
    refJetType = cms.int32(12),
    # trk selection
    trksrc = cms.InputTag("hiGenParticles"),
    anaTrkType = cms.int32(0)
    )

djcalo_tower = djcalo.clone(
    # trk selection
    anaTrkType = cms.int32(3),
    trksrc = cms.InputTag("towerMaker"),
    trkPtMin = cms.double(0.7)
    )

djcalo_pfcand = djcalo.clone(
    # trk selection
    anaTrkType = cms.int32(4),
    trksrc = cms.InputTag("particleFlow","")
    )

# trk ana
from edwenger.HiTrkEffAnalyzer.hitrkEffAnalyzer_cff import *

# Analysis Sequences
dijetAna_seq = cms.Sequence(
    djcalo *
    djcalo_tower *
    djcalo_pfcand *
    #djgen_trk *
    djcalo_genp *
    djgen
    )
