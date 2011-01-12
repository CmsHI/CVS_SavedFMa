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
process.GlobalTag.globaltag = "GR10_P_V12::All"
isData=True

# ===== Centrality =====
from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)
process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFhits"),
    nonDefaultGlauberModel = cms.string("AMPT_organ_2760GeV"),
    centralitySrc = cms.InputTag("hiCentrality")
    )
if (isData):
  process.HeavyIonGlobalParameters.nonDefaultGlauberModel = cms.string("")



# === Sample specific configs ===
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
#enableRECO(process,"Data","HI")
#enablePp(process,"PpRECO") # options: "PpRECO", "HIRECO"
enableData(process)
enableDataFilter(process,"HI")

# FJ
#process.dijetAna_seq*=process.djcaloic5
#process.dijetAna_seq*=process.djcaloak5
#process.dijetAna_seq*=process.djcalokt4
#process.dijetAna_seq*=process.djcalokt4
#process.dijetAna_seq*=process.djpfak5

# For MB
#for m in [process.djcalo,process.djcalo_tower]:
#  m.nearJetPtMin = 40

# First look at data
process.djcalo.nearJetPtMin = 100
process.djcalo_pfcand.nearJetPtMin = 100

process.reco = cms.Path( process.eventSelection * process.dj_reco_extra )
process.ana  = cms.Path( process.eventSelection * process.dijetAna_seq )

process.schedule = cms.Schedule(process.reco,process.ana)
