for i in {1..4}; do
  # Jan 16 Report
  root -l -b -q addJetWeight.C+'("../ntout/jskim_pp-full_ak3PF_Jan16_4bin_sm18_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin'$i'_ak3PF_gj0.root",'$i',"fig/Jan16Report/jet_spectrum_cmp1_sm2_rewt0_pbpbJan16_ppJan16.root")'
done
