import FWCore.ParameterSet.Config as cms

# Pat Jets
from Saved.QM11Ana.ExtraJetReco_cff import *
from PhysicsTools.PatAlgos.patHeavyIonSequences_cff import *
patJets.jetSource  = cms.InputTag("iterativeConePu5CaloJets")
patJets.addBTagInfo         = False
patJets.addTagInfos         = False
patJets.addDiscriminators   = False
patJets.addAssociatedTracks = False
patJets.addJetCharge        = False
patJets.addJetID            = False
patJets.getJetMCFlavour     = False
patJets.addGenPartonMatch   = True
patJets.addGenJetMatch      = True
patJets.embedGenJetMatch    = True
patJets.embedGenPartonMatch = True
patJets.embedCaloTowers	    = False


# === icpu5 ===
# jec
icPu5corr = patJetCorrFactors.clone(
  src = cms.InputTag("iterativeConePu5CaloJets"),
  levels = cms.vstring('L2Relative','L3Absolute'),
  payload = cms.string('IC5Calo')
  )

# gen
icPu5clean = heavyIonCleanedGenJets.clone( src = cms.InputTag('iterativeCone5HiGenJets') )

# matching
patJetPartonMatch.matched = cms.InputTag("hiPartons")

icPu5match = patJetGenJetMatch.clone(
  src = cms.InputTag("iterativeConePu5CaloJets"),
  matched = cms.InputTag("icPu5clean")
  )

icPu5parton = patJetPartonMatch.clone(
  src = cms.InputTag("iterativeConePu5CaloJets")
)

# analyzer
icPu5patJets = patJets.clone(
  jetSource = cms.InputTag("iterativeConePu5CaloJets"),
  genJetMatch = cms.InputTag("icPu5match"),
  genPartonMatch = cms.InputTag("icPu5parton"),
  jetCorrFactorsSource = cms.VInputTag(cms.InputTag("icPu5corr"))
  )

# === akPu3PF ===
akPu3PFcorr = icPu5corr.clone(
  src = cms.InputTag("akPu3PFJets"),
  payload = cms.string('AK5PF')
  )
akPu3PFclean = heavyIonCleanedGenJets.clone( src = cms.InputTag('ak3HiGenJets') ) 
akPu3PFmatch = patJetGenJetMatch.clone(
  src = cms.InputTag("akPu3PFJets"),
  matched = cms.InputTag("akPu3PFclean")
  )
akPu3PFparton = patJetPartonMatch.clone(
  src = cms.InputTag("akPu3PFJets")
)
akPu3PFpatJets = patJets.clone(
  jetSource = cms.InputTag("akPu3PFJets"),
  genJetMatch = cms.InputTag("akPu3PFmatch"),
  genPartonMatch = cms.InputTag("akPu3PFparton"),
  jetCorrFactorsSource = cms.VInputTag(cms.InputTag("akPu3PFcorr"))
  )

# === sequences ===
icPu5patSequence = cms.Sequence(
  iterativeConePu5CaloJets *
  icPu5corr *
  icPu5clean * icPu5match * icPu5parton *
  icPu5patJets
  )

akPu3PFpatSequence = cms.Sequence(
  akPu3PFJets *
  akPu3PFcorr *
  akPu3PFclean * akPu3PFmatch * akPu3PFparton *
  akPu3PFpatJets
  )


runAllJets = cms.Sequence(
  PFTowers *
  icPu5patSequence *
  akPu3PFpatSequence
  )
