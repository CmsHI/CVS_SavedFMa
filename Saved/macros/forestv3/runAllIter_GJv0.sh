if [ $# -lt 2 ]; then
   echo "$0 <pthatmin> <pthatmax>"
   exit 1
fi

pthat=$1
pthatMax=$2
corrset=IterTrkCorr_GammaJetv0

# for algo in akPu3PF akPu3Calo; do
for algo in akPu3Calo; do
#    ./runGJTrackingCorr.sh $corrset $algo $pthat $pthatMax 15 40 -1 -1 0
   ./runGJTrackingCorr.sh $corrset $algo $pthat $pthatMax 15 40 20 3.14159/2. 0
done
