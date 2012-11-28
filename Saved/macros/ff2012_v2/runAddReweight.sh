for i in {0..4}; do
  root -l -b -q addJetWeight.C+'("jskim_pp-full_ak3PF_Nov14_jetPt_50_jetEtaCut_2.00_noPbin_sm'$i'_ak3PF_gj0.root",'$i',"fig/Nov20/hisSmear1_Rewt0.root")'
done