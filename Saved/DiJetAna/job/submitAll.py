import os
os.system("./create_custom_python_cfg.sh")

scratch="/net/hidsk0001/d00/scratch/frankma"
#jobTag="OfficialSelv1"
jobTag="OfficialSelNoVtxPatchv1"

# ================== 362 ===============
samples  = {
    #"HCPR-GoodRuns-151027and151058":"ana"
    #"HCPR-J50U-150883to150887-PAT":"ana",
    #"HCPR-J50U-151020to151076-PAT":"ana",
    #"HCPR-J50U-151077to151211-PAT":"ana",
    #"HCPR-J50U-151217-PAT":"ana",
    #"HCPR-J50U-151238to151240-PAT":"ana",
    #"HCPR-J50U-151350to151353-PAT":"ana",
    #"HCPR-J50U-151878to151975-PAT":"ana",
    #"HCPR-J50U-151935to151969-PAT":"ana"
    #"HCPR-J50U-OfficialJSON_hiGoodMergedTracks_Runs_152561_to_152643_RECOPAT-v1":"ana"
    #"HCPR-J50U-Pre-OfficialJSON_hiGoodMergedTracks_Runs_152652_to_152791_RECOPAT-v0":"ana"
    #"HCPR-J50U-OfficialJSON_hiGoodMergedTracks_Runs_150883_to_152485_RECOPAT-v0_VtxPatch":"ana",
    #"HCPR-J50U-Pre-OfficialJSON_hiGoodMergedTracks_Runs_152652_to_152791_RECOPAT-v0_VtxPatch":"ana"
    # v1 is crab
    #"HCPR-J50U-OfficialJSON_hiGoodMergedTracks_Runs_152561_to_152643_RECOPAT-v1_VtxPatch":"ana"
    # v1 w/o VtxPatch
    "HCPR-J50U-OfficialJSON_hiGoodMergedTracks_Runs_150883_to_152485_RECOPAT-v1":"ana",
    "HCPR-J50U-OfficialJSON_hiGoodMergedTracks_Runs_152561_to_152643_RECOPAT-v1":"ana",
    "HCPR-J50U-Pre-OfficialJSON_hiGoodMergedTracks_Runs_152652_to_152791_RECOPAT-v1":"ana"
    }

for sample,PD in samples.items():
  inputList="~/inputs_data/"+sample+".txt"
  outputDir=scratch+"/"+PD+"/"+sample+"_"+jobTag
  cmd="./submitJob.sh "+inputList+" "+outputDir
  print cmd
  os.system(cmd)
