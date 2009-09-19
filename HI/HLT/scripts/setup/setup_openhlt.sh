#!/bin/bash -

project=HIOpenHLT_1_0
release=CMSSW_3_2_4
pname=${project}_${release}
scramv1 project -n $pname CMSSW $release
cd $pname/src
cmsenv

cvscms

# fix for hi mu hlt reco in 324
cvs co -r V01-00-04 RecoHI/HiTracking
cvs co -r V00-03-08 RecoHIMuon/HiMuTracking
cvs co -r V00-02-02 RecoHIMuon/HiMuPropagator
cvs co -r V00-03-02 RecoHIMuon/HiMuSeed

# christof's open hlt code so far in 324
cvs co -r $project UserCode/HIHLTStudy/HLTrigger
mv UserCode/HIHLTStudy/HLTrigger .

# genjet 32x code
cvs co -r cmshi_32X UserCode/CmsHi/JetAnalysis
cvs co -r V00-01-02 UserCode/CmsHi/Utilities

# other needed code to compile
cvs co -r V00-01-02 DataFormats/HeavyIonEvent

# move to compile directory structure
mv UserCode/CmsHi .

return 0

# build
scramv1 b

# clean up
rm -rf UserCode

