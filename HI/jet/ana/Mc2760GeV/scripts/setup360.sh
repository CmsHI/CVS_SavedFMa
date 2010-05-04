#!/bin/bash -

cd $CMSSW_BASE/src

# jet reco
addpkg RecoHI/HiJetAlgos 

# skiming code
cvs co -d ana/Mc2760GeV UserCode/SavedFMa/HI/jet/ana/Mc2760GeV

