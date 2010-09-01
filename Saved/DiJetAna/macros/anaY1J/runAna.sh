#!/bin/bash -
inputHard='~/scratch01/ana/JEx_Hard_GSR_ZS/mc0827_hltall/dijetAna_mc_output_*.root'
inputMB='~/scratch01/ana/JEx_MB_GSR_ZS/mc0827_hltall/dijetAna_mc_output_*.root'
try=$1

# anaDiJet
./runAnaDiJet.sh 0 $inputHard JExHdMc a0_$try dj JulyData-Hard dijetAna_mc $try
./runAnaDiJet.sh 0 $inputMB JExMbMc a0_$try dj JulyData-MB dijetAna_mc $try

# anaDiJetTrk
./runAnaDiJetTrk.sh 0 $inputHard JExHdMc a0_$try djtrk JulyData-Hard dijetAna_mc $try
./runAnaDiJetTrk.sh 0 $inputMB JExMbMc a0_$try djtrk JulyData-MB dijetAna_mc $try

exit
