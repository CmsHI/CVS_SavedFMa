#!/bin/bash -
anaV=CorrEtaTrkEffPtBin5RBin20v7
module=djcalo
./runAnaJetFragHI.sh dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0_${module}_120_50.root HCPR_J50U $anaV $module 0
./runAnaJetFragHI.sh dj_HydQ_DJUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv3_${module}_120_50.root Hydjet $anaV $module 0
./runAnaJetFragHI.sh dj_HydQ_DJUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv3_${module}_120_50.root Hydjet ${anaV}ReWt $module 1
./runAnaJetFragPp.sh dj_PyquenUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv3_${module}_120_50.root PyquenUQ80 $anaV $module 0
