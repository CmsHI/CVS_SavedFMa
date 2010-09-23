import FWCore.ParameterSet.Config as cms

def defineReco(process):
  process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
  process.load('Configuration.StandardSequences.MixingNoPileUp_cff')
  process.load('Configuration.StandardSequences.RawToDigi_cff')
  process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
  process.raw2digi_step = cms.EndPath(process.RawToDigi)
  #process.reconstruction_step = cms.EndPath(process.reconstructionHeavyIons)

def defineExtraHlt(process):
  process.hltHcalSimpleRecHitFilterCoincidence2 = process.hltHcalSimpleRecHitFilterCoincidence.clone(
    minNHitsNeg = cms.int32(2),
    minNHitsPos = cms.int32(2),
    doCoincidence= cms.bool(True)
    )
  process.HLT_HIActivityHF_Coincidence3_2Hit = process.HLT_HIActivityHF_Coincidence3.expandAndClone();
  process.HLT_HIActivityHF_Coincidence3_2Hit.replace(process.hltHcalSimpleRecHitFilterCoincidence,process.hltHcalSimpleRecHitFilterCoincidence2)

def defineExtraAna(process):
  from Saved.DiJetAna.customise_cfi import enableRECO
  enableRECO(process,"Data")
  process.patJets.jetSource = cms.InputTag("hltIterativeCone5PileupSubtractionCaloJets")
  process.load("Saved.DiJetAna.dijetAna_cff")
  for i,m in enumerate([process.dijetAna_data_calojet_tower]):
    m.hltsrc = cms.InputTag("TriggerResults","",process.process)
    m.hltNames = ["HLT_HIMinBiasBSC","HLT_HIActivityHF_Single3","HLT_HIActivityHF_Coincidence3","HLT_HIActivityHF_Coincidence3_2Hit"]
    print i, "hlt: ", m.hltsrc
  process.dijetAna_data_calojet_tower.jetEtaMax = 5
  process.dijetAna_data_calojet_tower.vtxsrc = cms.InputTag( "hltHIPixelClusterVertices" )
  process.dijetAna_data_calojet_tower.trksrc = cms.InputTag( "hltTowerMakerForAll" )
  process.extra_reco_step = cms.Path(process.reco_extra)
  process.extra_ana_step = cms.EndPath(process.dijetAna_data_calojet_tower)

