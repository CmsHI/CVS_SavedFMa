import os
os.system("./create_custom_python_cfg.sh")

scratch="/net/hidsk0001/d00/scratch/frankma"
jobTag="1116_trigana_LIGHT"

# ================== 362 ===============
samples  = {
    #"HCPR-J50U-151020to151076-PAT":"ana"
    "HCPR-MB-151020to151076-PAT":"ana"
    }

for sample,PD in samples.items():
  inputList="~/inputs_data/"+sample+".txt"
  outputDir=scratch+"/"+PD+"/"+sample+"_"+jobTag
  cmd="./submitJob.sh "+inputList+" "+outputDir
  print cmd
  os.system(cmd)
