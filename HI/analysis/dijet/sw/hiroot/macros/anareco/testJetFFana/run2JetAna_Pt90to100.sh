#!/bin/bash -
. /net/hisrv0001/home/frankma/UserCode/SavedFMa/bin/cgate/set_hiroot.sh

#== input vars==
export job=prodPtHat50
export gen0=pythia50
export gen1=pyquen50
export rootskim=iCone5_match35_jetEtCut30_ntJetEt
export infileName=test_50k.root
export input0="$HIROOT_BASEURL/$job/$gen0/$rootskim"
export input1="$HIROOT_BASEURL/$job/$gen1/$rootskim"

#== output vars ===
export ana=ptMin100
export plotbase=$dijetSens/jetana/plots
export NXIBIN=100

#=== cuts ===
export PythiaAnaJetEtMin=100t
export PythiaAnaJetEtMax=""
export PyquenAnaJetEtMin=100
export PyquenAnaJetEtMax=""

./runJetAnaBase.sh

