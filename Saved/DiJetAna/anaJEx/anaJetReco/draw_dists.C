void draw_dists(int pt_eta_phi=0){


  int draw_kt4=1;
  int draw_ak5=1;

  gStyle->SetOptStat(1);

  TFile *fReco = new TFile("../../process_aod1/fullJetAlgo/dijetAna_patJets_FJ0Tr1.root");

  
  TTree *trIC5 = (TTree*) fReco->Get("dijetAna_data/djTree");
  TTree *trKT4 = (TTree*) fReco->Get("dijetAna_data_kt4/djTree");
  TTree *trAK5 = (TTree*) fReco->Get("dijetAna_data_ak5/djTree");
  
  TH1F *hIC5, *kKT4, *AK5;

  if(pt_eta_phi==0){
    hIC5 = new TH1F("hIC5","hIC5",125,0,250);
    hKT4 = new TH1F("hKT4","hKT4",125,0,250);
    hAK5 = new TH1F("hAK5","hAK5",125,0,250);

    trIC5->Draw("nljet>>hIC5","abs(nljeta)<2");
    trKT4->Draw("nljet>>hKT4","abs(nljeta)<2","sames");
    trAK5->Draw("nljet>>hAK5","abs(nljeta)<2","sames");

    hIC5->SetTitle("E_{T} Distribution, |#eta|<2");
    hIC5->SetXTitle("E_{T} [GeV]");
  }
  if(pt_eta_phi==1){
    hIC5 = new TH1F("hIC5","hIC5",20,-2,2);
    hKT4 = new TH1F("hKT4","hKT4",20,-2,2);
    hAK5 = new TH1F("hAK5","hAK5",20,-2,2);

    trIC5->Draw("nljeta>>hIC5","abs(nljeta)<2 && nljet>80");
    trKT4->Draw("nljeta>>hKT4","abs(nljeta)<2 && nljet>80","sames");
    trAK5->Draw("nljeta>>hAK5","abs(nljeta)<2 && nljet>80","sames");

    hIC5->SetTitle("#eta Distribution, E_{T} > 80 GeV");
    hIC5->SetXTitle("#eta");

    hIC5->SetMinimum(1.);
    hIC5->SetMaximum(800.);
  }
  if(pt_eta_phi==2){
    hIC5 = new TH1F("hIC5","hIC5",35,-3.5,3.5);
    hKT4 = new TH1F("hKT4","hKT4",35,-3.5,3.5);
    hAK5 = new TH1F("hAK5","hAK5",35,-3.5,3.5);

    trIC5->Draw("nljphi>>hIC5","abs(nljeta)<2 && nljet>80");
    trKT4->Draw("nljphi>>hKT4","abs(nljeta)<2 && nljet>80","sames");
    trAK5->Draw("nljphi>>hAK5","abs(nljeta)<2 && nljet>80","sames");

    hIC5->SetTitle("#phi Distribution, E_{T} > 80 GeV, |#eta|<2");
    hIC5->SetXTitle("#phi [rad]");

    hIC5->SetMinimum(1.);
    hIC5->SetMaximum(800.);
  }


  TCanvas *c=new TCanvas("c","c",1);
  c->SetLogy();

  hKT4->SetLineColor(kBlue);
  hAK5->SetLineColor(kRed);
  
  hIC5->Draw();
  if(draw_kt4)hKT4->Draw("sames");
  if(draw_ak5)hAK5->Draw("sames");

  if(draw_kt4){
    gPad->Update();
    TPaveStats *st1 = (TPaveStats*)hKT4->FindObject("stats");                                                            
    st1->SetY1NDC(st1->GetY1NDC()-0.18);                                                                        
    st1->SetY2NDC(st1->GetY2NDC()-0.18);                                                                        
    st1->SetLineColor(kBlue);                                                                                      
    st1->SetTextColor(kBlue);                                                                                      
    st1->Draw();  
  }

  if(draw_ak5){
    gPad->Update();
    TPaveStats *st2 = (TPaveStats*)hAK5->FindObject("stats");                                                            
    st2->SetY1NDC(st2->GetY1NDC()-0.36);                                                                        
    st2->SetY2NDC(st2->GetY2NDC()-0.36);                                                                        
    st2->SetLineColor(kRed);                                                                                      
    st2->SetTextColor(kRed);                                                                                      
    st2->Draw();  
  }

  TLegend *t=new TLegend(0.6,0.7,0.8,0.85);
  t->SetBorderSize(0);
  t->SetFillStyle(0);
  t->AddEntry(hIC5,"ICPU5","l");
  if(draw_kt4)t->AddEntry(hKT4,"KT4","l");
  if(draw_ak5)t->AddEntry(hAK5,"AK5","l");
  if(draw_kt4||draw_ak5) t->Draw();

  c->Print(Form("pt_eta_phi_%d.gif",pt_eta_phi));
}
