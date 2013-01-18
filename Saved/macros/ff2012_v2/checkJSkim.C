{
  TH1D::SetDefaultSumw2();

  // check jet
  t0=(TTree*)_file0->Get("tdj");
  t1=(TTree*)_file1->Get("tdj");
  TCanvas * c2 = new TCanvas("c2","c2",800,400);
  c2->Divide(2,1);
  c2->cd(1);
  gPad->SetLogy();
  t0.Draw("jetPt>>h0","","hist");
  h0->SetTitle(";jet pT");
  t1.Draw("jetPt>>h1","","sameE");
  c2->cd(2);
  hr=(TH1D*)h1->Clone("hr");
  hr->Divide(h0);
  hr->Draw();

  // check trk
  // TCut trkSel="cBin<4 && jetPt>100 && yTrk.jetMatch==1 && yTrk.jetDr<0.3"; // pbpb
  TCut trkSel="jetPt>100 && yTrk.jetMatch==1 && yTrk.jetDr<0.3"; // pp
  TString trkWt="yTrk.trkWeight";
  trkSel*=trkWt;
  t0->AddFriend("yTrk=yongsunTrack",_file0);
  t1->AddFriend("yTrk=yongsunTrack",_file1);

  TCanvas * c3 = new TCanvas("c3","c3",800,400);
  c3->Divide(2,1);
  c3->cd(1);
  gPad->SetLogy();
  t0.Draw("yTrk.pt>>hTrk0",trkSel,"hist");
  hTrk0->SetTitle(";trk pT");
  t1.Draw("yTrk.pt>>hTrk1",trkSel,"sameE");
  c3->cd(2);
  hTrkr=(TH1D*)hTrk1->Clone("hTrkr");
  hTrkr->Divide(hTrk0);
  hTrkr->Draw();

  // check xi
  TCanvas * c4 = new TCanvas("c4","c4",800,400);
  c4->Divide(2,1);
  c4->cd(1);
  gPad->SetLogy();
  t0.Draw("log(jetPt/yTrk.pt)>>hXi0(14,0,7)",trkSel,"hist");
  hXi0->SetTitle(";#xi");
  t1.Draw("log(jetPt/yTrk.pt)>>hXi1(14,0,7)",trkSel,"sameE");
  c4->cd(2);
  hXir=(TH1D*)hXi1->Clone("hXir");
  hXir->Divide(hXi0);
  hXir->Draw();

  // check trkcorr
  TCanvas * c5 = new TCanvas("c5","c5",800,400);
  c5->Divide(2,1);
  c5->cd(1);
  gPad->SetLogy();
  t0.Draw("yTrk.trkWeight>>hTWt0",trkSel,"hist");
  t1.Draw("yTrk.trkWeight>>hTWt1",trkSel,"sameE");
  c5->cd(2);
  hTWtr=(TH1D*)hTWt1->Clone("hTWtr");
  hTWtr->Divide(hTWt0);
  hTWtr->Draw();
}