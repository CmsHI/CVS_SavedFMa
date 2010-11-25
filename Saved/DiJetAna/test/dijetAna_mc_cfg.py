import FWCore.ParameterSet.Config as cms

process = cms.Process("DJAna")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Saved.DiJetAna.eventSelection_cff")
process.load("Saved.DiJetAna.ExtraReco_cff")
process.load("Saved.DiJetAna.dijetAna_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      'file:jetAnaSkim.root'
      )
    )

# ===== Top Level =====
process.GlobalTag.globaltag = "START39_V5HI::All"
isData=False
isDataEmbed=True

# ===== Centrality =====
from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)
process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFhits"),
    nonDefaultGlauberModel = cms.string("Hydjet_Bass_2760GeV"),
    centralitySrc = cms.InputTag("hiCentrality")
    )
if (isData or isDataEmbed):
  process.HeavyIonGlobalParameters.nonDefaultGlauberModel = cms.string("")



# === Sample specific configs ===
for i,m in enumerate([process.djcalo,
  process.djcalo_tower,
  process.djcalo_genp,
  process.djgen]):
  m.hltsrc = cms.InputTag("TriggerResults","","RECOMIX")
  print i, "hlt: ", m.hltsrc

anaOutName = "dj_%s.root" % (process.djcalo.jetsrc.value())
process.TFileService = cms.Service('TFileService',
    fileName = cms.string(anaOutName)
    )

# =============== Final Paths =====================
from Saved.DiJetAna.customise_cfi import *
# Event Selection
#enableTrigger(process,"Jet")
# HLT Ana
enableOpenHlt(process,process.dijetAna_seq,isData)
process.hltanalysis.l1GtReadoutRecord = cms.InputTag( 'gtDigis','',"RECOMIX")
process.hltanalysis.hltresults = cms.InputTag( 'TriggerResults','',"RECOMIX")
process.hltanalysis.HLTProcessName = "RECOMIX"

#process.iterativeConePu5CaloJetsJOC = process.iterativeConePu5CaloJets.clone(
#   subtractorName = cms.string( "JetOffsetCorrector" )
#)
#process.hltanalysisJOC = process.hltanalysis.clone(recjets = cms.InputTag("iterativeConePu5CaloJetsJOC"))
#process.dijetAna_seq*=process.hltanalysisJOC
enableRECO(process,"MC","HI")
#process.patJetCorrFactors.jetSource = "iterativeConePu5CaloJetsJOC"
#process.patJets.jetSource = "iterativeConePu5CaloJetsJOC"
#enablePp(process,"PpRECO") # options: "PpRECO", "HIRECO"
#enableData(process)
#enableDataFilter(process,"HI")
#process.eventSelection.remove(process.siPixelRecHits) # tmp fix for running on skim
#process.eventSelection.remove(process.hltPixelClusterShapeFilter) # tmp fix for running on skim

#process.djcaloJOC = process.djcalo.clone(
#    jetsrc="patJets",
#    nearJetPtMin = cms.double(80)
#    )
#process.dijetAna_seq*=process.djcaloJOC

# FJ
#process.dijetAna_seq*=process.djcaloic5
#process.dijetAna_seq*=process.djcaloak5
#process.dijetAna_seq*=process.djcalokt4

# For MB
#for m in [process.djcalo,process.djcalo_tower]:
#  m.nearJetPtMin = 40

# First look at data
process.dijetAna_seq.remove(process.djcalo_genp)
process.dijetAna_seq.remove(process.djgen)
process.djcalo.nearJetPtMin = 100
process.djcalo_tower.nearJetPtMin = 100

process.reco = cms.Path( process.eventSelection * process.dj_reco_extra )
process.ana  = cms.Path( process.eventSelection * process.dijetAna_seq )

process.schedule = cms.Schedule(process.reco,process.ana)
