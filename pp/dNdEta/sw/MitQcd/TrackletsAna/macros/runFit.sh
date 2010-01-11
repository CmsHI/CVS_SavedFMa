#!/bin/bash -
if [ $# -eq 0 ]; then
  echo Usage
  echo "  $0 <Version> [tag]"
  exit 1
fi

version=$1
tag=$2
obs="EvtEta" # EaddEp, EsubEp, EvtEta
doSel=1 # 1, 4
anaMode=0
log=log/ana${version}_${obs}_${anaMode}_Sel${doSel}_$tag.log

echo start > $log

for DataSource in data pythia phojet; do
  for MCSource in pythia phojet; do
    # case EaddEp
    if [ $obs == "EaddEp" ]; then
      for EPzMax in 200 10 50; do
	root -b -q matchFrac.C+\(\"$version\",\"$DataSource\",\"$MCSource\",\"$obs\",$doSel,$anaMode,0,$EPzMax,5\) >> $log
      done
    fi
    # case Evt Eta
    if [ $obs == "EvtEta" ]; then
      root -b -q matchFrac.C+\(\"$version\",\"$DataSource\",\"$MCSource\",\"$obs\",$doSel,$anaMode,0,200,5\) >> $log
    fi
  done
done

