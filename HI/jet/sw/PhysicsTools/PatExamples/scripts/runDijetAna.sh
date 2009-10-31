if [ $# -eq 0 ]; then
  echo Usage
  echo "  $0 <jetType> <trackType> [tag]"
  exit 1
fi

jetType=$1
trackType=$2
anatag=$3
#input=/d01/frankma/scratch/data/pat/cmssw331/hydjetNoCollElossFix02/merged/all_50.root
input=rfio:/castor/cern.ch/user/f/frankma/data/HI/jet/pat/cmssw331/all_300.root

infname=${input##*/}
inname=${infname%.root}
tag=${inname}_${anatag}

cmd="DiJetFWLiteAnalyzer inputFiles=$input jetType=$jetType trackType=$trackType tag=$tag outputevery=1 >& log/${tag}_j${jetType}_t${trackType}.txt &"

echo $cmd
eval "time $cmd"

