#ifndef SAVEDFRANKTOOLS_H
#define SAVEDFRANKTOOLS_H
//#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TString.h"
#include "TRegexp.h"
#include "TFile.h"
#include "TSystem.h"
// private includes
#include "HistMath.C"

const Double_t PI = 3.14159;
const Double_t PI2 = 2*3.14159;

//
// Helper tools for root ana
// Todo
// - change TH1D to TH1D
//

void savedfrankTools() {}
//=================================Helper Functions==================================
//--- Debug helps---
void terminate(char * msg)
{
   printf("===Error:===\n");
   printf("   %s\n",msg);
   exit(1);
}

//--- Find File by name ---
TFile * findFile(const char * fn)
{
   TFile * infile = new TFile(fn);
   if (!infile->IsOpen())
      terminate(Form("%s cannot be opened!",fn));
   return infile;
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
      exit(1);
   }
}

//--- Find TTree by name ---
TTree * findTree(TFile * infile, const char * tn)
{
   TTree * t = dynamic_cast<TTree*>(infile->Get(tn));
   if (!t)
      terminate(Form("%s not found in %s",tn,infile->GetName()));

   return t;
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
   //printf("Print Canvas: %x\n",c);

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
  const int n=1;
  //char * formats[n] = {"gif","C"};
  char * formats[n] = {"gif"};
  TSeqCollection * sc = dynamic_cast<TSeqCollection*>(gROOT->GetListOfCanvases());

  for (int iform=0; iform<n; ++iform) {
    // Prepare dir
    gSystem->mkdir(Form("%s/%s",folder,formats[iform]),kTRUE);
    // save each canvas
    for (int i =0; i<sc->GetSize(); ++i) {
      TCanvas * c = dynamic_cast<TCanvas*>(sc->At(i));
      TString cname(c->GetName());
      //printf("save folder: %s\n",folder);
      printf("Canvas: %s/%s/%s.%s\n",folder,formats[iform],cname.Data(),formats[iform]);
      c->Print(Form("%s/%s/%s.%s",folder,formats[iform],cname.Data(),formats[iform]));
    }
  }
}

//--- Creat Histogram ---
TH1D * createHist(const char* name, const char* title, const int nbin, const double min, const double max)
{
   // Save the errors, so that later when scale or divided by another histo,
   // the errors will scale with the same factor and not recalculated after scale.
   // cf "Filling Histograms" in root manual
   TH1D * h = new TH1D(name,title,nbin,min,max);
   h->Sumw2();
   return h;
}

//--- Set Histogram ---
void setHist(TH1* h, const Color_t lc=0, const Style_t ls=0, const Width_t lw=0, const Size_t msz=0, const Style_t mst=0, const double norm = -1, const char* xtitle = "", const char* ytitle = "", const double ymax=0)
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
      Double_t binNorm = 1./h->GetBinWidth(1);
      h->Scale(binNorm*norm);
   }

   //--- set y scale ---
   if (ymax) h->SetAxisRange(0,ymax,"Y");
}

void setHist(const char* name, const Color_t lc=0, const Style_t ls=0, const Width_t lw=0, const Size_t msz=0, const Style_t mst=0, const double norm = -1, const char* xtitle = "", const char* ytitle = "", const double ymax=0)
{
   TH1D * h;
   if (h=dynamic_cast<TH1D*>(findHist(name))) setHist(h,lc,ls,lw,msz,mst,norm,xtitle,ytitle,ymax);
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
// draw tree helpers
Int_t drawTreeBasic(TTree* tr,const char* draw, const char * cut, const char* opt,const char* hname)
{
   // -- define draw command --
   TString drawcmd(Form("%s>>%s",draw,hname));
   Double_t n = tr->Draw(drawcmd, cut, opt);
   return n;
}

//--- function to draw 1D histograms from TTree ---
TH1D * drawTree(TTree* nt, const char* draw, const char* cut, const char* opt, const char* name, const char* title, const int nbin, const double min, const double max, bool log=false, const Color_t lc=0, const Style_t ls=0, const Width_t lw=0, const Size_t msz =0, const Style_t mst =0, double norm=-1.,const double ymax=0)
{
   //--- Print some info ---
   if (!TString(opt).Contains("same")) printf("\n");
   printf("%s, tree: %x. Draw: %s. Norm: %f\n", name, nt, draw, norm);

   //--- Make/set histogram ---
   printf("hist: %s %d %f %f, c: %d\n",name,nbin,min,max,lc);
   TH1D * h = createHist(name, title, nbin, min, max);

   //--- Draw Hist, get entries past cut ---
   TCanvas * c = makeCanvas(name,title,log,opt);
   Double_t n = drawTreeBasic(nt,draw, cut, opt,name);
   printf("%s has: %f entries\n",name,h->GetEntries());

   //--- Set Hist ---
   setHist(h,lc,ls,lw,msz,mst,norm,"","",ymax);

   //--- Draw final hist ---
   h->Draw(opt);

   return h;
}

//--- function to draw 2D histograms from TTree ---
void drawTree2(TTree* nt, const char* draw, const char* cut, const char* opt, const char* name, const char* title, const int nxbin, const double xmin, const double xmax, const int nybin, const double ymin, const double ymax,  UInt_t log=0)
{
   //--- Print some info ---
   if (!TString(opt).Contains("same")) printf("\n");
   printf("%s, tree: %x. Draw: %s\n", name, nt, draw);

   //--- Make/set histogram ---
   printf("hist: %s %d %f %f %d %f %f\n",name,nxbin,xmin,xmax,nybin,ymin,ymax);
   TH2D * h = new TH2D(name,title,nxbin,xmin,xmax,nybin,ymin,ymax);

   //--- Draw ---
   TCanvas * c = makeCanvas(name,title,false,opt);
   if (!TString(opt).Contains("same") && log) c->SetLogz();
   Double_t n = drawTreeBasic(nt,draw, cut, opt,name);
   printf("%s has: %f entries\n",name,h->GetEntries());
}

//--- function to divide histograms then draw---
void drawDivHist(const char* hn1, const char* hn2, const char* opt, const char* name, const char* title, const int nbin, const double min, const double max, bool log=false, const Color_t lc=0, const Style_t ls=0, const Width_t lw=0, const Size_t msz=0, const Style_t mst=0, const double ymax=0, int mode=0)
{
   // find input histos
   if (!TString(opt).Contains("same")) printf("\n");
   printf("h1: %s. h2: %s. Draw: %s\n", hn1, hn2, name);
   TH1D * h1; TH1D * h2;
   if (gROOT->FindObject(hn1))
      h1 = dynamic_cast<TH1D*>(gROOT->FindObject(hn1));
   else {
      printf("%s is not found, please check the histogram name\n", hn1);
      return;
   }
   if (gROOT->FindObject(hn2))
      h2 = dynamic_cast<TH1D*>(gROOT->FindObject(hn2));
   else {
      printf("%s is not found, please check the histogram name\n", hn2);
      return;
   }

   //--- Make/set histogram ---
   printf("hist: %s %d %f %f\n",name,nbin,min,max);
   TH1D * h = createHist(name, title, nbin, min, max);
   setHist(h,lc,ls,lw,msz,mst);

   //--- Action ---
   h->Divide(h1,h2);

   //--- Draw ---
   if (ymax) h->SetAxisRange(0,ymax,"Y");
   if (mode==0) TCanvas * c = makeCanvas(name,title,log,opt);
   h->Draw(opt);
}

//--- function to draw histograms ---
void drawNormHist(TH1* h, const char* opt="", const char* title="", const char* xtitle = "", const char* ytitle = "", const double norm = -1, bool log = false, const Color_t lc=0, const Style_t ls=0, const Width_t lw=0, const Size_t msz=0, const Style_t mst=0, const double ymax=0, int mode=0)
{
   setHist(h,lc,ls,lw,msz,mst,norm,xtitle,ytitle,ymax);
   //printf("---------------draw: %s\n",h->GetName());
   if (mode==0) makeCanvas(Form("normalized_%s",h->GetName()),title, log,opt);
   h->Draw(opt);
}
void drawNormHist(const char* hn, const char* opt="", const char* title="", const char* xtitle = "", const char* ytitle = "", const double norm = -1, bool log = false, const Color_t lc=0, const Style_t ls=0, const Width_t lw=0, const Size_t msz=0, const Style_t mst=0, const double ymax=0, int mode=0)
{
   TH1 * h;
   if (gROOT->FindObject(hn))
      h = dynamic_cast<TH1*>(gROOT->FindObject(hn));
   else {
      printf("**%s is not found, please check the histogram name\n", hn);
      return;
   }
   drawNormHist(h,opt,title,xtitle,ytitle,norm,log,lc,ls,lw,msz,mst,ymax,mode);
}
TH1 * drawNormHist(TFile * f, const char* hn, const char* opt="", const char* title="", const char* xtitle = "", const char* ytitle = "", const double norm = -1, bool log = false, const Color_t lc=0, const Style_t ls=0, const Width_t lw=0, const Size_t msz=0, const Style_t mst=0, const double ymax=0, int mode=0)
{
   TH1 * h;
   if (f->Get(hn))
      h = dynamic_cast<TH1*>(f->Get(hn));
   else {
      printf("**%s is not found, please check the histogram name\n", hn);
      return 0;
   }
   printf("retrieved: %s\n", h->GetName());
   drawNormHist(h,opt,title,xtitle,ytitle,norm,log,lc,ls,lw,msz,mst,ymax,mode);
   return h;
}

// === draw integral of histograms ===
TH1 * drawIntHist(const char* hinName, const char* houtName, const char* opt="", const char* title="", const char* xtitle = "", const char* ytitle = "",
                 const double norm = -1, int log = 0, const Color_t lc=0, const Style_t ls=0, const Width_t lw=0, const Size_t msz=0, const Style_t mst=0,
		 const double ymax=0)
{
   TH1D * hin = (TH1D*)findHist(hinName);

   // create integrated histogram
   TH1D * hout = integrateHist(hin);
   hout->SetName(houtName);
   hout->SetTitle(title);

   // set integrated histogram properties
   drawNormHist(hout,opt,title,xtitle,ytitle,norm,log,lc,ls,lw,msz,mst,ymax);
   return hout;
}
#endif //SAVEDFRANKTOOLS_H
