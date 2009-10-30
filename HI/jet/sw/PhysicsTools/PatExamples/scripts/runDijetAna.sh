if [ $# -eq 0 ]; then
  echo Usage
  echo "  $0 <jetType> <trackType> [tag]"
  exit 1
fi

jetType=$1
trackType=$2
anatag=$3
input=/d01/frankma/scratch/data/pat/cmssw331/hydjetNoCollElossFix02/merged/all_50.root

infname=${input##/*/}
inname=${infname%.root}
tag=${inname}_${anatag}

cmd="DiJetFWLiteAnalyzer inputFiles=/d01/frankma/scratch/data/pat/cmssw331/hydjetNoCollElossFix02/merged/all_50.root jetType=$jetType trackType=$trackType tag=$tag outputevery=1 >& log/${tag}_j${jetType}_t${trackType}.txt &"

echo $cmd
eval "time $cmd"

