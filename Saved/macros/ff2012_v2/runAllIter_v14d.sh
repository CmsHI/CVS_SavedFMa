pthat=$1
pthatMax=$2
corrset=IterTrkCorrv14dXSec # 14d: 4 cent bins, drMatch=0.3, jet id

for algo in akPu3PF; do
   for genJetMode in 0; do
     ./runFullTrackCorrection_one.sh $corrset $algo $pthat $pthatMax 15 100 -1 -1 $genJetMode
   done
done
