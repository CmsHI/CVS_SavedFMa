cvsdir=UserCode/SavedFMa/pp/HLT/sw/HLTrigger/HLTanalyzers
localdir=HLTrigger/HLTanalyzers
cd $CMSSW_BASE/src
eval `scramv1 runtime -sh`
cvs co -r CMSSW_3_3_4 HLTrigger/HLTanalyzers
cvs co $cvsdir
cp $cvsdir/src/HLTAnalyzer.cc $localdir/src
cp $cvsdir/interface/HLTAnalyzer.h $localdir/interface
cp $cvsdir/python/HLTAnalyser_cfi.py $localdir/python
cp $cvsdir/test/HLTAna.py $localdir/test
scramv1 b

