cvsdir=UserCode/SavedFMa/pp/HLT/sw/HLTrigger/HLTanalyzers
localdir=HLTrigger/HLTanalyzers

# setup project area
cd $CMSSW_BASE/src
eval `scramv1 runtime -sh`

# check-out code
cvs co HLTrigger/HLTanalyzers # head version (2009.9.29)
cvs co $cvsdir

# cp the private code into HLTrigger/HLTanalyzers
cp $cvsdir/src/HLTAnaInfo.cc $localdir/src
cp $cvsdir/interface/HLTAnaInfo.h $localdir/interface
cp $cvsdir/src/HLTBitAnalyzer.cc $localdir/src
cp $cvsdir/interface/HLTBitAnalyzer.h $localdir/interface
cp $cvsdir/test/HLTAna.py $localdir/test
scramv1 b

