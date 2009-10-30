if [ $# -eq 0 ]; then
  echo Usage
  echo "  $0 <jetType> <trackType>"
  exit 1
fi

jetType=$1
trackType=$2
input=/d01/frankma/scratch/data/pat/cmssw331/hydjetNoCollElossFix02/merged/all_50.root
infname=${input##/*/}
inname=${infname%.root}
cmd="DiJetFWLiteAnalyzer inputFiles=/d01/frankma/scratch/data/pat/cmssw331/hydjetNoCollElossFix02/merged/all_50.root jetType=$jetType trackType=$trackType tag=$inname outputevery=1 >& log/${inname}_j${jetType}_t${trackTyep}.txt &"

echo $cmd
eval "time $cmd"

