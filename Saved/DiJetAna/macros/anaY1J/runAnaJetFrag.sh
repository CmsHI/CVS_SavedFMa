#!/bin/bash -
anaV=CorrEtaHighPtv2
infileData="dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0.root"
#infilePyquen="dj_PyquenUQ80_hiGoodMergedTracks_OfficialSelv2.root"
infilePyquen="dj_PyquenUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv2GenAll.root"

module="djcalo"
BkgSub="EtaRefl"
NrJEtMin=120
TrkPtMin=1.5

root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","djcalo_genp",0,100,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infilePyquen'","PyquenUQ80")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,100,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infilePyquen'","PyquenUQ80")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,10,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",10,30,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,30,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",30,100,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'

root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,100,'$NrJEtMin',500,50,500,0.,0.24,'$TrkPtMin',"Ana","'$BkgSub'","'$infilePyquen'","PyquenUQ80")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,100,'$NrJEtMin',500,50,500,0.24,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infilePyquen'","PyquenUQ80")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,30,'$NrJEtMin',500,50,500,0.,0.24,'$TrkPtMin',"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,30,'$NrJEtMin',500,50,500,0.24,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
