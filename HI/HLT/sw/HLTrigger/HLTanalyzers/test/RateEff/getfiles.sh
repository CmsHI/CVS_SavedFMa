swdir=/net/pstore01/d00/scratch/frankma/HI/HLT/openhltCMSSW_3_2_4/HIOpenHLT_1_0/RateEff
rundir=/net/pstore01/d00/scratch/frankma/HI/HLT/openhltCMSSW_3_2_4/HIOpenHLT_1_0

cp -v $swdir/OHltConfig.cpp .
cp -v $swdir/OHltTree.cpp .
cp -v $swdir/PlotOHltEffCurves.cpp .
# with lead ntuple
cp -v $swdir/OHltTree.h .
cp -v $swdir/OHltRateEff.cpp .

cp -v $rundir/hltmenu_HIon_2009Sep.cfg .
