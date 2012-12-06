for i in {1..4}; do
  root -l -b -q addJetWeight.C+'("jskim_pp-full_ak3PF_Dec5newsmgt60steps_jetPt_60_jetEtaCut_2.00_noPbin_sm2bin'$i'_ak3PF_gj0.root",'$i',"fig/Dec5FinalPbPbCorr/hisCmp1_Smear2_Rewt0.root")'
done
