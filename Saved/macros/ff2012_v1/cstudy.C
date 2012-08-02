#include "CutAndBinCollection2011.h"

void cstudy (int lowCent=0, int highCent=6, float trigPtLow=50, float trigPtHigh=80, float asoPtLow=8, float asoPtHigh=15 )    {
   // TCut hoeCut = "hadronicOverEm<0.2";
   //  TCut isoCut = "(cc4j + cr4j < 5) && sigmaIetaIeta<0.01";
   TCut ptCut  = Form("lPt>%.f && lPt <%.f",trigPtLow,trigPtHigh);
   TString variable1 = "lPhi - ytrack.trkPhi";
   TCanvas* c2 = new TCanvas("c2","",700,700);
   c2->Divide(2,2);
   c2->cd(1);

   TString fname1 = "rootFiles/barrelHiForestPhoton35_Skim2011-v2.root";
   TFile *f1  =new TFile(fname1.Data());
   TTree *photon1 = (TTree*)f1->Get("yongsunPhotonTree");
   photon1->AddFriend("yEvt=yongsunHiEvt"    ,fname1.Data());
   photon1->AddFriend("yskim=yongsunSkimTree"   ,fname1.Data());
   photon1->AddFriend("yhlt=yongsunHltTree"     ,fname1.Data());
   photon1->AddFriend("ytrack=yongsunTrackTree"     ,fname1.Data());

   TCut collisionCut = "yskim.pcollisionEventSelection==1";
   TCut runCut1       = ""; //"yhlt.Run>= 181985 && yhlt.Run <=182099"; 
   TCut centCut1      = Form("(yEvt.hiBin >= %d) && (yEvt.hiBin<= %d)",lowCent,highCent);
   //   TCut spikeCut1     = "abs(seedTime)<4 && swissCrx<0.90 && sigmaIetaIeta>0.002";
   TCut basicCut1     = collisionCut && centCut1 && runCut1;

   TCut trigCut1    = ptCut ;//hoeCut && isoCut && ptCut; 
   TCut assoCut1    = Form("ytrack.trkPt>%f && ytrack.trkPt <%f",(float)asoPtLow,(float)asoPtHigh);
   TCut finalCut1     = basicCut1 && trigCut1 && assoCut1;
   TH1D* theDphi = new TH1D("hdphi","",1000,-10,10);
   photon1->Draw(Form("%s>>%s",variable1.Data(),theDphi->GetName()), finalCut1);
   cout << "finalCut = " << finalCut1.GetTitle() << endl;
   theDphi->Draw();
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
