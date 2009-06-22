#!/bin/bash

PtBin=20
amin=50
dphi=3.0

tar xf macros.tar

for i in $(seq 70 $PtBin 130); do
   pytmin=90
   pytmax=$(($pytmin+$PtBin))
   pyqmin=$i
   pyqmax=$(($pyqmin+$PtBin))
   echo $pytmin $pytmax $pyqmin $pyqmax $amin $dphi
   ./runGenericJetAnaPt.sh test_300k.root $pytmin $pytmax $amin $pyqmin $pyqmax $amin $dphi
   #./runGenericJetAnaPt.sh test_50k.root $pytmin $pytmax $amin $pyqmin $pyqmax $amin $dphi
done

