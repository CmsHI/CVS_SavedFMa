//#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TString.h"
#include "TRegexp.h"
#include "TFile.h"

const Float_t PI = 3.14159;
const Float_t PI2 = 2*3.14159;

void savedfrankTools() {}
//=================================Helper Functions==================================
//--- Debug helps---
void terminate(char * msg)
{
   printf("Error:\n");
   printf("---%s\n",msg);
   exit(1);
}

//--- Find Histogram by name ---
TH1* findHist(const char* hn1)
{
   TH1* h;
   if (gROOT->FindObject(hn1)) {
      h = dynamic_cast<TH1*>(gROOT->FindObject(hn1));
//      printf("hist: %d\n",h);
      return h;
   }
   else {
      printf("%s is not found, please check the histogram name\n", hn1);
      return 0;
   }
}

//--- Print Canvas with a given name ---
void printCanvas(const char* name, const char* title)//, TCanvas * c)
{
   TCanvas * c;
   TString cname(Form("c%s",name));
   if (gROOT->FindObject(Form("c%s",name)))
      c = dynamic_cast<TCanvas*>(gROOT->FindObject(Form("c%s",name)));
   else {
      printf("canvas for %s is not found, please check the canvas name\n", cname.Data());
      return;
   }
   printf("Print Canvas: %d\n",c);

   TString ttitle(title);
   TRegexp re("draw");
//   cout << ttitle << " contains: " << "draw" << "?   " << ttitle.Contains(re) << endl;
//   cout << "draw " << name << "?   " << ttitle.Contains(re) << endl;
   if (ttitle.Contains(re)) {
      c->Print(Form("plots/%s.gif",name));
   }
}

//--- Print all created canvasses ---
void printAllCanvases(const char* folder="plots")
{
   TSeqCollection * sc = dynamic_cast<TSeqCollection*>(gROOT->GetListOfCanvases());
   for (int i =0; i<sc->GetSize(); ++i) {
      TCanvas * c = dynamic_cast<TCanvas*>(sc->At(i));
      TString cname(c->GetName());
      printf("Canvas: %s/%s.gif\n",folder,cname.Data());
      c->Print(Form("%s/%s.gif",folder,cname.Data()));
   }
}

//--- Creat Histogram ---
TH1F * createHist(const char* name, const char* title, const int nbin, const float min, const float max)
{
   // Save the errors, so that later when scale or divided by another histo,
   // the errors will scale with the same factor and not recalculated after scale.
   // cf "Filling Histograms" in root manual
   TH1F * h = new TH1F(name,title,nbin,min,max);
   h->Sumw2();
   return h;
}

//--- Set Histogram ---
void setHist(TH1* h, const int lc=0, const int ls=0, const int lw=0, const int msz=0, const int mst=0, const double norm = -1, const char* xtitle = "", const char* ytitle = "")
{
   //--- Set histo properties ---
   if (lc!=0) h->SetLineColor(lc);
   if (ls!=0) h->SetLineStyle(ls);
   if (lw!=0) h->SetLineWidth(lw);
   if (lc!=0) h->SetMarkerColor(lc);
   if (msz!=0) h->SetMarkerSize(msz);
   if (mst!=0) h->SetMarkerStyle(mst);
   //printf("%s %s %f\n",xtitle,ytitle,norm);
   if (strcmp(xtitle,"")!=0)   h->SetXTitle(xtitle);
   if (strcmp(ytitle,"")!=0)   h->SetYTitle(ytitle);

   //--- Normalize ---
   if (norm>0.) {
      printf("Scale: %s by %f\n",h->GetName(),norm);
      Float_t binNorm = 1./h->GetBinWidth(1);
      h->Scale(binNorm*norm);
   }
}
void setHist(const char* name, const int lc=0, const int ls=0, const int lw=0, const int msz=0, const int mst=0, const double norm = -1, const char* xtitle = "", const char* ytitle = "")
{
   TH1F * h;
   if (h=dynamic_cast<TH1F*>(findHist(name))) setHist(h,lc,ls,lw,msz,mst,norm,xtitle,ytitle);
}

//--- Make Canvas ---
TCanvas * makeCanvas(const char* name, const char* title, bool log=false, const char* opt="", const int dx = 800, const int dy = 800)
{
   //printf("Canvas opt: %s\n", opt);
   TCanvas * c;
   TString cname(Form("c%s",name));

   if (!TString(opt).Contains("same")) {
      c = new TCanvas(cname.Data(),title,dx,dy);
//      printf("Canvas: %d\n",c);
      if (log) c->SetLogy();
   }
//   printf("Canvas %s: %d\n",cname.Data(), c);
   return c;
}

//=============================== Main Functions =====================================
//--- function to draw 1D histograms from TTree ---
Float_t drawTree(TTree* nt, const char* draw, const char* cut, const char* opt, const char* name, const char* title, const int nbin, const float min, const float max, bool log=false, const int lc=0, const int ls=0, const int lw=0, const int msz =0, const int mst =0, float norm=-1.)
{
   //--- Print some info ---
   if (!TString(opt).Contains("same")) printf("\n");
   printf("%s, tree: %d. Draw: %s. Norm: %f\n", name, nt, draw, norm);
//   nt->Print();

   //--- Make/set histogram ---
   printf("hist: %s %d %f %f\n",name,nbin,min,max);
   TH1F * h = createHist(name, title, nbin, min, max);

   //--- Draw Hist, get entries past cut ---
   TCanvas * c = makeCanvas(name,title,log,opt);
   Float_t n = nt->Draw(draw, cut, opt);
   printf("%s has: %f entries\n",name,h->GetEntries());

   //--- Set Hist ---
   setHist(h,lc,ls,lw,msz,mst,norm);

   //--- Draw final hist ---
   h->Draw(opt);

   return n;
}
//--- function to draw 2D histograms from TTree ---
void drawTree2(TTree* nt, const char* draw, const char* cut, const char* opt, const char* name, const char* title, const int nxbin, const float xmin, const float xmax, const int nybin, const float ymin, const float ymax,  UInt_t log=0)
{
   //--- Print some info ---
   if (!TString(opt).Contains("same")) printf("\n");
   printf("%s, tree: %d. Draw: %s\n", name, nt, draw);
//   nt->Print();

   //--- Make/set histogram ---
   printf("hist: %s %d %f %f %d %f %f\n",name,nxbin,xmin,xmax,nybin,ymin,ymax);
   TH2D * h = new TH2D(name,title,nxbin,xmin,xmax,nybin,ymin,ymax);

   //--- Draw ---
   TCanvas * c = makeCanvas(name,title,false,opt);
   if (!TString(opt).Contains("same") && log) c->SetLogz();
   nt->Draw(draw, cut, opt);
   printf("%s has: %f entries\n",name,h->GetEntries());
}

//--- function to divide histograms then draw---
void drawDivHist(const char* hn1, const char* hn2, const char* opt, const char* name, const char* title, const int nbin, const float min, const float max, bool log=false, const int lc=0, const int ls=0, const int lw=0, const int msz=0, const int mst=0, const float ymax=0)
{
   // find input histos
   if (!TString(opt).Contains("same")) printf("\n");
   printf("h1: %s. h2: %s. Draw: %s\n", hn1, hn2, name);
   TH1F * h1; TH1F * h2;
   if (gROOT->FindObject(hn1))
      h1 = dynamic_cast<TH1F*>(gROOT->FindObject(hn1));
   else {
      printf("%s is not found, please check the histogram name\n", hn1);
      return;
   }
   if (gROOT->FindObject(hn2))
      h2 = dynamic_cast<TH1F*>(gROOT->FindObject(hn2));
   else {
      printf("%s is not found, please check the histogram name\n", hn2);
      return;
   }

   //--- Make/set histogram ---
   printf("hist: %s %d %f %f\n",name,nbin,min,max);
   TH1F * h = createHist(name, title, nbin, min, max);
   setHist(h,lc,ls,lw,msz,mst);

   //--- Action ---
   h->Divide(h1,h2);

   //--- Draw ---
   if (ymax) h->SetAxisRange(0,ymax,"Y");
   h->SetStats(0);
   TCanvas * c = makeCanvas(name,title,log,opt);
   h->Draw(opt);
}

//--- function to draw histograms ---
void drawNormHist(TH1* h, const char* opt="", const char* title="", const char* xtitle = "", const char* ytitle = "", const double norm = -1, bool log = false, const int lc=0, const int ls=0, const int lw=0, const int msz=0, const int mst=0)
{
   setHist(h,lc,ls,lw,msz,mst,norm,xtitle,ytitle);
   makeCanvas(Form("normalized_%s",h->GetName()),title, log,opt);
   h->Draw(opt);
}
void drawNormHist(const char* hn, const char* opt="", const char* title="", const char* xtitle = "", const char* ytitle = "", const double norm = -1, bool log = false, const int lc=0, const int ls=0, const int lw=0, const int msz=0, const int mst=0)
{
   TH1 * h;
   if (gROOT->FindObject(hn))
      h = dynamic_cast<TH1*>(gROOT->FindObject(hn));
   else {
      printf("**%s is not found, please check the histogram name\n", hn);
      return;
   }
   drawNormHist(h,opt,title,xtitle,ytitle,norm,log,lc,ls,lw,msz,mst);
}
TH1 * drawNormHist(TFile * f, const char* hn, const char* opt="", const char* title="", const char* xtitle = "", const char* ytitle = "", const double norm = -1, bool log = false, const int lc=0, const int ls=0, const int lw=0, const int msz=0, const int mst=0)
{
   TH1 * h;
   if (f->Get(hn))
      h = dynamic_cast<TH1*>(f->Get(hn));
   else {
      printf("**%s is not found, please check the histogram name\n", hn);
      return 0;
   }
   printf("retrieved: %s", h->GetName());
   drawNormHist(h,opt,title,xtitle,ytitle,norm,log,lc,ls,lw,msz,mst);
   return h;
}

