#include "TFile.h"
#include "/home/frankma/UserCode/SavedFMa/analysis/root/macros/savedfrankTools.C"

void plotHiDistr (const char* infname="hipat_hievent.root")
{
  TFile * infile = new TFile(infname);
  drawNormHist(infile,"genjetPt","","","gen jet Pt [GeV]","#",1.,1);
  drawNormHist(infile,"genjetEta","","","gen jet Eta","#",1.,0);
  drawNormHist(infile,"genjetPhi","","","gen jet Phi","#",1.,0);

  drawNormHist(infile,"rawjetPt","","","calo jet Pt [GeV]","#",1.,1);

  drawNormHist(infile,"jetPt","","","calo jet Pt [GeV]","#",1.,1);
  drawNormHist(infile,"jetEta","","","calo jet Eta","#",1.,0);
  drawNormHist(infile,"jetPhi","","","calo jet Phi","#",1.,0);

  drawNormHist(infile,"jetDR","box","calo/gen jet dR","dR","pt",1.,0);
  drawNormHist(infile,"matjetDR","box","matched calo/genjet dR","dR","pt",1.,0);
  drawNormHist(infile,"jetPtCorrel","colz","","calo vs gen jet Pt [GeV]","#",1.,0);

  drawNormHist(infile,"partonPt","","","parton Pt [GeV]","#",1.,1);
  drawNormHist(infile,"partonEta","","","parton Eta","#",1.,0);
  drawNormHist(infile,"partonPhi","","","parton Phi","#",1.,0);

  drawNormHist(infile,"partlPt","","","gen partl Pt [GeV]","#",1.,1);
  drawNormHist(infile,"partlEta","","","gen partl Eta","#",1.,0);
  drawNormHist(infile,"partlPhi","","","gen partl Phi","#",1.,0);

  drawNormHist(infile,"trackPt","","","track Pt [GeV]","#",1.,1);
  drawNormHist(infile,"trackEta","","","track Eta","#",1.,0);
  drawNormHist(infile,"trackPhi","","","track Phi","#",1.,0);

  drawNormHist(infile,"pptrackPt","","","pptrack Pt [GeV]","#",1.,1);
  drawNormHist(infile,"pptrackEta","","","pptrack Eta","#",1.,0);
  drawNormHist(infile,"pptrackPhi","","","pptrack Phi","#",1.,0);

  drawNormHist(infile,"genb","","","gen b","",1.,0);
  drawNormHist(infile,"gennpart","","","gen npart","",1.,0);
  drawNormHist(infile,"genncoll","","","gen ncoll","",1.,0);
  printAllCanvases("plots");
}
