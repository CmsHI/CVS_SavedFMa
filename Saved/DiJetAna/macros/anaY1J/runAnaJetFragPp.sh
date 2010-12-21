#!/bin/bash -
infile=$1
srcName=$2
anaV=$3
module=$4
doReWeight=$5
BkgSub="EtaRefl"
AjMed0to30=0.22

AjLow0to30=0.15
AjHigh0to30=0.29

root -b -q 'anaJetFrag.C+("'$infile'","'$srcName'",0,"'$anaV'","'$module'",1,'$doReWeight',"'$BkgSub'",0,100,0,1)'
root -b -q 'anaJetFrag.C+("'$infile'","'$srcName'",0,"'$anaV'","'$module'",1,'$doReWeight',"'$BkgSub'",0,100,0,'$AjLow0to30')'
root -b -q 'anaJetFrag.C+("'$infile'","'$srcName'",0,"'$anaV'","'$module'",1,'$doReWeight',"'$BkgSub'",0,100,'$AjLow0to30','$AjHigh0to30')'
root -b -q 'anaJetFrag.C+("'$infile'","'$srcName'",0,"'$anaV'","'$module'",1,'$doReWeight',"'$BkgSub'",0,100,'$AjHigh0to30',1)'
