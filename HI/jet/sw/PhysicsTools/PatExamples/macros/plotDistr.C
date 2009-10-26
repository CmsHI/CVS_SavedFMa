#include "TFile.h"
#include "/home/frankma/UserCode/SavedFMa/analysis/root/macros/savedfrankTools.C"

void plotDistr (const char* infname="analyzePatBasics.root")
{
  TFile * infile = new TFile(infname);
  drawNormHist(infile,"analyzeBasicPat/jetPt","","","calo jet Pt [GeV]","#",1.,1);
  drawNormHist(infile,"analyzeBasicPat/jetEta","","","calo jet Eta","#",1.,0);
  drawNormHist(infile,"analyzeBasicPat/jetPhi","","","calo jet Phi","#",1.,0);
  drawNormHist(infile,"analyzeBasicPat/jetDR","box","calo/gen jet dR","dR","pt",1.,0);
  drawNormHist(infile,"analyzeBasicPat/matjetDR","box","matched calo/genjet dR","dR","pt",1.,0);
  drawNormHist(infile,"analyzeBasicPat/jetPtCorrel","","","calo vs gen jet Pt [GeV]","#",1.,0);

  drawNormHist(infile,"analyzeBasicPat/partlPt","","","gen partl Pt [GeV]","#",1.,1);
  drawNormHist(infile,"analyzeBasicPat/partlEta","","","gen partl Eta","#",1.,0);
  drawNormHist(infile,"analyzeBasicPat/partlPhi","","","gen partl Phi","#",1.,0);

  drawNormHist(infile,"analyzeBasicPat/trackPt","","","track Pt [GeV]","#",1.,1);
  drawNormHist(infile,"analyzeBasicPat/trackEta","","","track Eta","#",1.,0);
  drawNormHist(infile,"analyzeBasicPat/trackPhi","","","track Phi","#",1.,0);

  printAllCanvases("plots");
}
