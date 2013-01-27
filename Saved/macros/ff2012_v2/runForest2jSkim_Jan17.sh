subName='Jan17_4bin_sm18'
#
# Fix: Use unsmeared jet pt for looking up tracking efficiency table
# - Jan 17 Use unsmeared jet pt for looking up tracking efficiency table
# Features:
# - Jan 26, more smearing options for pbpb
#
useGenJet=0

# pp
jetAlgo='ak3PF'
for smMode in 2; do # 1=shift, 2=smear, 3=shift+smear
  for i in {1..4}; do # 0 = no centrality weight
    echo pp: $smMode
    # root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/yenjie/scratch/hiForest/prod/productionPP/CMSSW_4_4_2_patch6/test/ppForest2/pp_merged_full.root",  "jskim_pp-full_'$jetAlgo'_'$subName'",false,-1,"",false,"'$jetAlgo'",1,'$smMode,$i','$useGenJet')'
    # root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat50/mergedFile.root",  "jskim_dj50_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,'$smMode,$i','$useGenJet')'
    # root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat80/mergedFile.root",  "jskim_dj80_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,'$smMode,$i','$useGenJet')'
    # root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat120/mergedFile.root",  "jskim_dj120_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,'$smMode,$i','$useGenJet')'
  done
done
# root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/yenjie/scratch/hiForest/prod/productionPP/CMSSW_4_4_2_patch6/test/ppForest2/pp_merged_full.root",  "jskim_pp-full_'$jetAlgo'_'$subName'",false,-1,"",false,"'$jetAlgo'",1,0,0,'$useGenJet')'
# root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat50/mergedFile.root",  "jskim_dj50_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,0,0,'$useGenJet')'
# root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat80/mergedFile.root",  "jskim_dj80_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,0,0,'$useGenJet')'
# root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat120/mergedFile.root",  "jskim_dj120_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,0,0,'$useGenJet')'

# pbpb
jetAlgo='akPu3PF'
for smMode in 3; do # 0=do nothing, 1=shift
  echo pbpb: $smMode
  # data
  root -l -b -q 'forest2jetSkim.C+("/net/hidsk0001/d00/scratch/yjlee/merge/pbpbDijet_v20/promptskim-hihighpt-hltjet80-pt90-v20.root","jskim_hltjet80-pt90-v20_'$jetAlgo'_'$subName'",false,-1,"",false,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
  if [ $smMode -gt 1 ]; then break; fi
  # root -l -b -q 'forest2jetSkim.C+("/mnt/hadoop/cms/store/user/frankmalocal/forest/HiForest-promptskim-hiForest2_v21_HLTFix.root","jskim_hltjet80-v21_'$jetAlgo'_'$subName'",false,-1,"",false,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
  # mc
  # root -l -b -q 'forest2jetSkim.C++("/net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat50/Dijet50_HydjetDrum_v27_mergedV1.root","jskim_hydj50_'$jetAlgo'_'$subName'",true,-1,"",true,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
  # root -l -b -q 'forest2jetSkim.C++("/net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat80/Dijet80_HydjetDrum_v27_mergedV1.root","jskim_hydj80_'$jetAlgo'_'$subName'",true,-1,"",true,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
  # root -l -b -q 'forest2jetSkim.C++("/mnt/hadoop/cms/store/user/yenjie/HiForest_v27/Dijet100_HydjetDrum_v27_mergedV1.root","jskim_hydj100_'$jetAlgo'_'$subName'",true,-1,"",true,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
  # root -l -b -q 'forest2jetSkim.C++("/net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat120/mergedFile.root","jskim_hydj120_'$jetAlgo'_'$subName'",true,-1,"",true,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
done
