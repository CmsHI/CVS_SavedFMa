# $Id: setup.sh,v 1.12 2009/12/02 23:58:26 loizides Exp $

cvsdir=UserCode/SavedFMa/pp/HLT/sw/HLTrigger/HLTanalyzers
localdir=HLTrigger/HLTanalyzers

# check-out code
rm -rf HLTrigger/HLTanalyzers
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
rename HLTrigger/HLTanalyzers/python/HLTopen_cff.py HLTrigger/HLTanalyzers/python/HLTopen_cff.py.keep


