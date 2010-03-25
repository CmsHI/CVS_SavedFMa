#include "TLatex.h"

void printFinalCanvases(TCanvas * MyCanvas, const char * name, int log=0, int doCMS=2)
{
  MyCanvas->cd();
  MyCanvas->SetLogy(log);
  printf("canvas name: %s\n",name);

  // add some text labels
  double ndcX = 0.2;
  double ndcY = 0.894;
  TLatex * tex;

  if (doCMS>0) {
    tex = new TLatex(0.657,ndcY,"CMS Preliminary");
    tex->SetTextSize(0.04);
    tex->SetLineWidth(2);
    tex->SetNDC();
    tex->Draw();

    if (doCMS==1) {
      //TLatex *tex2 = new TLatex(0.88-0.132,ndcY-0.045,"pp #sqrt{s}=0.9 TeV");
      TLatex *tex2 = new TLatex(0.774,ndcY-0.045,"p+p");
      tex2->SetTextSize(0.04);
      tex2->SetLineWidth(2);
      tex2->SetNDC();
      tex2->Draw();
    }
  }

  MyCanvas->Print(Form("%s.eps",name));
  MyCanvas->Print(Form("%s.gif",name));
  MyCanvas->Print(Form("%s.pdf",name));
  /*

  tex = new TLatex(ndcX,ndcY,"(a)");
  tex->SetTextSize(0.04);
  tex->SetLineWidth(2);
  tex->SetNDC();
  tex->Draw();

  MyCanvas->Print(Form("%s_A.eps",name));
  MyCanvas->Print(Form("%s_A.gif",name));
  MyCanvas->Print(Form("%s_A.pdf",name));

  if (tex) tex->Delete();
  tex = new TLatex(ndcX,ndcY,"(b)");
  tex->SetTextSize(0.04);
  tex->SetLineWidth(2);
  tex->SetNDC();
  tex->Draw();

  MyCanvas->Print(Form("%s_B.eps",name));
  MyCanvas->Print(Form("%s_B.gif",name));
  MyCanvas->Print(Form("%s_B.pdf",name));
  */
}
