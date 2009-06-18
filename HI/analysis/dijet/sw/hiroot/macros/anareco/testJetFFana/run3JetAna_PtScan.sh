#!/bin/bash

for i in $(seq 70 10 130); do
   pytmin=$i
   pytmax=$((i+10))
   pyqmin=$i
   pyqmax=$((i+10))
   #echo $pytmin $pytmax
   ./runGenericJetAnaPt.sh test_50k.root $pytmin $pytmax $pyqmin $pyqmax
done
