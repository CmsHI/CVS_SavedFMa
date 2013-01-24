# Jan 17 fix trkwt
for i in {1..4}; do
  root -l addJetWeight.C+'("../ntout/jskim_pp-full_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin'$i'_ak3PF_gj0.root",'$i',"fig/Jan17/jet_spectrum_cmp1_sm2_rewt0_evsel0_pbpbJan17_ppJan17.root")'
done
# root -l -b -q addJetWeight.C+'("../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",1,"fig/Jan17/jet_spectrum_cmp4_sm1_rewt0_evsel1_pbpbJan17_ppJan17.root",1)'

# mc
# for i in {1..4}; do
#   root -l addJetWeight.C+'("../ntout/jskim_dj80_ak3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin'$i'_ak3PF_gj0.root",'$i',"fig/Jan17/mc_jet_spectrum_cmp1_sm2_rewt0_evsel0_pbpbJan17_ppJan17.root")'
# done
# root -l -b -q addJetWeight.C+'("../ntout/jskim_hltjet80-pt90-v20_akPu3PF_Jan17_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm1bin0_akPu3PF_gj0.root",1,"fig/Jan17/jet_spectrum_cmp4_sm1_rewt0_evsel1_pbpbJan17_ppJan17.root",1)'
