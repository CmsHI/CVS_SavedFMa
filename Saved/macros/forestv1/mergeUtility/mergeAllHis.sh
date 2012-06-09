outdir=../trkcorr/Forest2_v19
mkdir -p $outdir
for pthat in 170 120 80 50 30; do
   ./hadd_large.sh /mnt/hadoop/cms/store/user/yetkin/MC_Production/Pythia${pthat}_HydjetDrum_mix01/HiForest2_v19 $outdir/trkcorr_hy18dj${pthat}_Forest2_v19.root
done

for pthat in 170 120 80 50 30; do
   ./hadd_large.sh /mnt/hadoop/cms/store/user/yetkin/MC_Production/Pythia${pthat}_signal01/HiForest2_v19/ $outdir/trkcorr_sigdj${pthat}_Forest2_v19.root
done
