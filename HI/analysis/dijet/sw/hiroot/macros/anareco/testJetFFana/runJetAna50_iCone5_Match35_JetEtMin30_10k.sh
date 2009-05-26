#!/bin/bash -
. /net/hisrv0001/home/frankma/UserCode/SavedFMa/bin/cgate/set_hiroot.sh
#echo "HIROOT: $HIROOT"
#echo "with libs: $LD_LIBRARY_PATH"
#if [ ! -e ana ]; then mkdir ana; fi
#cp $HIROOT/macros/ana/savedfrankTools.C ana/

# basic var's
base=/net/pstore01/d00/scratch/frankma/hiroot
gen0=pythia50_10k
gen1=pyquen50_10k
#ana=iCone5_match35_jetEtCut30
ana=iCone5_match35_jetEtCut30_ntJetEt

# inputs
export HIROOT_BASEURL="$base/$gen0/$ana"
echo "HIROOT_BASEURL: $HIROOT_BASEURL"
pythiafile=$HIROOT_BASEURL/dijet_000777.root
echo $pythiafile
if [ ! -e $pythiafile ]; then
   echo bad input file: $pythiafile
   return
fi
export HIROOT_BASEURL="$base/$gen1/$ana"
echo "HIROOT_BASEURL: $HIROOT_BASEURL"
pyquenfile=$HIROOT_BASEURL/dijet_000777.root
echo $pyquenfile
if [ ! -e $pyquenfile ]; then
   echo bad input file: $pyquenfile
   return
fi

# outputs
gen=pTHatMin50
plotbase=$dijetSens/jetana/plots
plotanabase="$plotbase/$gen/$ana"
echo "plotanabase: $plotanabase"
if [ ! -e $plotbase/$gen ]; then mkdir $plotbase/$gen; fi
if [ ! -e $plotanabase ]; then mkdir $plotanabase; fi
plotjleading=$plotanabase/jleading
echo "$plotjleading"
if [ ! -e $plotjleading ]; then mkdir $plotjleading; fi
plotjFF=$plotanabase/jFF
echo "$plotjFF"
if [ ! -e $plotjFF ]; then mkdir $plotjFF; fi

# run
#if [ ! -e log ]; then mkdir log; fi
# -pythia-
root -b -q plotJetLeading.C+\(true,\"$pythiafile\",\"$pyquenfile\",\"$plotjleading\"\)
echo "root -b -q plotJetLeading.C+\(true,\"$pythiafile\",\"$pyquenfile\",\"$plotjleading\"\)"
echo
# -pyquen-
root -b -q plotFF.C+\(\"$pythiafile\",\"$pyquenfile\",\"$plotjFF\"\)
echo "root -b -q plotFF.C+\(\"$pythiafile\",\"$pyquenfile\",\"$plotjFF\"\)"

