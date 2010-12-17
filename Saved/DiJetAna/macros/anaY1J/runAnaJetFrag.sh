#!/bin/bash -
anaV=CorrEtaPtBin200RBin20
infileData="dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0.root"
#infilePyquen="dj_PyquenUQ80_hiGoodMergedTracks_OfficialSelv2.root"
infilePyquen="dj_PyquenUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv2GenAll.root"
infileHyd="dj_HydQ_DJUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv2GenAll.root"

module="djcalo"
BkgSub="EtaRefl"
NrJEtMin=120
TrkPtMin=1.5
AjMed0to30=0.22

# Pyquen
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","djcalo_genp",0,100,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infilePyquen'","PyquenUQ80")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,100,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infilePyquen'","PyquenUQ80")'

# Data
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,10,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",10,30,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,30,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",30,100,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'

# Hydjet
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,10,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileHyd'","Hydjet")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",10,30,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileHyd'","Hydjet")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,30,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileHyd'","Hydjet")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",30,100,'$NrJEtMin',500,50,500,0.,1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileHyd'","Hydjet")'

# Aj
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,100,'$NrJEtMin',500,50,500,0.,'$AjMed0to30','$TrkPtMin',"Ana","'$BkgSub'","'$infilePyquen'","PyquenUQ80")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,100,'$NrJEtMin',500,50,500,'$AjMed0to30',1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infilePyquen'","PyquenUQ80")'

root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,30,'$NrJEtMin',500,50,500,0.,'$AjMed0to30','$TrkPtMin',"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,30,'$NrJEtMin',500,50,500,'$AjMed0to30',1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileData'","HCPR_J50U")'

root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,30,'$NrJEtMin',500,50,500,0.,'$AjMed0to30','$TrkPtMin',"Ana","'$BkgSub'","'$infileHyd'","Hydjet")'
root -b -q 'anaJetFrag.C+(0,"S1","'$anaV'","'$module'",0,30,'$NrJEtMin',500,50,500,'$AjMed0to30',1.0,'$TrkPtMin',"Ana","'$BkgSub'","'$infileHyd'","Hydjet")'
