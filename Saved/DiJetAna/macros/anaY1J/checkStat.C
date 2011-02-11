#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TRandom.h>
#include <TNtuple.h>
#include <TF1.h>
#include <TCut.h>


void checkStat(char *infname ="ntv2_dj_HCPR-J50U-hiGoodMergedTracks_OfficialSelv2_Final0_djcalo_100_50.root")
{
   int fillType = 2;
   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->FindObjectAny("ntjt");

   Float_t met;
   t->SetBranchAddress("metx",&met);

   TFile *outf = new TFile("output.root","recreate");  
   TNtuple *nt = new TNtuple("nt","","mean:gausMean:rmsErr:gausErr");
   TH1D *htemp = new TH1D("htemp","",100,-300,300);

   TCut evtCut = "nljet>120&&abs(nljetacorr)<1.6&&aljet>50&&abs(aljetacorr)<1.6&&jdphi>2./3*TMath::Pi()&&!maskEvt";

   t->Draw("metx>>htemp",evtCut);  

   int nentries = t->GetEntries();

   for (int exp =0; exp<10000;exp++)
   {
      TH1D *h = new TH1D("h","",100,-300,300);
      h->Sumw2();
      if (exp%100 ==0) cout <<exp<<endl;
      if (fillType==1) {
         for (int i=0;i<nentries/10.;i++)
         {
            int it = gRandom->Rndm()*nentries;
            t->GetEntry(it);
            h->Fill(met);
         }
      } else {
         for (int i=0;i<nentries/1.;i++)
         {  
            h->Fill(htemp->GetRandom());
         }
      }
      double mean = h->GetMean();
      h->Fit("gaus","LL Q");
      double gmean = h->GetFunction("gaus")->GetParameter(1);
      double rmsErr = h->GetRMS()/sqrt(h->GetEntries());
      double gausErr = h->GetFunction("gaus")->GetParError(1);

      nt->Fill(mean,gmean,rmsErr,gausErr);
      
      delete h;
   }   
   
   outf->Write();
}
