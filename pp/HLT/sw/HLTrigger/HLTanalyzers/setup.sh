cvsdir=UserCode/SavedFMa/pp/HLT/sw/HLTrigger/HLTanalyzers
localdir=HLTrigger/HLTanalyzers

# setup project area
#cd $CMSSW_BASE/src
#eval `scramv1 runtime -sh`

# check-out code
cvs co -r HEAD HLTrigger/HLTanalyzers
cvs co -r HEAD $cvsdir

# cp the private code into HLTrigger/HLTanalyzers
cp $cvsdir/src/HLTAnaInfo.cc $localdir/src
cp $cvsdir/interface/HLTAnaInfo.h $localdir/interface
cp $cvsdir/src/HLTBitAnalyzer.cc $localdir/src
cp $cvsdir/interface/HLTBitAnalyzer.h $localdir/interface
cp $cvsdir/python/HLTBitAnalyser_cfi.py $localdir/python
cp $cvsdir/test/HLTBitAnalysis_cfg.py $localdir/test
touch HLTrigger/HLTanalyzers/BuildFile
#scramv1 b

