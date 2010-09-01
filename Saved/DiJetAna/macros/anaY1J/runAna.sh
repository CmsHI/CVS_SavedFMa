#!/bin/bash -
anaV=a4
try=$1

# -- Hydjet + DJ
inputHard='~/scratch01/ana/Hydj_Hard_GSR_NZS/UQDJ80_120_proc0831_10k.root'
./runAnaDiJet.sh 0 $inputHard HydUQDJ80 ${anaV}$try dj UnQuenDiJet80to120-Hydjet dijetAna_mc $try
./runAnaDiJetTrk.sh 0 $inputHard HydUQDJ80 ${anaV}$try djtrk UnQuenDiJet80to120-Hydjet dijetAna_mc $try

# -- JEx Hard
inputHard='~/scratch01/ana/JEx_Hard_GSR_ZS/mc0827_hltall/dijetAna_mc_output_*.root'
./runAnaDiJet.sh 0 $inputHard JExHardMcV2 ${anaV}$try dj JulyData-Hard dijetAna_mc $try
./runAnaDiJetTrk.sh 0 $inputHard JExHardMcV2 ${anaV}$try djtrk JulyData-Hard dijetAna_mc $try


# -- JEx MB
inputMB='~/scratch01/ana/JEx_MB_GSR_ZS/mc0827_hltall/dijetAna_mc_output_*.root'
./runAnaDiJet.sh 0 $inputMB JExMbMcV2 ${anaV}$try dj JulyData-MB dijetAna_mc $try
./runAnaDiJetTrk.sh 0 $inputMB JExMbMcV2 ${anaV}$try djtrk JulyData-MB dijetAna_mc $try

exit

