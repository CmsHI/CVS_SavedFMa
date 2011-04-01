import os
os.system("./create_custom_python_cfg.sh")

scratch="/net/hidsk0001/d00/scratch/frankma"
jobTag="JetTrkv2"

# ================== 362 ===============
samples  = {
    # 384
    #"HydjetQ_DJQ80_F10GSR":"ana",
    #"HydjetQ_DJUQ80_F10GSR":"ana"
    #"HydjetQ_DJUQ50_Pat_hiGoodMergedTracks_v1":"ana",
    #"Pyquen_DJUQ50_Pat_hiGoodMergedTracks_v1":"ana"
    #"HydjetQ_DJUQ80_PathiGoodMergedTracks1203":"ana"
    #"HydjetBass_DJQ110_GSR_384":"ana",
    #"HydjetBass_DJQ170_GSR_384":"ana",
    # Final MC
    #"Pyquen_DJUQ80_Pat_hiGoodMergedTracks_VtxPatch_v1":"ana",
    #"Pyquen_DJUQ50_Pat_hiGoodMergedTracks_VtxPatch_v1":"ana"
    #"HydjetQ_DJUQ80_Pat_hiGoodMergedTracks_VtxPatch_v1":"ana",
    #"HydjetQ_DJUQ50_Pat_hiGoodMergedTracks_VtxPatch_v1":"ana"
    # Embedded in Data
    #"Data_MinBias_DijetUnquenched80_d20101125and1126":"ana",
    #"Data_MinBias_DijetUnquenched50_d20101127":"ana"
    #"Data_MinBias_DijetUnquenched50_d20101211":"ana"
    # Tracking 50k per pt_hat in 393
    "HydjetBass_DJUQ30_GSR_v2":"ana",
    "HydjetBass_DJUQ50_GSR_v2":"ana",
    "HydjetBass_DJUQ80_GSR_v2":"ana"
    #"HydjetBass_DJUQ110_GSR_v2":"ana",
    #"HydjetBass_DJUQ170_GSR_v2":"ana",
    # Tracking high_pt high stat
    #"HydjetBass_DJUQ80_GSR_393_300k":"ana",
    #"HydjetBass_DJUQ110_GSR_393_300k":"ana",
    #"HydjetBass_DJUQ170_GSR_393_300k":"ana"
    }

for sample,PD in samples.items():
  inputList="~/inputs_mc/"+sample+".txt"
  outputDir=scratch+"/"+PD+"/"+sample+"_"+jobTag
  cmd="./submitJob.sh custom_HiJetTrk_mc_2760_PFTowers_noMuons_cfg.py "+inputList+" "+outputDir
  print cmd
  os.system(cmd)
