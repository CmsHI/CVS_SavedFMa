import FWCore.ParameterSet.Config as cms
 
# import the whole HLT menu
#from HLTrigger.Configuration.HLT_8E29_cff import *
#from HLTrigger.Configuration.HLT_1E31_cff import *
from HLTrigger.Configuration.HLT_HIon_cff import *

# create the HI jet HLT reco path
DoHLTHIJets = cms.Path(HLTBeginSequence + 
    HLTDoHIJetRecoSequence)

# create the muon HLT reco path
DoHLTHIMuon = cms.Path(
    HLTBeginSequence +
    HLTL2muonrecoSequence +
    HLTDoLocalPixelSequence +
    HLTHIRecopixelvertexingSequence +
    HLTDoLocalStripSequence +
    hltHIMML3Filter +
    HLTEndSequence)

# create the Egamma HLT reco paths
DoHLTHIPhoton = cms.Path(
    HLTBeginSequence +
    HLTDoCaloSequence +
    HLTDoHIEcalClusSequence +
    HLTEndSequence )
