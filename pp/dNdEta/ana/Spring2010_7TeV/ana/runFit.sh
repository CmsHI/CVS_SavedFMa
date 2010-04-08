#!/bin/bash -
if [ $# -eq 0 ]; then
  echo Usage
  echo "  $0 <Version>"
  exit 1
fi

# === Configure ===
version=$1

dataFile=../pixel_trees/collbx/pixelTree_run132440_PromptReco-v7_veryloosecuts_v4.root
#pythiaFile=../pixel_trees/mc/pixelTree_pythiaD6t_MB7TeV_356ReRecov1_1M.root
pythiaFile=../pixel_trees/mc/pixelTree_yilmaz-MinBiasATLAS_RECO_0332_v1.root
phojetFile=../pixel_trees/mc/

#tag=data_pythiaD6t
tag=data_pythiaAtlas
anaMode=0

# !! check again above to match tag with input file !!


# === run ===
for doSel in 1; do #1 4 10; do
#for doSel in 1 4 10; do #1 4 10; do
  #for obs in EvtEta EsubEp EaddEpPos EsubEpNeg; do
  #for obs in EvtEta EaddEpPos MinEPz EsubEpNeg; do
  for obs in EsubEpNeg; do
  #for obs in EaddEpPos; do
    log=log/ana${version}_${obs}_Mode${anaMode}_Sel${doSel}_$tag.log
    echo "$@" > $log
    for DataSource in data pythia; do
      if [ $DataSource == data ]; then dataF=$dataFile; fi
      if [ $DataSource == pythia ]; then dataF=$pythiaFile; fi
      for MCSource in pythia; do
	if [ $MCSource == pythia ]; then mcF=$pythiaFile; fi
	# case EaddEp
	if [ $obs != "EvtEta" ]; then
	  for EPzMax in 100 200 300; do
	  #for EPzMax in 125 150 250; do
	  #for EPzMax in 200; do
	    root -b -q matchFrac.C+\(\"$version\",\"$DataSource\",\"$dataF\",\"$MCSource\",\"$mcF\",\"$obs\",$doSel,$anaMode,0,$EPzMax,5\) >> $log
	    #echo $cmd >> $log
	    #eval $cmd
	  done
	fi
	# case Evt Eta
	if [ $obs == "EvtEta" ]; then
	  root -b -q matchFrac.C+\(\"$version\",\"$DataSource\",\"$dataF\",\"$MCSource\",\"$mcF\",\"$obs\",$doSel,$anaMode,0,200,5\) >> $log
	  #echo $cmd >> $log
	  #eval $cmd
	fi
      done
    done
  done
done
