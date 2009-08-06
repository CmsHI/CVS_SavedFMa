#!/bin/bash -

# inputs
pythiafile=$input0/$infileName
echo input0: $pythiafile
if [ ! -e $pythiafile ]; then
   echo bad input file: $pythiafile
   exit 1
fi
pyquenfile=$input1/$infileName
echo input1: $pyquenfile
if [ ! -e $pyquenfile ]; then
   echo bad input file: $pyquenfile
   exit 1
fi

# outputs
plotanabase="$plotbase/$job/$rootskim/$ana"
echo "plotanabase: $plotanabase"
if [ ! -e $plotbase/$job ]; then mkdir $plotbase/$job; fi
if [ ! -e $plotbase/$job/$rootskim ]; then mkdir $plotbase/$job/$rootskim; fi
if [ ! -e $plotanabase ]; then mkdir $plotanabase; fi
plotjleading=$plotanabase/jleading
echo "$plotjleading"
if [ ! -e $plotjleading ]; then mkdir $plotjleading; fi
plotjFF=$plotanabase/jFF
echo "$plotjFF"
if [ ! -e $plotjFF ]; then mkdir $plotjFF; fi
plotjShapes=$plotanabase/jShapes
echo "$plotjShapes"
if [ ! -e $plotjShapes ]; then mkdir $plotjShapes; fi

# run
# -Jet-
root -b -q plotJetLeading.C+\(true,\"$pythiafile\",\"$pyquenfile\",\"$plotjleading\"\)
#root -b -q checkBadCones.C+\(true,\"$pythiafile\",\"$pyquenfile\",\"$plotjleading\"\)
echo
# -Particles-
root -b -q plotFF.C+\(\"$pythiafile\",\"$pyquenfile\",$PythiaAnaNJetEtMin,$PythiaAnaNJetEtMax,$PythiaAnaAJetEtMin,$PyquenAnaNJetEtMin,$PyquenAnaNJetEtMax,$PyquenAnaAJetEtMin,$JDPhiMin,\"$plotjFF\",$NXIBIN,$XIMAX,$XIYMAX,$doCheck\)
root -b -q plotShapes.C+\(\"$pythiafile\",\"$pyquenfile\",$PythiaAnaNJetEtMin,$PythiaAnaNJetEtMax,$PythiaAnaAJetEtMin,$PyquenAnaNJetEtMin,$PyquenAnaNJetEtMax,$PyquenAnaAJetEtMin,$JDPhiMin,\"$plotjShapes\",$NXIBIN,$XIMAX,$XIYMAX,$doCheck\)

# final plots
plotFinalpath=$plotanabase/final
if [ ! -e $plotFinalpath ]; then mkdir $plotFinalpath; fi
leadfile=$plotanabase/jleading/LeadingHistos.root
FFfile=$plotanabase/jFF/FFHistos.root
root -b -q plotFinal.C+\(\"$leadfile\",\"$FFfile\",\"$plotFinalpath\"\)

