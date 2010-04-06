void aliases_bambu(TTree * Events, int type=0)
{
  // event-wide aliases
  Events->SetAlias("run","EventHeader.fRunNum");
  Events->SetAlias("evtno","EventHeader.fEvtNum");
  Events->SetAlias("bunch","EventHeader.fBunchCrossing");
  Events->SetAlias("npix","PixelHits.Entries()");
  Events->SetAlias("pxl","PixelHits.fArray");
  Events->SetAlias("npix1","Sum$(pxl.fType==1)");
  Events->SetAlias("npix2","Sum$(pxl.fType==2)");
  Events->SetAlias("npix3","Sum$(pxl.fType==3)");
  Events->SetAlias("bpix","Sum$(abs(pxl.fType)<10)");
  Events->SetAlias("fpix","Sum$(abs(pxl.fType)>10)");
  Events->SetAlias("nvtx","PrimaryVertexes.Entries()");
  Events->SetAlias("vtx","PrimaryVertexes.fArray");
  Events->SetAlias("vtxs","Sum$(PrimaryVertexes.fZErr<3)");
  Events->SetAlias("avtx","Pixel3Vertexes.fArray");
  Events->SetAlias("avtxs","Pixel3Vertexes.Entries()");
  Events->SetAlias("tvtx","TrackletVertexes.fArray");
  Events->SetAlias("ntrk","Tracks.Entries()");
  Events->SetAlias("ngoodtrk","Sum$(Tracks.fArray.NHits()>9)");

  Events->SetAlias("parasite","((bunch>=1830 && bunch<=1833) || bunch==3170)");
  Events->SetAlias("collision","(bunch==51 || bunch==2724)");

  // HLT bits (the bit numbers change from menu to menu)
  Events->SetAlias("hlt","HLTBits.fMask");
  Events->SetAlias("hltPix1","hlt.TestBit(73)"); // HLT_MinBiasPixel_SingleTrack (firstCollisions09 v1.7)

  // l1 technical bits:  https://twiki.cern.ch/twiki/bin/viewauth/CMS/GlobalTriggerMenu_L1Menu_Commissioning2009_v8
  Events->SetAlias("l1t","L1TechBitsBeforeMask.fMask");
  Events->SetAlias("l1t32","l1t.TestBit(32)"); // th1 (inner)
  Events->SetAlias("l1t34","l1t.TestBit(34)"); // OR
  Events->SetAlias("l1t40","l1t.TestBit(40)"); // th1 (all)
  Events->SetAlias("l1t41","l1t.TestBit(41)"); // th2
  Events->SetAlias("l1t36","l1t.TestBit(36)"); // halo bits 36-39
  Events->SetAlias("l1t37","l1t.TestBit(37)"); // 
  Events->SetAlias("l1t38","l1t.TestBit(38)"); // 
  Events->SetAlias("l1t39","l1t.TestBit(39)"); // 
  Events->SetAlias("l1t35","l1t.TestBit(35)"); // high mult
  Events->SetAlias("l1t0","l1t.TestBit(0)");   // BPTX AND
  Events->SetAlias("l1t4","l1t.TestBit(4)");   // BPTX AND (prescaled)
  Events->SetAlias("l1a","L1AlgoBitsBeforeMask.fMask");
  Events->SetAlias("l1a45","l1a.TestBit(45)");
  Events->SetAlias("l1a82","l1a.TestBit(82)"); // BPTX OR (not prescaled)
  Events->SetAlias("l1a124","l1a.TestBit(124)"); // BSC and (BPTX OR) (not prescaled)

  Events->SetAlias("noHalo","(!l1t36 && !l1t37 && !l1t38 && !l1t39)");
  Events->SetAlias("bptxXOR","(l1a82 && !l1t0)");

  //Events->Scan("l1t0:l1t34:l1t35:l1t40:l1t41"); return;

  // bx info
  Events->SetAlias("bx","L1TechBitsBxs.fArray.fBx");
  Events->SetAlias("l1tbx","L1TechBitsBxs.fArray.fMask");
  Events->SetAlias("l1t0bx","l1tbx.TestBit(0)");   // BPTX AND
  Events->SetAlias("l1t4bx","l1tbx.TestBit(4)");   // prescaled (12x) BPTX AND
  Events->SetAlias("l1t34bx","l1tbx.TestBit(34)"); // BSC OR
  Events->SetAlias("l1t40bx","l1tbx.TestBit(40)"); // BSC coincidence threshold 1
  Events->SetAlias("l1t42bx","l1tbx.TestBit(42)"); // BSC splash beam 1
  Events->SetAlias("l1t43bx","l1tbx.TestBit(43)"); // BSC splash beam 2
  Events->SetAlias("l1t32bx","l1tbx.TestBit(32)"); // BSC coincidence inner threshold 1

  // krisztian's 28 events with good agglomerative pixel vertex
  Events->SetAlias("kk1","Entry$==4 || Entry$==5 || Entry$==12 || Entry$==25 || Entry$==28 || Entry$==46 || Entry$==55 || Entry$==77 || Entry$==86 || Entry$==102");
  Events->SetAlias("kk2","Entry$==116 || Entry$==126 || Entry$==138 || Entry$==144 || Entry$==168 || Entry$==187 || Entry$==188 || Entry$==189 || Entry$==195 || Entry$==199");
  Events->SetAlias("kk3","Entry$==200 || Entry$==226 || Entry$==230 || Entry$==244 || Entry$==247 || Entry$==254 || Entry$==258 || Entry$==261");

  // events that are 75ns apart
  Events->SetAlias("ev1","evtno==2265620 || evtno==2265621");
  Events->SetAlias("ev2","evtno==2991369 || evtno==2991370");

  // pixel hit positions
  Events->SetAlias("layer","pxl.fType");
  Events->SetAlias("x","pxl.X()");
  Events->SetAlias("y","pxl.Y()");
  Events->SetAlias("z","pxl.Z()");
  Events->SetAlias("phi","atan(y/x)");
  Events->SetAlias("rho","sqrt(x*x+y*y)*sign(y)");
  Events->SetAlias("fpixPLUS","Sum$(pxl.fType>10)");
  Events->SetAlias("fpixMINUS","Sum$(pxl.fType<-10)");
  Events->SetAlias("sumCos","Sum$(cos(phi))");
  Events->SetAlias("sumSin","Sum$(sin(phi))");

  // CaloTowers
  Events->SetAlias("ct","CaloTowers.fArray");
  Events->SetAlias("cy","1100*sign(ct.Phi())*abs(tan(ct.Theta()))");
  Events->SetAlias("cx","cy/tan(ct.Phi())");
  Events->SetAlias("HFneg","Sum$(ct.E()*(ct.Eta()<-3))");
  Events->SetAlias("HFpos","Sum$(ct.E()*(ct.Eta()>3))");
  Events->SetAlias("nHFneg","Sum$((ct.E()>3)*(ct.Eta()<-3))");
  Events->SetAlias("nHFpos","Sum$((ct.E()>3)*(ct.Eta()>3))");
  Events->SetAlias("eHFneg","Sum$(ct.E()*(ct.E()>3)*(ct.Eta()<-3))");
  Events->SetAlias("eHFpos","Sum$(ct.E()*(ct.E()>3)*(ct.Eta()>3))");
  Events->SetAlias("hfCoinc","(nHFneg>0 && nHFpos>0)");
  Events->SetAlias("Eneg","Sum$(ct.E()*(ct.Eta()<0))");
  Events->SetAlias("Epos","Sum$(ct.E()*(ct.Eta()>0))");
  // E thresholds: 3 GeV in HF (eta>3), 0.8 in endcaps (1.5<eta<3), 0.7 in barrel (eta<1.5)
  Events->SetAlias("HFEplusPz","Sum$((ct.E()+ct.Mom().Z())*(ct.E()>3)*(abs(ct.Eta())>3))");
  Events->SetAlias("HFEminusPz","Sum$((ct.E()-ct.Mom().Z())*(ct.E()>3)*(abs(ct.Eta())>3))");
  Events->SetAlias("HEEplusPz","Sum$((ct.E()+ct.Mom().Z())*(ct.E()>0.8)*(abs(ct.Eta())<3 && abs(ct.Eta())>1.5))");
  Events->SetAlias("HEEminusPz","Sum$((ct.E()-ct.Mom().Z())*(ct.E()>0.8)*(abs(ct.Eta())<3 && abs(ct.Eta())>1.5))");
  Events->SetAlias("HBEplusPz","Sum$((ct.E()+ct.Mom().Z())*(ct.E()>0.7)*(abs(ct.Eta())<1.5))");
  Events->SetAlias("HBEminusPz","Sum$((ct.E()-ct.Mom().Z())*(ct.E()>0.7)*(abs(ct.Eta())<1.5))");
  Events->SetAlias("EplusPz","(HFEplusPz+HEEplusPz+HBEplusPz)");
  Events->SetAlias("EminusPz","(HFEminusPz+HEEminusPz+HBEminusPz)");
  Events->SetAlias("nHFpos","Sum$(ct.E()>3 && ct.Eta()>3)"); //# of calotowers above E=3 GeV threshold
  Events->SetAlias("nHFneg","Sum$(ct.E()>3 && ct.Eta()<-3)");

  // MC event info
  Events->SetAlias("pid","MCEventInfo.fProcessId");
  if(type==4) { //PHOJET
    Events->SetAlias("ND","pid==1");
    Events->SetAlias("SD1","pid==5");
    Events->SetAlias("SD2","pid==6");
    Events->SetAlias("DD","pid==4 || pid==7");
  } else { //PYTHIA
    Events->SetAlias("ND","pid==11 || pid==12 || pid==13 || pid==28 || pid==53 || pid==68 || pid==95");
    Events->SetAlias("SD1","pid==92");
    Events->SetAlias("SD2","pid==93");
    Events->SetAlias("DD","pid==94");
  }
  Events->SetAlias("SD","(SD1 || SD2)");
  Events->SetAlias("NSD","(ND || DD)");
  Events->SetAlias("inelastic","(ND || DD || SD)");
  Events->SetAlias("elastic","!(inelastic)");

  // MC particles
  Events->SetAlias("genp","MCParticles.fArray");
  Events->SetAlias("pdg","genp.AbsPdgId()");
  Events->SetAlias("status","genp.Status()");
  Events->SetAlias("chg","genp.Charge()");
  Events->SetAlias("pdvtx","genp.fDecayVertex");
  Events->SetAlias("pdvtxr","sqrt(pdvtx.X()*pdvtx.X()+pdvtx.Y()*pdvtx.Y())");

  // Event selection
  Events->SetAlias("qual","EvtSelData.fClusVtxQual");
  Events->SetAlias("qualDiff","EvtSelData.fClusVtxDiff");
  Events->SetAlias("tHFpos","EvtSelData.fHfPosTime");
  Events->SetAlias("tHFneg","EvtSelData.fHfNegTime");
  Events->SetAlias("eHFpos","EvtSelData.fHfPos");
  Events->SetAlias("eHFneg","EvtSelData.fHfNeg");
  Events->SetAlias("hpFrac","EvtSelData.fHPTrkFrac");

  // Tracks
  Events->SetAlias("trk","Tracks.fArray");

  //----------------Beamspot------------------
  //Events->Draw("vtx.Y():vtx.X()>>h(50,-0.5,0.5,50,-0.5,0.5)","l1t40 && vtx.fZErr<3 && run==123592","colz");
  //Events->Draw("vtx.Z()>>h(50,-20,20)","l1t40 && vtx.fZErr<3 && run==123592");
  

  //---------------Print Events-------------------------------------
  //Events->Scan("evtno:npix:hltBSC:hltBakBSC:hltForBSC","(kk1 || kk2 || kk3)");
  //Events->Scan("evtno:l1t40:l1t41","l1t40");
  //Events->Scan("evtno:bunch:bx:l1t0bx:l1t40bx:l1t43bx:l1t32bx","evtno==2265620 || evtno==2265621 || evtno==2991369 || evtno==2991370");
  //Events->Scan("EventHeader.fTimeStamp-54100060637e8","ev1");
  //Events->Scan("EventHeader.fBunchCrossing","ev1");


  //Events->Scan("evtno:ct.Eta():ct.Phi():ct.E():ct.Theta():cy:cx","abs(ct.Eta())>3 && evtno==1903639 && ct.E()>3");

  //--------------Draw Hits throughout detector in a single event --------------------------------
  // big events in run 123151: 2529237, 3177667, 1903639, 2449816, 1259548
  
  /*

  Int_t run=123592;
  Int_t evt=532528; //1712524, 105574, 532528, 632862

  TCanvas *c1 = new TCanvas("c1","c1",1200,300);
  c1->Divide(5,1);
  c1->cd(1);
  TH2F *dum1 = new TH2F("dum1",Form("Run %d Event %d HF-;x [cm]; y [cm]",run,evt),10,-150,150,10,-150,150);
  dum1->Draw();
  Events->Draw("cy:cx>>h1(20,-150,150,20,-150,150)",Form("ct.Eta()<3 && evtno==%d && ct.E()>1 && abs(cx)<100 && abs(cy)<100",evt),"goff");
  h1->Draw("same colz");
  c1->cd(2);
  TH2F *dum2 = new TH2F("dum2",Form("Run %d Event %d PXF-;x [cm]; y [cm]",run,evt),30,-15,15,30,-15,15);
  dum2->Draw();
  Events->Draw("y:x>>h2a(1000,-10,10,1000,-10,10)",Form("layer==-11 && evtno==%d",evt),"goff");
  h2a->SetMarkerColor(2); h2a->SetMarkerStyle(6);
  h2a->Draw("same");
  Events->Draw("y:x>>h2b(1000,-10,10,1000,-10,10)",Form("layer==-12 && evtno==%d",evt),"goff");
  h2b->SetMarkerColor(4); h2b->SetMarkerStyle(6);
  h2b->Draw("same");
  c1->cd(3);
  TH2F *dum3 = new TH2F("dum3",Form("Run %d Event %d PXB;x [cm]; y [cm]",run,evt),30,-15,15,30,-15,15);
  dum3->Draw();
  Events->Draw("y:x>>h3(1000,-10,10,1000,-10,10)",Form("abs(layer)<10 && evtno==%d",evt),"goff");
  h3->SetMarkerColor(4); h3->SetMarkerStyle(6);
  h3->Draw("same");
  c1->cd(4);
  TH2F *dum4 = new TH2F("dum4",Form("Run %d Event %d PXF+;x [cm]; y [cm]",run,evt),30,-15,15,30,-15,15);
  dum4->Draw();
  Events->Draw("y:x>>h4a(1000,-10,10,1000,-10,10)",Form("layer==11 && evtno==%d",evt),"goff");
  h4a->SetMarkerColor(2); h4a->SetMarkerStyle(6);
  h4a->Draw("same");
  Events->Draw("y:x>>h4b(1000,-10,10,1000,-10,10)",Form("layer==12 && evtno==%d",evt),"goff");
  h4b->SetMarkerColor(4); h4b->SetMarkerStyle(6);
  h4b->Draw("same");
  c1->cd(5);
  TH2F *dum5 = new TH2F("dum5",Form("Run %d Event %d HF+;x [cm]; y [cm]",run,evt),10,-150,150,10,-150,150);
  dum5->Draw();
  Events->Draw("cy:cx>>h5(20,-150,150,20,-150,150)","ct.Eta()>3 && evtno==1903639 && ct.E()>1 && abs(cx)<100 && abs(cy)<100","goff");
  h5->Draw("same colz");
  
  */


  //----------------Energy balance-------------------------------------------

  /*
  Double_t x[5] = {130,25,25,95,200};
  Double_t y[5] = {200,95,25,25,130};
  TPolyLine *pline = new TPolyLine(5,x,y);
  pline->SetLineColor(2);
  pline->SetLineWidth(2);

  //Events->SetAlias("cuts","Eneg>25 && Epos>25 && abs(Epos-Eneg)<70");
  //Events->SetAlias("cuts","npix>0");
  //Events->SetAlias("cuts","noHalo && !parasite");
  
  
  TCanvas *c1 = new TCanvas("c1","c1",800,800);
  c1->Divide(2,2);
  c1->cd(1);
  Events->Draw("Eneg/Epos:npix>>hHF1(25,0,600,25,0,6)","SD","goff");
  hHF1->Draw("colz");
  //pline->Draw("same");
  c1->cd(2);
  Events->Draw("Eneg/Epos:npix>>hHF2(25,0,600,25,0,6)","DD","goff");
  hHF2->Draw("colz"); 
  //pline->Draw("same"); 
  c1->cd(3);
  Events->Draw("Eneg/Epos:npix>>hHF3(25,0,600,25,0,6)","ND","goff");
  hHF3->Draw("colz"); 
  //pline->Draw("same"); 
  c1->cd(4);
  Events->Draw("Eneg/Epos:npix>>hHF4(25,0,600,25,0,6)","NSD","goff");
  hHF4->Draw("colz");
  //pline->Draw("same");
  */

  //Events->Draw("Eneg/Epos:npix>>h(25,0,600,25,0,6)","l1t40 && l1t0 && noHalo && !parasite","colz");

  //Events->Scan("Eneg:Epos:evtno:npix:bunch:parasite:noHalo:l1t0","abs(Epos-Eneg)>250 && npix>100 && !parasite");
  //Events->Draw("Eneg:Epos>>hHF(25,0,400,25,0,400)","l1t40 && run==123592 && noHalo","colz");

  //Events->Draw("(Epos-Eneg)/(Epos+Eneg):npix","collision && noHalo && l1t0 && l1t40","colz");

  
  //Events->Draw("l1t34>>h(2,-0.5,1.5)","l1a45 && run==123592");
  //Events->Draw("l1a45>>h2(2,-0.5,1.5)","l1t34 && run==123592","l text");


  //Events->Draw("(noHalo && l1t0):parasite>>hEvtSel(2,-0.5,1.5,2,-0.5,1.5)","run==123592 && l1t40","goff");
  //hEvtSel->SetTitle(";parasitic bunch crossings;BPTX coincidence AND NOT BSC halo");
  //hEvtSel->Draw("colz text");

  //pline->Draw("same");


  //------L1

  //Events->Draw("l1t40:l1t0","","colz text");
   
  //Events->Draw("qual:bpix>>h(50,0,2000,50,0,10)","qual<10  && l1t34","colz");
  //Events->Draw("tHFpos:tHFneg>>h2(50,-20,60,50,-20,60","l1t34 && min(eHFpos,eHFneg)>25","colz");
  //Events->Draw("log10(eHFpos):log10(eHFneg)>>h3(50,-1,3,50,-1,3)","l1t34 && eHFpos!=0 && eHFneg!=0 && abs(tHFpos-21)<10 && abs(tHFneg-21)<10","colz");
  //Events->Draw("min(tHFneg,tHFpos):min(eHFneg,eHFpos)>>h4(100,-20,100,100,-400,200)","l1t34 && (bpix/200*(qual-1))>1 && abs(tHFpos-21)<10 && abs(tHFneg-21)<10 && min(eHFpos,eHFneg)>0");

  //Events->Draw("bx>>bx(5,-2.5,2.5)","l1t40 && !l1t34 && l1t34bx");
  //Events->Draw("l1t40:l1t34","","colz text");

  //-------BSC efficiency (Johannes-style)
  /*
  //Events->Draw("qual:npix>>h(50,0,2000,50,0,10)","vtx.fNTracks>=5 && vtx.fZErr<3 && abs(vtx.Z())<10 && abs(vtx.X())<0.5 && abs(vtx.Y())<0.5 && l1t34","goff");
  Events->Draw("qual:npix>>h(50,0,2000,50,0,10)","vtx.fNTracks>=5 && vtx.fZErr<3 && abs(vtx.Z())<10 && abs(vtx.X())<0.5 && abs(vtx.Y())<0.5 && l1t41 && inelastic","goff");
  //h->SetTitle("Run 123596: Cluster shape compatibility with primary vertex; # of pixel hits; vertex uniqueness");
  h->SetTitle("PYTHIA inelastic: Cluster shape compatibility with primary vertex; # of pixel hits; vertex uniqueness");
  h->Draw("colz");
  */


  //------BSC efficiency (Ferenc-style)

  /*
  Events->SetAlias("edscut","(qual>(0.5+0.0045*npix) || npix<150)");
  Events->SetAlias("edscut2","(qual>(0.0045*npix) || npix<150 || qual>2)");
  Events->SetAlias("pogcut","(ntrk<10 || hpFrac>0.2)");
  Events->SetAlias("pogcut2","(ntrk<10 || hpFrac>0.25)");
  
  Events->Draw("ngoodtrk>>h34(10,0,40)","l1t34 && Pixel3Vertexes.GetEntries()>0 && edscut","goff");
  TH1F *h34 = gDirectory->Get("h34");
  h34->Sumw2();
  Events->Draw("ngoodtrk>>h40(10,0,40)","l1t40 && Pixel3Vertexes.GetEntries()>0 && edscut","goff");
  TH1F *h40 = gDirectory->Get("h40");
  h40->Sumw2();
  Events->Draw("ngoodtrk>>h41(10,0,40)","l1t41 && Pixel3Vertexes.GetEntries()>0 && edscut","goff");
  TH1F *h41 = gDirectory->Get("h41");
  h41->Sumw2();
  
  TFile *f;
  if(type==0) f = new TFile("bitratios/coll.root","RECREATE");
  else if(type==1) f = new TFile("bitratios/empty.root","RECREATE");
  else if(type==2) f = new TFile("bitratios/d6t.root","RECREATE");
  else if(type==3) f = new TFile("bitratios/atlas.root","RECREATE");
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
  
  Double_t x2[4] = {150,150,2/0.0045,1000};
  Double_t y2[4] = {0,0.0045*150,2,2};
  TPolyLine *pline2 = new TPolyLine(4,x2,y2);
  pline2->SetLineColor(2);
  pline2->SetLineWidth(2);
  pline2->SetLineStyle(2);
 
  

  if(type==0) { 
    
    //Events->Draw("qual:EvtSelData.Npix()>>h0(50,0,1500,50,0,10)","l1t40 && avtxs>0","goff");
    //h0->SetTitle("Run 123596 -- bit 40+vtx; # of pixel hits; cluster-vertex compatibility");
    
    //Events->Draw("qual:EvtSelData.Npix()>>h0(50,0,1500,50,0,10)","l1t41","goff");
    //h0->SetTitle("Run 124023 -- bit 41; # of pixel hits; cluster-vertex compatibility");
    
    //Events->Draw("qual:EvtSelData.Npix()>>h0(50,0,1500,50,0,10)","l1t34 && l1t0 && noHalo && avtxs>0 && hfCoinc","goff");
    //h0->SetTitle("Run 124023 -- BPTX_AND, no BSC halo, BSC_OR, pixel vertex, HF coinc; # of pixel hits; cluster-vertex compatibility");

    Events->Draw("qual:EvtSelData.Npix()>>collBeams(50,0,1500,50,0,10)","l1t34 && l1t0 && noHalo && avtxs>0 && hfCoinc","goff");
    collBeams->SetTitle("Run 132440 -- BPTX_AND, no BSC halo, BSC_OR, pixel vertex, HF coinc.; # of pixel hits; cluster-vertex compatibility");
    collBeams->SetMinimum(1); collBeams->SetMaximum(1000);

    TCanvas *c0 = new TCanvas("c0","c0",500,500);
    collBeams->Draw("colz");
    gPad->SetRightMargin(0.15);
    gPad->SetLogz();
    
  }
  
  if(type==1) { 
    //Events->Draw("qual:EvtSelData.Npix()>>h1(50,0,1000,50,0,10)","l1t34","goff");
    //h1->SetTitle("Run 124120 (empty bunches)  -- bit 34; # of pixel hits; cluster-vertex compatibility");

    //Events->Scan("bunch","l1t34 && bptxXOR && noHalo && avtxs>0");

    Events->Draw("qual:EvtSelData.Npix()>>singleBeam(50,0,1500,50,0,10)","l1t34 && bptxXOR && noHalo && avtxs>0","goff");
    singleBeam->SetTitle("Run 132422 -- BPTX_XOR, no BSC halo, BSC_OR, pixel vertex; # of pixel hits; cluster-vertex compatibility");
    singleBeam->SetMinimum(1); singleBeam->SetMaximum(1000);

    TCanvas *c1 = new TCanvas("c1","c1",500,500);
    singleBeam->Draw("colz");
    gPad->SetRightMargin(0.15);
    gPad->SetLogz();

  }
  
  if(type==2) { 
    //Events->Draw("qual:EvtSelData.Npix()>>h2(50,0,1000,50,0,10)","l1t34 && NSD","goff");
    //h2->SetTitle("PYTHIA D6T tune  -- bit 34+NSD; # of pixel hits; cluster-vertex compatibility");
    
    Events->Draw("qual:EvtSelData.Npix()>>pythiaD6T(50,0,1500,50,0,10)","l1t34 && avtxs>0 && hfCoinc","goff");
    pythiaD6T->SetTitle("PYTHIA D6T tune -- BSC_OR, pixel vertex, HF coinc; # of pixel hits; cluster-vertex compatibility");
    pythiaD6T->SetMinimum(1); pythiaD6T->SetMaximum(1000);

    TCanvas *c2 = new TCanvas("c2","c2",500,500);
    pythiaD6T->Draw("colz");
    gPad->SetRightMargin(0.15);
    gPad->SetLogz();

  }
  
  if(type==3) { 
    //Events->Draw("qual:EvtSelData.Npix()>>h3(50,0,1000,50,0,10)","l1t34 && NSD","goff");
    //h3->SetTitle("PYTHIA ATLAS tune  -- bit 34+NSD; # of pixel hits; cluster-vertex compatibility");

    Events->Draw("qual:EvtSelData.Npix()>>pythiaATLAS(50,0,1500,50,0,10)","l1t34 && avtxs>0","goff");
    pythiaATLAS->SetTitle("PYTHIA ATLAS tune -- BSC_OR, pixel vertex; # of pixel hits; cluster-vertex compatibility");
    pythiaATLAS->SetMinimum(1); pythiaATLAS->SetMaximum(1000);

    TCanvas *c3 = new TCanvas("c3","c3",500,500);
    pythiaATLAS->Draw("colz");
    gPad->SetRightMargin(0.15);
    gPad->SetLogz();

  }
  
  pline->Draw("same");
  pline2->Draw("same");
  
  */
  
  


  //--- check on monster cut's effect on high-pt tracks
  /*
  Events->SetAlias("goodtrk","(abs(trk.D0())<0.3 && trk.NHits()>7)");
  Events->Draw("trk.Pt()>>cut1(50,3,8)","edscut && goodtrk && trk.Pt()>3 && trk.Pt()<8 && avtxs>0 && l1t34 && hfCoinc","goff");
  Events->Draw("trk.Pt()>>cut2(50,3,8)","edscut2 && goodtrk && trk.Pt()>3 && trk.Pt()<8 && avtxs>0 && l1t34 && hfCoinc","goff");
  cut1->Draw();
  cut2->SetLineColor(2);
  cut2->Draw("same");
  */

  //--------------High-purity track fraction cut--------------------

  /*
  Double_t x3[3] = {10,10,500};
  Double_t y3[3] = {0,0.2,0.2};
  TPolyLine *pline3 = new TPolyLine(3,x3,y3);
  pline3->SetLineColor(2);
  pline3->SetLineWidth(2);

  Double_t x4[3] = {10,10,500};
  Double_t y4[3] = {0,0.25,0.25};
  TPolyLine *pline4 = new TPolyLine(3,x4,y4);
  pline4->SetLineColor(2);
  pline4->SetLineWidth(2);
  pline4->SetLineStyle(2);
  */

  //Events->Draw("hpFrac:ntrk","l1t34","colz");
  //Events->Draw("hpFrac:ntrk","l1t40","colz");
  //Events->Draw("hpFrac:ntrk","l1t40 && Pixel3Vertexes.GetEntries()>0","colz");

  /*
  Events->Draw("qual:EvtSelData.Npix()>>h0(50,0,1500,50,0,10)","PixelTracks.GetEntries()>0 && pogcut","colz");
  pline->Draw("same");
  pline2->Draw("same");
  */

  
  /*
  Events->Draw("hpFrac:ntrk>>h1(50,0,500,50,0,1)","PixelTracks.GetEntries()>0 && edscut2","colz");
  pline3->Draw("same");
  pline4->Draw("same");
  */

  //Events->Draw("pogcut:edscut2","PixelTracks.GetEntries()>0","colz");
  

  //-------------Diffractive fractions------------------------------
  
  //TCanvas *c1 = new TCanvas("c1","E threshold",600,500);c1->cd();
  //Events->Draw("ct.E():abs(ct.Eta())","ct.E()<8","colz");
  //gPad->SetLogz();
  //TCanvas *c2 = new TCanvas("c2","ET threshold",600,500);c2->cd();
  //Events->Draw("ct.Et():abs(ct.Eta())","ct.Et()<2","colz");
  //gPad->SetLogz();
  
  /*
  Events->Draw("l1t34>>h","l1t34","goff");
  //Int_t nevents = Events->GetSelectedRows();
  Int_t nevents = h->GetEntries();
  Events->Draw("ct.E()>>hHB(40,0,2)","abs(ct.Eta())<1.5 && l1t34","goff");
  TH1F *hHB = gDirectory->Get("hHB");
  hHB->Sumw2(); //hHB->Scale(1./nevents);
  Events->Draw("ct.E()>>hHE(40,0,2)","abs(ct.Eta())>1.5 && abs(ct.Eta())<3 && l1t34","goff");
  TH1F *hHE = gDirectory->Get("hHE");
  hHE->Sumw2(); //hHE->Scale(1./nevents);
  Events->Draw("ct.E()>>hHF(40,0,8)","abs(ct.Eta())>3 && l1t34","goff");
  TH1F *hHF = gDirectory->Get("hHF");
  hHF->Sumw2(); //hHF->Scale(1./nevents);
  Events->Draw("abs(ct.Eta())>>hEta","l1t34","goff");
  TH1F *hEta = gDirectory->Get("hEta");
  hEta->Sumw2(); //hEta->Scale(1./nevents);
  */

  //Events->Draw("0.5*log((HFEplusPz+HEEplusPz+HBEplusPz)/(HFEminusPz+HEEminusPz+HBEminusPz))","DD && l1t34");
  //Events->Draw("0.5*log((HFEplusPz)/(HFEminusPz))","DD && l1t34"); 
  //Events->Draw("HFEplusPz>>hfEpPz(75,0,25)","(ND) && l1t34"); 
  //Events->Draw("HFEplusPz + HEEplusPz + HBEplusPz","DD && l1t34"); 
  //Events->Draw("nHFpos:nHFneg>>h2(11,-0.5,10.5,11,-0.5,10.5)","(SD || DD) && l1t34 && avtxs>0","goff");
  //h2->Scale(1./h2->GetEntries());
  //h2->Draw("colz");


  /*
  Events->Draw("EplusPz:EminusPz>>h2(50,0,400,50,0,400)","SD && l1t34","goff");
  h2->Scale(1./h2->GetEntries());
  h2->SetMinimum(0.00005); h2->SetMaximum(0.05);
  h2->Draw("colz");
  gPad->SetLogz();
  */

  /*
  TFile *f;
  if(type==0) f = new TFile("diffraction/coll.root","RECREATE");
  else if(type==1) f = new TFile("diffraction/empty.root","RECREATE");
  else if(type==2) f = new TFile("diffraction/d6t.root","RECREATE");
  else if(type==3) f = new TFile("diffraction/atlas.root","RECREATE");
  f->cd();
  hHB->Write();
  hHE->Write();
  hHF->Write();
  hEta->Write();
  f->ls();
  f->Close();
  */

  //--------------High-pT Tracks----------------------------
  //Events->Scan("trk.Pt():trk.NHits():trk.D0():trk.fDsz","l1t34 && hfCoinc && avtxs>0 && edscut && trk.Pt()>10 && trk.Pt()<100");



  //--------------Miscellaneous------------------------------

  //Events->Draw("qualDiff:npix","l1t34 && npix<200","colz");
  //Events->Scan("qualDiff:npix:avtx.Z()","l1t40 && qual==0");
  //Events->Draw("qualDiff:npix>>h","l1t40 && Pixel3Vertexes.GetEntries()","goff");
  //h->Draw("colz");

  //Events->Draw("npix","l1t34 && Pixel3Vertexes.Entries()");
  //Events->Scan("evtno:avtx.fNTracks:avtx.Z()","l1t40");

  //Events->Draw("tHFpos:tHFneg","l1t34 && min(eHFpos,eHFneg)>25","colz");
  //Events->Draw("log10(eHFpos):log10(eHFneg)>>HF(50,-1,3,50,-1,3)","l1t34","colz");
  //Events->Draw("min(tHFneg,tHFpos):min(eHFneg,eHFpos)","l1t34");
  //Events->Draw("qual:bpix","qual<10 && bpix<1000 && l1t34");
  
  //Events->Scan("evtno","l1t41 && SD");

  //Events->Draw("Pixel3Vertexes.fNTracks>>h(80,-0.5,79.5)","l1t40 && Pixel3Vertexes.Entries()>0");


  //Events->Draw("eHFpos:eHFneg>>h2(30,0,300,30,0,300)","edscut && l1t41 && avtxs>0","colz");

  //Events->Scan("npix:sqrt(Sum$(cos(phi))*Sum$(cos(phi))+Sum$(sin(phi))*Sum$(sin(phi))):atan(Sum$(sin(phi))/Sum$(cos(phi)))","!edscut2 && l1t41 && avtxs>0","colz");
  //Events->Draw("Sum$(x/(x*x+y*y)*abs(pxl.fType)>10):Sum$(y/(x*x+y*y)*abs(pxl.fType)>10)","l1t41 && avtxs>0 && !edscut2","colz");

  //Events->Draw("Sum$((pxl.fType>10)*x)/Length$((pxl.fType>10)*x):Sum$((pxl.fType<-10)*x)/Length$((pxl.fType<-10)*x)","!edscut2","colz");

  //Events->Scan("avtxs:avtx[0].fNTracks:avtx[0].Z():avtx[1].fNTracks:avtx[1].Z():avtx[2].fNTracks:avtx[2].Z()","avtxs==3 && l1t34 && hfCoinc && edscut && (avtx[0].fNTracks==avtx[1].fNTracks || avtx[0].fNTracks==avtx[2].fNTracks || avtx[2].fNTracks==avtx[1].fNTracks)");


}
