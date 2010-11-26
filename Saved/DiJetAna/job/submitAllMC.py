import os
os.system("./create_custom_python_cfg.sh")

scratch="/net/hidsk0001/d00/scratch/frankma"
jobTag="StdJetGoodTrk1126"

# ================== 362 ===============
samples  = {
    #"HydjetQ_DJQ80_F10GSR":"ana",
    #"HydjetQ_DJUQ80_F10GSR":"ana"
    "Data_MinBias0to20_DijetUnquenched50_d20101124":"ana"
    }

for sample,PD in samples.items():
  inputList="~/inputs_mc/"+sample+".txt"
  outputDir=scratch+"/"+PD+"/"+sample+"_"+jobTag
  cmd="./submitJob.sh "+inputList+" "+outputDir
  print cmd
  os.system(cmd)
