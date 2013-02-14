subName='Feb14_4bin_sm18'

#################################################################################
# Fix: Use unsmeared jet pt for looking up tracking efficiency table
# - Jan 17 Use unsmeared jet pt for looking up tracking efficiency table
# - Jan 26 same Jan 17 fix, but for real this time
# Features:
# - Jan 26, more smearing options for pbpb
# - Feb 25, trkcorr_v14d
#   * trkcorr: drMatch=0.3, add jet id, ...
#################################################################################

useGenJet=0

# pp
jetAlgo='ak3PF'
for smMode in 2; do # 0=do nothing, 1=shift, 2=smear, 3=shift+smear
  for i in {1..4}; do # 0 = no centrality weight
  # for i in 1; do # 0 = no centrality weight, 1-4 four centrality bins
  # for i in 0; do # 0 = no centrality weight, 1-4 four centrality bins
    echo pp smearMode: $smMode
    root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/yenjie/scratch/hiForest/prod/productionPP/CMSSW_4_4_2_patch6/test/ppForest2/pp_merged_full.root",  "jskim_pp-full_'$jetAlgo'_'$subName'",false,-1,"",false,"'$jetAlgo'",1,'$smMode,$i','$useGenJet')'
    # root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat50/mergedFile.root",  "jskim_dj50_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,'$smMode,$i','$useGenJet')'
    root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat80/mergedFile.root",  "jskim_dj80_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,'$smMode,$i','$useGenJet',80,120)'
    root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat120/mergedFile.root",  "jskim_dj120_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,'$smMode,$i','$useGenJet',120,170)'
    root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat170/mergedFile.root",  "jskim_dj170_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,'$smMode,$i','$useGenJet')'
    # root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat250/mergedFile.root",  "jskim_dj250_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,'$smMode,$i','$useGenJet')'
  done
done
root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/yenjie/scratch/hiForest/prod/productionPP/CMSSW_4_4_2_patch6/test/ppForest2/pp_merged_full.root",  "jskim_pp-full_'$jetAlgo'_'$subName'",false,-1,"",false,"'$jetAlgo'",1,0,0,'$useGenJet')'
# root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat50/mergedFile.root",  "jskim_dj50_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,0,0,'$useGenJet')'
root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat80/mergedFile.root",  "jskim_dj80_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,0,0,'$useGenJet',80,120)'
root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat120/mergedFile.root",  "jskim_dj120_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,0,0,'$useGenJet',120,170)'
root -l -b -q 'forest2jetSkim.C++("/net/hisrv0001/home/zhukova/scratch/HIHighPt/forest/pthat170/mergedFile.root",  "jskim_dj170_'$jetAlgo'_'$subName'",false,-1,"",true,"'$jetAlgo'",1,0,0,'$useGenJet')'

# pbpb
jetAlgo='akPu3PF'
for smMode in 1; do # 0=do nothing, 1=shift
  echo pbpb smearMode: $smMode
  # data
  root -l -b -q 'forest2jetSkim.C+("/net/hidsk0001/d00/scratch/yjlee/merge/pbpbDijet_v20/promptskim-hihighpt-hltjet80-pt90-v20.root","jskim_hltjet80-pt90-v20_'$jetAlgo'_'$subName'",false,-1,"",false,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
  if [ $smMode -gt 1 ]; then break; fi
  # root -l -b -q 'forest2jetSkim.C+("/mnt/hadoop/cms/store/user/frankmalocal/forest/HiForest-promptskim-hiForest2_v21_HLTFix.root","jskim_hltjet80-v21_'$jetAlgo'_'$subName'",false,-1,"",false,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
  # mc
  # root -l -b -q 'forest2jetSkim.C++("/net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat50/Dijet50_HydjetDrum_v27_mergedV1.root","jskim_hydj50_'$jetAlgo'_'$subName'",true,-1,"",true,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
  root -l -b -q 'forest2jetSkim.C++("/net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat80/Dijet80_HydjetDrum_v27_mergedV1.root","jskim_hydj80_'$jetAlgo'_'$subName'",true,-1,"",true,"'$jetAlgo'",0,'$smMode',0,'$useGenJet',80,100)'
  root -l -b -q   'forest2jetSkim.C++("/net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat100/Dijet100_HydjetDrum_v27_mergedV1.root","jskim_hydj100_'$jetAlgo'_'$subName'",true,-1,"",true,"'$jetAlgo'",0,'$smMode',0,'$useGenJet',100,170)'
  # root -l -b -q 'forest2jetSkim.C++("/net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat120/mergedFile.root","jskim_hydj120_'$jetAlgo'_'$subName'",true,-1,"",true,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
  root -l -b -q 'forest2jetSkim.C++("/net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat170/Dijet170_HydjetDrum_v27_mergedV1.root","jskim_hydj170_'$jetAlgo'_'$subName'",true,-1,"",true,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
  # root -l -b -q 'forest2jetSkim.C++("/net/hidsk0001/d00/scratch/yjlee/merge/v27/pthat250/Dijet250_HydjetDrum_v28_mergedV1.root","jskim_hydj250_'$jetAlgo'_'$subName'",true,-1,"",true,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
  # root -l -b -q 'forest2jetSkim.C++("/export/d00/scratch/yjlee/merge/pyquen/pyquenWide80/pyquenWide80_HYDJET.root","jskim_hydj80quenwide_'$jetAlgo'_'$subName'",true,-1,"",true,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
  # root -l -b -q 'forest2jetSkim.C++("/export/d00/scratch/yjlee/merge/pyquen/pyquenWide100/pyquenWide100_HYDJET.root","jskim_hydj100quenwide_'$jetAlgo'_'$subName'",true,-1,"",true,"'$jetAlgo'",0,'$smMode',0,'$useGenJet')'
done
