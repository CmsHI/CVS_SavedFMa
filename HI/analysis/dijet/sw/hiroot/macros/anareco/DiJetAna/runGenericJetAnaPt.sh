#!/bin/bash -
. /net/hisrv0001/home/frankma/UserCode/SavedFMa/bin/cgate/set_hiroot.sh

echo "$@"

#== input vars==
export job=prodPtHat50
export gen0=pythia50
export gen1=pyquen50
export rootskim=iCone5_match35_jetEtCut30_ntJetEt
export infileName=test_50k.root
#export infileName=$1
export input0="$HIROOT_BASEURL/$job/$gen0/$rootskim"
export input1="$HIROOT_BASEURL/$job/$gen1/$rootskim"
export infile0=$input0/$infileName
export infile1=$input0/$infileName

#=== analysis vars ===
export doCheck="kTRUE"
export NXIBIN=25
export XIMAX=8
export XIYMAX=10
# --cuts--
export PythiaAnaNJetEtMin=$2
export PythiaAnaNJetEtMax=$3
export PythiaAnaAJetEtMin=$4
export PyquenAnaNJetEtMin=$5
export PyquenAnaNJetEtMax=$6
export PyquenAnaAJetEtMin=$7
export JDPhiMin=$8

#== output vars ===
export ana=${infileName%.*}_pythia_${PythiaAnaNJetEtMin}_${PythiaAnaNJetEtMax}_${PythiaAnaAJetEtMin}_pyquen_${PyquenAnaNJetEtMin}_${PyquenAnaNJetEtMax}_${PyquenAnaAJetEtMin}_jdphi_$JDPhiMin
export plotbase=$dijetSens/jetana/plots


#./runJetAnaBase.sh
root -b -q dijetsDriver.C+\(\"$infile0\",\"$infile1\"\)

