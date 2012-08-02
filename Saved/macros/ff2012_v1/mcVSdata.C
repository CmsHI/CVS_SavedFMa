#include "CutAndBinCollection2011.h"

void drawVariable(int lowCent=0, int highCent=0, double lowPt=0, double highPt=0, TH1D* hvar=0, TString var="", TCut addCut="", bool doLog=false,bool onleMC=false, float maxY = 1.2 );
void drawVarAllCent (double lowPt=0, double highPt=0, TH1D* hvar=0, TString var="", TString varName="", TCut addCut="", bool doLog=false, TString subName="", bool onleMC=false , float maxY = 1.2);


void mcVSdata() { 
   TCut hoeCut = "hadronicOverEm<0.1";
   
   TH1D* hpt   = new TH1D("hpt",";E_{T} (GeV);Normalized Entries",10,60,300);
   TString var= "corrPt";
   drawVarAllCent(60,10000,hpt, var, "Transverse Energy", "hadronicOverEm<0.1 && sigmaIetaIeta<0.01",true);
      
   TH1D* heta   = new TH1D("heta",";#eta;Normalized Entries",6,-1.5,1.5);
   var= "eta";
   drawVarAllCent(60,10000,heta, var, "Pseudo Rapidity ", "",false,"",false, 0.4);
   
   TH1D* hphi   = new TH1D("hphi",";#phi;Normalized Entries",6,-3.2,3.2);
   var= "phi";
   drawVarAllCent(60,10000,hphi, var, "#phi  ", "",false,"",false, 0.4);
   
   TH1D* hiso   = new TH1D("hiso",";#SigmaISO (Gev) ;Normalized Entries",30,-30,80);
   var= "(cc4+cr4+ct4PtCut20)/0.9";
   drawVarAllCent(60,10000,hiso, var, "#SigmaISO  ", "",false,"sumISO",true, 0.5);
   
   TH1D* hsee   = new TH1D("hsee",";#sigma_{#eta#eta} ;Normalized Entries",25,0,0.025);
   var= "sigmaIetaIeta";
   drawVarAllCent(60,10000,hsee, var, "#sigma_{#eta#eta}","" ,false,"see",true, 1.2);
   
   TH1D* hecal   = new TH1D("heiso",";Ecal ISO (Gev) ;Normalized Entries",30,-30,80);
   var= "cc4/.9";
   drawVarAllCent(60,10000,hecal, var, "Ecal ISO  (H/E<0.2)", "",false,"ecalIso",true, 0.8);
   
   TH1D* hhcal   = new TH1D("hhiso",";Hcal ISO (Gev) ;Normalized Entries",30,-30,80);
   var= "cr4/.9";
   drawVarAllCent(60,10000,hhcal, var, "Hcal ISO", "",false,"hcalIso",true, 0.8);

   TH1D* htrack   = new TH1D("htrack",";Track ISO (Gev) ;Normalized Entries",30,-30,80);
   var= "ct4PtCut20/.9";
   drawVarAllCent(60,10000,htrack, var, "Track ISO ", "",false,"trackIso",true, 0.8);

   TH1D* hFisher  = new TH1D("hfisher",";Fisher Discriminant ;Event Fraction",60,-4,4);
   drawVarAllCent(60,10000,hFisher, fisherVar, "Fisher Discriminant", "",false,"fisherIso",true, 0.8);   
   
}

void drawVarAllCent (double, double highPt, TH1D* hvar, TString var,TString varName,  TCut addCut, bool doLog, TString subName,bool onleMC, float maxY ) {
   
   TCanvas* c2 = new TCanvas(Form("c2_%s%s",var.Data(),subName.Data()),"",1000,400);
   makeMultiPanelCanvas(c2,3,1,0.0,0.0,0.2,0.15,0.02);
   for ( int icent = 1 ; icent<=3 ; icent++) {
      int lowCent = centBin_std[icent-1];
      int highCent = centBin_std[icent]-1; 
      c2->cd(4-icent);
      drawVariable(lowCent,highCent,60,10000,hvar, var, addCut,doLog,onleMC, maxY);
      if (doLog )    
	 gPad->SetLogy();
      
      drawText(Form("%.0f %% - %.0f %%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),0.6580963,0.9069118,1,16);
      //  drawText("Photon E_{T} > 60GeV",0.3680963,0.6569118,1,15);
      if ( icent == 1 ) {
	 if (!onleMC) drawCMS2011(0.338,0.906,84,16);
      }
      if (icent==2) { 
	 TH1D* data = new TH1D("data11","",1,0,1);
	 data->Sumw2();
	 TH1D* mcSig = new TH1D("mcSig11","",1,0,1);
	 mcStyle(mcSig);
	 TH1D* mcBkg = new TH1D("mcBkg11","",1,0,1);
         dijetStyle(mcBkg);
	 TLegend* leg1 = new TLegend(0.1586395,0.7137837,0.9992263,0.9626509,NULL,"brNDC");
	 easyLeg(leg1,varName.Data());
	 if( !onleMC) leg1->AddEntry(data,"Data","pl");
	 leg1->AddEntry(mcSig,"MC #gamma-jet + HYDJET","fl");
	 leg1->AddEntry(mcBkg,"MC dijet Background","fl");
	 leg1->Draw();
      }
   
      drawPatch(0,0,0.05,0.14,0,1001,"NDC");
      drawPatch(0.93,0.05,1.01,0.14,0,1001,"NDC");

   }
   
   if (subName ==""){
      c2->SaveAs(Form("%s_MCvsDATA_allCent.pdf",var.Data()));
      c2->SaveAs(Form("%s_MCvsDATA_allCent.gif",var.Data()));
   }
   else {
      c2->SaveAs(Form("%s_MCvsDATA_allCent.pdf",subName.Data()));
      c2->SaveAs(Form("%s_MCvsDATA_allCent.gif",subName.Data()));
   }
   
   
}

void drawVariable(int lowCent, int highCent, double lowPt, double highPt, TH1D* hvar, TString var, TCut addCut, bool doLog ,bool onleMC, float maxY ) { 
   
   TCut ptCut  = Form("pt>%.f && pt <%.f",lowPt,highPt);
   TString ptCutS = Form("E_{T} : %.0f - %.0f GeV",lowPt,highPt);
   if ( highPt > 200 ) 
      ptCutS = Form("E_{T} > %.0f GeV",lowPt);
 
   
   char* fnamePho50 = "forest/barrelHiForestPhoton_MCphoton50_37k.root";
   float csPho50 = 6.663e-7;
   float nEvtPho50     = 51200;
   float weightPho50 = csPho50/nEvtPho50;
   
   char* fnamePho80 = "forest/barrelHiForestPhoton_MCphoton80_25k.root";
   float csPho80 = 8.731e-8;
   float nEvtPho80     = 26000;
   float weightPho80 = csPho80/nEvtPho80;
   
   char* fnameEmj80 = "forest/barrelHiForestPhoton_MCemJet80_41007events.root";
   double csDij80 = 9.869e-5;
   float nEvtEmj80     = 60000;
   float effEmj80     = 0.204;
   float weightEmj80 = csDij80*effEmj80/nEvtEmj80;
   
   char* fnameEmj120 = "forest/barrelHiForestPhoton_MCemJet120_25308events.root";;
   float nEvtEmj120     = 14000;
   double csDij120 = 1.127e-5;
   float effEmj120     = 0.54;
   float weightEmj120 = csDij120*effEmj120/nEvtEmj120;

   
   TString fname1 = "forest/barrelHiForestPhotonV6.root";
   TFile *f1  =new TFile(fname1.Data());
   TTree *tdata = (TTree*)f1->Get("yongsunPhotonTree");
   tdata->AddFriend("yEvt=yongsunHiEvt"    ,fname1.Data());
   tdata->AddFriend("yskim=yongsunSkimTree"   ,fname1.Data());
   tdata->AddFriend("yhlt=yongsunHltTree"     ,fname1.Data());
   tdata->AddFriend("tgj"     ,fname1.Data());
   
   multiTreeUtil* photon1 = new multiTreeUtil();
   photon1->addFile( fnamePho50,  "yongsunPhotonTree", "" , weightPho50);
   photon1->addFile( fnamePho80,  "yongsunPhotonTree", "" , weightPho80);
   photon1->AddFriend("yEvt=yongsunHiEvt");
   photon1->AddFriend("tgj");
   
   multiTreeUtil* bkgs1 = new multiTreeUtil();
   bkgs1->addFile( fnameEmj80,  "yongsunPhotonTree", "" , weightEmj80);
   bkgs1->addFile( fnameEmj120,  "yongsunPhotonTree", "" , weightEmj120);
   bkgs1->AddFriend("yEvt=yongsunHiEvt");
   bkgs1->AddFriend("tgj");
   
   TCut evtSelCut = "tgj.anaEvtSel";
   TCut centCut     = Form("(yEvt.hiBin >= %d) && (yEvt.hiBin<= %d)",lowCent,highCent);
   TCut photonJetCut  = "tgj.photonEt>60  &&  tgj.jetEt>30";
   TCut dphiCut= "acos(cos(tgj.photonPhi-tgj.jetPhi))>2.0944";
   TCut lPhotCut= "leading==1";
   TCut generalCutMC   = photonJetCut && dphiCut && lPhotCut && centCut && addCut;
   TCut generalCutData = generalCutMC && evtSelCut;

   //   TCut srIsoCut = FisherCut && "sigmaIetaIeta <0.010";
   
   TCut finalCutData = generalCutData;// && srIsoCut ;
   TCut finalCutPho  = generalCutMC;// && srIsoCut ;
   TCut finalCutEmj  = generalCutMC;// && srIsoCut ;
   
   TH1D* tempHist    = (TH1D*)hvar->Clone(Form("%s_data",hvar->GetName()));
   TH1D* tempHistMC1 = (TH1D*)hvar->Clone(Form("%s_MC1",hvar->GetName()));
   TH1D* tempHistMC2 = (TH1D*)hvar->Clone(Form("%s_MC2",hvar->GetName()));
   
   tdata->Draw(Form("%s>>%s",var.Data(),tempHist->GetName()), finalCutData);
   photon1->Draw2(tempHistMC1, var.Data(), finalCutPho,"ncoll");
   bkgs1->Draw2  (tempHistMC2, var.Data(), finalCutPho,"ncoll");
   
   tempHist->Sumw2();
   tempHistMC1->Sumw2();
   tempHistMC2->Sumw2();
   
   handsomeTH1(tempHist);
   handsomeTH1(tempHistMC1);
   handsomeTH1(tempHistMC2);
   mcStyle(tempHistMC1);
   dijetStyle(tempHistMC2);
   if (!onleMC)       tempHistMC1->Add(tempHistMC2);
      
   float theNorm = tempHistMC1->Integral();
   tempHistMC1->Scale(1./theNorm);
   if (onleMC) 
      tempHistMC2->Scale(1./tempHistMC2->Integral());
   else
      tempHistMC2->Scale(1./theNorm);

   tempHist->Scale(1./tempHist->Integral());
   
   if ( doLog) cleverRangeLog(tempHistMC1,10,1.e-5);
   else cleverRange(tempHistMC1,2);
   tempHistMC1->SetAxisRange(0.0005,maxY,"Y");
   tempHistMC1->DrawCopy("hist");//hist");                                                                                   
   TH1D* tempHistMC1Err = (TH1D*)tempHistMC1->Clone("temptemp");
   tempHistMC1Err->SetLineColor(2);
   tempHistMC1Err->SetMarkerColor(2);
   tempHistMC1Err->SetMarkerSize(0);
   tempHistMC1Err->Draw("same");
   tempHistMC2->DrawCopy("hist same");//hist");                                                                                                                            
   if ( !onleMC)
      tempHist->DrawCopy("same");//hist");                                                                                                                                                                                   
   
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
