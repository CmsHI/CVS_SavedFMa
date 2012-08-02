#include "basicSpectra.C"

void letsFit() {
   
   TH1D* hData[5][8];
   TH1D* hSig[5][8];
   TH1D* hBkg[5][8];
   TH1D* hEff[5];
   TH1D* rawSpec[5];
   TH1D* fSpec[5];
   TH1D* f2011[5];
   TFile* inf1 = new TFile("templates2011.root");
   for ( int icent=1 ; icent<=3 ; icent++) { 
      int lowCent = centBin_std[icent-1];
      int highCent = centBin_std[icent]-1;
      for (int ipt = 1; ipt<=nEtBin ;ipt++){
	 hData[icent][ipt] = (TH1D*)inf1->Get(Form("hsieieData_cent%dto%d_pt%dto%d",lowCent,highCent,(int)etBin[ipt-1],(int)etBin[ipt]));
	 hBkg[icent][ipt]  = (TH1D*)inf1->Get(Form( "hsieieSB_cent%dto%d_pt%dto%d",lowCent,highCent,(int)etBin[ipt-1],(int)etBin[ipt]));
	 hSig[icent][ipt]  = (TH1D*)inf1->Get(Form("hsieieSig_cent%dto%d_pt%dto%d",lowCent,highCent,(int)etBin[ipt-1],(int)etBin[ipt]));
      }
   }
   
   TCanvas* c2 = new TCanvas("c2","",1000,400);
   makeMultiPanelCanvas(c2,3,1,0.0,0.0,0.2,0.15,0.02);
   for ( int icent =1 ; icent<=3 ; icent++) {
      int lowCent = centBin_std[icent-1];
      int highCent = centBin_std[icent]-1;
      c2->cd(icent);
      for ( int ipt = 1 ; ipt<=nEtBin ; ipt++) {
	 double nSig, nSigErr, chisq,purity011;
	 
	 double nev =  doFit ( hSig[icent][ipt], hBkg[icent][ipt], hData[icent][ipt], nSig, nSigErr, 0,0.025, (icent==1),chisq,purity011);
	 if ( etBin[ipt] > 1000) 
	    drawText(Form("Photon E_{T} > %.0f GeV",(float)etBin[ipt-1], float(etBin[ipt])),0.5380963,0.4569118,1,15);
	 else 
	    drawText(Form("%.0f - %.0f GeV",(float)etBin[ipt-1], float(etBin[ipt])),0.5980963,0.4569118,1,15);
	 drawText(Form("Purity : %d %%",(int)(nev*100.)),0.5980963,0.3869118,1,15);
	 cout <<" nSig = " << nSig << endl;
	
	 drawText(Form("%.0f %% - %.0f %%", float((float)lowCent*2.5), float((float)(highCent+1)*2.5)),0.2380963,0.8769118,2,20);
	 
      }
   }
   c2->SaveAs(Form("fittingPlot.gif"));
   c2->SaveAs(Form("fittingPlot.eps"));
}

