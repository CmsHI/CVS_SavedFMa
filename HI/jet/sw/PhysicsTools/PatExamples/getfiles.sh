#cmssw_base=/home/frankma/work/HI/jet/sw/patanaCMSSW_3_3_0_pre5
cmssw_base=/home/frankma/work/HI/jet/sw/pat/patanaCMSSW_3_3_1

echo =========== source code ==============
cp -v $cmssw_base/src/PhysicsTools/PatExamples/BuildFile .
cp -v $cmssw_base/src/PhysicsTools/PatExamples/interface/*.h interface
cp -v $cmssw_base/src/PhysicsTools/PatExamples/src/*.cc src
echo

echo ============= bin ====================
cp -v $cmssw_base/src/PhysicsTools/PatExamples/bin/BuildFile bin
cp -v $cmssw_base/src/PhysicsTools/PatExamples/bin/*.cc bin
echo

echo "============= test  ===================="
cp -v $cmssw_base/src/PhysicsTools/PatExamples/test/BuildFile test/
cp -v $cmssw_base/src/PhysicsTools/PatExamples/test/*.cc test/
echo

# macros
echo ============= macros ====================
cp -v $cmssw_base/src/PhysicsTools/PatExamples/macros/*.C macros
cp -v $cmssw_base/src/PhysicsTools/PatExamples/macros/*.h macros
cp -v -r $cmssw_base/src/PhysicsTools/PatExamples/macros/DiJetTreeAna/*.C macros/DiJetTreeAna/
cp -v -r $cmssw_base/src/PhysicsTools/PatExamples/macros/DiJetTreeAna/*.h macros/DiJetTreeAna/
cp -v -r $cmssw_base/src/PhysicsTools/PatExamples/macros/DiJetTreeAna/*.sh macros/DiJetTreeAna/
echo

# scripts
echo ============= scripts  ====================
cp -v $cmssw_base/src/PhysicsTools/PatExamples/scripts/* scripts/
echo

# data
echo ============= data ====================
cp -v $cmssw_base/src/PhysicsTools/PatExamples/data/*.sh data/
echo

