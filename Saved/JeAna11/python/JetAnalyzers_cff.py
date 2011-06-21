import FWCore.ParameterSet.Config as cms

from MNguyen.InclusiveJetAnalyzer.inclusiveJetAnalyzer_cff import *
icPu5JetAnalyzer = inclusiveJetAnalyzer.clone()
icPu5JetAnalyzer.eventInfoTag = cms.InputTag("hiSignal")

akPu5PFJetAnalyzer = icPu5JetAnalyzer.clone(
    jetTag = 'akPu5PFpatJets',
    genjetTag = 'ak5HiGenJets'
    )

akPu3PFJetAnalyzer = icPu5JetAnalyzer.clone(
    jetTag = 'akPu3PFpatJets',
    genjetTag = 'ak3HiGenJets'
    )

jetana_seq = cms.Sequence( icPu5JetAnalyzer * akPu5PFJetAnalyzer * akPu3PFJetAnalyzer)
