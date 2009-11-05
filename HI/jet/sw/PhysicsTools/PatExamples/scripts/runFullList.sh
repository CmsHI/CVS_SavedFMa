if [ $# -eq 0 ]; then
  echo Usage
  echo "  <n> <tag> <jtype> <cor> <trackType> [indir]"
  exit 1
fi

n=$1
tag=$2
jtype=$3
cor=$4
ptype=$5
indir=$6
if [ -z $indir ]; then indir=/castor/cern.ch/user/f/frankma/data/pat/CMSSW_3_3_1_fix03/Hydjet_MinBias_noColl_4TeV/; fi

for file in `rfdir $indir | awk '{print $NF}' | head -n $n`; do
  /home/frankma/work/HI/jet/sw/pat/patanaCMSSW_3_3_1/src/PhysicsTools/PatExamples/scripts/runDijetAna.sh $tag $jtype $cor $ptype $file $indir
done
