{
   TFile *a = new TFile("../sample-1224/TrackletTree-2360GeV-D6T.root");
   int nbin=50;
   TH2F *hSD1 = new TH2F("hSD1","",nbin,0,nbin,nbin,0,nbin);
   TH2F *hSD2 = new TH2F("hSD2","",nbin,0,nbin,nbin,0,nbin);
   TH2F *hSD = new TH2F("hSD","",nbin,0,nbin,nbin,0,nbin);
   TH2F *hDD = new TH2F("hDD","",nbin,0,nbin,nbin,0,nbin);
   TH2F *hND = new TH2F("hND","",nbin,0,nbin,nbin,0,nbin);
   
   hSD1->SetXTitle("nHFn SD1");
   hSD2->SetXTitle("nHFn SD2");
   hSD->SetXTitle("nHFn SD");
   hDD ->SetXTitle("nHFn DD");
   hND->SetXTitle("nHFn ND");
   hSD1->SetYTitle("nHFp");
   hSD2->SetYTitle("nHFp");
   hSD->SetXTitle("nHFp");
   hDD ->SetYTitle("nHFp");
   hND->SetYTitle("nHFp");

   TCanvas *c1 = new TCanvas("c1","",400,400);
   TrackletTree12->Draw("nHFp:nHFn>>hSD1","evtType==92","col");
   TCanvas *c2 = new TCanvas("c2","",400,400);
   TrackletTree12->Draw("nHFp:nHFn>>hSD2","evtType==93","col");
   TCanvas *c3 = new TCanvas("c3","",400,400);
   TrackletTree12->Draw("nHFp:nHFn>>hDD","evtType==94","col");
   TCanvas *c4 = new TCanvas("c4","",400,400);
   TrackletTree12->Draw("nHFp:nHFn>>hND","evtType!=94&&evtType!=93&&evtType!=92","col");
   TCanvas *c5 = new TCanvas("c5","",400,400);
   TrackletTree12->Draw("nHFp:nHFn>>hSD","evtType==92||evtType==93","col");

}
