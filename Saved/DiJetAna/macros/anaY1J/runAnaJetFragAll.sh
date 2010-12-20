#!/bin/bash -
anaV=CorrEtaTrkEffPtBin5RBin20v4
module=djcalo_genp
./runAnaJetFragHI.sh dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0_${module}_120_50.root HCPR_J50U $anaV $module
./runAnaJetFragHI.sh dj_HydQ_DJUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv2GenAll_${module}_120_50.root Hydjet $anaV $module
./runAnaJetFragPp.sh dj_PyquenUQ80_hiGoodMergedTracks_VtxPatch_v1_OfficialSelv2GenAll_${module}_120_50.root PyquenUQ80 $anaV $module
