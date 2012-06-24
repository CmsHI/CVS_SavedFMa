#ifndef HisMpt_h
#define HisMpt_h
#include "colorScheme.h"


TH1D *calcEff(TH1D* h1, TH1D* h2)
{
   h2->Divide(h1);
   return h2;
}

void subtractError(TH1* h1, TH1* h2)   {
   int nBins = h1->GetNbinsX();
   for ( int i = 1 ; i<=nBins ; i++ ) { 
      float oldErr = h1->GetBinError(i);
      float Err2   = h2->GetBinError(i);
      float newErr = sqrt ( oldErr*oldErr - Err2*Err2) ;
      h1->SetBinError(i,newErr);
   }
}

void drawErrorShift(TH1* hOld=0, float dx=0, float addSysErr=0.1) {
   int nBins = hOld->GetNbinsX();
   TLine* tl[100];
   for ( int i =1; i<=nBins ; i ++ ) { 
      float px = hOld->GetBinCenter(i);
      float py = hOld->GetBinContent(i);
      float tErrSta = hOld->GetBinError(i);
      float tErrSys = addSysErr*py;
      float tErr = sqrt( tErrSta*tErrSta + tErrSys*tErrSys);
      tl[i] = new TLine(px+dx,py-tErr, px+dx,py+tErr);
      tl[i]->SetLineWidth(1);
      tl[i]->Draw();
   }
}

void StackHistograms(int nBin, TH1D ** pe, TH1D ** ppos, TH1D ** pneg, int nBinAj) {
   // Styles
   for (int i=0;i<nBin;i++) {
      ppos[i] = (TH1D*)pe[i]->Clone(Form("%s_pos",pe[i]->GetName()));
      ppos[i]->Reset();
      ppos[i]->SetLineColor(1);     
      ppos[i]->SetMarkerColor(colors[i]);
      ppos[i]->SetFillColor(colors[i]);
      ppos[i]->SetFillStyle(1001);
      pneg[i] = (TH1D*)pe[i]->Clone(Form("%s_neg",pe[i]->GetName()));
      pneg[i]->Reset();
      pneg[i]->SetLineColor(1);     
      pneg[i]->SetMarkerColor(colors[i]);
      pneg[i]->SetFillColor(colors[i]);
      pneg[i]->SetFillStyle(1001);      
   }
   
   // Stack
   for (int i=nBin-1;i>=0;i--)
   {
      for(int iaj = 0 ; iaj< nBinAj ; iaj++) {
         double posVal=0, negVal=0;
         double posValErr=0, negValErr=0;
         if (i!=nBin-1) {
            posVal = ppos[i+1]->GetBinContent(iaj+1);
            posValErr = ppos[i+1]->GetBinError(iaj+1);
            negVal = pneg[i+1]->GetBinContent(iaj+1);
            negValErr = pneg[i+1]->GetBinError(iaj+1);
         }
         if (pe[i]->GetBinContent(iaj+1)<0) {
            negVal+=pe[i]->GetBinContent(iaj+1);
            negValErr=pe[i]->GetBinError(iaj+1);
            posValErr=0;
         } else if (pe[i]->GetBinContent(iaj+1)>0) {
            posVal+=pe[i]->GetBinContent(iaj+1);
            posValErr=pe[i]->GetBinError(iaj+1);
            negValErr=0;
         } 
         ppos[i]->SetBinContent(iaj+1,posVal);
         ppos[i]->SetBinError(iaj+1,posValErr);
         pneg[i]->SetBinContent(iaj+1,negVal);
         pneg[i]->SetBinError(iaj+1,negValErr);
      }
   }
}
#endif