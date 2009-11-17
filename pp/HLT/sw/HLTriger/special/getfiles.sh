# source code
cp -v /home/frankma/work/pp/HLT/sw/activityTrigCMSSW_3_3_2/src/HLTrigger/special/interface/HLTPixelActivityFilter.h interface/
cp -v /home/frankma/work/pp/HLT/sw/activityTrigCMSSW_3_3_2/src/HLTrigger/special/src/HLTCaloTowerFilter.cc src/
cp -v /home/frankma/work/pp/HLT/sw/activityTrigCMSSW_3_3_2/src/HLTrigger/special/src/HLTPixelActivityFilter.cc src/
# for plugin manager
cp -v /home/frankma/work/pp/HLT/sw/activityTrigCMSSW_3_3_2/src/HLTrigger/special/src/SealModule.cc src/


# python
cp -v /home/frankma/work/pp/HLT/sw/activityTrigCMSSW_3_3_2/src/HLTrigger/special/python/HLT_MinBiasPixel_cff.py python
cp -v /home/frankma/work/pp/HLT/sw/activityTrigCMSSW_3_3_2/src/HLTrigger/special/python/OnLine_HLT_TestEcal.py python
cp -v /d01/frankma/scratch/pp/HLT/activityTrigCMSSW_3_3_2/cfg/*.py python/

# test
cp -v /home/frankma/work/pp/HLT/sw/activityTrigCMSSW_3_3_2/src/HLTrigger/special/test/OnLine_HLT_Test.py test/
cp -v /d01/frankma/scratch/pp/HLT/activityTrigCMSSW_3_3_2/*.py test/

# scripts
cp -v /d01/frankma/scratch/pp/HLT/activityTrigCMSSW_3_3_2/*.sh scripts/
cp -v /d01/frankma/scratch/pp/HLT/activityTrigCMSSW_3_3_2/CRAFT09-v1/activity_hfmask1/*.sh scripts/

