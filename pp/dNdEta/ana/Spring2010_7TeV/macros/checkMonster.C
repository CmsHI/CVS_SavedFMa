#include "aliases.C"

void checkMonster(TTree * PixelTree, int source=0)
{
  // set come alisas
  aliases(PixelTree);
  TCut cutBeforeEds, evtSel;
  TString titleBeforeEds, titleLoose,outDir;
  if (source==0) {
    cutBeforeEds = "bscSelColBx&&vz[1]>-99";
    titleBeforeEds = "Run 132440 -- L1A124,BPTX_AND,noHalo,BSC_OR,vtx";
    outDir="plots/inspect/data_coll";
  } else if (source==1) {
    cutBeforeEds = "bscSelMc && vz[1]>-99";
    titleBeforeEds = "PythiaD6T -- noHalo,BSC_OR,vtx";
    outDir="plots/inspect/mc";
  } else if (source==2) {
    cutBeforeEds = "bscSelEmpBx && vz[1]>-99";
    titleBeforeEds = "Run 132422 (empty bx) -- L1A124,noHalo,BSC_OR,vtx";
    outDir="plots/inspect/data_emptyBx";
  }
  evtSel = cutBeforeEds && "edscut2";
  titleEvtSel = titleBeforeEds+",Ed's cut";

  // Printout some stat
  cout << "===== Sel Info =====" << endl;
  cout << "Total # of Events: " << PixelTree->GetEntries() << endl;
  cout << "Passing " << PixelTree->GetAlias("trigger") << ": " << PixelTree->GetEntries("trigger") << endl;
  cout << "Passing " << PixelTree->GetAlias("bscSelEmpBx") << ": " << PixelTree->GetEntries("bscSelEmpBx") << endl;
  cout << "Passing " << TString(cutBeforeEds) << ": " << PixelTree->GetEntries(cutBeforeEds) << endl;
  cout << "Passing " << TString(evtSel) << ": " << PixelTree->GetEntries(evtSel) << endl;

  // Draw
  PixelTree->Draw("vtxqual:npxhits>>hVtxQual(50,0,1500,50,0,10)",cutBeforeEds,"goff");
  hVtxQual->SetTitle(titleBeforeEds+"; # of pixel hits; cluster-vertex compatibility");
  hVtxQual->SetMinimum(1); hVtxQual->SetMaximum(1000);

  PixelTree->Draw("vtxqual:npxhits>>hVtxQualCut(50,0,1500,50,0,10)",evtSel,"goff");
  hVtxQualCut->SetTitle(titleEvtSel+"; # of pixel hits; cluster-vertex compatibility");
  hVtxQualCut->SetMinimum(1); hVtxQualCut->SetMaximum(1000);

  //---------------Monster Cut---------------------
  // Check cut
  Double_t x[3] = {150,150,1000};
  Double_t y[3] = {0,0.5+0.0045*150,0.5+0.0045*1000};
  TPolyLine *pline = new TPolyLine(3,x,y);
  pline->SetLineColor(2);
  pline->SetLineWidth(2);

  Double_t x2[4] = {150,150,2/0.0045,1000};
  Double_t y2[4] = {0,0.0045*150,2,2};
  TPolyLine *pline2 = new TPolyLine(4,x2,y2);
  pline2->SetLineColor(2);
  pline2->SetLineWidth(2);
  pline2->SetLineStyle(2);

  TCanvas *cBeforeEdsCut = new TCanvas("cBeforeEdsCut","cBeforeEdsCut",500,500);
  hVtxQual->Draw("colz");
  gPad->SetRightMargin(0.15);
  gPad->SetLogz();

  pline->Draw("same");
  pline2->Draw("same");
  gSystem->mkdir(outDir.Data(),kTRUE);
  cBeforeEdsCut->Print(Form("%s/checkMonster.gif",outDir.Data()));

  // Apply Cut
  TCanvas *cAfterEdsCut = new TCanvas("cAfterEdsCut","cAfterEdsCut",500,500);
  hVtxQualCut->Draw("colz");
  gPad->SetRightMargin(0.15);
  gPad->SetLogz();

  pline->Draw("same");
  pline2->Draw("same");
}

