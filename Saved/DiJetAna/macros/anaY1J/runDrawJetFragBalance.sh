#!/bin/bash -
anaV=1208b
module="djcalo"
BkgSub="SubEtaRefl"
root -b -q 'drawJetFragBalance.C("jfh'$anaV'_PyquenUQ80_'$module'_Cent0to100_Aj0to100_'$BkgSub'")'
root -b -q 'drawJetFragBalance.C("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to30_Aj0to100_'$BkgSub'")'
root -b -q 'drawJetFragBalance.C("jfh'$anaV'_HCPR_J50U_'$module'_Cent30to100_Aj0to100_'$BkgSub'")'

root -b -q 'drawJetFragBalance.C("jfh'$anaV'_PyquenUQ80_'$module'_Cent0to100_Aj24to100_'$BkgSub'")'
root -b -q 'drawJetFragBalance.C("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to30_Aj24to100_'$BkgSub'")'
root -b -q 'drawJetFragBalance.C("jfh'$anaV'_HCPR_J50U_'$module'_Cent0to30_Aj0to24_'$BkgSub'")'
