#!/bin/bash -
anaV=CorrEtaLowPt
infileData="dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2.root"
#infilePyquen="dj_PyquenUQ80_hiGoodMergedTracks_OfficialSelv2.root"
infilePyquen="dj_PyquenUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv2GenAll.root"
module="djcalo"
BkgSub="EtaRefl"
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","djcalo_genp",0,100,120,500,50,500,2.,2.094,0.,1.0,"Ana","'$BkgSub'","'$infilePyquen'","PyquenUQ80")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,100,120,500,50,500,2.,2.094,0.,1.0,"Ana","'$BkgSub'","'$infilePyquen'","PyquenUQ80")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,10,120,500,50,500,2.,2.094,0.,1.0,"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",10,30,120,500,50,500,2.,2.094,0.,1.0,"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,30,120,500,50,500,2.,2.094,0.,1.0,"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",30,100,120,500,50,500,2.,2.094,0.,1.0,"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'

root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,100,120,500,50,500,2.,2.094,0.24,1.0,"Ana","'$BkgSub'","'$infilePyquen'","PyquenUQ80")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,30,120,500,50,500,2.,2.094,0.,0.24,"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,30,120,500,50,500,2.,2.094,0.24,1.0,"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
