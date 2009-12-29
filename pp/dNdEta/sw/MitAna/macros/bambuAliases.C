gStyle->SetOptStat("nemr");

void bambuAliases(Int_t type=0){

  TChain *ev = new TChain("Events");

  //data
  //ev->Add("/home/edwenger/123596-merged-emptytarget.root");
  //ev->Add("/home/edwenger/123596-merged-targettarget.root");
  //if(type==0) ev->Add("/home/edwenger/123596-mergetestv4.root");
  //if(type==0) ev->Add("/home/edwenger/123596v5-targettarget.root");
  //if(type==0) ev->Add("/home/edwenger/124120-merged.root");
  if(type==0) ev->Add("/d100/data/ZeroBias-rereco/124022a3a4_v2_1/bambu*.root");
  if(type==1) ev->Add("/home/edwenger/123596v5-emptytarget.root");

  //mc
  //ev->Add("/home/loizides/bambu-summer09-MinBias-GEN-SIM-RAW-STARTUP3X_V8I_900GeV-v2.root");
  //ev->Add("/d101/yetkin/mc/CMSSW_3_3_5/BSC_Tuned_v1/Pythia_MinBias_D6T_900GeV_d20091210/bambu/0024CE3C-56E5-DE11-ABDD-002618943930.root");
  if(type==2) ev->Add("/home/edwenger/merge_BSC_Tuned_v1_Pythia_MinBias_D6T_900GeV_d20091210.root");
  if(type==3) ev->Add("/home/edwenger/merge_BSC_Tuned_v1_Pythia_MinBias_ATLAS_900GeV_d20091208.root");

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

  ev->SetAlias("parasite","((bunch>=1830 && bunch<=1833) || bunch==3170)");
  ev->SetAlias("collision","(bunch==51 || bunch==2724)");

  // HLT bits
  ev->SetAlias("hlt","HLTBits.fMask");
  ev->SetAlias("hltBSC","hlt.TestBit(66)");     //HLT_MinBiasBSC
  ev->SetAlias("hltBSC_OR","hlt.TestBit(67)");  //HLT_MinBiasBSC_OR
  ev->SetAlias("hltBakBSC","hlt.TestBit(78)");  //HLT_BackwardBSC
  ev->SetAlias("hltForBSC","hlt.TestBit(79)");  //HLT_ForwardBSC
  ev->SetAlias("hltCSCHalo","hlt.TestBit(74)"); //HLT_CSCBeamHalo
  ev->SetAlias("hltPix1","hlt.TestBit(71)");    //HLT_MinBiasPixel_SingleTrack
  ev->SetAlias("hltPix2","hlt.TestBit(72)");    //HLT_MinBiasPixel_DoubleTrack
  ev->SetAlias("hltEG1","hlt.TestBit(64)");

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

  // krisztian's 28 events with good agglomerative pixel vertex
  ev->SetAlias("kk1","Entry$==4 || Entry$==5 || Entry$==12 || Entry$==25 || Entry$==28 || Entry$==46 || Entry$==55 || Entry$==77 || Entry$==86 || Entry$==102");
  ev->SetAlias("kk2","Entry$==116 || Entry$==126 || Entry$==138 || Entry$==144 || Entry$==168 || Entry$==187 || Entry$==188 || Entry$==189 || Entry$==195 || Entry$==199");
  ev->SetAlias("kk3","Entry$==200 || Entry$==226 || Entry$==230 || Entry$==244 || Entry$==247 || Entry$==254 || Entry$==258 || Entry$==261");

  // events that are 75ns apart
  ev->SetAlias("ev1","evtno==2265620 || evtno==2265621");
  ev->SetAlias("ev2","evtno==2991369 || evtno==2991370");

  // pixel hit positions
  ev->SetAlias("layer","pxl.fType");
  ev->SetAlias("x","pxl.X()");
  ev->SetAlias("y","pxl.Y()");
  ev->SetAlias("z","pxl.Z()");
  ev->SetAlias("phi","atan(y/x)");
  ev->SetAlias("rho","sqrt(x*x+y*y)*sign(y)");
  ev->SetAlias("fpixPLUS","Sum$(pxl.fType>10 )");
  ev->SetAlias("fpixMINUS","Sum$(pxl.fType<-10)");

  // CaloTowers
  ev->SetAlias("ct","CaloTowers.fArray");
  ev->SetAlias("cr","1100*abs(tan(ct.Theta()))");
  ev->SetAlias("cx","cr*cos(ct.Phi())");
  ev->SetAlias("cy","cr*sin(ct.Phi())");
  ev->SetAlias("HFneg","Sum$(ct.E()*(ct.Eta()<-3))");
  ev->SetAlias("HFpos","Sum$(ct.E()*(ct.Eta()>3))");
  ev->SetAlias("Eneg","Sum$(ct.E()*(ct.Eta()<0))");
  ev->SetAlias("Epos","Sum$(ct.E()*(ct.Eta()>0))");
  ev->SetAlias("HfPos","ct.Eta()>3 && ct.E()>3");
  ev->SetAlias("HfNeg","ct.Eta()<3 && ct.E()>3");
  ev->SetAlias("HfAll","abs(ct.Eta())>3 && ct.E()>3");
  
  // diffrative vars
  ev->SetAlias("ctEp","ct.E()*cos(ct.Theta())");
  ev->SetAlias("ctEsubEp","ct.E()-ctEp");
  ev->SetAlias("ctEaddEp","ct.E()+ctEp");
  ev->SetAlias("ctSumEsubEp","Sum$(ctEsubEp)");
  ev->SetAlias("ctSumEaddEp","Sum$(ctEsubEp)");
  // Christof's tips
  ev->SetAlias("Epzn","Sum$((ct.E()-ct.Mom().Pz())*(ct.E()>3&&ct.Eta()<-3))");
  ev->SetAlias("Epzp","Sum$((ct.E()+ct.Mom().Pz())*(ct.E()>3&&ct.Eta()>3))");
  ev->SetAlias("hltHF3","hlt.TestBit(106)");
  ev->SetAlias("hltTrk1","hlt.TestBit(73)");
  ev.Draw("ct.E()-ct.Mom().Pz()","hltTrk1&&l1t34");
  ev.Draw("Epzn:Epzp","hltTrk1&&l1t34","box");

  // MC event info
  ev->SetAlias("pid","MCEventInfo.fProcessId");
  ev->SetAlias("ND","pid==11 || pid==12 || pid==13 || pid==28 || pid==53 || pid==68 || pid==95");
  ev->SetAlias("SD","pid==92 || pid==93");
  ev->SetAlias("DD","pid==94");
  ev->SetAlias("NSD","pid!=92 && pid!=93");
  ev->SetAlias("inelastic","ND || DD || SD");
  ev->SetAlias("elastic","!(inelastic)");

  // Event selection
  ev->SetAlias("qual","EvtSelData.fClusVtxQual");
  ev->SetAlias("qualDiff","EvtSelData.fClusVtxDiff");
  ev->SetAlias("tHFpos","EvtSelData.fHfPosTime");
  ev->SetAlias("tHFneg","EvtSelData.fHfNegTime");
  ev->SetAlias("eHFpos","EvtSelData.fHfPos");
  ev->SetAlias("eHFneg","EvtSelData.fHfNeg");

  // beamspot------------------
  //ev->Draw("vtx.Y():vtx.X()>>h(50,-0.5,0.5,50,-0.5,0.5)","l1t40 && vtx.fZErr<3 && run==123592","colz");
  //ev->Draw("vtx.Z()>>h(50,-20,20)","l1t40 && vtx.fZErr<3 && run==123592");
  

  // print events-------------------------------------
  //ev->Scan("evtno:npix:hltBSC:hltBakBSC:hltForBSC","(kk1 || kk2 || kk3)");
  //ev->Scan("evtno:l1t40:l1t41","l1t40");
  //ev->Scan("evtno:bunch:bx:l1t0bx:l1t40bx:l1t43bx:l1t32bx","evtno==2265620 || evtno==2265621 || evtno==2991369 || evtno==2991370");
  //ev->Scan("EventHeader.fTimeStamp-54100060637e8","ev1");
  //ev->Scan("EventHeader.fBunchCrossing","ev1");


  //ev->Scan("evtno:ct.Eta():ct.Phi():ct.E():ct.Theta():cy:cx","abs(ct.Eta())>3 && evtno==1903639 && ct.E()>3");

  // draw hits throughout detector in a single event --------------------------------------
  // big events in run 123151: 2529237, 3177667, 1903639, 2449816, 1259548
  
  /*

  Int_t run=123592;
  Int_t evt=532528; //1712524, 105574, 532528, 632862

  TCanvas *c1 = new TCanvas("c1","c1",1200,300);
  c1->Divide(5,1);
  c1->cd(1);
  TH2F *dum1 = new TH2F("dum1",Form("Run %d Event %d HF-;x [cm]; y [cm]",run,evt),10,-150,150,10,-150,150);
  dum1->Draw();
  ev->Draw("cy:cx>>h1(20,-150,150,20,-150,150)",Form("ct.Eta()<3 && evtno==%d && ct.E()>1 && abs(cx)<100 && abs(cy)<100",evt),"goff");
  h1->Draw("same colz");
  c1->cd(2);
  TH2F *dum2 = new TH2F("dum2",Form("Run %d Event %d PXF-;x [cm]; y [cm]",run,evt),30,-15,15,30,-15,15);
  dum2->Draw();
  ev->Draw("y:x>>h2a(1000,-10,10,1000,-10,10)",Form("layer==-11 && evtno==%d",evt),"goff");
  h2a->SetMarkerColor(2); h2a->SetMarkerStyle(6);
  h2a->Draw("same");
  ev->Draw("y:x>>h2b(1000,-10,10,1000,-10,10)",Form("layer==-12 && evtno==%d",evt),"goff");
  h2b->SetMarkerColor(4); h2b->SetMarkerStyle(6);
  h2b->Draw("same");
  c1->cd(3);
  TH2F *dum3 = new TH2F("dum3",Form("Run %d Event %d PXB;x [cm]; y [cm]",run,evt),30,-15,15,30,-15,15);
  dum3->Draw();
  ev->Draw("y:x>>h3(1000,-10,10,1000,-10,10)",Form("abs(layer)<10 && evtno==%d",evt),"goff");
  h3->SetMarkerColor(4); h3->SetMarkerStyle(6);
  h3->Draw("same");
  c1->cd(4);
  TH2F *dum4 = new TH2F("dum4",Form("Run %d Event %d PXF+;x [cm]; y [cm]",run,evt),30,-15,15,30,-15,15);
  dum4->Draw();
  ev->Draw("y:x>>h4a(1000,-10,10,1000,-10,10)",Form("layer==11 && evtno==%d",evt),"goff");
  h4a->SetMarkerColor(2); h4a->SetMarkerStyle(6);
  h4a->Draw("same");
  ev->Draw("y:x>>h4b(1000,-10,10,1000,-10,10)",Form("layer==12 && evtno==%d",evt),"goff");
  h4b->SetMarkerColor(4); h4b->SetMarkerStyle(6);
  h4b->Draw("same");
  c1->cd(5);
  TH2F *dum5 = new TH2F("dum5",Form("Run %d Event %d HF+;x [cm]; y [cm]",run,evt),10,-150,150,10,-150,150);
  dum5->Draw();
  ev->Draw("cy:cx>>h5(20,-150,150,20,-150,150)","ct.Eta()>3 && evtno==1903639 && ct.E()>1 && abs(cx)<100 && abs(cy)<100","goff");
  h5->Draw("same colz");
  
  */
  
  Double_t x[5] = {130,25,25,95,200};
  Double_t y[5] = {200,95,25,25,130};
  TPolyLine *pline = new TPolyLine(5,x,y);
  pline->SetLineColor(2);
  pline->SetLineWidth(2);

  //ev->SetAlias("cuts","Eneg>25 && Epos>25 && abs(Epos-Eneg)<70");
  //ev->SetAlias("cuts","npix>0");
  //ev->SetAlias("cuts","noHalo && !parasite");
  
  /*
  TCanvas *c1 = new TCanvas("c1","c1",800,800);
  c1->Divide(2,2);
  c1->cd(1);
  ev->Draw("Eneg/Epos:npix>>hHF1(25,0,600,25,0,6)","SD","goff");
  hHF1->Draw("colz");
  //pline->Draw("same");
  c1->cd(2);
  ev->Draw("Eneg/Epos:npix>>hHF2(25,0,600,25,0,6)","DD","goff");
  hHF2->Draw("colz"); 
  //pline->Draw("same"); 
  c1->cd(3);
  ev->Draw("Eneg/Epos:npix>>hHF3(25,0,600,25,0,6)","ND","goff");
  hHF3->Draw("colz"); 
  //pline->Draw("same"); 
  c1->cd(4);
  ev->Draw("Eneg/Epos:npix>>hHF4(25,0,600,25,0,6)","NSD","goff");
  hHF4->Draw("colz");
  //pline->Draw("same");
  */

  //ev->Draw("Eneg/Epos:npix>>h(25,0,600,25,0,6)","l1t40 && l1t0 && noHalo && !parasite","colz");

  //ev->Scan("Eneg:Epos:evtno:npix:bunch:parasite:noHalo:l1t0","abs(Epos-Eneg)>250 && npix>100 && !parasite");
  //ev->Draw("Eneg:Epos>>hHF(25,0,400,25,0,400)","l1t40 && run==123592 && noHalo","colz");

  //ev->Draw("(Epos-Eneg)/(Epos+Eneg):npix","collision && noHalo && l1t0 && l1t40","colz");

  
  //ev->Draw("l1t34>>h(2,-0.5,1.5)","l1a45 && run==123592");
  //ev->Draw("l1a45>>h2(2,-0.5,1.5)","l1t34 && run==123592","l text");


  //ev->Draw("(noHalo && l1t0):parasite>>hEvtSel(2,-0.5,1.5,2,-0.5,1.5)","run==123592 && l1t40","goff");
  //hEvtSel->SetTitle(";parasitic bunch crossings;BPTX coincidence AND NOT BSC halo");
  //hEvtSel->Draw("colz text");

  //pline->Draw("same");


  //------L1

  //ev->Draw("l1t40:l1t0","","colz text");
   
  //ev->Draw("qual:bpix>>h(50,0,2000,50,0,10)","qual<10  && l1t34","colz");
  //ev->Draw("tHFpos:tHFneg>>h2(50,-20,60,50,-20,60","l1t34 && min(eHFpos,eHFneg)>25","colz");
  //ev->Draw("log10(eHFpos):log10(eHFneg)>>h3(50,-1,3,50,-1,3)","l1t34 && eHFpos!=0 && eHFneg!=0 && abs(tHFpos-21)<10 && abs(tHFneg-21)<10","colz");
  //ev->Draw("min(tHFneg,tHFpos):min(eHFneg,eHFpos)>>h4(100,-20,100,100,-400,200)","l1t34 && (bpix/200*(qual-1))>1 && abs(tHFpos-21)<10 && abs(tHFneg-21)<10 && min(eHFpos,eHFneg)>0");



  //-------BSC efficiency (Johannes-style)
  /*
  //ev->Draw("qual:npix>>h(50,0,2000,50,0,10)","vtx.fNTracks>=5 && vtx.fZErr<3 && abs(vtx.Z())<10 && abs(vtx.X())<0.5 && abs(vtx.Y())<0.5 && l1t34","goff");
  ev->Draw("qual:npix>>h(50,0,2000,50,0,10)","vtx.fNTracks>=5 && vtx.fZErr<3 && abs(vtx.Z())<10 && abs(vtx.X())<0.5 && abs(vtx.Y())<0.5 && l1t41 && inelastic","goff");
  //h->SetTitle("Run 123596: Cluster shape compatibility with primary vertex; # of pixel hits; vertex uniqueness");
  h->SetTitle("PYTHIA inelastic: Cluster shape compatibility with primary vertex; # of pixel hits; vertex uniqueness");
  h->Draw("colz");
  */


  //------BSC efficiency (Ferenc-style)

  ev->SetAlias("edscut","(qual>(0.5+0.0045*npix) || npix<150)");
  
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

  //ev->Draw("Pixel3Vertexes.fNTracks>>h(80,-0.5,79.5)","l1t40 && Pixel3Vertexes.Entries()>0");
}
