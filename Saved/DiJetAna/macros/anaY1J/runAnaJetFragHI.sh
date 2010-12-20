#!/bin/bash -
infile=$1
srcName=$2
anaV=$3
module=$4
BkgSub="EtaRefl"
AjMed0to30=0.22

AjLow0to30=0.15
AjHigh0to30=0.29

root -b -q 'anaJetFrag.C+("'$infile'","'$srcName'",0,"'$anaV'","'$module'",1,"'$BkgSub'",0,10,0,1)'
root -b -q 'anaJetFrag.C+("'$infile'","'$srcName'",0,"'$anaV'","'$module'",1,"'$BkgSub'",0,30,0,1)'
root -b -q 'anaJetFrag.C+("'$infile'","'$srcName'",0,"'$anaV'","'$module'",1,"'$BkgSub'",10,30,0,1)'
root -b -q 'anaJetFrag.C+("'$infile'","'$srcName'",0,"'$anaV'","'$module'",1,"'$BkgSub'",30,100,0,1)'

root -b -q 'anaJetFrag.C+("'$infile'","'$srcName'",0,"'$anaV'","'$module'",1,"'$BkgSub'",0,30,0,'$AjLow0to30')'
root -b -q 'anaJetFrag.C+("'$infile'","'$srcName'",0,"'$anaV'","'$module'",1,"'$BkgSub'",0,30,'$AjLow0to30','$AjHigh0to30')'
root -b -q 'anaJetFrag.C+("'$infile'","'$srcName'",0,"'$anaV'","'$module'",1,"'$BkgSub'",0,30,'$AjHigh0to30',1)'
