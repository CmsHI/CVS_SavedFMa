for pthat in 120 170 200; do
  for i in `find /net/hisrv0001/home/frankma/hadoopfranklocal/forest/Hydjet18/DiJet${pthat}_iterTrk_v1/*.root -size -300`; do
    ls -ltr $i;
    rm $i
  done
done
