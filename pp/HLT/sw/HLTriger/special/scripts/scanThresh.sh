#!/bin/bash -
if [ $# -eq 0 ]; then
  echo Usgae:
  echo "  $0 <# of points>"
  exit 1
fi

./create_custom_python_cfg.sh
infile=/d01/frankma/scratch/data/pp/CRAFT09/Cosmics/RAW/v1/167707BF-DF96-DE11-8F9D-001617E30CE8.root
N=15
N=$1
EfEStep=0.2
EcalEStep=0.1
EcalEStart=0.5
for iE in `seq 0 $((N-1))`; do
  EfECut=$(echo "$EfEStep*$iE" | bc)
  EcalECut=$(echo "$EcalEStep*$iE+$EcalEStart" | bc)
  cmd="mycmsRun custom_OnLine_HLT_Test.py print files=file:$infile maxEvents=1000 HfEMin=$EfECut EcalEMin=$EcalECut PixClustMin=$iE -a 1k_Hf${EfECut}_Ecal${EcalECut}_Pix${iE}"
  echo $cmd
  eval $cmd
  sleep 15
done
