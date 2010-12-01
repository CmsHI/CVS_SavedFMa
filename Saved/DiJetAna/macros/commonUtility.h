#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TF1.h>
#include <TCanvas.h>
#include "TError.h"
#include "TLatex.h"

void makeMultiPanelCanvas(TCanvas*& canv, const Int_t columns,
                          const Int_t rows, const Float_t leftOffset=0.,
                          const Float_t bottomOffset=0.,
                          const Float_t leftMargin=0.2,
                          const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05) {
   if (canv==0) {
      Error("makeMultiPanelCanvas","Got null canvas.");
      return;
   }
   canv->Clear();

   TPad* pad[columns][rows];
   
   Float_t Xlow[columns];
   Float_t Xup[columns];
   Float_t Ylow[rows];
   Float_t Yup[rows];
   Float_t PadWidth =
      (1.0-leftOffset)/((1.0/(1.0-leftMargin)) +
			   (1.0/(1.0-edge))+(Float_t)columns-2.0);
   Float_t PadHeight =
      (1.0-bottomOffset)/((1.0/(1.0-bottomMargin)) +
			  (1.0/(1.0-edge))+(Float_t)rows-2.0);
   Xlow[0] = leftOffset;
   Xup[0] = leftOffset + PadWidth/(1.0-leftMargin);
   Xup[columns-1] = 1;
   Xlow[columns-1] = 1.0-PadWidth/(1.0-edge);
   
   Yup[0] = 1;
   Ylow[0] = 1.0-PadHeight/(1.0-edge);
   Ylow[rows-1] = bottomOffset;
   Yup[rows-1] = bottomOffset + PadHeight/(1.0-bottomMargin);
   
   for(Int_t i=1;i<columns-1;i++) {
      Xlow[i] = Xup[0] + (i-1)*PadWidth;
      Xup[i] = Xup[0] + (i)*PadWidth;
   }
   Int_t ct = 0;
   for(Int_t i=rows-2;i>0;i--) {
      Ylow[i] = Yup[rows-1] + ct*PadHeight;
      Yup[i] = Yup[rows-1] + (ct+1)*PadHeight;
      ct++;
   }

   TString padName;
   for(Int_t i=0;i<columns;i++) {
      for(Int_t j=0;j<rows;j++) {
         canv->cd();
         padName = Form("p_%d_%d",i,j);
         pad[i][j] = new TPad(padName.Data(),padName.Data(),
			      Xlow[i],Ylow[j],Xup[i],Yup[j]);
         if(i==0) pad[i][j]->SetLeftMargin(leftMargin);
         else pad[i][j]->SetLeftMargin(0);

         if(i==(columns-1)) pad[i][j]->SetRightMargin(edge);
         else pad[i][j]->SetRightMargin(0);

         if(j==0) pad[i][j]->SetTopMargin(edge);
         else pad[i][j]->SetTopMargin(0);
	 
         if(j==(rows-1)) pad[i][j]->SetBottomMargin(bottomMargin);
         else pad[i][j]->SetBottomMargin(0);
	 
         pad[i][j]->Draw();
         pad[i][j]->cd();
         pad[i][j]->SetNumber(columns*j+i+1);
      }
   }
}


   


void twikiSave(TCanvas* c=0, char* name="",int w=0,int h=0)
{
   if ( w==0) w = c->GetWindowWidth();
   if ( h==0) h = c->GetWindowHeight();
   
   c->SaveAs(name);
   cout << Form(" <br/>   <img src=\"%%ATTACHURLPATH%%/%s\" alt=\"%s\" width='%d' height='%d'/>",name,name,w,h)<< endl;
}

void centralityBinning(float *b=0)
{
  b[0]=      0;
  b[1]=  5.045;
  b[2]=  7.145;
  b[3]=  8.755;
  b[4]= 10.105;
  b[5]=  11.294;
  b[6]= 12.373;
  b[7]=  13.359;
  b[8]= 14.283;
  b[9]=  15.202;
  b[10] = 100;
 }

void handsomeTH1( TH1 *a=0, int col =1, float size=1, int markerstyle=20)
{
  //a->SetMarkerColor(col);
  //a->SetMarkerSize(size);
  //a->SetMarkerStyle(markerstyle);
  //a->SetLineColor(col);
  //a->GetYaxis()->SetTitleOffset(1.25);
  a->GetXaxis()->CenterTitle();
  a->GetYaxis()->CenterTitle();
}

void handsomeTGraph(TGraphAsymmErrors* a, int col=1)
{
   a->SetLineColor(col);
   a->SetMarkerColor(col);
   a->SetMarkerSize(1.0);
   a->SetMarkerStyle(24);
}

void scaleInt( TH1 *a=0)
{
   float fac = a->Integral(1,a->GetNbinsX());
   if ( fac>0) a->Scale(1./fac);
}




void handsomeTH1Sumw2( TH1 *a=0, int col =1, float size=1, int markerstyle=20)
{
   handsomeTH1(a,col,size,markerstyle);
   a->Sumw2();
}



void handsomeTH1N( TH1 *a=0, int col =1)
{
   handsomeTH1(a,col);
   a->Scale(1./a->GetEntries());
}


void handsomeTH1OnlyColor( TH1 *a=0, int col =1)
{
   a->SetMarkerColor(col);
   a->SetLineColor(col);
   a->GetYaxis()->SetTitleOffset(1.25);
}


void easyLeg( TLegend *a=0 , char * head="")
{
  a->SetBorderSize(0);
  a->SetHeader(head);
  a->SetTextFont(62);
  a->SetLineColor(1);
  a->SetLineStyle(1);
  a->SetLineWidth(1);
  a->SetFillColor(0);
  a->SetFillStyle(0);

}



TH1* cleverRange(TH1* h,float fac=1.2, float minY=1.e-3)
{
   float maxY =  fac * h->GetBinContent(h->GetMaximumBin());
   cout <<" ranged will be set as " << minY << " ~ " << maxY << endl; 
   h->SetAxisRange(minY,maxY,"Y");
}


TF1* cleverGaus(TH1* h, char* title="h", float c = 2.5)
{
   if ( h->GetEntries() == 0 )
      {
	 TF1 *fit0 = new TF1(title,"gaus",-1,1);
	 fit0->SetParameters(0,0,0);
	 return fit0;
      }
   
   int peakBin  = h->GetMaximumBin();
   double peak =  h->GetBinCenter(peakBin);
   double sigma = h->GetRMS();
  
   TF1 *fit1 = new TF1(title,"gaus",peak-c*sigma,peak+c*sigma);
   h->Fit(fit1,"LL M R Q");
   return fit1;
}

void addCms(TCanvas *c)
{
  c->cd();
  TLatex *cms = new TLatex(0.6013,0.89,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(16);
  cms->SetNDC();
  cms->Draw();
}


