#include "TString.h"
#include "TRegexp.h"

const Float_t PI = 3.14159;

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
   cout << "draw " << name << "?   " << ttitle.Contains(re) << endl;
   if (ttitle.Contains(re)) {
      c->Print(Form("plots/%s.gif",name));
   }
}

//--- Print all created canvasses ---
void printAllCanvases()
{
   TSeqCollection * sc = dynamic_cast<TSeqCollection*>(gROOT->GetListOfCanvases());
   for (int i =0; i<sc->GetSize(); ++i) {
      TCanvas * c = dynamic_cast<TCanvas*>(sc->At(i));
      TString cname(c->GetName());
      printf("Canvas: %s\n",cname.Data());
      c->Print(Form("plots/%s.gif",cname.Data()));
   }
}

//--- Set Histogram ---
void setHist(TH1* h, const int lc=0, const int ls=0, const int lw=0, const char* xtitle = "", const char* ytitle = "", const int norm = 1)
{
   //   printf("color %d\n",lc);
   if (lc!=0) h->SetLineColor(lc);
   if (ls!=0) h->SetLineStyle(ls);
   if (lw!=0) h->SetLineWidth(lw);

}

//--- Make Canvas ---
TCanvas * makeCanvas(const char* name, const char* title, bool log=false, const char* opt="", const int dx = 1000, const int dy = 700)
{
   //printf("Canvas opt: %s\n", opt);
   TCanvas * c;
   TString cname(Form("c%s",name));

   if (strcmp(opt,"same")!=0) {
      c = new TCanvas(cname.Data(),title,dx,dy);
//      printf("Canvas: %d\n",c);
      if (log) c->SetLogy();
   }
//   printf("Canvas %s: %d\n",cname.Data(), c);
   return c;
}

//--- function to draw histograms from TTree ---
void drawTree(TTree* nt, const char* draw, const char* cut, const char* opt, const char* name, const char* title, const int nbin, const float min, const float max, bool log=false, const int lc=0, const int ls=0, const int lw=0)
{
   //--- Print some info ---
   if (strcmp(opt,"same")!=0) printf("\n");
   printf("%s, tree: %d. Draw: %s\n", name, nt, draw);
//   nt->Print();

   //--- Make/set histogram ---
   printf("hist: %s %d %f %f\n",name,nbin,min,max);
   TH1F * h = new TH1F(name, title, nbin, min, max);
   setHist(h,lc,ls,lw);

   //--- Draw ---
   TCanvas * c = makeCanvas(name,title,log,opt);
   nt->Draw(draw, cut, opt);
   printf("%s has: %f entries\n",name,h->GetEntries());
}

//--- function to draw divided histograms ---
void drawDivHist(const char* hn1, const char* hn2, const char* opt, const char* name, const char* title, const int nbin, const float min, const float max, bool log=false, const int lc=0, const int ls=0, const int lw=0)
{
   // find input histos
   if (strcmp(opt,"same")!=0) printf("\n");
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
   TH1F * h = new TH1F(name, title, nbin, min, max);
   setHist(h,lc,ls,lw);

   //--- Action ---
   h->Divide(h1,h2);

   //--- Draw ---
   TCanvas * c = makeCanvas(name,title,log,opt);
   h->Draw(opt);
}
