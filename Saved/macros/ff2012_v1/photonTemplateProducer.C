#include "CutAndBinCollection2011.h"
#include "photonIDEfficiency.C"
const int kData =  0;
const int kSig  =  1;
const int kSBB  =  2;
const int kMCBsr = 3;
const int kMCBsb = 4;

const int kSBBpp = 5;


const int kSumIso = 1;
const int k3dIso =  2;
const int kFisher = 3;
const int kNoIso  = 4;
const int kSumIso2= 5;
const int kSumIso3= 6;


const int kHI = 1;
const int kPP = 2;

char ndcOpt[1000] = "NDC";


TString fNameHIdata = "skimmed/yskim_HiForestPhoton-v7-noDuplicate_nMix10_NoEvtPlReq_v4Mixed.root"; //"barrelHiForestPhotonV7_mixedJetsWihtquarterPI_reactionPln.root";
TString fNamePPdata = "barrelHiForestPhotonPP7TeV-v3.root";

void getTemplate(int ppHI = kHI, TH1D* h1=0, TString fname1="ss", int isoChoice =kSumIso, float isoCut=-100, int iTemp=kData, int lowCent=0, int highCent =3, TCut addCut="",bool onlygjEvents=true, float specialSbCut=10, float theShift=0);

TCut getIsoCut( int isoChoice=0, float isoCut = -100 ) {
  
   if ( (isoChoice== kSumIso2) && ( isoCut > -99))
      return Form("(cc4+cr4+ct4PtCut20)/0.9 < %f",isoCut);
   if         (isoChoice == kSumIso) return isoSumCut;
   if         (isoChoice == kSumIso3) return isoSumCut;
   //  else    if (isoChoice == k3dIso ) return iso3dCut;
   //  else    if (isoChoice == kFisher) return FisherCut;
   else    if (isoChoice == kNoIso) return "";
   else  { 
     cout << "!!!!!!! No such isolation choice" << endl;
     return "no iso selection";
   }
}
TString getIsoLabel ( int isoChoice=0) {
   if ( isoChoice == kSumIso) return "sumIso";
   if ( isoChoice == k3dIso)  return "3dIso";
   if ( isoChoice == kFisher) return "fisherIso";
   if ( isoChoice == kSumIso2) return "sumIso";
   if ( isoChoice == kSumIso3) return "sumIso";

   else   cout << "!!!!!!! No such isolation choice" << endl;
   return "no iso selection";
}
void photonTemplateProducer(int cutOpt=3, int ppHI = kHI, int isoChoice = kSumIso, int isoCut = -100, bool onlygjEvents=false, float specialSbCut=10, float mcSigShift=0, float sbBkgShift=0) {
  
  CutAndBinCollection cab;
  cab.setCuts(cutOpt);
  int nPtBin = cab.getNPtBin();
  vector<double> ptBinVector = cab.getPtBin();
  double ptBin[100];
  for ( int i = 0 ; i<=nPtBin ;i++) {
    ptBin[i] = ptBinVector[i];
  }
  
  int nCentBin = cab.getNCentBin();
  vector<double> centBinVector = cab.getCentBin();
  double centBin[100];
  for ( int i = 0 ; i<=nCentBin ;i++) {
    centBin[i] = centBinVector[i];
  }
  
  TString pphiLabel = "";
  if ( ppHI == kPP) pphiLabel = "pp";
  
  TCanvas* c1[5];
    
   TH1D* hData[5][5];
   TH1D* hSig[5][5];
   TH1D* hBkg[5][5];
   
   //   TH1D* hDatapp[5];
   //   TH1D* hSigpp[5];
   //   TH1D* hBkgpp[5];
   
   TH1D* hBkgMCsr[5][5];
   TH1D* hBkgMCsb[5][5];
      
   TH1D* rawSpectra[5];
   TH1D* finSpectra[5];
    
   TH2D* hPurity2D = new TH2D("hPurity2D",";pT(GeV);Centrality bin",nPtBin,ptBin,nCentBin,centBin);
   
   int nCent(-1);
   if ( ppHI == kHI ) nCent = nCentBin;
   if ( ppHI == kPP ) nCent = 1;
   
   for ( int icent = 1 ; icent<=nCent ; icent++) {
      rawSpectra[icent] = new TH1D(Form("rawSpec_icent%d_%s",icent,getIsoLabel(isoChoice).Data()),"",nPtBin,ptBin);
   }

   
   for (int ipt = 1; ipt <= nPtBin ; ipt++) { 
      c1[ipt] = new TCanvas(Form("c1_ipt%d",ipt),"",700,700);
      if ( ppHI == kHI )  makeMultiPanelCanvas(c1[ipt],nCent/2,2,0.0,0.0,0.2,0.15,0.02);

      TCut ptCut = Form("corrPt>%.2f && corrPt<%.2f",(float)ptBin[ipt-1],(float)ptBin[ipt]); 
      

      for ( int icent = 1 ; icent<=nCent ; icent++) { 
	 int lowCent =  centBinVector[icent-1];    
	 int highCent = centBinVector[icent]-1;
	 
	 hData[icent][ipt]  = new TH1D(Form("hData_cent%d_pt%d",icent,ipt),";shower shape (#sigma_{#eta#eta});Entries per photon candidate;",25,0,0.025);
	 hSig[icent][ipt]   = (TH1D*)hData[icent][ipt]->Clone(Form("hSig_cent%d_pt%d",icent,ipt));
	 hBkg[icent][ipt]   = (TH1D*)hData[icent][ipt]->Clone(Form("hBkg_cent%d_pt%d",icent,ipt));
	 hBkgMCsr[icent][ipt] = (TH1D*)hData[icent][ipt]->Clone(Form("hBkgMCsr_cent%d_pt%d",icent,ipt));
	 hBkgMCsb[icent][ipt] =(TH1D*)hData[icent][ipt]->Clone(Form("hBkgMCsb_cent%d_pt%d",icent,ipt));
	
	 TString fNamedata = fNameHIdata;
	 if ( ppHI == kPP ) fNamedata = fNamePPdata;
	 
	 getTemplate(ppHI, hSig[icent][ipt],"meaningless",isoChoice,isoCut, kSig,lowCent,highCent,ptCut,onlygjEvents,specialSbCut,mcSigShift);
	 getTemplate(ppHI, hData[icent][ipt],fNamedata ,isoChoice,isoCut, kData,lowCent,highCent,ptCut,onlygjEvents,specialSbCut);
	 if ( ppHI == kHI) {
	   getTemplate(ppHI, hBkg[icent][ipt], fNamedata ,isoChoice,isoCut, kSBB,lowCent,highCent,ptCut,onlygjEvents,specialSbCut,sbBkgShift);
	 }
	 if ( ppHI == kPP)
           getTemplate(ppHI, hBkg[icent][ipt], fNamedata ,isoChoice,isoCut, kSBBpp,lowCent,highCent,ptCut,onlygjEvents,specialSbCut);
	 
      }
            
      
      for ( int icent = 1 ; icent<=nCent ; icent++) {
	int lowerCent = centBinVector[icent-1];
	int upperCent =centBinVector[icent]-1;
	 
	 c1[ipt]->cd(nCent - icent+1);
	 fitResult fitr = doFit ( hSig[icent][ipt], hBkg[icent][ipt], hData[icent][ipt], 0.005,0.025);
	 if ( icent== nCent) drawPatch(0,0,0.05,0.14,0,1001, "ndc");
	 
	 cout << " shift = " << mcSigShift << endl;
	 cout << " purity = " << fitr.purity010 << endl;

	 
	 if ( ptBin[ipt]> 200)
	    drawText(Form(" E_{T}^{#gamma} > %d GeV", (int)ptBin[ipt-1]),0.5680963,0.529118);
	 else
	    drawText(Form("%d - %d GeV", (int)ptBin[ipt-1], (int)ptBin[ipt]),0.5680963,0.529118);
	 
	 if ( ppHI == kHI) {
	   drawText(Form("%.0f%% - %.0f%%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.5680963,0.4369118);
	 }
	 else if ( ppHI == kPP) {
	   drawText("7TeV pp",0.5680963,0.4369118);
         }
	 
	 
	 
	 if ( (icent == nCent) || (icent == 2)) 
	   drawText(Form("Purity(#sigma_{#eta#eta} < 0.01) : %.0f%%", (float)fitr.purity010*100),0.5680963,0.3569118,1,15);
	 else 
	   drawText(Form("Purity(#sigma_{#eta#eta} < 0.01) : %.0f%%", (float)fitr.purity010*100),0.4980963,0.3569118,1,15);
	 
	 drawText(Form("#pm %.0f%% (stat)", float( 100. * fitr.purity010 * (float)fitr.nSigErr / (float)fitr.nSig ) ),0.6680963,0.2869118,1,15);
	 
	 
	 hPurity2D->SetBinContent(ipt,icent,fitr.purity010);
	 hPurity2D->SetBinError  (ipt,icent,fitr.purity010* fitr.nSigErr/fitr.nSig);

	 rawSpectra[icent]->SetBinContent( ipt, fitr.nSig);
	 rawSpectra[icent]->SetBinError(   ipt,fitr.nSigErr);

	 TString aa = "";
	 if (isoChoice == kSumIso)  aa = "Sum Iso Method";
	 if (isoChoice == k3dIso)   aa = "3d Cut Method";
	 if (isoChoice == kFisher)  aa = "Fisher Method";
	 
	 if ( (ppHI == kHI)  && ( icent==nCent -1) )
	   drawText(aa.Data(),0.1980963,0.8569118,1,20);
	 else if ( ppHI == kPP) 
	   drawText(aa.Data(),0.1980963,0.8569118,1,20);
	 
	 if ( icent<= 2) drawPatch(0,0,0.05,0.14,0,1001, ndcOpt);
	 //	 drawPatch(0.9,0.05,1.01,0.14,0,1001,ndcOpt);
	 if  ( (ppHI == kPP) && ( mcSigShift != 0 )) 
           drawText(Form("Signal template shifted by %f",mcSigShift),0.1980963,0.7569118,1,15);
	 if  ( (ppHI == kHI) && ( mcSigShift != 0 ) && (icent==3))
	   drawText(Form("Signal template shifted by %f",mcSigShift),0.1980963,0.7569118,1,15);


      }   
      
      TString ppLabel ="";  
      if ( ppHI == kPP) ppLabel = "_pp";
      TString shiftLabel="";
      if ( mcSigShift != 0 )  shiftLabel = Form("_%fSigShifted",(float)mcSigShift);
      c1[ipt]->SaveAs(Form("fittingPurity%s_%s_pt%d%s.pdf",ppLabel.Data(), getIsoLabel(isoChoice).Data(),ipt,shiftLabel.Data()));

   }
   
   // efficiency plots          
   TCanvas* c2  = new TCanvas("c2","",700,700); //100 + nCent_std*300,400);
   if ( ppHI == kHI) makeMultiPanelCanvas(c2,nCent/2,2,0.0,0.0,0.2,0.15,0.02);
   
   TH1D* heff[7][5];
   TH1D* heffGj[5];
   TH1D* heffDphi[5];
   TH1D* effSingleBin = new TH1D("effSingleBin","",1,60,100000);
   TGraphAsymmErrors* geff[7][5];
   TGraphAsymmErrors* gSingleBin = new TGraphAsymmErrors();
   TGraphAsymmErrors* geffGj[5];
   TGraphAsymmErrors* geffDphi[5];
   
   for (int icent = 1; icent <=nCent; icent++) {
     for ( int iid=1 ; iid<=5; iid++) {
       heff[icent][iid] = new TH1D(Form("heff_icent%d_id%d",icent,iid),";photon E_{T} (GeV);Efficiency",nPtBin, ptBin);
       if ( isoChoice == kSumIso2)
	 heff[icent][iid]->SetName(Form("heff_icent%d_id%d_isoCut%d",icent,iid,(int)isoCut));
       if ( isoChoice == kSumIso3)
	 heff[icent][iid]->SetName(Form("heff_icent%d_id%d_sbIsoCut%d",icent,iid,(int)specialSbCut));
       
       geff[icent][iid] = new TGraphAsymmErrors();
       geff[icent][iid]->SetName(Form("geff_%s",heff[icent][iid]->GetName()));
     }
     heffGj[icent]   = new TH1D(Form("heff_icent%d_Gj",  icent),";x_J#gamma;Efficiency",10,0,2);
     heffDphi[icent] = new TH1D(Form("heff_icent%d_Dphi",icent),";#Delta#phi of pair;Efficiency",9,0.3141592,3.141592);
     geffGj[icent] = new TGraphAsymmErrors();
     geffDphi[icent] = new TGraphAsymmErrors();

   }
   
   TCut srIsoCut = getIsoCut(isoChoice,isoCut);
   int nId=4;
   for (int icent = 1; icent <=nCent ; icent++) {
     int lowCent = centBinVector[icent-1];
     int highCent = centBinVector[icent]-1;
     TCut centCut      = Form("yEvt.hiBin >= %d && yEvt.hiBin<= %d",lowCent,highCent);
     if ( ppHI == kPP ) centCut = "";
     getEff("genMatchedPt",heff[icent][1],geff[icent][1],centCut, "swissCrx<0.90 && seedTime<4");
     getEff("genMatchedPt",heff[icent][2],geff[icent][2],centCut, "swissCrx<0.90 && seedTime<4 && hadronicOverEm<0.1");
     getEff("genMatchedPt",heff[icent][3],geff[icent][3],centCut, "swissCrx<0.90 && seedTime<4 && hadronicOverEm<0.1" && srIsoCut);
     getEff("genMatchedPt",heff[icent][4],geff[icent][4],centCut, "swissCrx<0.90 && seedTime<4 && hadronicOverEm<0.1 && sigmaIetaIeta<0.010"&& srIsoCut);
     effSingleBin->Reset();
     getEff("genMatchedPt",effSingleBin, gSingleBin, centCut, "swissCrx<0.90 && seedTime<4 && hadronicOverEm<0.1" && srIsoCut);
     
     
     cout << " here Gj" << endl;
     getEff("yJet.jtpt/photonEt",heffGj[icent],geffGj[icent], centCut && " genIso<5 && abs(genMomId)<=22 && photonEt>60 && abs(yJet.jteta)<2 && yJet.jtpt>30 && acos(cos(photonPhi-yJet.jtphi))>2.749", "hovere<0.1 && sigmaIetaIeta<0.010 && (cc4+cr4+ct4PtCut20)/0.9<1",true);
     getEff("acos(cos(photonPhi-yJet.jtphi))",heffDphi[icent],geffDphi[icent],centCut && " genIso<5 && abs(genMomId)<=22 && photonEt>60 && abs(yJet.jteta)<2 && yJet.jtpt>30", "hovere<0.1 && sigmaIetaIeta<0.010 && (cc4+cr4+ct4PtCut20)/0.9<1",true);
   }
   
   for (int icent = 1; icent <=nCent; icent++) {
     for ( int iid=1 ; iid<=nId ; iid++) {
       handsomeTH1(heff[icent][iid],ycolor[iid]);
       handsomeTGraph(geff[icent][iid],ycolor[iid]);
     }
   }
   

   
   
   
   TH1D* htmp = (TH1D*)heff[1][1]->Clone("htmp");
   htmp->Reset();
   htmp->SetAxisRange(0,1.3,"Y");
   htmp->SetYTitle("Efficiency");
   handsomeTH1(htmp);
   
   for (int icent = 1; icent <=nCent; icent++) {
      int lowerCent = centBinVector[icent-1];
      int upperCent = centBinVector[icent]-1;
      if ( ppHI == kHI) c2->cd(nCent - icent + 1);
      htmp->DrawCopy();
      for ( int iid=1 ; iid<=nId ; iid++) {
         heff[icent][iid]->Draw("p same");
         geff[icent][iid]->Draw("p");
	 
      }
      if ( ( icent == nCent ) ||   ( ppHI == kPP)  )
         {
            TLegend* leg1 =  new TLegend(0.25,0.20,0.95,0.55,NULL,"brNDC");
            easyLeg(leg1,"Photon ID efficiency");
            leg1->AddEntry(heff[icent][1],"spike rejection","lp");
            leg1->AddEntry(heff[icent][2],"+ H/E < 0.1","lp");
	    if (isoChoice == kSumIso)
	      leg1->AddEntry(heff[icent][3],"+ SumIso cut","lp");
	    if (isoChoice == kFisher) 
	      leg1->AddEntry(heff[icent][3],"+ Fisher cut","lp");
	    leg1->AddEntry(heff[icent][4],"+ #sigma_{#eta#eta} <0.010","lp");
            leg1->Draw();
         }
      drawText(Form("%.0f%% - %.0f%%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),0.5680963,0.8369118);
      if ( icent<=2) drawPatch(0,0,0.05,0.14,0,1001,ndcOpt);
      //  drawPatch(0.9,0.05,1.01,0.14,0,1001,ndcOpt);
      
   }
   
   
   c2->SaveAs(Form("photonID_efficiency_%s.pdf",getIsoLabel(isoChoice).Data()));  

   TCanvas* c2b  = new TCanvas("c2b","",1000,500); //100 + nCent_std*300,400);                                             
   c2b->Divide(2,1);
   c2b->cd(1);
   TH1D* htmpG = (TH1D*)heffGj[1]->Clone("htmpG");
   htmpG->Reset();
   htmpG->SetAxisRange(0,1.3,"Y");
   htmpG->SetYTitle("Efficiency");
   handsomeTH1(htmpG);   
      
   TLegend* legCent = new TLegend(0.4657258,0.2245763,1,0.4512712,NULL,"brNDC");
   easyLeg(legCent,"Centrality");
   if (isoChoice == kSumIso)  easyLeg(legCent,"SumIso Method");
   if (isoChoice == kFisher)  easyLeg(legCent,"Fisher Method");
   cout<< " heffGj "<< endl << endl<< endl;
   for (int icent = 1; icent <=nCent; icent++) {
     handsomeTH1(heffGj[icent],ycolor[icent]);
     heffGj[icent]->Fit("pol1");
     heffGj[icent]->GetFunction("pol1")->SetLineColor(ycolor[icent]);
     heffGj[icent]->GetFunction("pol1")->SetLineStyle(7);
   }
   htmpG->DrawCopy();
   for (int icent = 1; icent <=nCent; icent++) {
     heffGj[icent]->Draw("same");
     int lowerCent = centBinVector[icent-1];     int upperCent = centBinVector[icent]-1;
     legCent->AddEntry(heffGj[icent],Form("%.0f%% - %.0f%%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),"pl");
   }
   legCent->Draw();

   c2b->cd(2);
   TH1D* htmpDp = new TH1D("htmpDp",";#Delta#phi of pair;Efficiency",10,0,3.14);
   htmpDp->Reset();
   htmpDp->SetAxisRange(0,1.3,"Y");
   htmpDp->SetYTitle("Efficiency");
   handsomeTH1(htmpDp);
   cout << " heffDphi " << endl << endl << endl ;
   for (int icent = 1; icent <=nCent; icent++) {
     handsomeTH1(heffDphi[icent],ycolor[icent]);
     heffDphi[icent]->Fit("pol1");
     heffDphi[icent]->GetFunction("pol1")->SetLineColor(ycolor[icent]);
     heffDphi[icent]->GetFunction("pol1")->SetLineStyle(7);
   }
   htmpDp->DrawCopy();
   for (int icent = 1; icent <=nCent; icent++) {
     heffDphi[icent]->Draw("same");
   }
   legCent->Draw();
   
   c2b->SaveAs(Form("photonID_efficiency_%s_2.pdf",getIsoLabel(isoChoice).Data()));
   
   
   TCanvas* c3 = new TCanvas("cPurity","",500,500);

   TH1D* hPurity[10];
   for (int icent = 1; icent <=nCent; icent++) {
     hPurity[icent] = (TH1D*)hPurity2D->ProjectionX(Form("purity1D_icent%d",icent),icent,icent);
     hPurity[icent]->Fit("pol1");
     hPurity[icent]->GetFunction("pol1")->SetLineColor(ycolor[icent]);
     hPurity[icent]->GetFunction("pol1")->SetLineStyle(7);
   }
   TH1D* tempPurity = (TH1D*)hPurity[1]->Clone("purityTemp");
   tempPurity->Reset();
   handsomeTH1(tempPurity,1);
   tempPurity->SetXTitle("pT (Gev)");
   tempPurity->SetYTitle("Purity");
   tempPurity->SetAxisRange(0.45,1.2,"Y");

   tempPurity->Draw();
   for (int icent = 1; icent <=nCent; icent++) {
     handsomeTH1(hPurity[icent],ycolor[icent]);
     hPurity[icent]->Draw("same");
   }
   TLegend* legPurity = new TLegend(0.4657258,0.2245763,1,0.4512712,NULL,"brNDC");
   easyLeg(legPurity,"Purity");
   if (isoChoice == kSumIso)  easyLeg(legPurity,"SumIso Method");
   if (isoChoice == kFisher)  easyLeg(legPurity,"Fisher Method");
   for (int icent = 1; icent <=nCent; icent++){
     int lowerCent = centBinVector[icent-1];     int upperCent = centBinVector[icent]-1;
     legPurity->AddEntry(hPurity[icent],Form("%.0f%% - %.0f%%", float((float)lowerCent*2.5), float((float)(upperCent+1)*2.5)),"pl");
   }
   legPurity->Draw();
   
   if ( !onlygjEvents) 
     drawText("inclusive photon",0.25,0.2);
   
   c3->SaveAs(Form("purity_%s.pdf",getIsoLabel(isoChoice).Data()));
  
   
   TCanvas* c4 = new TCanvas("efficiencyCorrection","",1000,500);
   c4->Divide(2,1);
   c4->cd(1);
   for (int icent = 1; icent <=nCent; icent++) {
      TH1ScaleByWidth(rawSpectra[icent]);    // divide by width
      finSpectra[icent] = (TH1D*)rawSpectra[icent]->Clone(Form("finSpec_icent%d_%s",icent,getIsoLabel(isoChoice).Data()));
      if ( isoChoice == kSumIso2)
	 finSpectra[icent]->SetName(Form("finSpec_icent%d_%s_isoCut%d",icent,getIsoLabel(isoChoice).Data(),(int)isoCut));
      if ( isoChoice == kSumIso3)
         finSpectra[icent]->SetName(Form("finSpec_icent%d_%s_sbisoCut%d",icent,getIsoLabel(isoChoice).Data(),(int)specialSbCut));
      
      finSpectra[icent]->Divide(heff[icent][3]);
      handsomeTH1(finSpectra[icent],ycolor[icent]);
   }
   // TAA and centrality 
   //

   TFile outf = TFile(cab.getPurityFileName(),"recreate");
   hPurity2D->Write();
   for ( int icent=1 ; icent<=nCent ; icent++) {
      heff[icent][3]->Write();
      heff[icent][4]->Write();
      finSpectra[icent]->Write();
      hPurity[icent]->Write();
      for (int ipt = 1; ipt <= nPtBin ; ipt++) {
	hData[icent][ipt]->Write();
      }
      //      hBkgMCRatioFit[icent][1]->Write();

   }



   outf.Close();
}

void getTemplate(int ppHI, TH1D* h1, TString fname1, int isoChoice, float isoCut, int iTemp, int lowCent, int highCent, TCut addCut,bool onlygjEvents, float specialSbCut, float theShift) { 
 
  char fnamePho30[500] = "skimmed/yskim_qcdAllPhoton30_allCent_nMix10_v4.root";
  float nEvtPho30    =  45000;
  char fnamePho50[500] = "skimmed/yskim_qcdAllPhoton50_allCent_nMix10_v4.root";
  float nEvtPho50     = 45188;
  char fnamePho80[500] = "skimmed/barrelHiforestv2_qcdAllPhoton80_allCent.root";
  float nEvtPho80     = 45308;
    
  char fnameEmj80[500] = "barrelHiForestPhoton_MCemJet80_41007events.root";
  char fnameEmj120[500] = "barrelHiForestPhoton_MCemJet120_25308events.root";
  char fnameEmj80_cent10[500] = "barrelHiForestPhoton_emJet80_cent10_10016evnts.root";
  char fnameEmj120_cent10[500] = "barrelHiForestPhoton_emJet120_cent10_9540Evts.root";
  
  
  char fnamePho30pp[500] = "mcbarrelHiForestPhoton_AllQCDPhoton30_PP7TeV.root";

   
   double csDij80 = 9.869e-5;
   float nEvtEmj80     = 41007;
   float effEmj80     = 0.204;
   float weightEmj80 = csDij80*effEmj80/nEvtEmj80;
   float nEvtEmj120     = 25308;
   double csDij120 = 1.127e-5;
   float effEmj120     = 0.54;
   float weightEmj120 = csDij120*effEmj120/nEvtEmj120;
   
   float nEvtEmj80_cent10      = 9540;
   float nEvtEmj120_cent10     = 10016;
   float weightEmj80_cent10 = csDij80*effEmj80/nEvtEmj80_cent10;
   float weightEmj120_cent10 = csDij120*effEmj120/nEvtEmj120_cent10;
   
   
   TCut evtSelCut = "tgj.anaEvtSel";
   if ( ppHI ==kPP ) evtSelCut = "";


   TCut centCut     = Form("yEvt.hiBin >= %d && yEvt.hiBin<= %d",lowCent,highCent);
   if ( ppHI ==kPP ) centCut = "";
   
   TCut noEleCut   = "isEle ==0";
   
   TCut photonJetCut  = "tgj.photonEt>50  &&  tgj.jetEt>30";
   TCut dphiCut= "acos(cos(tgj.photonPhi-tgj.jetPhi))>2.748893";
   TCut lPhotCut= "leading==1";
   TCut hoeCut ="hadronicOverEm<0.1";
   TCut islandButCut = " seedTime != 0";
   TCut generalCutMC   = lPhotCut && centCut && noEleCut && hoeCut && addCut &&islandButCut;
   if (onlygjEvents)
      generalCutMC = generalCutMC && photonJetCut && dphiCut;
    
   TCut generalCutData = generalCutMC && evtSelCut;
   if (onlygjEvents)
     generalCutData = generalCutData && photonJetCut && dphiCut;
   

   TCut srIsoCut = getIsoCut(isoChoice,isoCut);

   TCut finalCut="";
   if ( iTemp == kData )
     finalCut = generalCutData && srIsoCut ;
   else if ( iTemp == kSig )
     finalCut = generalCutMC && genMatchCut1  && srIsoCut ;
   else if ( iTemp == kSBB )
     finalCut = generalCutData && sbIsoCut;
   else if ( iTemp == kMCBsr )
     finalCut = generalCutMC && srIsoCut;
   else if ( iTemp == kMCBsb )
     finalCut = generalCutMC && sbIsoCut;
   else if ( iTemp == kSBBpp )
     finalCut = generalCutData && sbIsoPPCut;

   if ( (isoChoice == kSumIso3) && ( iTemp == kSBB )) {
     finalCut = generalCutMC && (TCut)(Form(" (cc4+cr4+ct4PtCut20)/0.9 > %f &&  (cc4+cr4+ct4PtCut20)/0.9 < %f",(float)specialSbCut,(float)(specialSbCut+10.))) ;
      if ( specialSbCut == -1 ) 
	finalCut = generalCutMC && "(cc4+cr4+ct4PtCut20)/0.9 > 10 &&  (cc4+cr4+ct4PtCut20)/0.9 < 20";
      //cout << " special cut : " << Form("( (cc4+cr4+ct4PtCut20)/0.9 > %f) && ( (cc4+cr4+ct4PtCut20)/0.9 < %f)",(float)specialSbCut,(float)(specialSbCut+10)) << " is used " << endl;
   }
   multiTreeUtil* photon1 = new multiTreeUtil();
   
   float csPho30 = 1.59e-6;
   float csPho50 = 7.67e-7;
   float csPho80 = 1.72e-7;
   float weightPho50 = csPho50/nEvtPho50;
   float weightPho80 = csPho80/nEvtPho80;
   float weightPho30 = csPho30/nEvtPho30;

   TString weightBit = "";
   if ( ppHI == kHI) {
     if ( iTemp  == kSig) {
       photon1->addFile( fnamePho30,  "yongsunPhotonTree", "ptHat>30 && ptHat<50" , weightPho30- weightPho50);
       photon1->addFile( fnamePho50,  "yongsunPhotonTree", "ptHat>50 && ptHat<80" , weightPho50 - weightPho80);
       photon1->addFile( fnamePho80,  "yongsunPhotonTree", "ptHat<80" , weightPho80);
       weightBit = "tgj.reweight";
     }
     else if ( (iTemp == kSBB) || (iTemp == kData) )  {
       // if this is data
       photon1->addFile( fname1 , "yongsunPhotonTree", "" ,1);
       weightBit = "";
     }
     
     else if ( (iTemp == kMCBsr) || (iTemp == kMCBsb)){
       photon1->addFile(fnameEmj80,  "yongsunPhotonTree", "" , weightEmj80);
       photon1->addFile(fnameEmj120, "yongsunPhotonTree", "" , weightEmj120);
       photon1->addFile(fnameEmj80_cent10,  "yongsunPhotonTree", "yEvt.hiBin<4" , weightEmj80_cent10);
       photon1->addFile(fnameEmj120_cent10,  "yongsunPhotonTree", "yEvt.hiBin<4" , weightEmj120_cent10);
       weightBit = "tgj.reweight";
     }
   }
   if ( ppHI == kPP) {
     if ( iTemp  == kSig) {
       photon1->addFile( fnamePho30pp,  "yongsunPhotonTree", "" , weightPho30);
       weightBit = "";
     }
     else if ( (iTemp == kSBB) || (iTemp == kData) || (iTemp == kSBBpp) ) {
       photon1->addFile( fname1 , "yongsunPhotonTree", "" ,1);
       weightBit = "";
     }
   }
   
   photon1->AddFriend("yEvt=yongsunHiEvt");
   //   photon1->AddFriend("yJet=yongsunJetakPu3PF");
   photon1->AddFriend("tgj");
   
   
   h1->Reset();
   h1->Sumw2();
   TH1D* htemp = (TH1D*)h1->Clone("htemp");
   htemp->Reset();
   htemp->Sumw2();
   cout << "cut = " << finalCut.GetTitle() <<  endl;

   photon1->Draw2(htemp,  Form("sigmaIetaIeta+%f",theShift),   finalCut,  weightBit);
   cout << " this is it" << endl;
   //  cout << " MC final Cut = " << finalCut.GetTitle() <<endl;
   TH1D* hcBins = new TH1D("hncoll","",40,-.5,39.5);
   if ( iTemp == kData ) {
      cout << "Data entries = "<<  htemp->Integral() << endl;
      //   cout << "cut = " << finalCut.GetTitle() <<  endl;
      photon1->Draw2(hcBins,  "yEvt.hiBin",   finalCut);
   }
   
   h1->Add(htemp);
   htemp->Reset();
   
      
   delete photon1;
   
   
}
