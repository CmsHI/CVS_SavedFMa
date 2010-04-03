#include "aliases.C"

void checkHFnHit(TTree * PixelTree, int source=0)
{
  // set come alisas
  aliases(PixelTree);
  TCut evtSel, cutLoose;
  TString titleSel, titleLoose, outDir;
  if (source==0) {
    evtSel = "anaSelColBx";
    titleSel = "Run 132440 -- L1A124, BPTX_AND,noHalo,BSC_OR,vtx,ed's cut";
    outDir="plots/inspect/data_coll";
  } else if (source==1) {
    evtSel = "anaSelMc";
    titleSel = "PythiaD6T -- noHalo, BSC_OR,vtx,ed's cut";
    outDir="plots/inspect/mc";
  } else if (source==2) {
    evtSel = "anaSelEmpBx";
    titleSel = "Run 132422 (empty bx) -- L1A124,noHalo,BSC_OR,vtx,ed's cut";
    outDir="plots/inspect/data_emptyBx";
  }

  // Printout some stat
  cout << "===== Sel Info =====" << endl;
  cout << "Total # of Events: " << PixelTree->GetEntries() << endl;
  cout << "Passing " << TString(evtSel) << ": " << PixelTree->GetEntries(evtSel) << endl;
  cout << "Passing " << TString(evtSel)+"+HF Coinc." << ": " << PixelTree->GetEntries(evtSel&&"nHFp>0&&nHFn>0") << endl;

  Int_t NBINS=35;
  TH2D * hPM = new TH2D("hPM",titleSel+";# hit HF- towers;# hit HF+ towers",NBINS,0,NBINS,NBINS,0,NBINS);

  TCanvas * cHFnHitpm = new TCanvas("cHFnHitpm","cHFnHitpm",500,500);
  PixelTree->Draw("nHFp:nHFn>>hPM",evtSel,"colz");
  gPad->SetRightMargin(0.15);
  gPad->SetLogz();

  TLine * l1 = new TLine(1,1,NBINS,1);
  l1->SetLineColor(kRed);
  l1->SetLineWidth(3);
  l1->SetLineStyle(7);
  TLine * l2 = new TLine(1,1,1,NBINS);
  l2->SetLineColor(kRed);
  l2->SetLineWidth(3);
  l2->SetLineStyle(7);
  l1->Draw();
  l2->Draw();

  gSystem->mkdir(outDir.Data(),kTRUE);
  cHFnHitpm->Print(Form("%s/nTowerCount.gif",outDir.Data()));
}
