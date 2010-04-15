#!/bin/bash -
if [ $# -eq 0 ]; then
  echo Usage
  echo "  $0 <Version>"
  exit 1
fi

# === Configure ===
version=$1
fitVersion=Fit0

dataFile=../pixel_trees/collbx/pixelTree_run132440_PromptReco-v7_veryloosecuts_v4.root
pythiaD6tFile=../pixel_trees/mc/pixelTree_pythiaD6t_MB7TeV_356ReRecov1_1M.root
pythiaAtlasFile=../pixel_trees/mc/pixelTree_yilmaz-MinBiasATLAS_RECO_0332_v1.root
phojetFile=../pixel_trees/mc/pixelTree_yilmaz-MinBiasPhojet_RECO_0413_v1.root

anaMode=0

# !! check again above to match tag with input file !!


# === run ===
for doSel in 1; do #1 4 10; do
  for DataSource in data pythiaAtlas phojet pythiaD6t; do
    if [ $DataSource == data ]; then dataF=$dataFile; fi
    if [ $DataSource == pythiaD6t ]; then dataF=$pythiaD6tFile; fi
    if [ $DataSource == pythiaAtlas ]; then dataF=$pythiaAtlasFile; fi
    if [ $DataSource == phojet ]; then dataF=$phojetFile; fi
    for MCSource in pythiaAtlas phojet pythiaD6t; do
      if [ $MCSource == data ]; then mcF=$dataFile; fi
      if [ $MCSource == pythiaD6t ]; then mcF=$pythiaD6tFile; fi
      if [ $MCSource == pythiaAtlas ]; then mcF=$pythiaAtlasFile; fi
      if [ $MCSource == phojet ]; then mcF=$phojetFile; fi
      # output log
      tag=${DataSource}_${MCSource}
      log=log/ana${version}_Mode${anaMode}_Sel${doSel}-${tag}_fit.log
      echo "input pars: $version $DataSource $dataF $MCSource $mcF $doSel $anaMode" > $log
      # Different obs
      root -b -q fit_shapes.C+\(\"$version\",\"$DataSource\",\"$dataF\",\"$MCSource\",\"$mcF\",\"$fitVersion\",\"EvtEta\",$doSel,$anaMode,0,200,5\) >> $log
      for obs in MinEPz EaddEpPos EsubEpNeg; do
	# different EPz ranges
	for EPzMax in 100 200 300; do
	  root -b -q fit_shapes.C+\(\"$version\",\"$DataSource\",\"$dataF\",\"$MCSource\",\"$mcF\",\"$fitVersion\",\"$obs\",$doSel,$anaMode,0,$EPzMax,5\) >> $log
	done
      done
    done
  done
done
