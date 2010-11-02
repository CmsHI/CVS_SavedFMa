import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.ReconstructionHeavyIons_cff import *
from GeneratorInterface.HiGenCommon.HeavyIon_cff import *
from RecoHI.HiCentralityAlgos.CentralityBin_cfi import *
from HLTrigger.HLTanalyzers.HLTAnalyser_cfi import * 

# jets
hltanalysis.genjets           = cms.InputTag("iterativeCone5HiGenJets")
hltanalysis.recjets           = cms.InputTag("hltIterativeCone5PileupSubtractionCaloJets")

# photons
hltanalysis.BarrelPhoton      = cms.InputTag("hltCorrectedIslandBarrelSuperClustersHI")
hltanalysis.EndcapPhoton      = cms.InputTag("hltCorrectedIslandEndcapSuperClustersHI")

# muons
#hltanalysis.MuCandTag3       = cms.InputTag("hltL3GenMuonCandidates") ### if DoL3Muons is true

hltanalysis.Centrality    = cms.InputTag("hiCentrality")
hltanalysis.CentralityBin    = cms.InputTag("centralityBin")
hltanalysis.EvtPlane      = cms.InputTag("hiEvtPlane","recoLevel")
hltanalysis.mctruth       = cms.InputTag("hiGenParticles")
hltanalysis.HiMC          = cms.InputTag("heavyIon")    

hltanalysis.PrimaryVertices             = cms.InputTag("hiSelectedVertex")
