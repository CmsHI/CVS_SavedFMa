import os
os.system("./create_custom_python_cfg.sh")

scratch="/net/hidsk0001/d00/scratch/frankma"
jobTag="JetTrkv5Json"

# ================== 362 ===============
samples  = {
    # rereco
    "HAZSv2_SD_JetHI-v1":"ana"
    }

for sample,PD in samples.items():
  inputList="~/inputs_data/"+sample+".txt"
  outputDir=scratch+"/"+PD+"/"+sample+"_"+jobTag
  cmd="./submitMergedJob.sh custom_HiJetTrk_data_2760_PFTowers_noMuons_cfg.py "+inputList+" "+outputDir
  print cmd
  os.system(cmd)
