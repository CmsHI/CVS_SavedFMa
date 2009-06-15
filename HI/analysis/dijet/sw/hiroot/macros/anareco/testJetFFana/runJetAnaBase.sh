#!/bin/bash -
#echo "HIROOT: $HIROOT"
#echo "with libs: $LD_LIBRARY_PATH"
#if [ ! -e ana ]; then mkdir ana; fi
#cp $HIROOT/macros/ana/savedfrankTools.C ana/

## basic var's
#base=/net/pstore01/d00/scratch/frankma/hiroot
##== input vars==
#job=prodPtHat50
#gen0=pythia50
#gen1=pyquen50
#infileName=test_50k.root
##ana=iCone5_match35_jetEtCut30
#skim=iCone5_match35_jetEtCut30_ntJetEt
##== output vars ===
#ana=ptMin100
#plotbase=$dijetSens/jetana/plots

# inputs
pythiafile=$input0/$infileName
echo input0: $pythiafile
if [ ! -e $pythiafile ]; then
   echo bad input file: $pythiafile
   exit
fi
pyquenfile=$input1/$infileName
echo input1: $pyquenfile
if [ ! -e $pyquenfile ]; then
   echo bad input file: $pyquenfile
   exit
fi

# outputs
#job=prodPtHat50
plotanabase="$plotbase/$job/$skim/$ana"
echo "plotanabase: $plotanabase"
if [ ! -e $plotbase/$job ]; then mkdir $plotbase/$job; fi
if [ ! -e $plotbase/$job/$ana ]; then mkdir $plotbase/$job/$ana; fi
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

# final plots
plotFinalpath=$plotanabase/final
if [ ! -e $plotFinalpath ]; then mkdir $plotFinalpath; fi
leadfile=$dijetSens/jetana/runAna/LeadingHistos.root
FFfile=$dijetSens/jetana/runAna/FFHistos.root
root -b -q plotFinal.C+\(\"$leadfile\",\"$FFfile\",\"$plotFinalpath\"\)

