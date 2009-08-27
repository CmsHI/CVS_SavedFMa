#!/bin/bash -
startdir=$PWD
cd $SavedFMa/HI/analysis/dijet/sw/hiroot/macros/anareco/DiJetAna/gen/
tar -cvf macros.tar *.C
mv macros.tar $startdir
cd $startdir
