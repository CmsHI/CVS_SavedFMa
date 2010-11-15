import FWCore.ParameterSet.Config as cms

from Saved.DiJetAna.dijetAna_cfi import *

# different jet reco - mc
djcalokt4 = djcalo.clone(
    jetsrc = cms.InputTag("kt4patJets"),
    #doFJL1Corr = cms.bool(True),
    refjetsrc = cms.InputTag("kt4patJets")
    )

djcaloak5 = djcalokt4.clone(
    jetsrc = cms.InputTag("ak5patJets"),
    refjetsrc = cms.InputTag("ak5patJets")
    )
