#!/bin/bash -
inputHard='~/scratch01/ana/JEx_Hard_GSR_ZS/mc0824_hlt/dijetAna_mc_output_*.root'
inputMB='~/scratch01/ana/JEx_MB_GSR_ZS/mc0824_hlt/dijetAna_mc_output_*.root'
try=$1

# anaDiJet
./runAnaDiJet.sh 0 $inputHard JExHdMc a1 dj JulyData-Hard dijetAna_mc $try
#./runAnaDiJet.sh 0 $inputMB JExMbMc a1 dj JulyData-MB dijetAna_mc $try

exit
