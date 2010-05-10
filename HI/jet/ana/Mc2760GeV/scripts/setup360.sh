#!/bin/bash -

cd $CMSSW_BASE/src

# jet reco
addpkg RecoHI/HiJetAlgos 

# skiming code
cvs co -d ana/Mc2760GeV UserCode/SavedFMa/HI/jet/ana/Mc2760GeV

# For centrality see
# - https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideHeavyIonCentrality#Using_centrality_in_analysis
cvs co -r V00-01-04 RecoHI/HiCentralityAlgos
cvs co -r V00-01-09 DataFormats/HeavyIonEvent
cvs co -r V00-00-03 CondFormats/HIObjects
cvs co -r V00-00-00 CondCore/HIPlugins
