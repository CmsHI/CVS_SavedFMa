// stole from ed: /home/edwenger/prod/CMSSW_3_3_5//src/MitQcd/Production/python/test.C

void test(){

  TChain *ev = new TChain("Events");
  Events->Add("bambu_jan29_124023.root");

  // event selection (bit 34 + cluter-vertex cleaning + HF coincidence already applied)
  Events->SetAlias("goodVtx","Pixel3Vertexes.GetEntries()>0");
  Events->SetAlias("hpFracCut","(EvtSelData.fArray.fHPTrkFrac>0.2 || Tracks.GetEntries()<10)");
  Events->SetAlias("evtSel","goodVtx && hpFracCut");

  // HLT trigger bits (careful with bit numbers. double-check against MitAna/macros/examples/runHLTExample.C)
  Events->SetAlias("hlt","HLTBits.fMask");
  Events->SetAlias("hltJet6U","hlt.TestBit(6)");
  Events->SetAlias("hltJet15U","hlt.TestBit(7)");
  Events->SetAlias("hltJet30U","hlt.TestBit(8)");
  Events->SetAlias("hltJet50U","hlt.TestBit(9)");

  // L1 algo bits (menu v11 used in run 124023)
  Events->SetAlias("l1a","L1AlgoBitsBeforeMask.fMask");
  Events->SetAlias("l1Jet6U","l1a.TestBit(15)");
  Events->SetAlias("l1Jet10U","l1a.TestBit(16)");
  Events->SetAlias("l1Jet20U","l1a.TestBit(17)");
  Events->SetAlias("l1Jet30U","l1a.TestBit(18)");
  Events->SetAlias("l1Jet40U","l1a.TestBit(19)");
  Events->SetAlias("l1Jet50U","l1a.TestBit(20)");
  Events->SetAlias("l1Jet60U","l1a.TestBit(21)");

  //Events->Draw("l1Jet10U:hltJet15U>>h2(2,-0.5,1.5,2,-0.5,1.5)","evtSel","colz text"); return;

  // tracks
  Events->SetAlias("trk","Tracks.fArray");

  // D0Corrected with beamspot for run 124023
  Events->SetAlias("lXM","-1*sin(trk.Phi())*trk.D0()");
  Events->SetAlias("lYM","cos(trk.Phi())*trk.D0()");
  Events->SetAlias("lDY","(lYM + 0.0291624)");
  Events->SetAlias("lDX","(lXM + 0.1060256)");
  Events->SetAlias("d0Corr","(trk.Px()*lDY - trk.Py()*lDX)/trk.Pt()");

  // sigXY = quadratic sum of d0Err and beamspot width
  Events->SetAlias("sigXY","sqrt(trk.D0Err()*trk.D0Err() + 0.04*0.04)");
  
  // high purity tracks
  Events->SetAlias("highPurity","trk.fQuality.fQualityMask.fBitMask.TestBit(2)");

  // approximation of ptErr ignoring covariance term
  Events->SetAlias("ptErr","sqrt(pow(trk.P()*trk.Pt()*trk.fQOverPErr/trk.Charge(),2) + pow(trk.Pz()*trk.fLambdaErr,2))");

  // associated supercluster
  Events->SetAlias("sc","trk.SCluster()");


  Events->Draw("sc.Et():trk.Pt()>>h2","highPurity && abs(d0Corr/sigXY)<3 && ptErr/trk.Pt()<0.1 && trk.NHits()>7  && evtSel"); return;

  // draw hists
  Events->Draw("trk.Pt()>>hPt(40,0,20)","highPurity && abs(d0Corr/sigXY)<3 && ptErr/trk.Pt()<0.1 && evtSel","goff");
  //Events->Draw("trk.Eta()>>hEta(25,-2.5,2.5)","highPurity && abs(d0Corr/sigXY)<3 && ptErr/trk.Pt()<0.1 && evtSel","goff");
  //Events->Draw("trk.Phi()>>hPhi(30,-3.1416,3.1416)","highPurity && abs(d0Corr/sigXY)<3 && ptErr/trk.Pt()<0.1 && evtSel","goff");
  Events->Draw("1>>hEvt","evtSel","goff");

  Events->Draw("trk.Pt()>>hPt6U(40,0,20)","highPurity && abs(d0Corr/sigXY)<3 && ptErr/trk.Pt()<0.1 && evtSel && hltJet6U","goff");
  //Events->Draw("trk.Eta()>>hEta6U(25,-2.5,2.5)","highPurity && abs(d0Corr/sigXY)<3 && ptErr/trk.Pt()<0.1 && evtSel && hltJet6U","goff");
  //Events->Draw("trk.Phi()>>hPhi6U(30,-3.1416,3.1416)","highPurity && abs(d0Corr/sigXY)<3 && ptErr/trk.Pt()<0.1 && evtSel && hltJet6U","goff");
  Events->Draw("1>>hEvt6U","evtSel && hltJet6U","goof");

  //Events->Draw("trk.Pt()>>hInvYield(80,0,8)","(highPurity && abs(d0Corr/sigXY)<3 && ptErr/trk.Pt()<0.1 && evtSel)*(0.1591546/trk.Pt())","goff");

  Float_t nEvt = (Float_t) hEvt->GetEntries();
  Float_t nEvt6U = (Float_t) hEvt6U->GetEntries();

  Float_t ptWidth = hPt->GetBinWidth(2);
  //Float_t etaWidth = hEta->GetBinWidth(2);
  //Float_t phiWidth = hPhi->GetBinWidth(2);
  //Float_t invYieldWidth = hInvYield->GetBinWidth(2);

  // draw canvas
  TCanvas *c1 = new TCanvas("c1","c1",700,600);
  hPt->Sumw2();
  hPt->Scale(1./nEvt); hPt->Scale(0.2/ptWidth); // 5 units of rapidity
  hPt->SetMaximum(100); hPt->SetMinimum(0.000001);
  hPt->SetMarkerStyle(20);
  hPt->SetTitle("Uncorrected Spectrum (|#eta|<2.5); p_{T} [GeV/c]; d^{2}N/d#etadp_{T}");
  hPt->Draw("pz");
  hPt6U->Sumw2();
  //hPt6U->Scale(1./nEvt6U); hPt6U->Scale(1./ptWidth);
  hPt6U->Scale(1./nEvt); hPt6U->Scale(1./ptWidth);  // scale to match minbias
  hPt6U->SetMarkerStyle(20);
  hPt6U->SetMarkerColor(2);
  hPt6U->Draw("pzsame");
  gPad->SetLogy();

  /*
  TCanvas *c2 = new TCanvas("c2","c2",700,600);
  hEta->Sumw2();
  hEta->Scale(1./nEvt); hEta->Scale(1./etaWidth);
  hEta->SetMaximum(10); hEta->SetMinimum(0);
  hEta->SetMarkerStyle(20);
  hEta->SetTitle("Uncorrected Multiplicity; #eta; dN/d#eta");
  hEta->Draw("pz");
  hEta6U->Sumw2();
  hEta6U->Scale(1./nEvt6U); hEta6U->Scale(1./etaWidth);
  hEta6U->SetMarkerStyle(20);
  hEta6U->SetMarkerColor(2);
  hEta6U->Draw("pzsame");

  TCanvas *c3 = new TCanvas("c3","c3",700,600);
  hPhi->Sumw2();
  hPhi->Scale(1./nEvt); hPhi->Scale(1./phiWidth);
  hPhi->SetMaximum(5); hPhi->SetMinimum(0);
  hPhi->SetMarkerStyle(20);
  hPhi->SetTitle("Uncorrected Multiplicity; #phi; dN/d#phi");
  hPhi->Draw("pz");
  hPhi6U->Sumw2();
  hPhi6U->Scale(1./nEvt6U); hPhi6U->Scale(1./phiWidth);
  hPhi6U->SetMarkerStyle(20);
  hPhi6U->SetMarkerColor(2);
  hPhi6U->Draw("pzsame");

  TCanvas *c4 = new TCanvas("c4","c4",700,600);
  hInvYield->Sumw2();
  hInvYield->Scale(1./nEvt); hInvYield->Scale(0.2/invYieldWidth); // 5 units of rapidity
  hInvYield->SetMarkerStyle(20);
  hInvYield->SetTitle("Uncorrected Invariant Yield (|#eta|<2.5); p_{T} [GeV/c]; #frac{1}{2#pip_{T}} d^{2}N/d#etadp_{T}");
  hInvYield->Draw("pz");
  gPad->SetLogy();

  */
}
