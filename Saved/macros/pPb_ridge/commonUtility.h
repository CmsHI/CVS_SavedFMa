#ifndef COMMONUTILITY_Yongsun_H
#define COMMONUTILITY_Yongsun_H
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TBox.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TGaxis.h>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>


struct kinem {
  double pt;
  double eta;
  double phi;
};

double getDPHI( double phi1, double phi2) {
  double dphi = phi1 - phi2;
 
  if ( dphi > 3.141592653589 )
    dphi = dphi - 2. * 3.141592653589;
  if ( dphi <= -3.141592653589 ) 
    dphi = dphi + 2. * 3.141592653589;
  
  if ( fabs(dphi) > 3.141592653589 ) {
    cout << " commonUtility::getDPHI error!!! dphi is bigger than 3.141592653589 " << endl;
  }
  
  return dphi;
}

double getDR( double eta1, double phi1, double eta2, double phi2){ 
  double theDphi = getDPHI( phi1, phi2);
  double theDeta = eta1 - eta2;
  return sqrt ( theDphi*theDphi + theDeta*theDeta);
}

void divideWOerr( TH1* h1, TH1* h2) {  //by Yongsun Jan 26 2012                                                                              
  if ( h1->GetNbinsX() != h2->GetNbinsX() ) {
    cout << " different bin numbers!!" << endl;
    return;
  }

  for ( int i=1 ; i<=h1->GetNbinsX() ; i++) {
    if ( (h2->GetBinContent(i) == 0 ) ) {
      h1->SetBinContent(i, 0);
      h1->SetBinError (i, 0);
    }
    else {
      float newV = h1->GetBinContent(i)/ h2->GetBinContent(i);
      float newE = h1->GetBinError(i)  / h2->GetBinContent(i);
      h1->SetBinContent(i, newV);
      h1->SetBinError  (i, newE);
    }
  }
}



void AddBinError( TH1* h=0, int binNumber=0 , float val=0){
  float valO = h->GetBinError(binNumber);
  float newVal = sqrt( valO*valO + val*val);
  h->SetBinError(binNumber,newVal);
}

void drawSys(TH1 *h,double *sys, int theColor= kYellow, int fillStyle = -1, int lineStyle = -1)
{
   for (int i=1;i<=h->GetNbinsX();i++)
      {
	 double val = h->GetBinContent(i);
	 double err = val * sys[i-1];
	 TBox *b = new TBox(h->GetBinLowEdge(i),val-err,h->GetBinLowEdge(i+1),val+err);
	 //      b->SetFillStyle(3001);                                                                                                                                                                              
	 b->SetLineColor(theColor);
	 b->SetFillColor(theColor);
	 if ( fillStyle > -1 ) b->SetFillStyle(fillStyle);
	 if ( lineStyle > -1 ) b->SetLineStyle(lineStyle);
	 
	 b->Draw();
      }
}

void drawSys(TGraph *h, double *sys, double width=5, int theColor= kYellow, int fillStyle = -1, int lineStyle = -1)
{
  for (int i=0;i<h->GetN();i++)
    {
      double val;
      double theX;
      h->GetPoint(i,theX,val);
      double err = val * sys[i];
      TBox *b = new TBox(theX-width,val-err,theX+width,val+err);
      
      b->SetLineColor(theColor);
      b->SetFillColor(theColor);
      if ( fillStyle > -1 ) b->SetFillStyle(fillStyle);
      if ( lineStyle > -1 ) b->SetLineStyle(lineStyle);
      
      b->Draw();
    }
}



void drawSysAbs(TH1 *h,TH1 *sys, int theColor= kYellow, int fillStyle = -1, int lineStyle = -1)
{
   for (int i=1;i<=h->GetNbinsX();i++)
      {
         double val = h->GetBinContent(i);
         double err = fabs(sys->GetBinContent(i));
	 if (err == 0  ) continue;
	 TBox *b = new TBox(h->GetBinLowEdge(i),val-err,h->GetBinLowEdge(i+1),val+err);
         b->SetLineColor(theColor);
         b->SetFillColor(theColor);
         if ( fillStyle > -1 ) b->SetFillStyle(fillStyle);
         if ( lineStyle > -1 ) b->SetLineStyle(lineStyle);

         b->Draw();
      }
}

void integerizeTH1(TH1* h1) {
   for ( int j = 0 ; j <= h1->GetNbinsX()+1 ; j++) {
      float vTemp = h1->GetBinContent(j);
      h1->SetBinContent( j , (int)vTemp ) ;
   }
}

void multiplyBonA(TH1* h1, TH1* h2) {
   if ( h1->GetNbinsX() != h2->GetNbinsX() ) { 
      cout << " different bin numbers.." << endl ;
      return ;
   }
   
   for ( int j=1 ; j<= h1->GetNbinsX(); j++) {
      float v1 = h1->GetBinContent(j);
      float e1 = h1->GetBinError(j);
      float v2 = h2->GetBinContent(j);
      //      float e2 = h2->GetBinError(j);
      
      float v3 = v1 * v2;
      float e3 = e1 * v2;
      h1->SetBinContent(j, v3);
      h1->SetBinError(j, e3);
   }
}

void drawPatch(float x1, float y1, float x2, float y2, int color =0, int style=1001, char* ops = ""){
   TLegend *t1=new TLegend(x1,y1,x2,y2,NULL,ops);
   if ( color ==0) t1->SetFillColor(kWhite);
   else t1->SetFillColor(color);
   t1->SetBorderSize(0);
   t1->SetFillStyle(style);
   t1->Draw("");
}

void drawErrorBox(float x1,float y1, float x2, float y2, int theColor=kSpring+8)
{
   TBox* tt = new TBox(x1,y1,x2,y2);
   tt->SetFillColor(theColor);
   tt->SetFillStyle(3001);
   tt->Draw();
}

void drawErrorBand(TH1* h, double* err, int theColor=kSpring+8)
{
   for ( int j=1 ; j<= h->GetNbinsX()-1; j++) {
      double theCont = h->GetBinContent(j);
      double theErr  = err[j] * h->GetBinContent(j);
      TBox* tt = new TBox(h->GetBinLowEdge(j), theCont-theErr, h->GetBinLowEdge(j)+ h->GetBinWidth(j), theCont+theErr);
      tt->SetFillColor(theColor);
      tt->SetFillStyle(3001);
      tt->Draw();
   }
}



void drawText(const char *text, float xp, float yp, int textColor=kBlack, int textSize=18, bool setNDC=true) {
  
  TLatex *tex = new TLatex(xp,yp,text);
  tex->SetNDC(setNDC);
  tex->SetTextFont(43);
  //   if(bold)tex->SetTextFont(43);
  tex->SetTextSize(textSize);
  tex->SetTextColor(textColor);
  tex->SetLineWidth(1);
  //tex->SetNDC();
  tex->Draw();
}


void drawText2(const char *text, float xp, float yp, int textSize=18){
   TLatex *tex = new TLatex(xp,yp,text);
   tex->SetTextFont(43);
   tex->SetTextSize(textSize);
   tex->SetTextColor(kBlack);
   tex->SetLineWidth(1);
   tex->Draw();
}

void jumSun(double x1=0,double y1=0,double x2=1,double y2=1,int color=1, double width=1)
{
   TLine* t1 = new TLine(x1,y1,x2,y2);
   t1->SetLineWidth(width);
   t1->SetLineStyle(7);
   t1->SetLineColor(color);
   t1->Draw();
}


void onSun(double x1=0,double y1=0,double x2=1,double y2=1,int color=1, double width=1)
{
  TLine* t1 = new TLine(x1,y1,x2,y2);
  t1->SetLineWidth(width);
  t1->SetLineStyle(1);
  t1->SetLineColor(color);
  t1->Draw();
}
void regSun(double x1=0,double y1=0,double x2=1,double y2=1,int color=1, double width=1)
{
   TLine* t1 = new TLine(x1,y1,x2,y2);
   t1->SetLineWidth(width);
   t1->SetLineStyle(1);
   t1->SetLineColor(color);
   t1->Draw();
}

void mcStyle1(TH1* h=0) {
   h->SetLineColor(kRed);
   h->SetFillColor(kRed-9);
   h->SetFillStyle(3004);
}
void mcStyle2(TH1* h=0) {
   h->SetLineColor(kBlue);
   h->SetFillColor(kAzure-8);
   h->SetFillStyle(3005);
}

void mcStyle3(TH1* h=0) {
  h->SetLineColor(kBlue);
  h->SetFillColor(kAzure-8);
  h->SetFillStyle(3005);
  h->SetMarkerSize(0);
}

void makeMultiPanelCanvas(TCanvas*& canv, const Int_t columns,
                          const Int_t rows, const Float_t leftOffset=0.,
                          const Float_t bottomOffset=0.,
                          const Float_t leftMargin=0.2,
                          const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05) {
   if (canv==0) {
//       Error("makeMultiPanelCanvas","Got null canvas.");
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
void handsomeTH2( TH2 *a=0)
{
   a->GetYaxis()->SetTitleOffset(1.25);
   a->GetXaxis()->CenterTitle();
   a->GetYaxis()->CenterTitle();
}

void handsomeTH1( TH1 *a=0, int col =1, float size=1, int markerstyle=20)
{
  a->SetMarkerColor(col);
  a->SetMarkerSize(size);
  a->SetMarkerStyle(markerstyle);
  a->SetLineColor(col);
  a->GetYaxis()->SetTitleOffset(1.25);
  a->GetXaxis()->CenterTitle();
  a->GetYaxis()->CenterTitle();
}
void fixedFontAxis(TGaxis * ax)
{
   ax->SetLabelFont(43);
   ax->SetLabelOffset(0.01);
   ax->SetLabelSize(25);
   ax->SetTitleFont(43);
   ax->SetTitleSize(14);
   ax->SetTitleOffset(2);
}

void fixedFontHist (TH1 * h, Float_t xoffset=1.3, Float_t yoffset=1.2,int size=25)
{
   h->SetLabelFont(43,"X");
   h->SetLabelFont(43,"Y");
   //h->SetLabelOffset(0.01);
   h->SetLabelSize(size);
   h->SetTitleFont(43);
   h->SetTitleSize(size+5);
   h->SetLabelSize(size,"Y");
   h->SetLabelSize(size,"X");
   h->SetTitleFont(43,"Y");
   h->SetTitleSize(size+5,"Y");
   h->SetTitleSize(size+5,"X");
   h->SetTitleOffset(xoffset,"X");
   h->SetTitleOffset(yoffset,"Y");
   h->GetXaxis()->CenterTitle();
   h->GetYaxis()->CenterTitle();
}

void handsomeTH1Fill( TH1 *a=0, int nFill=1) {
   handsomeTH1(a,nFill);
   a->SetFillColor(nFill);
}
void handsomeTGraph(TGraphAsymmErrors* a, int col=1)
{
   a->SetLineColor(col);
   a->SetMarkerColor(col);
   a->SetMarkerSize(1);
   a->SetMarkerStyle(24);
}

void TH1ScaleByWidth(TH1* h=0) 
{
   int nBins = h->GetNbinsX();
   // cout << "Start scaling by width" << endl;
   for ( int j=1; j<=nBins ;j++)
      {
         double theWidth = h->GetBinWidth(j);
         //      cout << "width = " << theWidth << ",   " ;                                                                                  
	 double cont = h->GetBinContent(j);
         double err =  h->GetBinError(j);
         h->SetBinContent(j, cont/theWidth);
         h->SetBinError  (j, err/theWidth);
      }
   //   cout << endl;
}

void scaleInt( TH1 *a=0, double normFac=1., double minX=-999.21231, double maxX=-999.21231)
{
  float fac=0;
  int lowBin=1; 
  int highBin=a->GetNbinsX();
  if ( minX != -999.21231)
    lowBin = a->FindBin(minX);
  if ( maxX != -999.21231)
    highBin=a->FindBin(maxX);

  fac =  a->Integral( lowBin, highBin);
  if ( fac>0) a->Scale(normFac/fac);
}



double goodIntegral( TH1 *a=0, int lower=-123, int upper=-123)
{
   int nBins = a->GetNbinsX();
   
   if ( (lower==-123) || (upper==-123)) {
      lower = 1;
      upper = nBins;
   }
   double tempInt=0;
   for ( int j=lower; j<=upper; j++) {
      tempInt = tempInt + a->GetBinContent(j) * a->GetBinWidth(j);
   }
   return tempInt;
}

double goodIntegralError( TH1 *a=0, int lower=-123, int upper=-123)
{
   int nBins = a->GetNbinsX();
   if ( (lower==-123) || (upper==-123)) {
      lower = 1;
      upper = nBins;
   }
   
   double tempInt=0;
   for ( int j=lower; j<=upper; j++) {
      tempInt = tempInt + a->GetBinError(j) * a->GetBinError(j) * a->GetBinWidth(j) *  a->GetBinWidth(j);
   }
   return sqrt(tempInt);
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


void easyLeg( TLegend *a=0 , TString head="", int size=25)
{
  a->SetBorderSize(0);
  a->SetHeader(head);
  //  a->SetTextFont(62);
  //h->SetLabelOffset(0.01);                                                                                                                
  a->SetTextFont(43);
  a->SetTextSize(size);
  a->SetLineColor(1);
  a->SetLineStyle(1);
  a->SetLineWidth(1);
  a->SetFillColor(0);
  a->SetFillStyle(0);

}



double cleverRange(TH1* h,float fac=1.2, float minY=1.e-3)
{
   float maxY =  fac * h->GetBinContent(h->GetMaximumBin());
   //   cout <<" range will be set as " << minY << " ~ " << maxY << endl; 
   h->SetAxisRange(minY,maxY,"Y");
   return maxY;
}


double getCleverRange(TH1* h)
{
  double maxY = -1000000;
  for ( int ibin = 1 ; ibin <= h->GetNbinsX() ; ibin++) {
    if (maxY < h->GetBinContent(ibin) ) 
      maxY = h->GetBinContent(ibin);
  }
  return maxY;
}

double cleverRange(TH1* h,TH1* h2, float fac=1.2, float minY=1.e-3)
{
  float maxY1 =  fac * h->GetBinContent(h->GetMaximumBin());
  float maxY2 =  fac * h2->GetBinContent(h2->GetMaximumBin());
  
  //   cout <<" range will be set as " << minY << " ~ " << maxY << endl;                                                                    
  h->SetAxisRange(minY,max(maxY1,maxY2),"Y");
  h2->SetAxisRange(minY,max(maxY1,maxY2),"Y");
  return max(maxY1,maxY2);
}

void cleverRangeLog(TH1* h,float fac=1.2, float theOrder=1.e-4)
{
  float maxY =  fac * h->GetBinContent(h->GetMaximumBin());
  //   cout <<" range will be set as " << minY << " ~ " << maxY << endl;                                               
  h->SetAxisRange(maxY * theOrder,maxY,"Y");
}



TF1* cleverGaus(TH1* h, char* title="h", float c = 2.5, bool quietMode=true)
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
   if (quietMode) h->Fit(fit1,"LL M O Q R");
   else    h->Fit(fit1,"LL M O Q R");
   return fit1;
}




void drawCMS(float px, float py, float nLumi) {
   TLatex *cms = new TLatex(px,py,"CMS Preliminary");
   cms->SetTextFont(63);
   cms->SetTextSize(15);
   cms->SetNDC();
   cms->Draw();
   TLatex *lumi = new TLatex(px+0.35,py,Form("#intL dt = %.1f #mub^{-1}",nLumi));
   lumi->SetTextFont(63);
   lumi->SetTextSize(13);
   lumi->SetNDC();
   lumi->Draw();
}

void drawCMSpp(float px, float py, float nLumi) {
  TLatex *cms = new TLatex(px,py,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(15);
  cms->SetNDC();
  cms->Draw();
  TLatex *lumi = new TLatex(px+0.35,py,Form("#intL dt = %.1f #nb^{-1}",nLumi));
  lumi->SetTextFont(63);
  lumi->SetTextSize(13);
  lumi->SetNDC();
  lumi->Draw();
}



void drawCMS2(float px, float py, float nLumi, int textSize=15) {
   TLatex *cms = new TLatex(px,py,"CMS Preliminary");
   cms->SetTextFont(63);
   cms->SetTextSize(textSize);
   cms->SetNDC();
   cms->Draw();
   TLatex *lumi = new TLatex(px,py-0.08,Form("#intL dt = %.1f #mub^{-1}",nLumi));
   lumi->SetTextFont(63);
   lumi->SetTextSize(textSize-2);
   lumi->SetNDC();
   lumi->Draw();
}
void drawCMS2011(float px, float py, float nLumi=3.8, int textSize=15) {
   TLatex *cms = new TLatex(px,py,"CMS Preliminary");
   cms->SetTextFont(63);
   cms->SetTextSize(textSize);
   cms->SetNDC();
   cms->Draw();
   TLatex *lumi = new TLatex(px,py-0.08,Form("L_{Int} dt = %.0f #mub^{-1}",nLumi));
   lumi->SetTextFont(63);
   lumi->SetTextSize(textSize-2);
   lumi->SetNDC();
   lumi->Draw();
}

void drawCMS3(float px, float py, float nLumi, int textSize=15) {
   TLatex *cms = new TLatex(px,py,"CMS");
   cms->SetTextFont(63);
   cms->SetTextSize(textSize);
   cms->SetNDC();
   cms->Draw();
   TLatex *lumi = new TLatex(px,py-0.08,Form("PbPb  #sqrt{s_{NN}}=2.76TeV  #intL dt = %.0f #mub^{-1}",nLumi));
   lumi->SetTextFont(63);
   lumi->SetTextSize(textSize-2);
   lumi->SetNDC();
   lumi->Draw();
}
void drawCMS4(float px, float py, float nLumi, int textSize=15) {
   TLatex *cms = new TLatex(px,py,"CMS Preliminary");
   cms->SetTextFont(63);
   cms->SetTextSize(textSize);
   cms->SetNDC();
   cms->Draw();
   
   TLatex *pbpb = new TLatex(px,py-0.08,"PbPb  #sqrt{s_{NN}}=2.76TeV");
   pbpb->SetTextFont(63);
   pbpb->SetTextSize(textSize-2);
   pbpb->SetNDC();
   pbpb->Draw();
   
   TLatex *lumi = new TLatex(px,py-0.16,Form("#intL dt = %.1f #mub^{-1}",nLumi));
   lumi->SetTextFont(63);
   lumi->SetTextSize(textSize-2);
   lumi->SetNDC();
   lumi->Draw();
   
   
}



void getNiceBins( TH1* h=0, int nDiv=4) {
   int nBins = h->GetNbinsX();
   double allInt = h->Integral(1,nBins);
   cout<< " All integral = " << allInt<< endl;

   TH1F* hacc = (TH1F*)h->Clone(Form("%s_accu",h->GetName()));
   hacc->Reset();
   double acc =0;
   

   int j=0;
   for ( int i=1 ; i<= nBins ; i++ ) {
      acc = acc + h->GetBinContent(i);
      hacc->SetBinContent( i , acc ) ;
       if ( ( hacc->GetBinContent(i) > j*allInt/nDiv ) && ( hacc->GetBinContent(i-1) <= j*allInt/nDiv)) {
	 j++;
	 cout << j << "th bin = " << i <<  "    value  = " << h->GetBinCenter(i) << endl;
       }
   }     
   cout << " acc = " << acc << endl;
   TCanvas* c1 = new TCanvas(Form("%s_c1Acc",h->GetName()),"",400,400);
   c1->Draw();
   handsomeTH1(hacc,1);
   handsomeTH1(h,2);
   hacc->DrawCopy();
   h->DrawCopy("same");
  
   
}

void stripErr(TH1* theHist=0) {
   for ( int ibin = 1 ; ibin <= theHist->GetNbinsX() ; ibin++)
      theHist->SetBinError(ibin,0);
}


double getPolyArea(TH1* h1, TH1* h2, double minX, double maxX) { 
  if ( (h1->GetNbinsX() != h2->GetNbinsX()) || ( h1->FindBin(minX) != h2->FindBin(minX)) || (h1->FindBin(maxX) != h2->FindBin(maxX)) ) 
    {
      cout <<" binnings are not matched!!! " << endl;
      return -1;
    }
  
  int minBin = h1->FindBin(minX);
  int maxBin = h1->FindBin(maxX);
  cout << " getPolyArea : from " << h1->GetBinLowEdge(minBin) << " to " << h1->GetBinLowEdge(maxBin+1) << endl;
  double area = 0;
  for ( int ibin = minBin ; ibin <= maxBin ; ibin++) {
    area = area + ( h2->GetBinContent(ibin) - h1->GetBinContent(ibin) ) * h1->GetBinWidth(ibin) ;
  }
  
  return area;
}

double getPolyAreaErr(TH1* h1, TH1* h2, double minX, double maxX) {
  if ( (h1->GetNbinsX() != h2->GetNbinsX()) || ( h1->FindBin(minX) != h2->FindBin(minX)) || (h1->FindBin(maxX) != h2->FindBin(maxX)) )
    {
      cout <<" binnings are not matched!!! " << endl;
      return -1;
    }

  int minBin = h1->FindBin(minX);
  int maxBin = h1->FindBin(maxX);
  double err2 = 0;
  for ( int ibin = minBin ; ibin <= maxBin ; ibin++) {
    double addErr2 = (h2->GetBinError(ibin)*h2->GetBinError(ibin) + h1->GetBinError(ibin)*h1->GetBinError(ibin)) * (h1->GetBinWidth(ibin)*h1->GetBinWidth(ibin)) ;
    err2 = err2 + addErr2;
  }
  return sqrt(err2);
}

TH1D* getShiftedTH1D(TH1D* h1, double shift) {
  double newBinning[200];
  for ( int ibin = 0 ; ibin <= h1->GetNbinsX() ; ibin++) {
    newBinning[ibin] = h1->GetBinLowEdge(ibin+1) + shift ;
  }
  TH1D* res = new TH1D(Form("%s_shiftedby%f",h1->GetName(),(float)shift), "", h1->GetNbinsX(), newBinning);
  for ( int ibin = 0 ; ibin <= h1->GetNbinsX() ; ibin++) {
    res->SetBinContent( ibin, h1->GetBinContent(ibin));
    res->SetBinError(ibin, h1->GetBinError(ibin));
  }
  return res;
}


int getNcollFrom40Bin(int cBin) { 
  if (cBin == 0) return  1747.86 ;
  if (cBin == 1) return  1567.53 ;
  if (cBin == 2) return  1388.39 ;
  if (cBin == 3) return  1231.77 ;
  if (cBin == 4) return  1098.2 ;
  if (cBin == 5) return  980.439 ;
  if (cBin == 6) return  861.609 ;
  if (cBin == 7) return  766.042 ;
  if (cBin == 8) return  676.515 ;
  if (cBin == 9) return  593.473 ;
  if (cBin == 10) return  521.912 ;
  if (cBin == 11) return  456.542 ;
  if (cBin == 12) return  398.546 ;
  if (cBin == 13) return  346.647 ;
  if (cBin == 14) return  299.305 ;
  if (cBin == 15) return  258.344 ;
  if (cBin == 16) return  221.216 ;
  if (cBin == 17) return  188.677 ;
  if (cBin == 18) return  158.986 ;
  if (cBin == 19) return  134.7 ;
  if (cBin == 20) return  112.547 ;
  if (cBin == 21) return  93.4537 ;
  if (cBin == 22) return  77.9314 ;
  if (cBin == 23) return  63.5031 ;
  if (cBin == 24) return  52.0469 ;
  if (cBin == 25) return  42.3542 ;
  if (cBin == 26) return  33.9204 ;
  if (cBin == 27) return  27.3163 ;
  if (cBin == 28) return  21.8028 ;
  if (cBin == 29) return  17.2037 ;
  if (cBin == 30) return  13.5881 ;
  if (cBin == 31) return  10.6538 ;
  if (cBin == 32) return  8.35553 ;
  if (cBin == 33) return  6.40891 ;
  if (cBin == 34) return  5.13343 ;
  if (cBin == 35) return  3.73215 ;
  if (cBin == 36) return  3.06627 ;
  if (cBin == 37) return  2.41926 ;
  if (cBin == 38) return  2.11898 ;
  if (cBin == 39) return  1.76953 ;
  return -100000;
}


float  getNpart(int ibin) {
  if (ibin ==0) return  393.633;
  if (ibin ==1) return  368.819;
  if (ibin ==2) return  343.073;
  if (ibin ==3) return  317.625;
  if (ibin ==4) return  292.932;
  if (ibin ==5) return  271.917;
  if (ibin ==6) return  249.851;
  if (ibin ==7) return  230.72;
  if (ibin ==8) return  212.465;
  if (ibin ==9) return  194.752;
  if (ibin ==10) return  178.571;
  if (ibin ==11) return  163.23;
  if (ibin ==12) return  149.187;
  if (ibin ==13) return  136.011;
  if (ibin ==14) return  123.414;
  if (ibin ==15) return  111.7;
  if (ibin ==16) return  100.831;
  if (ibin ==17) return  90.7831;
  if (ibin ==18) return  80.9823;
  if (ibin ==19) return  72.6236;
  if (ibin ==20) return  64.1508;
  if (ibin ==21) return  56.6284;
  if (ibin ==22) return  49.9984;
  if (ibin ==23) return  43.3034;
  if (ibin ==24) return  37.8437;
  if (ibin ==25) return  32.6659;
  if (ibin ==26) return  27.83;
  if (ibin ==27) return  23.7892;
  if (ibin ==28) return  20.1745;
  if (ibin ==29) return  16.8453;
  if (ibin ==30) return  14.0322;
  if (ibin ==31) return  11.602;
  if (ibin ==32) return  9.52528;
  if (ibin ==33) return  7.6984;
  if (ibin ==34) return  6.446;
  if (ibin ==35) return  4.96683;
  if (ibin ==36) return  4.23649;
  if (ibin ==37) return  3.50147;
  if (ibin ==38) return  3.16107;
  if (ibin ==39) return  2.7877;
  return -100000;
}





#endif 
#include "TCanvas.h"

void makeMultiPanelCanvasNew(TCanvas*& canv,
                          const Int_t columns,
                          const Int_t rows,
                          const Float_t leftOffset,
                          const Float_t bottomOffset,
                          const Float_t leftMargin,
                          const Float_t bottomMargin,
			  const Float_t edge,
			  const Float_t scaleX,
			  const Float_t scaleY
			  ) {
   if (canv==0) {
      //      Error("makeMultiPanelCanvas","Got null canvas.");
      return;
   }
   canv->Clear();
   
   TPad* pad[columns][rows];

   Float_t Xlow[columns];
   Float_t Xup[columns];
   Float_t Ylow[rows];
   Float_t Yup[rows];
   Float_t PadWidth = 
      (scaleX-leftOffset)/((scaleX/(scaleX-leftMargin)) +
			(scaleX/(scaleX-edge))+(Float_t)columns-2.0*scaleX);
   Float_t PadHeight =
     (scaleY-bottomOffset)/((scaleY/(scaleY-bottomMargin)) +
			  (scaleY/(scaleY-edge))+(Float_t)rows-2.0*scaleY);
   Xlow[0] = leftOffset;
   Xup[0] = leftOffset + PadWidth/(scaleX-leftMargin);
   Xup[columns-1] = scaleX;
   Xlow[columns-1] = scaleX-PadWidth/(scaleX-edge);

   Yup[0] = scaleY;
   Ylow[0] = scaleY-PadHeight/(scaleY-edge)- edge;
   Ylow[rows-1] = bottomOffset;
   Yup[rows-1] = bottomOffset + PadHeight/(scaleY-bottomMargin) -edge;

   for(Int_t i=1;i<columns-1;i++) {
      Xlow[i] = Xup[0] + (i-1)*PadWidth;
      Xup[i] = Xup[0] + (i)*PadWidth;
   }
   Int_t ct = 0;
   for(Int_t i=rows-2;i>0;i--) {
      Ylow[i] = Yup[rows-1] + ct*PadHeight;
      Yup[i] = Yup[rows-1] + (ct+1)*PadHeight;
      ct++;
      cout << Ylow[i] << " " <<  Yup[i] << endl;
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

   /*
   TString padName;
   for(Int_t i=0;i<columns;i++) {
      for(Int_t j=0;j<rows;j++) {
         canv->cd();
         padName = Form("p_%d_%d",i,j);
         pad[i][j] = new TPad(padName.Data(),padName.Data(),
			      Xlow[i],Ylow[j],Xup[i],Yup[j]);
        
         if(i==0) pad[i][j]->SetLeftMargin(leftMargin);
         else pad[i][j]->SetLeftMargin(0);

         if(i==(columns-1)) pad[i][j]->SetRightMargin(rightmargin);
         else pad[i][j]->SetRightMargin(0);

         if(j==0) pad[i][j]->SetTopMargin(topmargin);
         else pad[i][j]->SetTopMargin(0);

         if(j==(rows-1)) pad[i][j]->SetBottomMargin(bottomMargin);
         else pad[i][j]->SetBottomMargin(0);

         pad[i][j]->Draw();
         pad[i][j]->cd();
         pad[i][j]->SetNumber(columns*j+i+1);
      }
   }
   */
}

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TLatex.h"
#include "TLegend.h"

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

#define merge 0

void drawPatch(float x1, float y1, float x2, float y2){
  TLegend *t1=new TLegend(x1,y1,x2,y2);
  t1->SetFillColor(kWhite);
  t1->SetBorderSize(0);
  t1->SetFillStyle(1001);
  t1->Draw("");
}


void normalizeBinWidth(TH1* h){

  for(int i = 1; i < h->GetNbinsX()+1; ++i){
    double y = h->GetBinContent(i);
    double e =  h->GetBinError(i);
    double w = h->GetBinWidth(i);

    h->SetBinContent(i,y/w);
    h->SetBinError(i,e/w);

  }


}

void setBlack(TH1* h){
  h->SetMarkerColor(1);
  h->SetLineColor(1);
}

void mergeBin(TH1* h, int bin1, int bin2, int mc = 0){

  double y1 = h->GetBinContent(bin1);
  double y2 = h->GetBinContent(bin2);
  double e1 = h->GetBinError(bin1);
  double e2 = h->GetBinError(bin2);

  double y = y2+y1;
  double e = sqrt(e1*e1+e2*e2);

  if(mc){
    h->SetBinContent(bin2,y);
    h->SetBinError(bin2,e);
    
    h->SetBinContent(bin1,y);
    h->SetBinError(bin1,0.);
  }else{
    h->SetBinContent(bin2,y);
    h->SetBinError(bin2,e);
    h->SetBinContent(bin1,0.);
    h->SetBinError(bin1,0.);
  }
}

void normalize(TH1* h, int rm, int nf, int nl, bool mc = 1){

  int a = rm % 1000;
  int b = (rm - a)/1000;

  for(int i = 1; i < 10; ++i){
    double r = h->GetBinContent(i);

    if(r != 0 && i <= b && b != 0){
      h->SetBinContent(i,(h->GetBinContent(b+1)/nf)*((int)mc));
      h->SetBinError(i,(h->GetBinError(b+1)/nf)*((int)mc));
    }
    if(r != 0 && i >= a && a != 0){
      h->SetBinContent(i,(h->GetBinContent(a-1)/nl)*((int)mc));
      h->SetBinError(i,(h->GetBinError(a-1)/nl)*((int)mc));
    }
  }

  h->SetBinContent(10,0);
  h->SetBinError(10,0.1);


  if(1){
    if(a != 0){
      h->SetBinContent(a-1,(h->GetBinContent(a-1)/nl));
      h->SetBinError(a-1,(h->GetBinError(a-1)/nl));
    }
    if(b != 0){
      h->SetBinContent(b+1,(h->GetBinContent(b+1)/nf));
      h->SetBinError(b+1,(h->GetBinError(b+1)/nf));
    }
  }

}


int mergeDivide(TH1* h1, TH1* h2, int aj = 0, int ijet = 0){

  int ilastlead = 9;
  int ilastsub[4] = {9,8,8,7};

  double statLimit = 0.9;

  TH1F* hDiv = (TH1F*)h1->Clone(Form("%s_clone",h1->GetName()));
  hDiv->Divide(h2);

  int result = 0;
  int result0 = 0;

  int nl = 0;
  int nf = 0;

  if(0){
  for(int i = 1; i < 10; ++i){
    double r = hDiv->GetBinContent(i);
    double e0 = hDiv->GetBinError(i);
    if(1){
    cout<<"Shoulld we merge ? "<<endl;
    cout<<"AJ : "<<aj<<" jet : "<<ijet<<" bin : "<<i<<" error : "<<e0<<endl;
    }

    if(r > 0 && e0 > statLimit){
      if(1){ 
	cout<<"MERGING ..."<<endl;
	cout<<"AJ : "<<aj<<" jet : "<<ijet<<" bin : "<<i<<" error : "<<e0<<endl;
      }
      result0 = 1000*i;
      nf++;
      mergeBin(h1,i,i+1,0);
      mergeBin(h2,i,i+1,1);
    }else if(e0 != 0){
      break;
    }
  }
  }

  for(int i = h1->GetNbinsX(); i >=h1->GetNbinsX()/2; --i){
    double r = hDiv->GetBinContent(i);
    double e0 = hDiv->GetBinError(i);
    if(r > 0 && e0 > statLimit){
      result = result0+i;
      nl++;
      mergeBin(h1,i,i-1,0);
      mergeBin(h2,i,i-1,1);
    }
  }
  cout<<"result : "<<result<<" nf : "<<nf<<" nl : "<<nl<<endl;
  normalize(h1,result,nf,nl,0);
  normalize(h2,result,nf,nl,1);

  return result;
}


void maybeMerge(TH1* h, int rm, bool mc = 1){

  int a = rm % 1000;

  cout<<"MAYBE : "<<rm<<endl;
  cout<<"a : "<<a<<endl;
  int nf = 0;
  int nl = 0;
  if(a != 0){
    for(int i = h->GetNbinsX(); i >=a; --i){
      if(h->GetBinContent(i) != 0){
	nl++;
	mergeBin(h,i,a-1,mc);
      }
    }    
  }

  int b = (rm - a)/1000;
  cout<<"b : "<<b<<endl;

  if(b != 0){
    if(h->GetBinContent(b) != 0){
      nf++;
      mergeBin(h,b,b+1,mc);
    }
  }
  cout<<"rm : "<<rm<<" nf : "<<nf<<" nl : "<<nl<<endl;
  normalize(h,rm,nf,nl,mc);

}

TH1D* invertHistogram(TH1* h){

  TH1D* hh = new TH1D(Form("%s_inv",h->GetName()),Form("%s_inv",h->GetTitle()),h->GetNbinsX(),h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());

  for(int i = 0; i < h->GetNbinsX(); ++i){

    double y0 = h->GetBinContent(i);
    double e0 = h->GetBinError(i);

    double y1 = 0;
    double e1 = 0;
    if(y0 > 0){
      y1 = 1./y0;      
      e1 = (e0/y0)*y1;
    }

    hh->SetBinContent(i,y1);
    hh->SetBinError(i,e1);

  }
  hh->SetEntries(h->GetEntries());
  return hh;
}


void preparePlot(TH1* h, double scaleX = 1. , double scaleY = 1.){

  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();

  h->GetXaxis()->SetTitleOffset(h->GetXaxis()->GetTitleOffset()*scaleX);    
  h->GetYaxis()->SetTitleOffset(h->GetYaxis()->GetTitleOffset()*scaleY);
}

void makeHorizontal(TH1* h){

  h->SetMarkerColor(0);
  h->SetMarkerSize(0);

  for(int i = 0; i < h->GetNbinsX(); ++i){
    h->SetBinError(i,h->GetBinWidth(1)/2.);
  }

}


void eraseLine(TH1* h, double cutoff = 0.04){

  TGraph* ladder = new TGraph();
  ladder->SetLineStyle(h->GetLineStyle());
  ladder->SetLineWidth(h->GetLineWidth());
  ladder->SetLineColor(h->GetLineColor());
  //  ladder->SetMarkerSize(5);  
  int point = 0;
  for(int i = 1; i < h->GetNbinsX(); ++i){

    if(h->GetBinContent(i) > cutoff){
      ladder->SetPoint(point,h->GetBinLowEdge(i),h->GetBinContent(i));
      point++;
      ladder->SetPoint(point,h->GetBinLowEdge(i+1),h->GetBinContent(i));
      point++;
    }

    if(0){
    if(h->GetBinContent(i) <= cutoff){
      cout<<"Bottom reached."<<endl;      
      if(h->GetBinCenter(i) > 2){
	h->GetXaxis()->SetRange(h->GetXaxis()->GetXmin(),i-1);
	break;
      }else{
        h->GetXaxis()->SetRange(i+1,h->GetXaxis()->GetXmax());
      }    
      if(0){
      TLine* line = new TLine(h->GetBinLowEdge(i+1),0,h->GetBinLowEdge(i+1),h->GetBinContent(i+1));
      line->SetLineWidth(h->GetLineWidth());
      line->SetLineColor(0);
      line->Draw("same");
      line = new TLine(h->GetBinLowEdge(i),0,h->GetBinLowEdge(i),h->GetBinContent(i-1));
      line->SetLineWidth(h->GetLineWidth());
      line->SetLineColor(0);
      line->Draw("same");
      }
     
   }
    }
  }

  //  h->Draw("hist same");
  ladder->Draw("L");

}




void fixLegend(TLegend* leg){
  
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);

  double offsetX1 = 0.5;
  double offsetX2 = 0.;
  
  leg->SetX1NDC(leg->GetX1NDC()+offsetX1);
  leg->SetX2NDC(leg->GetX2NDC()+offsetX2);

}

void divideIntegral(TH1* h, TF1* f){

  double width = h->GetBinWidth(1);
  for(int i = 1; i < h->GetNbinsX()+1; ++i){
    double y = h->GetBinContent(i);
    if(y == 0) continue;
    double e = h->GetBinError(i);
    double b = f->Integral(h->GetBinLowEdge(i),h->GetBinLowEdge(i+1))/width;
    h->SetBinContent(i,y/b);
    h->SetBinError(i,e/b);
  }

}




void divideFunction(TF1* fD, TF1* f1, TF1* f2, int np = 5){
  fD->SetLineColor(f1->GetLineColor());
  fD->SetLineStyle(f1->GetLineStyle());

  for(int i = 0; i < np; ++i){
    fD->SetParameter(i,f1->GetParameter(i));
    fD->SetParameter(i+np,f2->GetParameter(i));
  }
}


TLegend* makeLegend(double x1 = 0.5, double y1 = 0.7, double x2 = 0.95, double y2 = 0.95){

  TLegend *leg=new TLegend(x1,y1,x2,y2);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(43);
  leg->SetTextSize(19);
  return leg;

}

TH1* load(const char* file = "D6T_Errors.root", const char* name = "Ratio_D6T_Data_ChargedHadrons_0040_0050_inv"){

  TFile* inf = new TFile(file);

  TH1* h = 0;
  h = (TH1*)inf->Get(name);
  if (!h) { cout << "bad " << file << ": " << name << endl; exit(1); }
  h->SetStats(0);
  //  h->GetXaxis()->SetTitleFont(43);

  setBlack(h);
  h->SetDirectory(0);
  inf->Close();
  return h;

}

TH1* loadError(const char* file = "D6T_Errors.root", const char* name = "Ratio_D6T_Data_ChargedHadrons_0040_0050_inv"){
  return load(file,name);
}


TF1* invertFit(TF1* f){
  TF1* fInv = new TF1(Form("%s_inverted",f->GetName()),"1./([0]+[1]*x+[2]*x*x+[3]*x*x*x)",0,4.5);
  fInv->SetParameter(0,f->GetParameter(0));
  fInv->SetParameter(1,f->GetParameter(1));
  fInv->SetParameter(2,f->GetParameter(2));
  fInv->SetParameter(3,f->GetParameter(3));

  fInv->SetLineColor(1);
  fInv->SetLineColor(1);

  return fInv;

}

TGraph* cheatFit(TH1* h, TF1* f){

  TGraph* g = new TGraph();
  g->SetName(Form("%s_fitted",h->GetName()));

  for(int i = 0; i < h->GetNbinsX(); ++i){

    double x = h->GetBinCenter(i);
    g->SetPoint(i, x, f->Eval(x)*h->GetBinContent(i));

  }

  g->SetLineStyle(2);
  //  g->SetLineWidth(2);
  return g;

}



TGraph* drawSysErr(TH1* h, TH1** he, int Nerror = 1, int ijet = 0, bool cheatEnd = 0,int npar = 3, bool symmetric = 1, int opt = 0, int infill = 0, TF1* f = 0, TPad * inc=0, TPad * cerr=0, int theColor=TColor::GetColor(0xFFEE00), int theStyle=1001){

  bool plot = 1;
  
  int color[2] = {theColor,1};

  int errorColor = theColor;

  int fillStyle[2] = {1,1};
  int fillColor[2] = {1,1};
  int markerColor[2] = {1,1};
  int sysMarkerColor[20] = {kBlack,kGray+2,kBlue,kGreen+2,kOrange+2,kRed,kYellow-3,kCyan+3};

  float subtract = 1;
  bool reference = 0;
  bool fitData = 0;
  bool useFit = 1;
  int fill = 1;
  bool skipZero = 1;

  bool box = 1;

  cout << "Sys error: " << Nerror << endl;

  int n = h->GetNbinsX()+1;

  if (fitData) {
    if(f == 0) f = new TF1(Form("%s_fit%d",h->GetName(),fill),Form("pol%d",npar),0,5.25);
    h->Fit(f,"QRN");
    cout<<"FITTED Data"<<endl;
  }

  TF1* fe[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  for(int ie = 0; ie< Nerror; ++ie){
    if (ie<6) fe[ie] = new TF1(Form("%s_fit%d_%d",h->GetName(),fill,ie),"pol3",0,5.5);
    else fe[ie] = new TF1(Form("%s_fit%d_%d",h->GetName(),fill,ie),"pol4",0,5.5);
    he[ie]->Fit(fe[ie],"QRN");
  }
  
  TH1D * hErrorTot = (TH1D*)he[1]->Clone(Form("%s_tot",he[1]->GetName()));
  if (cerr) {
     cerr->cd();
     for(int ie = 0; ie< Nerror; ++ie){
     cout << "draw errors" << he[ie]->GetName() << endl;
      he[ie]->SetMarkerStyle(kOpenCircle);
      he[ie]->SetMarkerColor(sysMarkerColor[ie]);
      if (ie==0) he[ie]->DrawClone();
      else he[ie]->DrawClone("same");
      if (fe[ie]){
        fe[ie]->SetLineColor(sysMarkerColor[ie]);
        fe[ie]->Draw("same");
      }
    }
  }
  if (inc) inc->cd();

  int ig = 0;
  double e[50] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  double sign[50] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

  for(int i = 1; i < n; ++i){
    if(skipZero && h->GetBinContent(i) ==0) continue;
    double x0 = h->GetBinCenter(i);
    cout << "##xi: " << x0 << endl;
    
    e[i] = 0;
    // Add Errors in Quadrature
    for(int ie = 0; ie< Nerror; ++ie){
      double xbin = he[ie]->FindBin(x0);
//       cout << "error" << ie << " before subtract: " << he[ie]->GetBinContent(xbin) << endl;
      double ec = he[ie]->GetBinContent(xbin) - subtract;
//       cout << "error" << ie << " after subtract: " << ec << endl;
      if(useFit) ec = fe[ie]->Eval(x0) - subtract;
//       cout << "error" << ie << " after fit: " << ec << endl;
      e[i] += pow(ec,2);
//       cout << "error" << ie << " add: " << pow(ec,2) << " tot: " << sqrt(e[i]) << endl;
      if(ec < 0) sign[i] = -1;
      else sign[i] = 1;
    }
    hErrorTot->SetBinContent(i,1+sqrt(e[i]));
    cout << "tothist: " << i << ": " << hErrorTot->GetBinContent(i) << endl;
    // Multiply error to y value
    double y = h->GetBinContent(i);
    if (fitData) if(y > 1.5) y = f->Eval(x0);
    e[i] = sqrt(e[i])*y;
    // Make Box for Final Errors
    if(box && plot){
      double xg = h->GetBinCenter(i);
      double wg = h->GetBinWidth(1)/2;
      double yg = h->GetBinContent(i);
      double eg = fabs(e[i]);

      int right = (int)merge*((int)(h->GetBinContent(i+1)==0 && i < h->GetNbinsX() - 1))*(2+2*((i == 7))) + 1;
      int left = (int)merge*((int)(h->GetBinContent(i-1)==0 && i > 1))*2 + 1;

      TGraph* gp = new TGraph();
      
      gp->SetLineColor(markerColor[ijet]);
      gp->SetPoint(0,xg-left*wg,yg-eg);
      gp->SetPoint(1,xg-left*wg,yg+eg);
      gp->SetPoint(2,xg+right*wg,yg+eg);
      gp->SetPoint(3,xg+right*wg,yg-eg);
      gp->SetPoint(4,xg-left*wg,yg-eg);
      
      if(yg-eg < 0){
        gp->SetPoint(0,xg-left*wg,0.07);
        gp->SetPoint(3,xg+right*wg,0.07);
        gp->SetPoint(4,xg-left*wg,0.07);
      }
      
      if(opt == 1){
        gp->SetFillColor(errorColor);
	gp->SetFillStyle(theStyle);

	gp->Draw("F same");
//         cout << "x: " << xg << ": " << eg << " low: " << yg-eg << " high: " << yg+eg << endl;
      }else{
        gp->Draw("l same");	  
      }
    }
  }
  
  // Show The Individual Errors
  if (cerr) {
    cerr->cd();
    hErrorTot->SetLineColor(kBlack);
    hErrorTot->Draw("hist same");
    inc->cd();

    TFile outf("finalSys.root","update");
    for(int ie = 0; ie< Nerror; ++ie){
      he[ie]->Write();
      fe[ie]->Write();
    }
    hErrorTot->Write();
    outf.Close();
  }
}


//--------------------------------------------------
int CountDigitsDecimal(float x) {
   using namespace std;
   stringstream out;
   out  << setprecision(3) << fabs(x);
   string s = out.str();
   string t = s.substr(s.find(".")+1);
   return t.length();
}


//--------------------------------------------------
void PrintHistogram(TH1 *h, float * syse=0) {
   using namespace std;
   cout << h->GetName() << " min: " << h->GetMinimum() << " max: " << h->GetMaximum() << endl;
   for (Int_t i=1; i<=h->GetNbinsX(); ++i) {
      Float_t x=h->GetBinCenter(i);
      Float_t y=h->GetBinContent(i);
      Float_t ye= h->GetBinError(i);
      Int_t yndec = CountDigitsDecimal(ye);
      if (syse) {
         yndec = CountDigitsDecimal(std::max(ye,syse[i-1]));
      }
      cout << setprecision(3) << x << " ";
      cout << fixed << setprecision(yndec) << y << " ";
      cout.unsetf(ios::floatfield);
      cout << setprecision(3) << ye << " ";
      if (syse) {
         cout << syse[i-1];
      }
      cout << endl;
   }
}
