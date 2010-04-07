#!/bin/bash -
if [ $# -eq 0 ]; then
  echo Usage
  echo "  $0 <Version>"
  exit 1
fi

version=$1
tag=data_mc
anaMode=0

for doSel in 1; do #1 4 10; do
#for doSel in 1 4 10; do #1 4 10; do
  #for obs in EaddEp EvtEta EsubEp EaddEpPos EsubEpNeg; do
  #for obs in EaddEpPos EvtEta EsubEpNeg EaddEp; do
  for obs in EvtEta EaddEpPos MinEPz EaddEpNeg; do
    log=log/ana${version}_${obs}_Mode${anaMode}_Sel${doSel}_$tag.log
    echo "$@" > $log
    for DataSource in data pythia; do
      for MCSource in pythia; do
	# case EaddEp
	if [ $obs != "EvtEta" ]; then
	  #for EPzMax in 100 200 300 400; do
	  for EPzMax in 125 150 250; do
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
