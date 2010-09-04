#!/bin/bash -

doMC=1
AnaVersion="a6j2t0"
#modName="dijetAna_mc"
modName="dijetAna_mc_calojet_genp"
#modName="dijetAna_mc_genjet_genp"

Sel=S1_100_200_50_25
inFile0Name="~/scratch01/ana/Hydj_Hard_GSR_NZS/UQDJ80_120_proc0831_10k.root"
SrcName="HydUQDJ80"
header="Hydjet+UQDJ80"
AnaType="ff"

mkdir -p log/$SrcName/$AnaVersion
root -b -q anaDiJetFF.C+\($doMC,\"$AnaVersion\",\"$modName\",\"$inFile0Name\",\"$SrcName\",\"$header\",\"$AnaType\"\) >& log/$SrcName/$AnaVersion/anaDiJetFF_${modName}.txt

doCorr=0
root -l finalDiJetFF.C+\($doCorr,\"out/$SrcName/$Sel/$AnaVersion\",\"$header\",\"$AnaType\"\)
