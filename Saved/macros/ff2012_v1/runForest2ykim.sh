
# MINBIAS data
root -l -b -q 'forest2Yskim.C+("/mnt/hadoop/cms/store/user/velicanu/forest/HiForestMinBias_v2.root","yskim_HiForestMinBias_v5_first.root",-1,false,15000,"",false,false)'

root -l -b -q 'forest2Yskim.C++("/mnt/hadoop/cms/store/user/velicanu/forest/HiForestMinBias_v2.root","yskim_HiForestMinBias_v5_second.root",-1,false,10000,"yskim_HiForestMinBias_v5_first.root",false,true)'

# triggered data
root -l -b -q 'forest2Yskim.C++("/mnt/hadoop/cms/store/user/velicanu/forest/HiForestPhoton-v7-noDuplicate.root","yskim_HiForestPhoton-v7-noDuplicate_nMix10_NoEvtPlReq_v5Mixed.root",60,false,8000,"yskim_HiForestMinBias_v5_second.root",false,false,"yskim_HiForestMinBias_v5_first.root")'

# MC
#root -l -b -q 'forest2Yskim.C++("/mnt/hadoop/cms/store/user/jazzitup/hiForest/mergedFiles/forest_hydjetDrum_MB_20k.root","yskim_forest_hydjetDrum_MB_20k_v5_first.root",-1,false,-1,"",true,false)'

#root -l -b -q 'forest2Yskim.C++("/mnt/hadoop/cms/store/user/jazzitup/hiForest/mergedFiles/forest_hydjetDrum_MB_20k.root","yskim_forest_hydjetDrum_MB_20k_v5_second.root",-1,false,-1,"yskim_forest_hydjetDrum_MB_20k_v5_first.root",true,true)'

#root -l -b -q 'forest2Yskim.C++("/mnt/hadoop/cms/store/user/jazzitup/hiForest/mergedFiles/qcdAllPhoton30_allCent.root","yskim_qcdAllPhoton30_allCent_nMix10_v5.root",50,true,-1,"yskim_forest_hydjetDrum_MB_20k_v5_second.root",true,false)'                                                               
#root -l -b -q 'forest2Yskim.C++("/mnt/hadoop/cms/store/user/jazzitup/hiForest/mergedFiles/qcdAllPhoton50_allCent.root","yskim_qcdAllPhoton50_allCent_nMix10_v5.root",50,true,-1,"yskim_forest_hydjetDrum_MB_20k_v5_second.root",true,false)'   

