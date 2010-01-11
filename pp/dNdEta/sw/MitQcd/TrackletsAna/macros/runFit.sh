#!/bin/bash -
if [ $# -eq 0 ]; then
  echo Usage
  echo "  $0 <tag>"
  exit 1
fi

tag=$1
log=log/fit_$tag.log
version=V1

echo start > $log
for DataSource in data pythia phojet; do
  for MCSource in pythia phojet; do
    root -b -q matchFrac.C+\(\"$version\",\"$DataSource\",\"$MCSource\",4,0,0,200,5\) >> $log
    root -b -q matchFrac.C+\(\"$version\",\"$DataSource\",\"$MCSource\",4,0,18,200,5\) >> $log
    root -b -q matchFrac.C+\(\"$version\",\"$DataSource\",\"$MCSource\",4,0,0,50,5\) >> $log
  done
done
