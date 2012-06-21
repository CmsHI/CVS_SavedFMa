#for i in 120 170 200; do
#  indir=/net/hisrv0001/home/frankma/hadoopfranklocal/forest/Hydjet18/DiJet${i}_iterTrk_v1
#  outfile=/net/hidsk0001/d00/scratch/frankma/tmp/DiJet${i}_iterTrk_v1.root
#  root -b -q mergeForest_mpt_mc_pthats.C+'("'$indir'","'$outfile'")'
#  mv $outfile /net/hisrv0001/home/frankma/hadoopfranklocal/forest/Hydjet18/
#done
root -b -q mergeForest2.C+
mv /net/hidsk0001/d00/scratch/frankma/tmp/HiForest-promptskim-hihighpt-hltjet80-pt90-v2_v3_part2.root /mnt/hadoop/cms/store/user/frankmalocal/forest/


