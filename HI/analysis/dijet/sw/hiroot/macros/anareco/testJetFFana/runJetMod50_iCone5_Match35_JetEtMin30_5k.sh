#!/bin/bash -
. /net/hisrv0001/home/frankma/UserCode/SavedFMa/bin/cgate/set_hiroot.sh

# file var's
base=/net/pstore01/d00/scratch/frankma/hiroot
gen0=pythia50_10k
gen1=pyquen50_10k

# ana var's
dRCut="0.35"
EtCut="30."
ana=iCone5_match35_jetEtCut30_ntJetEt

#--pythia--
export HIROOT_BASEURL="$base/$gen0/$ana"
echo "HIROOT_BASEURL: $HIROOT_BASEURL"
if [ ! -e $HIROOT_BASEURL ]; then mkdir $HIROOT_BASEURL; fi
log=$HIROOT_BASEURL/log
if [ ! -e $log ]; then mkdir $log; fi

echo root -b -q runJFPythia.C+\(777,\"$base/$gen0/hrt000777s000.root\",$dRCut,$EtCut\) >& "$log/runJFPythia_${gen0}_${ana}.txt"
root -b -q runJFPythia.C+\(777,\"$base/$gen0/hrt000777s000.root\",$dRCut,$EtCut\) >& $log/runJFPythia_${gen0}_${ana}.txt
echo

#--pyquen--
export HIROOT_BASEURL="$base/$gen1/$ana"
echo "HIROOT_BASEURL: $HIROOT_BASEURL"
if [ ! -e $HIROOT_BASEURL ]; then mkdir $HIROOT_BASEURL; fi
log=$HIROOT_BASEURL/log
if [ ! -e $log ]; then mkdir $log; fi

echo root -b -q runJFPythia.C+\(777,\"$base/$gen1/hrt000777s000.root\",$dRCut,$EtCut\) >& "$log/runJFPythia_${gen1}_${ana}.txt"
root -b -q runJFPythia.C+\(777,\"$base/$gen1/hrt000777s000.root\",$dRCut,$EtCut\) >& $log/runJFPythia_${gen1}_${ana}.txt


