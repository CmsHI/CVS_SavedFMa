#!/bin/bash -
#macro="drawMeanAJ.C"
macro="drawJetFragBalance_DR.C"
anaV=CorrEtaTrkEffPtBin5RBin20v4
module="djcalo_genp"
BkgSub="SubEtaRefl"
Title="Track"

root -b -q $macro'+("jfh'$anaV'_PyquenUQ80_'$module'_Cent0to100_Aj0to100_'$BkgSub'.root","'$Title'")'

root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to10_Aj0to100_'$BkgSub'.root","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to30_Aj0to100_'$BkgSub'.root","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_Cent10to30_Aj0to100_'$BkgSub'.root","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_Cent30to100_Aj0to100_'$BkgSub'.root","'$Title'")'

root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_Cent0to10_Aj0to100_'$BkgSub'.root","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_Cent0to30_Aj0to100_'$BkgSub'.root","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_Cent10to30_Aj0to100_'$BkgSub'.root","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_Cent30to100_Aj0to100_'$BkgSub'.root","'$Title'")'

root -b -q $macro'+("jfh'$anaV'_PyquenUQ80_'$module'_Cent0to100_Aj0to15_'$BkgSub'.root","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_PyquenUQ80_'$module'_Cent0to100_Aj15to29_'$BkgSub'.root","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_PyquenUQ80_'$module'_Cent0to100_Aj29to100_'$BkgSub'.root","'$Title'")'

root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to30_Aj0to15_'$BkgSub'.root","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to30_Aj15to29_'$BkgSub'.root","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to30_Aj29to100_'$BkgSub'.root","'$Title'")'

root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_Cent0to30_Aj0to15_'$BkgSub'.root","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_Cent0to30_Aj15to29_'$BkgSub'.root","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_Cent0to30_Aj29to100_'$BkgSub'.root","'$Title'")'
