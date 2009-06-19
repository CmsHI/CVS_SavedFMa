#!/bin/bash -
. /net/hisrv0001/home/frankma/UserCode/SavedFMa/bin/cgate/set_hiroot.sh

#== input vars==
export job=prodPtHat50
export gen0=pythia50
export gen1=pyquen50
export rootskim=iCone5_match35_jetEtCut30_ntJetEt
#export infileName=test_50k.root
export infileName=$1
export input0="$HIROOT_BASEURL/$job/$gen0/$rootskim"
export input1="$HIROOT_BASEURL/$job/$gen1/$rootskim"

#=== analysis vars ===
export doCheck="kTRUE"
export NXIBIN=25
export XIMAX=8
export XIYMAX=10
# --cuts--
export PythiaAnaJetEtMin=$2
export PythiaAnaJetEtMax=$3
export PyquenAnaJetEtMin=$4
export PyquenAnaJetEtMax=$5
export JDPhiMin=$6

#== output vars ===
export ana=${infileName%.*}_pythia_${PythiaAnaJetEtMin}_${PythiaAnaJetEtMax}_pyquen_${PyquenAnaJetEtMin}_${PyquenAnaJetEtMax}_jdphi_$JDPhiMin
export plotbase=$dijetSens/jetana/plots


./runJetAnaBase.sh

