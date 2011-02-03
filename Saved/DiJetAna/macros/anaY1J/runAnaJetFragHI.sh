#!/bin/bash -
infile=$1
srcName=$2
anaV=$3
module=$4
doReWeight=$5
BkgSub=$6
EtaCorr=$7

# Note: AjMed0to30=0.22
#for c in '0,10' '0,30' '10,30' '30,100'; do
#for c in '0,30' '30,100'; do
#  alist='0,1'
#  for a in $alist; do
#    echo cent: $c  Aj: $a
#    root -b -q 'anaJetFrag.C+("'$infile'",1,'$EtaCorr','$doReWeight',"'$BkgSub'",0,"'$srcName'","'$anaV'","'$module'",'$c','$a')'
#  done
#done

for c in '0,30' '30,100'; do
  alist='0,0.11 0.11,0.22 0.22,0.33 0.33,1'
  for a in $alist; do
    echo cent: $c  Aj: $a
    #root -b -q 'anaJetFrag.C+("'$infile'",1,'$EtaCorr','$doReWeight',"'$BkgSub'",0,"'$srcName'","'$anaV'","'$module'",'$c','$a')'
    # == Random Cone Study ==
    root -b -q 'anaJetFrag.C+("'$infile'",1,'$EtaCorr','$doReWeight',"'$BkgSub'",0,"'${srcName}Offset0'","'$anaV'","'$module'",'$c','$a',120,500,50,500,0.5,"S1","Ana",0)'
    root -b -q 'anaJetFrag.C+("'$infile'",1,'$EtaCorr','$doReWeight',"'$BkgSub'",0,"'${srcName}Offset1'","'$anaV'","'$module'",'$c','$a',120,500,50,500,0.5,"S1","Ana",1)'
  done
done
