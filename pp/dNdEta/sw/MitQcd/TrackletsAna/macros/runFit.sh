#!/bin/bash -
if [ $# -eq 0 ]; then
  echo Usage
  echo "  $0 <Version> [tag]"
  exit 1
fi

version=$1
tag=$2
doSel=1 # 1, 4
anaMode=0


#for obs in EaddEp; do
#for obs in EaddEp EvtEta EsubEp; do
for obs in EaddEp EsubEp; do
  log=log/ana${version}_${obs}_Mode${anaMode}_Sel${doSel}_$tag.log
  echo start > $log
  for DataSource in data pythia phojet; do
    for MCSource in pythia phojet; do
      # case EaddEp
      if [ $obs == "EaddEp" ]; then
	for EPzMax in 200 100 50; do
	  root -b -q matchFrac.C+\(\"$version\",\"$DataSource\",\"$MCSource\",\"$obs\",$doSel,$anaMode,0,$EPzMax,5\) >> $log
	done
      fi
      # case Evt Eta
      if [ $obs == "EvtEta" ]; then
	root -b -q matchFrac.C+\(\"$version\",\"$DataSource\",\"$MCSource\",\"$obs\",$doSel,$anaMode,0,200,5\) >> $log
      fi
    done
  done
done

