src=/home/frankma/work/pp/dNdEta/sw/anaCMSSW_3_3_6/src
rundir=/d01/frankma/scratch/pp/dNdEta/anaCMSSW_3_3_6

# === pixel tree ===
swdir=MitQcd/TrackletsAna
# sw
# don't cp, switched to head version of MITQCD/TrackletAna
# as of 2010/1/11
#cp -v $src/$swdir/interface/PixelsMakerMod.h  interface/
#cp -v $src/$swdir/src/PixelsMakerMod.cc src/

# macros
cp -v $rundir/trackletAna/macros/*.C macros/
#cp -v $rundir/trackletAna/macros/*.h macros/
#project=loop
#project=diffractive_ana_900GeV
project=diffractive_ana_2TeV
cp -v $rundir/trackletAna/$project/*.C macros/
cp -v $rundir/trackletAna/*.h macros/

# scripts
cp -v $rundir/trackletAna/scripts/*.sh scripts/
cp -v $rundir/trackletAna/input/*.sh scripts/

