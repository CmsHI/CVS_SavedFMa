#include "CutAndBinCollection2011.h"

int kNotConverted = 1;
int kConverted = 2;


struct scalePair {
   double val;
   double err;
   double absVal;
   double res;
   double resErr;
};

double resFun(double *v, double *par) {
   Double_t fitval = par[0] + par[1] / sqrt(v[0]) + par[2] / v[0] ;
   return fitval;
}
scalePair energyCorrectionDiff(TCut centCut1, float lowPt=40, float highPt=50, float lowEta=0, float highEta=0.5,TCut addCut="");
void energyCorrectionSet(TCut addCut="", TString addName="", bool drawRes=false) ;

void energyCorrection() { 
   TCanvas* c1 = new TCanvas("c1","",1000,500);
   c1->Divide(2,1);
   c1->cd(1);
   energyCorrectionSet("r9>0.94" , "r9gt94", false);
   drawText("r9  > 0.94",0.2580963,0.8869118,13);
   c1->cd(2);
   energyCorrectionSet("r9<0.94" , "r9lt94", false);
   drawText("r9  < 0.94",0.2580963,0.8869118,13);
   c1->SaveAs("photonEnergyCorrection.eps");
   c1->SaveAs("photonEnergyCorrection.gif");
   
   TCanvas* c2 = new TCanvas("c22","",500,500);
   energyCorrectionSet("" , "all", true);
   drawText("Energy resolution |#eta|<1.44",0.2580963,0.8869118,13);
   c2->SaveAs("photonEnergyRecolution.eps");
   c2->SaveAs("photonEnergyRecolution.gif");
}


void energyCorrectionSet(TCut addCut, TString addName, bool drawRes) { 
   const int nPtBin = 5;
   double ptBin[nPtBin+1] = {40, 60,80,100,140,200};
   TH1D* hscale[10];
   //    const int nLt = 4;                                                                                                               
   // double ltBin[nLt+1] = {0,10,20,30,40};   for ( int icent = 1 ; icent<=nCent_std ; icent++) {
   // for ( int icent = 1 ; icent<=nLt ; icent++) {
   
   
   for ( int icent = 1 ; icent<=nCent_std ; icent++) {                                                                                  
      int lowCent = centBin_std[icent-1];
      int highCent = centBin_std[icent]-1;
      TCut centCut1      = Form("yEvt.hiBin >= %d && yEvt.hiBin<= %d",lowCent,highCent);
      //int lowCent = ltBin[icent-1];
      //int highCent = ltBin[icent]-1;
      // TCut centCut1  = Form("locNtrk >= %d && locNtrk<= %d",lowCent,highCent);        
      hscale[icent] = new TH1D(Form("hscale_%s_%d",addName.Data(),icent),";pT(GeV);scale",nPtBin,ptBin);
      if (drawRes) 
	 hscale[icent]->SetName(Form("hres_%s_%d",addName.Data(),icent));
      for ( int ipt = 1 ; ipt<=nPtBin ; ipt++) { 
	 float lowPt = ptBin[ipt-1]; 
	 float highPt= ptBin[ipt];
	 scalePair theScale = energyCorrectionDiff( centCut1, lowPt, highPt,0,1.44,addCut);
	 if (drawRes){ 
	    hscale[icent]->SetBinContent(ipt, theScale.res);
	    hscale[icent]->SetBinError  (ipt, theScale.resErr);
	 }
	 else {
            hscale[icent]->SetBinContent(ipt, theScale.val);
            hscale[icent]->SetBinError  (ipt, theScale.err);
	 }
      }
      
   }  
   TH1D* htemp = (TH1D*)hscale[1]->Clone("htemp");
   htemp->SetTitle(";Photon E_{T}^{Gen} (GeV) ; E_{T}^{Reco} / E_{T}^{Gen}");
   handsomeTH1(htemp);
   htemp->SetAxisRange(0.9,1.2,"Y");
   if ( drawRes )
      htemp->SetAxisRange(0,0.1,"Y");

   TF1 *func[10];
   for ( int icent = 1 ; icent<=nCent_std ; icent++) {
      // for ( int icent = 1 ; icent<=nLt ; icent++) {
      handsomeTH1(hscale[icent],color[icent]);
      func[icent] = new TF1(Form("fit_%s",hscale[icent]->GetName()),resFun,10,1000,3);
      func[icent]->SetParameters(0.03, 0.2, 0.7 );
      func[icent]->SetParNames("Constants","power");
      hscale[icent]->Fit( func[icent]->GetName(),"LL M O Q");
      func[icent]->SetLineColor(color[icent]);
   }
   
   htemp->Draw();
   for ( int icent = 1 ; icent<=nCent_std ; icent++) {
      int lowCent = centBin_std[icent-1];
      int highCent = centBin_std[icent]-1;
      hscale[icent]->Draw("same");
      func[icent]->Draw("same");
   }
   TLegend* leg1 = new TLegend(0.25,0.65,0.95,0.95,NULL,"brNDC");
   easyLeg(leg1,"");
   leg1->AddEntry(hscale[1],"0 - 10%","lp");
   leg1->AddEntry(hscale[2],"10 - 30%","lp");
   leg1->AddEntry(hscale[3],"30 - 100%","lp");
   leg1->Draw();

   TFile outf = TFile("photonEnergyScaleTable.root","UPDATE");
   for ( int icent = 1 ; icent<=nCent_std ; icent++) {
      hscale[icent]->Write();
      func[icent]->Write();
   }
   outf.Close();
   
   
   
   
}

scalePair energyCorrectionDiff(TCut centCut1, float lowPt, float highPt, float lowEta, float highEta,TCut addCut) { 
   
   TString fname1 = "forest/barrelHiForestPhoton_MCphoton50_51k.root";
   if ( lowPt > 90 ) 
      fname1 = "forest/barrelHiForestPhoton_MCphoton80_28k.root";
   TFile *f1  =new TFile(fname1.Data());
   TTree *photon1 = (TTree*)f1->Get("yongsunPhotonTree");
   photon1->AddFriend("yEvt=yongsunHiEvt"    ,fname1.Data());
   photon1->AddFriend("ySkim=yongsunSkimTree"   ,fname1.Data());
   photon1->AddFriend("yHlt=yongsunHltTree"     ,fname1.Data());
   
   TCut collisionCut = "ySkim.pcollisionEventSelection==1";
   
   TCut hoeCut = "hadronicOverEm<0.2";
   TCut isoCut = "cc4 + cr4 + ct4j20 < 5 && sigmaIetaIeta<0.011";
   TCut ptCut  = Form("genMatchedPt>%.f && genMatchedPt <%.f",lowPt, highPt);
   TCut etaCut = Form("abs(eta)>%f && abs(eta)<%f",lowEta,highEta);
      
   TCut finalCut1 = genMatchCut1 && collisionCut && centCut1 && hoeCut && isoCut && ptCut && etaCut && addCut ; 
   TString variable1 = "pt/genMatchedPt";
   
   TH1D* hScale = new TH1D("hScale","",100,.5,1.5);
   TH1D* hdpt = new TH1D("hdpt","",100,-20,20);
   photon1->Draw(Form("%s>>%s",variable1.Data(),hScale->GetName()), finalCut1);
   photon1->Draw(Form("pt-genMatchedPt>>%s",hdpt->GetName()),       finalCut1);
   cout << "cut = " << finalCut1.GetTitle() <<endl;
   hScale->Draw();
   hdpt->Draw();
   TF1* ff =  cleverGaus(hScale);
   
   scalePair ret;
   double *ps = ff->GetParameters();
   ret.val = ps[1];
   ret.err = ff->GetParError(1);
   // resErr = ff->GetParError(2);
   ret.res  = ps[2];
   ret.resErr = ff->GetParError(2);  
   ret.absVal = hdpt->GetMean();
   cout <<"scale = " << ret.val << " +-" << ret.err << endl;
   return ret;
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
