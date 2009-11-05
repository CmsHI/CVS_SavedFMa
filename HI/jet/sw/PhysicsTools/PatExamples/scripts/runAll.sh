#signal=pyquen_unquendijet_nocoll_pt120to170_d20091025
signal=pyquen_quendijet_nocoll_pt120to170_d20091024
indir=rfio:/castor/cern.ch/user/f/frankma/data/pat/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV/$signal
n=2
tag=sw46_2
/home/frankma/work/HI/jet/sw/pat/patanaCMSSW_3_3_1/src/PhysicsTools/PatExamples/scripts/runFullList.sh $n $tag 2 1 1 $indir
/home/frankma/work/HI/jet/sw/pat/patanaCMSSW_3_3_1/src/PhysicsTools/PatExamples/scripts/runFullList.sh $n $tag 2 0 1 $indir
/home/frankma/work/HI/jet/sw/pat/patanaCMSSW_3_3_1/src/PhysicsTools/PatExamples/scripts/runFullList.sh $n $tag 1 1 0 $indir
/home/frankma/work/HI/jet/sw/pat/patanaCMSSW_3_3_1/src/PhysicsTools/PatExamples/scripts/runFullList.sh $n $tag 0 1 0 $indir
