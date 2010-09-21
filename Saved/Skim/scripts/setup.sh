#!/bin/bash -
# - Based on
# * UserCode/CmsHi/JetAnalysis/scripts
# * HLTrigger/HLTanalyzers/scripts/setup.sh

# HI Reco
cvs co RecoHI/Configuration
cvs co RecoHI/HiCentralityAlgos
cvs co RecoHI/HiEvtPlaneAlgos
cvs co RecoHI/HiJetAlgos
cvs co RecoHI/HiEgammaAlgos
cvs co RecoHI/HiMuonAlgos
cvs co RecoHI/HiTracking

cvs co CondFormats/HIObjects
cvs co GeneratorInterface/HiGenCommon
cvs co DataFormats/HeavyIonEvent
cvs co SimDataFormats/HiGenData

# analysis specific
cvs co RecoJets/JetAlgorithms
cvs co RecoJets/JetProducers
cvs co HeavyIonsAnalysis/Configuration

cvs co -d Saved UserCode/SavedFMa/Saved

# check dependencies
#checkdeps
