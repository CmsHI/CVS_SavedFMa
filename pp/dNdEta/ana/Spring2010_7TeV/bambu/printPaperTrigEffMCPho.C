//
{

  TChain *ev = new TChain("Events");

  //ev->Add("/d100/mc/reco/private_7TeV/yilmaz-MinBiasPhojet_RECO_0413_v1/bambu_000_1?_1.root");
  ev->Add("/d100/mc/reco/private_7TeV/yilmaz-MinBiasPhojet_RECO_0413_v1/bambu_000_1?_1.root");

  // event-wide aliases
  ev->SetAlias("run","EventHeader.fRunNum");
  ev->SetAlias("evtno","EventHeader.fEvtNum");
  ev->SetAlias("bunch","EventHeader.fBunchCrossing");
  ev->SetAlias("npix","PixelHits.Entries()");
  ev->SetAlias("pxl","PixelHits.fArray");
  ev->SetAlias("npix1","Sum$(pxl.fType==1)");
  ev->SetAlias("npix2","Sum$(pxl.fType==2)");
  ev->SetAlias("npix3","Sum$(pxl.fType==3)");
  ev->SetAlias("bpix","Sum$(abs(pxl.fType)<10)");
  ev->SetAlias("fpix","Sum$(abs(pxl.fType)>10)");
  ev->SetAlias("nvtx","PrimaryVertexes.Entries()");
  ev->SetAlias("vtx","PrimaryVertexes.fArray");
  ev->SetAlias("avtx","Pixel3Vertexes.fArray");
  ev->SetAlias("tvtx","TrackletVertexes.fArray");
  ev->SetAlias("ntrk","Tracks.Entries()");
  ev->SetAlias("ngoodtrk","Sum$(Tracks.fArray.NHits()>9)");
  //ev->SetAlias("ngoodtrk","Sum$(Tracks.fArray.NHits()>3 && abs(Tracks.fArray.D0())<0.5)");

  ev->SetAlias("parasite","((bunch>=1830 && bunch<=1833) || bunch==3170 || bunch == 51)");
  ev->SetAlias("collision","bunch>100");

  // HLT bits
  ev->SetAlias("hlt","HLTBits.fMask");
  ev->SetAlias("hltBSC","hlt.TestBit(66)");     //HLT_MinBiasBSC
  ev->SetAlias("hltBSC_OR","hlt.TestBit(67)");  //HLT_MinBiasBSC_OR
  ev->SetAlias("hltEG1","hlt.TestBit(64)");
  ev->SetAlias("hltHF3","hlt.TestBit(106)");
  ev->SetAlias("hltTrk1","hlt.TestBit(73)");

  // l1 technical bits:  https://twiki.cern.ch/twiki/bin/viewauth/CMS/GlobalTriggerMenu_L1Menu_Commissioning2009_v8
  ev->SetAlias("l1t","L1TechBitsBeforeMask.fMask");
  ev->SetAlias("l1t32","l1t.TestBit(32)"); // th1 (inner)
  ev->SetAlias("l1t34","l1t.TestBit(34)"); // OR
  ev->SetAlias("l1t40","l1t.TestBit(40)"); // th1 (all)
  ev->SetAlias("l1t41","l1t.TestBit(41)"); // th2
  ev->SetAlias("l1t36","l1t.TestBit(36)"); // halo bits 36-39
  ev->SetAlias("l1t37","l1t.TestBit(37)"); // 
  ev->SetAlias("l1t38","l1t.TestBit(38)"); // 
  ev->SetAlias("l1t39","l1t.TestBit(39)"); // 
  ev->SetAlias("l1t35","l1t.TestBit(35)"); // high mult
  ev->SetAlias("l1t0","l1t.TestBit(0)");   // BPTX AND
  ev->SetAlias("l1t4","l1t.TestBit(4)");   // BPTX AND (instance 1)
  ev->SetAlias("l1a","L1AlgoBitsBeforeMask.fMask");
  ev->SetAlias("l1a45","l1a.TestBit(45)");

  ev->SetAlias("noHalo","(!l1t36 && !l1t37 && !l1t38 && !l1t39)");

  // bx info
  ev->SetAlias("bx","L1TechBitsBxs.fArray.fBx");
  ev->SetAlias("l1tbx","L1TechBitsBxs.fArray.fMask");
  ev->SetAlias("l1t0bx","l1tbx.TestBit(0)");   // BPTX AND
  ev->SetAlias("l1t4bx","l1tbx.TestBit(4)");   // prescaled (12x) BPTX AND
  ev->SetAlias("l1t34bx","l1tbx.TestBit(34)"); // BSC OR
  ev->SetAlias("l1t40bx","l1tbx.TestBit(40)"); // BSC coincidence threshold 1
  ev->SetAlias("l1t42bx","l1tbx.TestBit(42)"); // BSC splash beam 1
  ev->SetAlias("l1t43bx","l1tbx.TestBit(43)"); // BSC splash beam 2
  ev->SetAlias("l1t32bx","l1tbx.TestBit(32)"); // BSC coincidence inner threshold 1

  // pixel hit positions
  ev->SetAlias("layer","pxl.fType");
  ev->SetAlias("x","pxl.X()");
  ev->SetAlias("y","pxl.Y()");
  ev->SetAlias("z","pxl.Z()");
  ev->SetAlias("phi","atan(y/x)");
  ev->SetAlias("fpixPLUS","Sum$(pxl.fType>10 )");
  ev->SetAlias("fpixMINUS","Sum$(pxl.fType<-10)");

  // CaloTowers
  ev->SetAlias("ct","CaloTowers.fArray");
  ev->SetAlias("cy","1100*sign(ct.Phi())*abs(tan(ct.Theta()))");
  ev->SetAlias("cx","cy/tan(ct.Phi())");
  ev->SetAlias("HFneg","Sum$(ct.E()*(ct.Eta()<-3))");
  ev->SetAlias("HFpos","Sum$(ct.E()*(ct.Eta()>3))");
  ev->SetAlias("nHFn","Sum$((ct.E()>3)*(ct.Eta()<-3))");
  ev->SetAlias("nHFp","Sum$((ct.E()>3)*(ct.Eta()>3))");
  ev->SetAlias("Eneg","Sum$(ct.E()*(ct.Eta()<0))");
  ev->SetAlias("Epos","Sum$(ct.E()*(ct.Eta()>0))");

  // Event selection
  ev->SetAlias("qual","EvtSelData.fClusVtxQual");
  ev->SetAlias("qualDiff","EvtSelData.fClusVtxDiff");
  ev->SetAlias("tHFpos","EvtSelData.fHfPosTime");
  ev->SetAlias("tHFneg","EvtSelData.fHfNegTime");
  ev->SetAlias("eHFpos","EvtSelData.fHfPos");
  ev->SetAlias("eHFneg","EvtSelData.fHfNeg");

  // MC event info
  ev->SetAlias("pid","MCEventInfo.fProcessId");
  ev->SetAlias("ND","pid==1 || pid==2 || pid==3");
  ev->SetAlias("SD","pid==5 || pid==6");
  ev->SetAlias("DD","pid==7 || pid==4");
  ev->SetAlias("NSD","ND || DD");
  ev->SetAlias("inelastic","ND || DD || SD");
  ev->SetAlias("elastic","!(inelastic)");
  
  Double_t x[5] = {130,25,25,95,200};
  Double_t y[5] = {200,95,25,25,130};
  TPolyLine *pline = new TPolyLine(5,x,y);
  pline->SetLineColor(2);
  pline->SetLineWidth(2);

  //ev->SetAlias("cuts","Eneg>25 && Epos>25 && abs(Epos-Eneg)<70");
  //ev->SetAlias("cuts","npix>0");
  //ev->SetAlias("cuts","noHalo && !parasite");
  ev->SetAlias("cuts","noHalo && !parasite");
  ev->SetAlias("edscut","(qual>(0.5+0.0045*npix) || npix<150)");
  //data

  TH1F *h40 = new TH1F("h40","h40",2,0,2);
  TProfile *p40 = new TProfile("p40","p40",2,0,2);

  ev->Draw("SD:1.5>>p40","","prof");
  printf("SD: %f\n",p40->GetBinContent(2));
  
  ev->Draw("DD:1.5>>p40","","prof");
  printf("DD: %f\n",p40->GetBinContent(2));

  ev->Draw("ND:1.5>>p40","","prof");
  printf("ND: %f\n",p40->GetBinContent(2));

  ev->Draw("NSD:1.5>>p40","","prof");
  printf("NSD: %f\n",p40->GetBinContent(2));


  ev->Draw("Pixel3Vertexes.GetEntries()>0&&(nHFn>=1&&nHFp>=1):1.5>>p40","","prof");
  printf("tot eff: %f +/- %f\n",p40->GetBinContent(2),p40->GetBinError(2));

  ev->Draw("Pixel3Vertexes.GetEntries()>0&&(nHFn>=1&&nHFp>=1):1.5>>p40","SD","prof");
  printf("SD eff: %f +/- %f\n",p40->GetBinContent(2),p40->GetBinError(2));

  ev->Draw("Pixel3Vertexes.GetEntries()>0&&(nHFn>=1&&nHFp>=1):1.5>>p40","DD","prof");
  printf("DD eff: %f +/- %f\n",p40->GetBinContent(2),p40->GetBinError(2));

  ev->Draw("Pixel3Vertexes.GetEntries()>0&&(nHFn>=1&&nHFp>=1):1.5>>p40","ND","prof");
  printf("ND eff: %f +/- %f\n",p40->GetBinContent(2),p40->GetBinError(2));

  ev->Draw("Pixel3Vertexes.GetEntries()>0&&(nHFn>=1&&nHFp>=1):1.5>>p40","NSD","prof");
  printf("NSD eff: %f +/- %f\n",p40->GetBinContent(2),p40->GetBinError(2));
  
  /*
  ev->Draw("l1t40>>h40","cuts&& Pixel3Vertexes.GetEntries()>0 && edscut &&nHFn>=1&&nHFp>=1","");

  TH1F *h41 = new TH1F("h41","h41",2,0,2);
  ev->Draw("l1t41>>h41","cuts&& Pixel3Vertexes.GetEntries()>0 && edscut &&nHFn>=2&&nHFp>=2","");
  */
  //mc
  /*TH2F *hHF1 = new TH2F("hHF1","hHF1",20,0,20,20,0,20);
  ev->Draw("nHFn:nHFp>>hHF1","cuts && edscut &&nHFn>=1&&nHFp>=1","box");

  TH2F *hHF2 = new TH2F("hHF2","hHF2",20,0,20,20,0,20);
  ev->Draw("nHFn:nHFp>>hHF2","cuts && edscut &&nHFn>=2&&nHFp>=2","box");
  */
  //  printf("%d %d -> %f BSC eff 40: %f 41: %f\n",hHF1->GetEntries(),hHF2->GetEntries(),hHF2->GetEntries()/hHF1->GetEntries(),h40->GetMean(),h41->GetMean());
  /* TCanvas *c1 = new TCanvas("c1","c1",600,600);
  // c1->Divide(2,2);
  // c1->cd(1);
  ev->Draw("nHFn:nHFp>>hHF2","","");
  hHF2->Draw("colz");
  */

 


  
  /*
  ev->Draw("ngoodtrk>>h34(10,0,40)","l1t34 && Pixel3Vertexes.GetEntries()>0 && edscut","goff");
  TH1F *h34 = gDirectory->Get("h34");
  h34->Sumw2();
  ev->Draw("ngoodtrk>>h40(10,0,40)","l1t40 && Pixel3Vertexes.GetEntries()>0 && edscut","goff");
  TH1F *h40 = gDirectory->Get("h40");
  h40->Sumw2();
  ev->Draw("ngoodtrk>>h41(10,0,40)","l1t41 && Pixel3Vertexes.GetEntries()>0 && edscut","goff");
  TH1F *h41 = gDirectory->Get("h41");
  h41->Sumw2();
  
  TFile *f;
  if(type==0) f = new TFile("coll.root","RECREATE");
  else if(type==1) f = new TFile("empty.root","RECREATE");
  else if(type==2) f = new TFile("d6t.root","RECREATE");
  else if(type==3) f = new TFile("atlas.root","RECREATE");
  f->cd();
  h34->Write();
  h40->Write();
  h41->Write();
  f->ls();
  f->Close();
  */

  //---------------Monster Cut---------------------
  /*
  Double_t x[3] = {150,150,1000};
  Double_t y[3] = {0,0.5+0.0045*150,0.5+0.0045*1000};
  TPolyLine *pline = new TPolyLine(3,x,y);
  pline->SetLineColor(2);
  pline->SetLineWidth(2);
  
  if(type==0) { 
    ev->Draw("qual:EvtSelData.Npix()>>h0(50,0,1000,50,0,10)","l1t40","goff");
    h0->SetTitle("Run 123596 -- bit 34; # of pixel hits; cluster-vertex compatibility");
    h0->Draw("colz");
  }

  if(type==1) { 
    ev->Draw("qual:EvtSelData.Npix()>>h1(50,0,1000,50,0,10)","l1t34","goff");
    h1->SetTitle("Run 123596 (empty bunches)  -- bit 34; # of pixel hits; cluster-vertex compatibility");
    h1->Draw("colz");
  }

  if(type==2) { 
    ev->Draw("qual:EvtSelData.Npix()>>h2(50,0,1000,50,0,10)","l1t34 && NSD","goff");
    h2->SetTitle("PYTHIA D6T tune  -- bit 34; # of pixel hits; cluster-vertex compatibility");
    h2->Draw("colz");
  }

  if(type==3) { 
    ev->Draw("qual:EvtSelData.Npix()>>h3(50,0,1000,50,0,10)","l1t34 && NSD","goff");
    h3->SetTitle("PYTHIA ATLAS tune  -- bit 34; # of pixel hits; cluster-vertex compatibility");
    h3->Draw("colz");
  }
  
  pline->Draw("same");
  


  //ev->Draw("qualDiff:npix","l1t34 && npix<200","colz");
  //ev->Scan("qualDiff:npix:avtx.Z()","l1t40 && qual==0");
  //ev->Draw("qualDiff:npix>>h","l1t40 && Pixel3Vertexes.GetEntries()","goff");
  //h->Draw("colz");


  //ev->Draw("npix","l1t34 && Pixel3Vertexes.Entries()");

  //ev->Scan("evtno:avtx.fNTracks:avtx.Z()","l1t40");


  //ev->Draw("tHFpos:tHFneg","l1t34 && min(eHFpos,eHFneg)>25","colz");
  //ev->Draw("log10(eHFpos):log10(eHFneg)>>HF(50,-1,3,50,-1,3)","l1t34","colz");
  //ev->Draw("min(tHFneg,tHFpos):min(eHFneg,eHFpos)","l1t34");
  //ev->Draw("qual:bpix","qual<10 && bpix<1000 && l1t34");
  
  //ev->Scan("evtno","l1t41 && SD");
  */
  //ev->Draw("Pixel3Vertexes.fNTracks>>h(80,-0.5,79.5)","l1t40 && Pixel3Vertexes.Entries()>0");
}
