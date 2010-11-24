import os
os.system("./create_custom_python_cfg.sh")

scratch="/net/hidsk0001/d00/scratch/frankma"
jobTag="JEta5_1124"

# ================== 362 ===============
samples  = {
    "HCPR-J50U-150883to150887-PAT":"ana",
    "HCPR-J50U-151020to151076-PAT":"ana",
    "HCPR-J50U-151077to151211-PAT":"ana",
    "HCPR-J50U-151217-PAT":"ana",
    #"HCPR-GoodRuns-151027and151058":"ana"
    "HCPR-J50U-151238to151240-PAT":"ana",
    "HCPR-J50U-151350to151353-PAT":"ana",
    "HCPR-J50U-151878to151975-PAT":"ana",
    "HCPR-J50U-151935to151969-PAT":"ana"
    }

for sample,PD in samples.items():
  inputList="~/inputs_data/"+sample+".txt"
  outputDir=scratch+"/"+PD+"/"+sample+"_"+jobTag
  cmd="./submitJob.sh "+inputList+" "+outputDir
  print cmd
  os.system(cmd)
