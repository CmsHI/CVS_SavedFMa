subName='Nov20'

# pbpb data
jetAlgo='akPu3PF'
root -l -b -q 'forest2jetSkim.C+("/net/hidsk0001/d00/scratch/yjlee/merge/pbpbDijet_v20/promptskim-hihighpt-hltjet80-pt90-v20.root","jskim_hltjet80-pt90-v20_'$jetAlgo'_'$subName'",false,-1,"",false,"'$jetAlgo'",0,0,0)'

# pp
jetAlgo='ak3PF'
for i in {0..4}; do # 0 = no centrality weight 
  root -l -b -q 'forest2jetSkim.C+("/net/hisrv0001/home/yenjie/scratch/hiForest/prod/productionPP/CMSSW_4_4_2_patch6/test/ppForest2/pp_merged_full.root",  "jskim_pp-full_'$jetAlgo'_'$subName'",false,-1,"",false,"'$jetAlgo'",1,'$i',0)'
done

# old archive
#root -l -b -q 'forest2jetSkim.C+("/mnt/hadoop/cms/store/user/frankmalocal/forest/HiForest-promptskim-hiForest2_v21_HLTFix.root",  "jskim_hltjet80-v21_'$jetAlgo'_'$subName'",false,1000,"",false,"'$jetAlgo'",0,0,0)'
