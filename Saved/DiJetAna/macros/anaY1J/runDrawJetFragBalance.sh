#!/bin/bash -
#macro="drawMeanAJ.C"
macro="drawJetFragBalance_DR.C"
anaV=CorrEtaHighPtv3RBin20
module="djcalo"
BkgSub="SubEtaRefl"
Title="Track"

root -b -q $macro'+("jfh'$anaV'_PyquenUQ80_djcalo_genp_Cent0to100_Aj0to100_'$BkgSub'","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_PyquenUQ80_'$module'_Cent0to100_Aj0to100_'$BkgSub'","'$Title'")'

root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to10_Aj0to100_'$BkgSub'","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to30_Aj0to100_'$BkgSub'","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_Cent10to30_Aj0to100_'$BkgSub'","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_Cent30to100_Aj0to100_'$BkgSub'","'$Title'")'

root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_Cent0to10_Aj0to100_'$BkgSub'","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_Cent0to30_Aj0to100_'$BkgSub'","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_Cent10to30_Aj0to100_'$BkgSub'","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_Cent30to100_Aj0to100_'$BkgSub'","'$Title'")'

root -b -q $macro'+("jfh'$anaV'_PyquenUQ80_'$module'_Cent0to100_Aj0to24_'$BkgSub'","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_PyquenUQ80_'$module'_Cent0to100_Aj24to100_'$BkgSub'","'$Title'")'

root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to30_Aj0to24_'$BkgSub'","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to30_Aj24to100_'$BkgSub'","'$Title'")'

root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_Cent0to30_Aj0to24_'$BkgSub'","'$Title'")'
root -b -q $macro'+("jfh'$anaV'_Hydjet_'$module'_Cent0to30_Aj24to100_'$BkgSub'","'$Title'")'
