import FWCore.ParameterSet.Config as cms

def defineReco(process):
  process.raw2digi_step = cms.Path(process.RawToDigi)
  process.reconstruction_step = cms.Path(process.reconstructionHeavyIons)

def defineExtraHlt(process):
  process.hltHcalSimpleRecHitFilterCoincidence2 = process.hltHcalSimpleRecHitFilterCoincidence.clone(
    minNHitsNeg = cms.int32(2),
    minNHitsPos = cms.int32(2),
    doCoincidence= cms.bool(True)
    )
  process.HLT_HIActivityHF_Coincidence3_2Hit = process.HLT_HIActivityHF_Coincidence3.expandAndClone();
  process.HLT_HIActivityHF_Coincidence3_2Hit.replace(process.hltHcalSimpleRecHitFilterCoincidence,process.hltHcalSimpleRecHitFilterCoincidence2)

def defineExtraAna(process):
  process.load("Saved.DiJetAna.dijetAna_cff")
  for i,m in enumerate([process.dijetAna_mc,
    process.dijetAna_mc_calojet_tower,
    process.dijetAna_mc_calojet_genp,
    process.dijetAna_mc_genjet_genp]):
    m.hltsrc = cms.InputTag("TriggerResults","",process.process)
    m.hltNames = ["HLT_HIMinBiasBSC","HLT_HIActivityHF_Single3","HLT_HIActivityHF_Coincidence3","HLT_HIActivityHF_Coincidence3_2Hit"]
    print i, "hlt: ", m.hltsrc
  process.dijetAna_mc.anaTrkType = cms.int32(3)
  from Saved.DiJetAna.customise_cfi import enableRECO
  enableRECO(process)
  process.extra_reco_step = cms.Path(process.reco_extra)
  process.extra_ana_step = cms.Path(process.dijetAna_mc_seq)

