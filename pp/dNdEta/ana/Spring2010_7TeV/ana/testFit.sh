#!/bin/bash -
if [ $# -eq 0 ]; then
  echo Usage
  echo "  $0 <Version>"
  exit 1
fi

version=$1
tag=mc
anaMode=0

for doSel in 10; do #1 4 10; do
  #for obs in EaddEp EvtEta EsubEp EaddEpPos EsubEpNeg; do
  #for obs in EaddEpPos EvtEta EsubEpNeg EaddEp; do
  for obs in EaddEpPos; do
    log=log/ana${version}_${obs}_Mode${anaMode}_Sel${doSel}_$tag.log
    echo "$@" > $log
    for DataSource in pythia; do
      for MCSource in pythia; do
	# case EaddEp
	if [ $obs != "EvtEta" ]; then
	  for EPzMax in 200; do
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
done
