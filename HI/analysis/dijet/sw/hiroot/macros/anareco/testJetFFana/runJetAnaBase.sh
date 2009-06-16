#!/bin/bash -

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
plotanabase="$plotbase/$job/$rootskim/$ana"
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
# -pythia-
root -b -q plotJetLeading.C+\(true,\"$pythiafile\",\"$pyquenfile\",\"$plotjleading\"\)
echo
# -pyquen-
root -b -q plotFF.C+\(\"$pythiafile\",\"$pyquenfile\",\"$PythiaAnaJetEtMin\",\"$PythiaAnaJetEtMax\",\"$PyquenAnaJetEtMin\",\"$PyquenAnaJetEtMax\",\"$plotjFF\",$NXIBIN\)

# final plots
plotFinalpath=$plotanabase/final
if [ ! -e $plotFinalpath ]; then mkdir $plotFinalpath; fi
leadfile=$dijetSens/jetana/runAna/LeadingHistos.root
FFfile=$dijetSens/jetana/runAna/FFHistos.root
root -b -q plotFinal.C+\(\"$leadfile\",\"$FFfile\",\"$plotFinalpath\"\)

