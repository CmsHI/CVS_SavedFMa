#include "TString.h"
#include "TRegexp.h"

const Float_t PI = 3.14159;

//--- Print Canvas with a given name ---
void printCanvas(const char* name, const char* title, TCanvas * c)
{
   TString ttitle(title);
   TRegexp re("draw");
//   cout << ttitle << " contains: " << "draw" << "?   " << ttitle.Contains(re) << endl;
   cout << "draw " << name << "?   " << ttitle.Contains(re) << endl;
   if (ttitle.Contains(re)) {
      c->Print(Form("plots/%s.gif",name));
   }
}

//--- function to draw histograms from TTree ---
void drawTree(TTree* nt, const char* draw, const char* cut, const char* opt, const char* name, const char* title, const int nbin, const float min, const float max, bool log=false, const int lc=0, const int ls=0, const int lw=0)
{
   printf("%s, tree: %d. Draw: %s\n", name, nt, draw);
//   nt->Print();
   printf("hist: %s %d %f %f\n",name,nbin,min,max);
   TH1F * h = new TH1F(name, title, nbin, min, max);
   TCanvas * c;
   if (strcmp(opt,"same")!=0) {
      c = new TCanvas(Form("c%s",name),title,1000,700);
      if (log) c->SetLogy();
   }

//   printf("color %d\n",lc);
   if (lc!=0) h->SetLineColor(lc);
   if (ls!=0) h->SetLineStyle(ls);
   if (lw!=0) h->SetLineWidth(lw);

   nt->Draw(draw, cut, opt);

   //--- save plot ---
   printCanvas(name,title, c);
}

//--- function to draw divided histograms ---
void drawDivHist(const char* hn1, const char* hn2, const char* opt, const char* name, const char* title, const int nbin, const float min, const float max, bool log=false, const int lc=0, const int ls=0, const int lw=0)
{
   // find input histos
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

   printf("hist: %s %d %f %f\n",name,nbin,min,max);
   TH1F * h = new TH1F(name, title, nbin, min, max);
   TCanvas * c;
   if (strcmp(opt,"same")!=0) {
      c = new TCanvas(Form("c%s",name),title,1000,700);
      if (log) c->SetLogy();
   }

//   printf("color %d\n",lc);
   if (lc!=0) h->SetLineColor(lc);
   if (ls!=0) h->SetLineStyle(ls);
   if (lw!=0) h->SetLineWidth(lw);

   h->Divide(h1,h2);
   h->Draw(opt);

   //--- save plot ---
   printCanvas(name, title, c);
}
