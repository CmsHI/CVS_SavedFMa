#!/bin/bash -
scramv1 project -n simCMSSW_3_3_0_pre2_TrackerVR CMSSW CMSSW_3_3_0_pre2
cd simCMSSW_3_3_0_pre2_TrackerVR/src
cmsenv

addpkg EventFilter/SiStripRawToDigi

# dataformats
# this fixes the "SiStripNoises for a strip out of range" problem
cvs co -r V15-03-02 CondFormats/SiStripObjects

# this should work for 330pre2 b/c Ivan just used 2009/9/4
cp -r /d00/icali/CMSSW_3_1_0/src/SimTracker .

# db
cp /d00/icali/CMSSW_3_1_0/src/dbfile.db .

# run!
scramv1 b
