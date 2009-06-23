#!/bin/bash

PtBin=20
amin=50
dphi=2.5
dphibin=0.1

for i in $(seq 1 6); do
   pytmin=90
   pytmax=$(($pytmin+$PtBin))
   pyqmin=90
   pyqmax=$(($pyqmin+$PtBin))
   dphi=$(echo "$dphi+$dphibin" | bc)
   echo $pytmin $pytmax $pyqmin $pyqmax $amin $dphi
   #./runGenericJetAnaPt.sh test_50k.root $pytmin $pytmax $amin $pyqmin $pyqmax $amin $dphi
   #./runGenericJetAnaPt.sh test_300k.root $pytmin $pytmax $amin $pyqmin $pyqmax $amin $dphi
   #./condor64.sh runGenericJetAnaPt.sh test_50k.root $pytmin $pytmax $amin $pyqmin $pyqmax $amin $dphi
   ./condor64.sh runGenericJetAnaPt.sh test_300k.root $pytmin $pytmax $amin $pyqmin $pyqmax $amin $dphi
done

