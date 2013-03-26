# pp
for smMode in {0,2}; do
  for smBin in {1..4}; do
    inBin=$smBin;
    if [ $smMode -eq 0 ]; then inBin=0; fi
    echo pp: smMode $smMode bin $i
    root -l -b -q addMultiWeight.C+'("../ntout/jskim_pp-full_ak3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm'$smMode'bin'$inBin'_ak3PF_gj0.root",true,'$smBin',0)'
    # root -l -b -q addMultiWeight.C+'("../ntout/jskim_dj80_ak3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm'$smMode'bin'$inBin'_ak3PF_gj0.root",true,'$smBin')'
    # root -l -b -q addMultiWeight.C+'("../ntout/jskim_dj120_ak3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm'$smMode'bin'$inBin'_ak3PF_gj0.root",true,'$smBin')'
    # root -l -b -q addMultiWeight.C+'("../ntout/jskim_dj170_ak3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm'$smMode'bin'$inBin'_ak3PF_gj0.root",true,'$smBin')'
  done
done

# HI
echo HI
root -l -b -q addMultiWeight.C+'("../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",false,-1,0)'
# root -l -b -q addMultiWeight.C+'("../ntout/jskim_hydj80_akPu3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",false)'
# root -l -b -q addMultiWeight.C+'("../ntout/jskim_hydj100_akPu3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",false)'
# root -l -b -q addMultiWeight.C+'("../ntout/jskim_hydj170_akPu3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",false)'

# quenched
root -l -b -q addMultiWeight.C+'("../ntout/jskim_hydj80quenwide_akPu3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",false)'
root -l -b -q addMultiWeight.C+'("../ntout/jskim_hydj100quenwide_akPu3PF_Mar19job0_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",false)'
