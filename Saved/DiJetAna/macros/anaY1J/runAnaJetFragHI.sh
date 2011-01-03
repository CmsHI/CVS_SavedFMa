#!/bin/bash -
infile=$1
srcName=$2
anaV=$3
module=$4
doReWeight=$5
BkgSub="EtaReflSingle"
AjMed0to30=0.22

AjLow0to30=0.15
AjHigh0to30=0.29

for c in '0,10' '0,30' '10,30' '30,100'; do
  for a in '0,1' '0,0.11' '0.11,0.22' '0.22,0.33' '0.33,0.5' '0.33,1'; do
    echo cent: $c  Aj: $a
    root -b -q 'anaJetFrag.C+("'$infile'",1,'$doReWeight',"'$BkgSub'",0,"'$srcName'","'$anaV'","'$module'",'$c','$a')'
  done
done
