import FWCore.ParameterSet.Config as cms

from Saved.DiJetAna.dijetAna_cfi import *

# different jet reco - mc
dijetAna_mc_kt4 = dijetAna_mc.clone(
    jetsrc = cms.InputTag("kt4patJets"),
    applyAnaJEC = cms.int32(1),
    refjetsrc = cms.InputTag("kt4patJets"),
    trksrc = cms.InputTag("allTracks"),
    anaTrkType = cms.int32(3)
    )

dijetAna_mc_ak5 = dijetAna_mc_kt4.clone(
    jetsrc = cms.InputTag("ak5patJets"),
    refjetsrc = cms.InputTag("ak5patJets")
    )

# different jet reco - data
dijetAna_data_kt4 = dijetAna_data.clone(
    jetsrc = cms.InputTag("kt4patJets"),
    applyAnaJEC = cms.int32(1),
    trksrc = cms.InputTag("allTracks"),
    anaTrkType = cms.int32(3)
    )

dijetAna_data_ak5 = dijetAna_data_kt4.clone(
    jetsrc = cms.InputTag("ak5patJets")
    )

# different ana modes
evtAna_data = dijetAna_data.clone()
evtAna_data.evtAnaOnly = cms.untracked.bool(True)

# Analysis Sequences
dijetAna_data_seq = cms.Sequence(
    evtAna_data +
    dijetAna_data
    )

dijetAna_mc_seq = cms.Sequence(
    dijetAna_mc *
    #dijetAna_mc_genjet_trk *
    dijetAna_mc_calojet_genp *
    dijetAna_mc_genjet_genp
    )
