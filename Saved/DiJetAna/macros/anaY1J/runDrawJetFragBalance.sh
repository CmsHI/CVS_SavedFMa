#!/bin/bash -
#macro="drawMeanAJ.C"
macro="drawJetFragBalance_DR.C"
anaV=CorrEtaTrkEffPtBin5RBin20v6
module="djcalo"
BkgSub="SubEtaRefl"
Title="Track"

# No Weight
for c in 'Cent0to100'; do
  for a in 'Aj0to100' 'Aj0to11' 'Aj11to22' 'Aj22to33' 'Aj33to100'; do
    echo cent: $c  Aj: $a
    root -b -q $macro'+("jfh'$anaV'_PyquenUQ80_'$module'_'$c'_'$a'_'$BkgSub'.root","'$Title'")'
  done
done

for c in 'Cent0to10' 'Cent0to30' 'Cent10to30' 'Cent30to100'; do
  for a in 'Aj0to100' 'Aj0to11' 'Aj11to22' 'Aj22to33' 'Aj33to100'; do
    echo cent: $c  Aj: $a
    root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_'$c'_'$a'_'$BkgSub'.root","'$Title'")'
    root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_'$c'_'$a'_'$BkgSub'.root","'$Title'")'
    root -b -q $macro'+("jfh'$anaV'ReWt_Hydjet_'$module'_'$c'_'$a'_'$BkgSub'.root","'$Title'")'
  done
done
