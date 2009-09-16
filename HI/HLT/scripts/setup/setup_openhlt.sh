project=openhlt
release=CMSSW_3_2_4
pname=${project}${release}
scramv1 project -n $pname CMSSW $release
cd $pname/src
cmsenv

# christof's open hlt code so far in 324
cvs co UserCode/HIHLTStudy
mv UserCode/HIHLTStudy/* .

# genjet 32x code
cvs co -r cmshi_32X UserCode/CmsHi/JetAnalysis
cvs co -r V00-01-02 UserCode/CmsHi/Utilities

# other needed code to compile
cvs co -r V00-01-02 DataFormats/HeavyIonEvent

# move to compile directory structure
mv UserCode/CmsHi .

# build
scramv1 b

# clean up
rm -rf UserCode

