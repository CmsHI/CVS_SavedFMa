pthat=$1
pthatMax=$2
corrset=IterTrkCorrv14dXSec_pp # d. drMatch<0.3 and add trackmaxpt/jetpt > 0.01 cut, no bkg leading jet filter

# for algo in akPu3PF; do
#for algo in akPu3Calo; do
for algo in ak3PF; do
   for genJetMode in 0; do
     ./runFullTrackCorrection_pp.sh $corrset $algo $pthat $pthatMax 15 100 -1 -1 $genJetMode
     #./runFullTrackCorrection_pp.sh $corrset $algo $pthat $pthatMax 15 -1 -1 -1 $genJetMode
   done
done
