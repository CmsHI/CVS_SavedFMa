if [ $# -eq 0 ]; then
  echo Usage
  echo "  <n> <tag> <jtype> <ptype> <cor>"
  exit 1
fi

n=$1
tag=$2
jtype=$3
ptype=$4
cor=$5
dir=/castor/cern.ch/user/f/frankma/data/pat/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV/
for file in `rfdir $dir | awk '{print $NF}' | head -n $n`; do
  /home/frankma/work/HI/jet/sw/pat/patanaCMSSW_3_3_1/src/PhysicsTools/PatExamples/scripts/runDijetAna.sh $tag $jtype $ptype $cor $file
done
