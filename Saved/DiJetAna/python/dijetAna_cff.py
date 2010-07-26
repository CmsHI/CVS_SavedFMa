import FWCore.ParameterSet.Config as cms

from Saved.DiJetAna.dijetAna_cfi import *

# different jet reco
dijetAna_data_kt4 = dijetAna_data.clone(
    jetsrc = cms.untracked.InputTag("kt4patJets"),
    fillL1Corr = cms.untracked.bool(True),
    trksrc = cms.untracked.InputTag("allTracks"),
    anaTrkType = cms.untracked.int32(3)
    )

dijetAna_data_ak5 = dijetAna_data_kt4.clone(
    jetsrc = cms.untracked.InputTag("ak5patJets")
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
