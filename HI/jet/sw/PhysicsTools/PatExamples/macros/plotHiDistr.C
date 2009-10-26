#include "TFile.h"
#include "/home/frankma/UserCode/SavedFMa/analysis/root/macros/savedfrankTools.C"

void plotHiDistr (const char* infname="hipat_hievent.root")
{
  TFile * infile = new TFile(infname);
  drawNormHist(infile,"genb","","","gen b","",1.,0);
  drawNormHist(infile,"gennpart","","","gen npart","",1.,0);
  drawNormHist(infile,"genncoll","","","gen ncoll","",1.,0);
  printAllCanvases("plots");
}
