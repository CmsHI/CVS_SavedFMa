


{

  TH1::SetDefaultSumw2();

  TFile* inf1 = new TFile("output.root");

  TChain* input = new TChain("jets/t");
  input->Add("tree*.root");


  /*
  TH1D* ha1 = new TH1D("ha1",";A_{J}",30,-0.5,1);
  TH1D* ha2 = new TH1D("ha2",";A_{J}",30,-0.5,1);
  TH1D* ha3 = new TH1D("ha3",";A_{J}",30,-0.5,1);
  TH1D* ha4 = new TH1D("ha4",";A_{J}",30,-0.5,1);
  TH1D* hc1 = new TH1D("hc1",";A_{J}",30,-0.5,1);
  TH1D* hc2 = new TH1D("hc2",";A_{J}",30,-0.5,1);
  TH1D* hc3 = new TH1D("hc3",";A_{J}",30,-0.5,1);
  TH1D* hc4 = new TH1D("hc4",";A_{J}",30,-0.5,1);
  */


  TH1D* ha0 = new TH1D("ha0",";A_{J}",20,-0.,1);
  TH1D* hc0 = new TH1D("hc0",";A_{J}",20,-0.,1);

  TH1D* ha1 = new TH1D("ha1",";A_{J}",20,-0.,1);
  TH1D* ha2 = new TH1D("ha2",";A_{J}",20,-0.,1);
  TH1D* ha3 = new TH1D("ha3",";A_{J}",20,-0.,1);
  TH1D* ha4 = new TH1D("ha4",";A_{J}",20,-0.,1);
  TH1D* hc1 = new TH1D("hc1",";A_{J}",20,-0.,1);
  TH1D* hc2 = new TH1D("hc2",";A_{J}",20,-0.,1);
  TH1D* hc3 = new TH1D("hc3",";A_{J}",20,-0.,1);
  TH1D* hc4 = new TH1D("hc4",";A_{J}",20,-0.,1);


  input->Draw("(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])>>hc0","jtpt[0] > 120 && jtpt[1] > 50 && acos(cos(jtphi[0]-jtphi[1]))> 2*3.14/3 && abs(jteta[0]) < 2. && abs(jteta[1]) < 2. ");
  input->Draw("(jtpt[0]-jtpt[1])/(jtpt[0]+jtpt[1])>>ha0","jtpt[0] > 100 && jtpt[1] > 25 && acos(cos(jtphi[0]-jtphi[1]))> 3.14/2 && abs(jteta[0]) < 2.8 && abs(jteta[1]) < 2.8");

  TNtuple* nt = (TNtuple*)inf1->Get("nt");

  nt->Draw("(pt1-pt2)/(pt1+pt2)>>hc1","pt1 > 120 && pt2 > 50 && acos(cos(phi1-phi2))> 2*3.14/3 && abs(eta1) < 2. && abs(eta2) < 2. ");
  nt->Draw("(ptt1-ptt2)/(ptt1+ptt2)>>hc2","pt1 > 120 && pt2 > 50 && acos(cos(phi1-phi2))> 2*3.14/3 && abs(eta1) < 2. && abs(eta2) < 2. ");
  nt->Draw("(pt1-pt2)/(pt1+pt2)>>hc3","ptt1 > 120 && ptt2 > 50 && acos(cos(phi1-phi2))> 2*3.14/3 && abs(eta1) < 2. && abs(eta2) < 2. ");
  nt->Draw("(ptt1-ptt2)/(ptt1+ptt2)>>hc4","ptt1 > 120 && ptt2 > 50 && acos(cos(phi1-phi2))> 2*3.14/3 && abs(eta1) < 2. && abs(eta2) < 2. ");

  nt->Draw("(pt1-pt2)/(pt1+pt2)>>ha1","pt1 > 100 && pt2 > 25 && acos(cos(phi1-phi2))> 3.14/2 && abs(eta1) < 2.8 && abs(eta2) < 2.8 ");
  nt->Draw("(ptt1-ptt2)/(ptt1+ptt2)>>ha2","pt1 > 100 && pt2 > 25 && acos(cos(phi1-phi2))> 3.14/2 && abs(eta1) < 2.8 && abs(eta2) < 2.8 ");
  nt->Draw("(pt1-pt2)/(pt1+pt2)>>ha3","ptt1 > 100 && ptt2 > 25 && acos(cos(phi1-phi2))> 3.14/2 && abs(eta1) < 2.8 && abs(eta2) < 2.8 ");
  nt->Draw("(ptt1-ptt2)/(ptt1+ptt2)>>ha4","ptt1 > 100 && ptt2 > 25 && acos(cos(phi1-phi2))> 3.14/2 && abs(eta1) < 2.8 && abs(eta2) < 2.8 ");

  ha0->Scale(1./ha0->Integral("width"));
  hc0->Scale(1./hc0->Integral("width"));

  ha1->Scale(1./ha1->Integral("width"));
  ha2->Scale(1./ha2->Integral("width"));
  ha3->Scale(1./ha3->Integral("width"));
  ha4->Scale(1./ha4->Integral("width"));

  hc1->Scale(1./hc1->Integral("width"));
  hc2->Scale(1./hc2->Integral("width"));
  hc3->Scale(1./hc3->Integral("width"));
  hc4->Scale(1./hc4->Integral("width"));

  ha2->SetMarkerColor(2);
  ha2->SetLineColor(2);
  ha3->SetMarkerColor(3);
  ha3->SetLineColor(3);
  ha4->SetMarkerColor(4);
  ha4->SetLineColor(4);

  hc2->SetMarkerColor(2);
  hc2->SetLineColor(2);
  hc3->SetMarkerColor(3);
  hc3->SetLineColor(3);
  hc4->SetMarkerColor(4);
  hc4->SetLineColor(4);

  TLegend *leg = new TLegend(0.5313131,0.6515054,0.95,0.8739785,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetTextFont(62);
  leg->SetTextSize(0.022);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->AddEntry(ha1,"S = Smeared, U = Unsmeared","");
  leg->AddEntry(ha1,"Selection : S, Values : S, Cuts : S","pl");
  leg->AddEntry(ha2,"Selection : S, Values : U, Cuts : S","pl");
  leg->AddEntry(ha3,"Selection : S, Values : S, Cuts : U","pl");
  leg->AddEntry(ha4,"Selection : S, Values : U, Cuts : U","pl");

  TCanvas* cc1 = new TCanvas("cc1","",500,500);
  ha1->SetMaximum(5);
  ha1->Draw();
  ha2->Draw("same");
  ha3->Draw("same");
  ha4->Draw("same");
  leg->Draw();

  TLegend *legA = new TLegend(0.4713131,0.87115054,0.91,0.9439785,NULL,"brNDC");
  legA->SetBorderSize(0);
  legA->SetTextFont(62);
  legA->SetLineColor(1);
  legA->SetLineStyle(1);
  legA->SetLineWidth(1);
  legA->SetFillColor(0);
  legA->SetFillStyle(0);
  legA->AddEntry(ha4,"ATLAS selection","");
  legA->Draw();

  cc1->Print("Atlas_smeared_pos.gif");

  TCanvas* cc2 = new TCanvas("cc2","",500,500);
  hc1->SetMaximum(5);
  hc1->Draw();
  hc2->Draw("same");
  hc3->Draw("same");
  hc4->Draw("same");
  leg->Draw();
  TLegend *legC = new TLegend(0.4713131,0.87115054,0.91,0.9439785,NULL,"brNDC");
  legC->SetBorderSize(0);
  legC->SetTextFont(62);
  legC->SetLineColor(1);
  legC->SetLineStyle(1);
  legC->SetLineWidth(1);
  legC->SetFillColor(0);
  legC->SetFillStyle(0);
  legC->AddEntry(ha4,"CMS selection","");
  legC->Draw();


  cc2->Print("CMS_smeared_pos.gif");





}


