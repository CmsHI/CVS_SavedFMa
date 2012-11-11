subName='Oct31fixsmear-v8-8VtxBin-24PlnBin-40CentBin'
# subName='Oct16-v8-8VtxBin-24PlnBin-40CentBin-icPu5Calo90Skim'

# pbpb data
jetAlgo='akPu3PF'
root -l -b -q 'forest2jetSkim.C++("/net/hidsk0001/d00/scratch/yjlee/merge/pbpbDijet_v20/promptskim-hihighpt-hltjet80-pt90-v20.root",  "jskim_HiForest-promptskim-hihighpt-hltjet80-pt90-v20_'$jetAlgo'_'$subName'",false,10000,"/mnt/hadoop/cms/store/user/jazzitup/hiForest/skimmed/yskim_HiForestMinBias_v6_first_July-v8-8VtxBin-24PlnBin-40CentBin.root",false,"'$jetAlgo'",0,0,0)'

# pbpb mc
# root -l -b -q 'forest2jetSkim.C++("/mnt/hadoop/cms/store/user/yenjie/HiForest_v27/Dijet50_HydjetDrum_v27_mergedV1.root","jskim_Dijet50_HydjetDrum_v27_mergedV1_nMix10_'$jetAlgo'_'$subName'",true,-1,"yskim_HydjetDrum03_HiForest_v05_merged_test02_first_July-v6-8VtxBin-24PlnBin-40CentBin.root", true,"'$jetAlgo'",0,0,0)'

# pp
# jetAlgo='ak3PF'
# for i in {0..4}; do # 0 = no centrality weight
#   root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/yenjie/scratch/hiForest/prod/productionPP/CMSSW_4_4_2_patch6/test/ppForest2/pp_merged_full.root","jskim_forest2_2760GeV_ppJet_full_'$jetAlgo'_'$subName'",false,-1,"/mnt/hadoop/cms/store/user/jazzitup/hiForest/skimmed/yskim_HiForestMinBias_v6_first_July-v8-8VtxBin-24PlnBin-40CentBin.root",false,"'$jetAlgo'",1,'$i',0)'
# done
# 
