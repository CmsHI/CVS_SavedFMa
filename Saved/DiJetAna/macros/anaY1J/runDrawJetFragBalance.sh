#!/bin/bash -
anaV=1208
module="djcalo_tower"
BkgSub="SubEtaRefl"
root -b -q 'drawJetFragBalance.C("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to10_Aj0to100_'$BkgSub'")'
root -b -q 'drawJetFragBalance.C("jfh'$anaV'_HCPR_J50U_'$module'_Cent10to30_Aj0to100_'$BkgSub'")'
root -b -q 'drawJetFragBalance.C("jfh'$anaV'_HCPR_J50U_'$module'_Cent30to100_Aj0to100_'$BkgSub'")'
root -b -q 'drawJetFragBalance.C("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to30_Aj24to100_'$BkgSub'")'
root -b -q 'drawJetFragBalance.C("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to30_Aj0to24_'$BkgSub'")'
root -b -q 'drawJetFragBalance.C("jfh'$anaV'_PyquenUQ80_'$module'_Cent0to100_Aj0to100_'$BkgSub'")'
