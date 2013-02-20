# pp
for i in {0..4}; do
  smMode=2
  smBin=$i
  if [ $i -eq 0 ]; then 
    smMode=0
    smBin=4
  fi
  echo pp: smMode $smMode bin $i
  root -l -b -q addMultiWeight.C+'("../ntout/jskim_pp-full_ak3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm'$smMode'bin'$i'_ak3PF_gj0.root",true,'$smBin',0)'
  root -l -b -q addMultiWeight.C+'("../ntout/jskim_dj80_ak3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm'$smMode'bin'$i'_ak3PF_gj0.root",true,'$smBin')'
  root -l -b -q addMultiWeight.C+'("../ntout/jskim_dj120_ak3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm'$smMode'bin'$i'_ak3PF_gj0.root",true,'$smBin')'
  root -l -b -q addMultiWeight.C+'("../ntout/jskim_dj170_ak3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm'$smMode'bin'$i'_ak3PF_gj0.root",true,'$smBin')'
done

# HI
echo HI
root -l -b -q addMultiWeight.C+'("../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",false,-1,0)'
root -l -b -q addMultiWeight.C+'("../ntout/jskim_hydj80_akPu3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",false)'
root -l -b -q addMultiWeight.C+'("../ntout/jskim_hydj100_akPu3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",false)'
root -l -b -q addMultiWeight.C+'("../ntout/jskim_hydj170_akPu3PF_Feb14v2_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",false)'
