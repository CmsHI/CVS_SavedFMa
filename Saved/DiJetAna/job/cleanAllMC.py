import os
scratch="/net/hidsk0001/d00/scratch/frankma"
jobTag="MatchedJetGoodTrk1127v2"

# ================== 362 ===============
samples  = {
    #"HydjetQ_DJQ80_F10GSR":"ana",
    #"HydjetQ_DJUQ80_F10GSR":"ana"
    "Data_MinBias_DijetUnquenched80_d20101125and1126":"ana",
    "Data_MinBias_DijetUnquenched50_d20101127":"ana"
    }

for sample,PD in samples.items():
  outputDir=scratch+"/"+PD+"/"+sample+"_"+jobTag
  cmd="cd "+outputDir+";/net/hisrv0001/home/frankma/work/HI/jet/sw/Y1JAna_CMSSW_3_9_2_patch5/src/Saved/DiJetAna/job/cleanJob.sh; cd -"
  print cmd
  os.system(cmd)
