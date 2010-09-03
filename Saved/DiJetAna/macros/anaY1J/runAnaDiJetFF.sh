#!/bin/bash -

doMC=1
AnaVersion="a6"
#modName="dijetAna_mc"
modName="dijetAna_mc_calojet_genp"
#modName="dijetAna_mc_genjet_genp"

inFile0Name="~/scratch01/ana/Hydj_Hard_GSR_NZS/UQDJ80_120_proc0831_10k.root"
SrcName="HydUQDJ80"
header="Hydjet+UQDJ80"
AnaType="ff"

root -b -q anaDiJetFF.C+\($doMC,\"$AnaVersion\",\"$modName\",\"$inFile0Name\",\"$SrcName\",\"$header\",\"$AnaType\"\)
