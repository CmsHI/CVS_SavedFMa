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

#=== analysis vars ===
export doCheck="kTRUE"
export NXIBIN=25
export XIMAX=9
# --cuts--
export PythiaAnaJetEtMin=90
export PythiaAnaJetEtMax=""
export PyquenAnaJetEtMin=90
export PyquenAnaJetEtMax=""

#== output vars ===
export ana=pythia_${PythiaAnaJetEtMin}_${PythiaAnaJetEtMax}_pyquen_${PyquenAnaJetEtMin}_${PyquenAnaJetEtMax}
export plotbase=$dijetSens/jetana/plots


./runJetAnaBase.sh

