#root -l -b -q 'addOrder.C++("/d100/velicanu/forest/HiForestPhoton35_Skim.root","barrelHiForestPhoton35_Skim2011.root",1.444,10, false)'
#root -l -b -q 'addOrder.C+("/d100/yjlee/hiForest/merged_HI2010_SD_Photon40_prod02.root","barrelHiForestPhoton35_Skim2010.root",1.444,35, true,"")'
#root -l -b -q 'addOrder.C+("/d100/velicanu/forest/merged/HiForestPhoton_v1.root","barrelHiForestPhoton35_Skim2011-Nov29.root",1.444, 0, false,"HLT_HIMinBiasHfOrBSC_v1")'         
#root -l -b -q 'addOrder.C+("mc/photon30_25k.root","barrelHiForestPhoton35_MCphoton30_25k.root",1.444,40,false)'
#root -l -b -q 'addOrder.C+("mc/photon50_25k.root","barrelHiForestPhoton35_MCphoton50_25k.root",1.444,40,false)'
#root -l -b -q 'addOrder.C+("mc/photon80_25k.root","barrelHiForestPhoton35_MCphoton80_25k.root",1.444,40,false)'
root -l -b -q 'addOrder.C+("/d102/velicanu/forest/merged/HiForestPhoton_v2.root","barrelHiForestPhoton35_Skim2011-Dec04.root",1.444, 40, false,"")'                                         

