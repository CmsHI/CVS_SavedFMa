se=/mnt/hadoop/cms/store/user/frankmalocal/forest/qm12

# pbpb
cp /net/hidsk0001/d00/scratch/yjlee/merge/pbpbDijet_v20/promptskim-hihighpt-hltjet80-pt90-v20.root $se/ # full stat but with icpu5calo 90 skim
cp /mnt/hadoop/cms/store/user/frankmalocal/forest/HiForest-promptskim-hiForest2_v21_HLTFix.root $se/ # full stat, no tracks

# pp
cp /net/hidsk0001/d00/scratch/yjlee/hiForest/prod/productionPP/CMSSW_4_4_2_patch6/test/ppForest2/pp_merged_full.root $se/HiForest-ppskim-hihighpt-full.root

# pbpb mc
cp /net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat50/Dijet50_HydjetDrum_v27_mergedV1.root $se/
cp /net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat80/Dijet80_HydjetDrum_v27_mergedV1.root $se/
cp /net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat100/Dijet100_HydjetDrum_v27_mergedV1.root $se/
cp /net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat170/Dijet170_HydjetDrum_v27_mergedV1.root $se/
cp /net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat200/Dijet200_HydjetDrum_v28_mergedV1.root $se/
cp /net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat250/Dijet250_HydjetDrum_v28_mergedV1.root $se/
cp /net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat300/Dijet300_HydjetDrum_v28_mergedV1.root $se/

# pp mc
cp /net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat50/mergedFile.root $se/Dijet50_v27_merged.root
cp /net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat80/mergedFile.root $se/Dijet80_v27_merged.root
cp /net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat120/mergedFile.root $se/Dijet120_v27_merged.root
cp /net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat200/mergedFile.root $se/Dijet200_v27_merged.root
cp /net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat250/mergedFile.root $se/Dijet250_v27_merged.root
cp /net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat300/mergedFile.root $se/Dijet300_v27_merged.root