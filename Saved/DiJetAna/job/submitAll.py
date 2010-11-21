import os
os.system("./create_custom_python_cfg.sh")

scratch="/net/hidsk0001/d00/scratch/frankma"
jobTag="djlook1120"

# ================== 362 ===============
samples  = {
    "HCPR-J50U-150883to150887-PAT":"ana",
    "HCPR-J50U-151020to151076-PAT":"ana",
    "HCPR-J50U-151077to151211-PAT":"ana",
    "HCPR-J50U-151217-PAT":"ana"
    }

for sample,PD in samples.items():
  inputList="~/inputs_data/"+sample+".txt"
  outputDir=scratch+"/"+PD+"/"+sample+"_"+jobTag
  cmd="./submitJob.sh "+inputList+" "+outputDir
  print cmd
  os.system(cmd)
