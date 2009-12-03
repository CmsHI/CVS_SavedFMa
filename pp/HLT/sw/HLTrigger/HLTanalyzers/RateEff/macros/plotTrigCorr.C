void plotTrigCorr(const char * fname = "../hltmenu_1TeV_1.0e10_startup_pythiaD6T_ohlt_30k.root",
		  const char * tag = "Pythia_MinBias")
{
   TFile * f = new TFile(fname);
   f->ls();
   TH1F * hpaths;
   f->GetObject("individual",hpaths);
   TCanvas * cpaths = new TCanvas("cpaths","cpaths",400,400);
   cpaths->SetLogy();
   hpaths->Draw();
   hpaths->LabelsOption("v","X");
   cpaths->Print(Form("%srate_paths.gif",tag));


   TH2F * hcorr;
   f->GetObject("overlap",hcorr);
   TCanvas * ccorr = new TCanvas("ccorr","ccorr",1200,700);
   hcorr->Draw("text");
   hcorr->LabelsOption("v","X");
   ccorr->Print(Form("%s_trig_corr.gif",tag));
   
   TCanvas *ccorr_colz = new TCanvas("ccorr_colz","ccorr_colz",1200,700);
   hcorr->Draw("colz");
   ccorr_colz->Print(Form("%strig_corr_colz.gif",tag));

}
