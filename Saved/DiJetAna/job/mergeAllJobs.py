import os

scratch="/net/hidsk0001/d00/scratch/frankma"
jobTag="HighPtv1"

# ================== 39X ===============
hisamples  = {
    # 384
    "HydjetBass_DJQ110_GSR_384":"ana",
    "HydjetBass_DJQ170_GSR_384":"ana",
    #"HydjetBass_DJUQ30_GSR_v2":"ana",
    "HydjetBass_DJUQ50_GSR_v2":"ana",
    "HydjetBass_DJUQ80_GSR_v2":"ana",
    "HydjetBass_DJUQ110_GSR_v2":"ana",
    "HydjetBass_DJUQ170_GSR_v2":"ana",
    # Tracking high_pt high stat
    "HydjetBass_DJUQ110_GSR_393_300k":"ana",
    "HydjetBass_DJUQ170_GSR_393_300k":"ana"
    }

inFolder="mergeAll"
outFolder="mergeAll/mergeAll"
for sample,PD in hisamples.items():
  indir=scratch+"/"+PD+"/"+sample+"_"+jobTag
  cmd="./mergeJob.sh " + indir+"/"+inFolder + " dj " + indir+"/"+outFolder
  print cmd
  os.system(cmd)
