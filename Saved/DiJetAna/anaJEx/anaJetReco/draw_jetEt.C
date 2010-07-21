void draw_jetEt(){

  TColor *pal = new TColor();
  Int_t kred     = pal->GetColor(190,  0,  3);
  Int_t kgreen   = pal->GetColor( 15, 85, 15);
  Int_t kazure   = pal->GetColor(  0, 48, 97);
  Int_t kcyan    = pal->GetColor(  0, 83, 98);

  TFile *fMBNZP = new TFile("../test/dijetAna_anaJet_MinBias.root");
  TFile *fHENZP = new TFile("../test/dijetAna_anaJet_HardEnriched.root");
  TFile *fHEZP = new TFile("../franktrees/dijetAna_JEx_ZP_Hard_proc0_all.root");



  fMBNZP->cd("dijetAna_data");

  TH1F *h1 = new TH1F("hMBNZP","hMBNZP",170,0,170);
  
  TTree *tMBNZP = (TTree*)fMBNZP->Get("dijetAna_data/djTree");
  cout<<tMBNZP->GetEntries()<<endl;
  tMBNZP->Draw("nljet>>hMBNZP","abs(nljeta)<2.");


  fHENZP->cd("dijetAna_data");
  
  TH1F *h2 = new TH1F("hHENZP","hHENZP",170,0,170);
  
  TTree *tHENZP = (TTree*)fHENZP->Get("dijetAna_data/djTree");
  cout<<tHENZP->GetEntries()<<endl;
  tHENZP->Draw("nljet>>hHENZP","abs(nljeta)<2.");

  TH1F *h3 = new TH1F("hHEZP","hHEZP",170,0,170);
  
  TTree *tHEZP = (TTree*)fHEZP->Get("dijetAna_data/djTree");
  cout<<tHEZP->GetEntries()<<endl;
  tHEZP->Draw("nljet>>hHEZP","abs(nljeta)<2.");


  h1->Scale(1./h1->Integral("width"));
  h2->Scale(1./h2->Integral("width"));
  h3->Scale(1./h3->Integral("width"));
  
  h1->SetTitle("Minimum Bias (NZP) vs. Hard Enriched (ZP, NZP)");
  h1->GetXaxis()->SetTitle("E_{T} [GeV]");
  h1->GetYaxis()->SetTitle("dN/dE_{T} (Arb. Norm.)");

  h1->Draw();

  h2->SetLineColor(kred);
  h2->Draw("sames");

  h3->SetLineColor(4);
  h3->Draw("sames");

  TLegend *t=new TLegend(0.5,0.5,0.9,0.8);
  t->SetBorderSize(0);
  t->SetFillStyle(0);
  t->AddEntry(h1,"Minimum Bias, NZP","l");
  t->AddEntry(h2,"Hard Enriched, NZP","l");
  t->AddEntry(h3,"Hard Enriched, ZP","l");
  t->Draw();
    
}
