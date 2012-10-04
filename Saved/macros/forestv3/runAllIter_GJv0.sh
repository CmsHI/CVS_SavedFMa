if [ $# -lt 2 ]; then
   echo "$0 <pthatmin> <pthatmax>"
   exit 1
fi

pthat=$1
pthatMax=$2
corrset=IterTrkCorr_GammaJetv1test

#for algo in akPu3Calo; do
for algo in akPu3PF; do
  if [ $pthat -ge 0 ]; then
    ./runGJTrackingCorr.sh $corrset $algo $pthat $pthatMax 15 40 -1 -1 0
  else
    ./runGJTrackingCorr.sh $corrset $algo $pthat $pthatMax 15 -1 -1 -1 0
  fi
done
