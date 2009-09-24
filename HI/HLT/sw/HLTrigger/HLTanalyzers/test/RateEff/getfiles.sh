swdir=/net/hisrv0001/home/frankma/work/HI/HLT/sw/openhltCMSSW_3_2_4/RateEffHI_HEAD/src/HLTrigger/HLTanalyzers/test/RateEff

cp -v $swdir/OHltConfig.cpp .
cp -v $swdir/OHltTree.cpp .
cp -v $swdir/PlotOHltEffCurves.cpp .
# with lead ntuple
cp -v $swdir/OHltTree.h .
cp -v $swdir/OHltRateEff.cpp .

cp -v $swdir/hltmenu_HIon_2009Sep.cfg .
