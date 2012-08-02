#include "CutAndBinCollection2011.h"
void drawSingleVariable(TH1D* theHist=0, int lowCent=0, int highCent=39, TString variable1="sigmaIetaIeta", TCut addCut1="", TString variable2="", TCut addCut2="", int normChoice=0,bool overDraw=false, TH1D* theMCSig=0);

void basicSpectraSingle(int lowCent=0, int highCent=0, double lowPt=0, double highPt=0 );
void basicSpectra() {
   for ( int icent = 1 ; icent<=3 ; icent++) { 
      int lowCent = centBin_std[icent-1];
      int highCent = centBin_std[icent]-1;
      //basicSpectraSingle( lowCent, highCent, 40,50);
      basicSpectraSingle( lowCent, highCent, 60,10000);
      //      basicSpectraSingle( lowCent, highCent, 50,80);
      //  basicSpectraSingle( lowCent, highCent, 80,120);
      
   }
}

void basicSpectraSingle(int lowCent, int highCent, double lowPt, double highPt ) { 
      
   TCut hoeCut = "hadronicOverEm<0.1";
   TCut ptCut  = Form("pt>%.f && pt <%.f",lowPt,highPt);
   TString ptCutS = Form("E_{T} : %.0f - %.0f GeV",lowPt,highPt);
   if ( highPt > 200 ) 
      ptCutS = Form("E_{T} > %.0f GeV",lowPt);
   
   // eta
   // phi 
   // pT >30GeV   // normalize it by 40GeV 
   // energy/rawenergy
   TCanvas* c2 = new TCanvas("c2","",700,700);
   c2->Divide(2,2);
   c2->cd(1);
   TH1D* hpt    = new TH1D("hpt",";E_{T} (GeV);Normalized by p_{T} > 40GeV",40,40,500);
   drawSingleVariable(hpt,lowCent,highCent,"pt","","","",0);

   //   jumSun(40,1e-3,40,1);
   gPad->SetLogy();
   c2->cd(2);
   TH1D* hhoe    = new TH1D("hhoe",";H/E;Entries per photon candidate",50,0,1);
   drawSingleVariable(hhoe,lowCent,highCent,"hadronicOverEm",ptCut && etaCut,"","",0);
   drawText(ptCutS,0.5,0.66);
   
   //  jumSun(0.2,0,0.2,0.24);
   
   //  c2->cd(3);
   //   TH1D* hcc4    = new TH1D("hcc4",";Ecal ISO (GeV);",40,-20,60);
   //  drawSingleVariable(hcc4,lowCent,highCent,"cc4", hoeCut && ptCut,"","",0);
   
   //   c2->cd(4);
   //  TH1D* hcr4    = new TH1D("hcr4",";Hcal ISO (GeV);",40,-20,60);
   //  drawSingleVariable(hcr4,lowCent,highCent,"cr4",ptCut &&hoeCut,"","",0);
   
   //   c2->cd(5);
   //   TH1D* hct4    = new TH1D("hct4",";track ISO (GeV);",40,-20,60);
   //  drawSingleVariable(hct4,lowCent,highCent,"ct4PtCut20",ptCut&& hoeCut,"ct4PtCut","",0);
   
   c2->cd(3);
   TH1D* hIso    = new TH1D("hIso",";Sum ISO (GeV);Entries per photon candidate",40,-30,100);
   drawSingleVariable(hIso,lowCent,highCent,"(ct4PtCut20+cc3+cr3)/0.9", ptCut && hoeCut,"(ct4PtCut+cc3+cr3)/0.9","",0);
   
   drawText(ptCutS,0.5,0.66);
   drawText("H/E < 0.2",0.5,0.6);
   //   jumSun(5,0,5,0.24);
   //c2->cd(5);
   // TH1D* trackIso    = new TH1D("trackIsoo",";track ISO (no BKG sub) (GeV);",40,0,100);
   //  drawSingleVariable(trackIso,lowCent,highCent,"trkSumPtHollowConeDR04",ptCut&& hoeCut,"","",0);
   //   c2->cd(5);
   //  TH1D* hr9    = new TH1D("hr9",";r9 ;",50,0,1.1);
   // drawSingleVariable(hr9,lowCent,highCent,"r9", ptCut && hoeCut,"","",0);

        // "cc4*2.1394389581550754e-04 + cr4 * -1.1920380855810875e-02 + ct4PtCut20* 3.3720653764304644e-02 + cc3*-5.1750216598978376e-02 + cr3*-3.6800703345337124e-02 + ct3PtCut20*-9.10880235455440575e-02 > -0.5";
   TCut sbCut = "(cc3+ cr3 + ct3PtCut20)>5 && (cc4j+ cr4j + ct3PtCut20)<10";
   c2->cd(4);
   
   TH1D* hsieieData  = new TH1D(Form("hsieieData_cent%dto%d_pt%dto%d",lowCent,highCent,(int)lowPt,(int)highPt),";shower shape (#sigma_{#eta#eta});Entries per photon candidate;",25,0,0.025);
   TH1D* hsieieSig   = (TH1D*)hsieieData->Clone(Form("hsieieSig_cent%dto%d_pt%dto%d",lowCent,highCent,(int)lowPt,(int)highPt));
   drawSingleVariable(hsieieData,lowCent,highCent,"sigmaIetaIeta",ptCut && hoeCut && FisherCut,"",ptCut && hoeCut && "cc4j<1 && cr4j<1 && ct4PtCut<1",-1,false,hsieieSig);
   drawText(ptCutS,0.5,0.66);
   drawText("H/E < 0.2",0.5,0.6);
   drawText("Sum ISO < 5GeV",0.5,0.54);   

       
   TH1D* hsieieSB  = new TH1D(Form("hsieieSB_cent%dto%d_pt%dto%d",lowCent,highCent,(int)lowPt,(int)highPt),";shower shape (#sigma_{#eta#eta});Entries per photon;",25,0,0.025);
   drawSingleVariable(hsieieSB,lowCent,highCent,"sigmaIetaIeta",ptCut && hoeCut && sbCut,"",ptCut && hoeCut &&  "cc3<1 && cr3<1 && ct4PtCut>2 && ct4PtCut<6",0);
   
   
   
   TFile outf = TFile("templates2011.root","update");
   hsieieData->Write();
   hsieieSB->Write();
   hsieieSig->Write();
   outf.Close();   
   
   
   
}





void drawSingleVariable(TH1D* theHist, int lowCent, int highCent, TString variable1, TCut addCut1, TString variable2, TCut addCut2, int normChoice,bool overDraw, TH1D* theMCSig) { 
   theHist->Reset();   
   if (addCut2 =="")  
      addCut2 = addCut1;
   if (variable2 =="")
      variable2 = variable1;

   cout << "addCut2 = " << addCut2.GetTitle() << endl;
   cout << "variable2 = "<< variable2 << endl;

   TString fname1 = "rootFiles/barrelHiForestPhoton35_Skim2011-Dec04-withTracks.root";//barrelHiForestPhoton35_Skim2011-v2.root";
   TFile *f1  =new TFile(fname1.Data());
   TTree *photon1 = (TTree*)f1->Get("yongsunPhotonTree");
   photon1->AddFriend("yEvt=yongsunHiEvt"    ,fname1.Data());
   photon1->AddFriend("yskim=yongsunSkimTree"   ,fname1.Data());
   photon1->AddFriend("yhlt=yongsunHltTree"     ,fname1.Data());

   TString fname2 = "rootFiles/barrelHiForestPhoton35_Skim2010.root";
   TFile *f2  =new TFile(fname2.Data());
   TTree *photon2 = (TTree*)f2->Get("yongsunPhotonTree");
   //  photon2->AddFriend("yEvt=yongsunHiEvt"    ,fname2.Data());
   photon2->AddFriend("yskim=yongsunSkimTree"   ,fname2.Data());
   photon2->AddFriend("yhlt=yongsunHltTree"     ,fname2.Data());
   
   TString fnameMC ;
   TFile *fMC;
   TTree *photonSig;
   if ( theMCSig){ 
      fnameMC = "rootFiles/barrelHiForestPhoton35_MCPhoton50_25k.root";
      fMC  =new TFile(fnameMC.Data());
      photonSig = (TTree*)fMC->Get("yongsunPhotonTree");
      photonSig->AddFriend("yEvt=yongsunHiEvt"    ,fnameMC.Data());
      cout << " MC is running " << endl;
   }
   //   photonSig->AddFriend("yskim=yongsunSkimTree"   ,fnameMC.Data());
   //  photonSig->AddFriend("yhlt=yongsunHltTree"     ,fnameMC.Data());
   
   //genMatchCut1 
   TCut collisionCut = "yskim.pcollisionEventSelection==1";
   
   TCut runCut1       = ""; //"yhlt.Run>= 181985 && yhlt.Run <=182099"; 
   
   TCut centCut1     = Form("(yEvt.hiBin >= %d) && (yEvt.hiBin<= %d)",lowCent,highCent);
   TCut centCut2     = Form("(cBin >= %d) && (cBin <= %d)",lowCent,highCent);
   
   TCut spikeCut1     = "abs(seedTime)<4 && swissCrx<0.90 && sigmaIetaIeta>0.002";
   TCut spikeCut2   = "abs(seedTime)<4 && (1 - (eRight+eLeft+eTop+eBottom)/eMax)<0.90 && sigmaIetaIeta>0.002";
   TCut spikeCutSig = "swissCrx<0.90 && sigmaIetaIeta>0.002";

   TCut nonElectron = "!isEle";
   
   TCut hltCut1       = "yhlt.HLT_HISinglePhoton30_v2==1"; // 181695 => H/E cut is removed from L1
   TCut hltCut2       = "yhlt.HLT_HIPhoton30==1";
      
   TCut basicCut1       = collisionCut && nonElectron && etaCut && spikeCut1 && centCut1 && runCut1 && hltCut1;
   TCut basicCut2       = collisionCut && nonElectron && etaCut && spikeCut2 && centCut2; //&& hltCut2;
   TCut basicCutSig     =                 nonElectron && etaCut && spikeCutSig && centCut1;
   
   
   TCut finalCut1     = basicCut1   && addCut1;
   TCut finalCut2     = basicCut2   && addCut2;
   TCut finalCutSig   = basicCutSig && addCut1 && genMatchCut1;
   
   TH1D* tempHist1 = (TH1D*)theHist->Clone("tempHist1");
   TH1D* tempHist2 = (TH1D*)theHist->Clone("tempHist2");
   TH1D* tempHistSig = (TH1D*)theHist->Clone("tempHistSig");
   
   
   photon1->Draw(Form("%s>>%s",variable1.Data(),tempHist1->GetName()),finalCut1);
   photon2->Draw(Form("%s>>%s",variable2.Data(),tempHist2->GetName()),finalCut2);
   if ( theMCSig) {
      photonSig->Draw(Form("%s>>%s",variable1.Data(),tempHistSig->GetName()),  Form("(%s)*ncoll",finalCutSig.GetTitle()));
      cout << " mc cut = " << finalCutSig.GetTitle() << endl;
   }

   tempHist1->Sumw2();
   tempHist2->Sumw2();
   if ( theMCSig)
      tempHistSig->Sumw2();
   
   handsomeTH1(tempHist1,2);
   handsomeTH1(tempHist2,1);
   tempHist2->SetMarkerStyle(24);
   if ( normChoice == 0) {
      scaleInt(tempHist1);
      scaleInt(tempHist2);
   }
   if ( normChoice == 1) {
      double nEvents1 = photon1->GetEntries("leading==1" && basicCut1 && "pt>40 && hadronicOverEm<0.2");
      double nEvents2 = photon2->GetEntries("leading==1" && basicCut2 && "pt>40 && hadronicOverEm<0.2");
      tempHist1->Scale(1./nEvents1);
      tempHist2->Scale(1./nEvents2);
      cout <<" this file is<<" << nEvents1/nEvents2 * 6.7 << "micro barn" << endl;
   }
   if ( normChoice == 2) {
      double nLumi1 = 17.4   ; //microb^-1
      double nLumi2 = 7.0  ;
      tempHist1->Scale(1./nLumi1);
      tempHist2->Scale(1./nLumi2);
   }
   
   cleverRange(tempHist2,1.5);
   if ( overDraw) 
      tempHist2->DrawCopy("same");//hist");
   else 
      tempHist2->DrawCopy("");//hist");                                                                                                  

   tempHist1->DrawCopy("same");
   
   TLegend* leg1 =  new TLegend(0.4012097,0.7584746,1,0.9300847,NULL,"brNDC");
   easyLeg(leg1,"");
   leg1->AddEntry(tempHist2,"2010 PbPb, 6.8 #mub^{-1}","pl");
   leg1->AddEntry(tempHist1,"2011 PbPb, 84  #mub^{-1}","pl");
   leg1->Draw();
   theHist->Reset();
   theHist->Add(tempHist1);
   if ( theMCSig)
      theMCSig->Add(tempHistSig);
   
}



/*  
   // TH1D* den         = new TH1D("den1",";Leading Photon E_{T} (GeV);",3,0,100);
   TH1D* den = (TH1D*)heff->Clone("den321");  
   TH1D* num = (TH1D*)heff->Clone("num321");
   
   photon1->Draw("pt>>den321",generalCut);
   photon1->Draw("pt>>num321",generalCut&&trigCut);                                                                                                                                                     
   geff->BayesDivide(num,den);
   
   handsomeTH1(num,2);
   den->DrawCopy();
   num->DrawCopy("same");
   num->Divide(den);
   //   den->DrawCopy();
   // photon1->Draw("pt>>num",generalCut&&trigCut);
   //  num->Divide(den);
   
   
   for ( int i = 1 ; i <= heff->GetNbinsX() ; i++) {
      heff->SetBinContent( i  ,  num->GetBinContent(i)   );
      //heff->SetBinError  ( i  ,  num->GetBinError  (i)   );
   }
   
   //   heff->Divide(den);
  
}
*/
