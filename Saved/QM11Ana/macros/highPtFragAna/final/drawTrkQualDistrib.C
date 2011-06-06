#include "../../commonUtility.h"
#include "../../HisMath.C"

void drawQual(TFile * inf, TCanvas * can, Int_t ic, Float_t ymin, Float_t ymax, Int_t doLog, Float_t xmin=0, Float_t xmax=0, Int_t rebinFactor=2)
{
	cout << "ic: " << ic << " ymin: " << ymin << " ymax " << ymax << " xmin " << xmin	<< " xmax " << xmax << endl;
	TH3F * hQual3D_precut_mc = (TH3F*)inf->FindObjectAny(Form("hTrkQual%dPreCut3D_mc80_c0to36",ic));
	TH3F * hQual3D_precut_data = (TH3F*)inf->FindObjectAny(Form("hTrkQual%dPreCut3D_dataj35_c0to36",ic));
	cout << hQual3D_precut_mc->GetName() << ": " << hQual3D_precut_mc << endl;
	cout << hQual3D_precut_data->GetName() << ": " << hQual3D_precut_data << endl;
	TH1D * hPtBin = (TH1D*)hQual3D_precut_data->Project3D("y");
	TH1D * hQual_precut_mc[10][8], * hQual_precut_data[10][8];
	Int_t colors[10] = {kBlack, kGray+2, kBlue+2,kCyan+2,kGreen+2,kYellow+2,kOrange+2,kRed};
	
	for (Int_t p=0; p<=9; ++p){
		for (Int_t c=0; c<=7; ++c) {
			Int_t ptBin=p;
			Int_t cBin=c;
			hQual3D_precut_mc->GetYaxis()->SetRange(ptBin,ptBin);
			//hQual3D_precut_mc->GetZaxis()->SetRange(cBin,cBin+5);
			hQual3D_precut_data->GetYaxis()->SetRange(ptBin,ptBin);
			//hQual3D_precut_data->GetZaxis()->SetRange(cBin,cBin+5);

			hQual_precut_mc[p][c] = (TH1D*)hQual3D_precut_mc->Project3D("x");
			hQual_precut_mc[p][c]->SetName(Form("%s_p%d_c%d",hQual3D_precut_mc->GetName(),ptBin,cBin));
			hQual_precut_data[p][c] = (TH1D*)hQual3D_precut_data->Project3D("x");
			hQual_precut_data[p][c]->SetName(Form("%s_p%d_c%d",hQual3D_precut_data->GetName(),ptBin,cBin));
			//cout << Form("Project data %d: %.1f-%.1f GeV/c: ",ptBin, hPtBin->GetBinLowEdge(ptBin),hPtBin->GetBinLowEdge(ptBin+1)) << hQual_precut_data[p][c]->GetEntries() << endl;
			//rebin
			hQual_precut_data[p][c]->Rebin(rebinFactor);
			hQual_precut_mc[p][c]->Rebin(rebinFactor);
			// styles
			hQual_precut_mc[p][c]->SetMarkerColor(kRed-9);
			hQual_precut_mc[p][c]->SetLineColor(kRed-9);
			hQual_precut_mc[p][c]->SetMarkerStyle(kOpenSquare);
			hQual_precut_mc[p][c]->SetFillStyle(3001);
			hQual_precut_mc[p][c]->SetFillColor(kRed-9);
			hQual_precut_data[p][c]->SetMarkerColor(colors[c]);
			hQual_precut_mc[p][c]->Scale(1./hQual_precut_mc[p][c]->GetEntries()/rebinFactor);
			hQual_precut_data[p][c]->Scale(1./hQual_precut_data[p][c]->GetEntries()/rebinFactor);
			// set title
			fixedFontHist(hQual_precut_mc[p][c]);
			fixedFontHist(hQual_precut_data[p][c]);
			hQual_precut_mc[p][c]->SetYTitle("unit normalization");
			hQual_precut_data[p][c]->SetYTitle("unit normalization");
			// set axis
			if(xmin!=xmax) {
				hQual_precut_mc[p][c]->SetAxisRange(xmin,xmax,"X");
				hQual_precut_data[p][c]->SetAxisRange(xmin,xmax,"X");
			}
			hQual_precut_mc[p][c]->SetAxisRange(ymin,ymax,"Y");
			hQual_precut_data[p][c]->SetAxisRange(ymin,ymax,"Y");
			hQual_precut_mc[p][c]->GetXaxis()->SetNdivisions(508);
			hQual_precut_data[p][c]->GetXaxis()->SetNdivisions(508);
		}
	}
	
	//can->Divide(5,1);
	Int_t centBin=0;
	Int_t ipad=1;
	for (Int_t p=3; p<=7; ++p) {
		can->cd(ipad);
		if (doLog==1) gPad->SetLogy();
		Int_t ptBin=p;
		cout << "ptBin: " << ptBin << endl;
		handsomeTH1(hQual_precut_data[ptBin][centBin]);
		hQual_precut_data[ptBin][centBin]->Draw("E");
		hQual_precut_mc[ptBin][centBin]->Draw("histsame");
		hQual_precut_data[ptBin][centBin]->Draw("Esame");
		cout << Form("%.1f-%.1f GeV/c: ",hPtBin->GetBinLowEdge(ptBin),hPtBin->GetBinLowEdge(ptBin+1)) << hQual_precut_data[ptBin][centBin]->GetEntries() << endl;
		++ipad;
	}
	can->cd(1);
  //TLegend *legdata = new TLegend(0.19,0.71,0.49,0.90);
  TLegend *legdata = new TLegend(0.26,0.72,0.56,0.92);
  legdata->SetFillStyle(0);
  legdata->SetBorderSize(0);
  legdata->SetTextSize(0.035);
	legdata->AddEntry(hQual_precut_data[0][0],"0-90%","");
  legdata->AddEntry(hQual_precut_data[0][0],"Data Jet35U","p");
  legdata->AddEntry(hQual_precut_mc[0][0],"MC, #hat{p}_{T} 80 GeV/c","p");
	legdata->Draw();
	
	ipad=1;
	for (Int_t p=3; p<=7; ++p) {
		can->cd(ipad);
		Int_t ptBin=p;
		cout << "ptBin leg: " << ptBin << endl;
		Float_t px=0.4,py=0.88;
		if (ipad==1) px=0.63;
		drawText(Form("%.1f - %.1f GeV/c",hPtBin->GetBinLowEdge(ptBin),hPtBin->GetBinLowEdge(ptBin+1)),px,py);
		++ipad;
	}
}

void drawTrkQualDistrib(
												//TString infname="trkqualhists_dataj35_mc80j1_80_v12_hiGlobPrim_allcent_startBaseCut_pt4.root"
												TString infname="trkqualhists_dataj35_mc80j1_80_v12_hiGlobPrim_allcent_startNhit13_pt4.root"
												//TString infname="trkqualhists_dataj35_mc80j1_80_v12_hiGlobPrim_allcent_unCut_pt4.root"
    )
{
	Bool_t doLog = 0;
  cout << "input: " << infname << endl;
  TFile * inf = new TFile(infname);	
	//TString outdir = "out/20110519/"+infname;
	TString outdir = "out/20110531/"+infname;
	if (!doLog) outdir += "/lin";
	outdir.ReplaceAll(".root","");
	gSystem->mkdir(outdir,kTRUE);
  cout << "output: " << outdir << endl;

  TCanvas * c4 = new TCanvas("c4","c4",1600,400);
	makeMultiPanelCanvas(c4,5,1,0,0,0.25);
  if (doLog) drawQual(inf,c4,4,1e-5,10,1,-3,3);
  else drawQual(inf,c4,4,1e-5,0.45,0,-3,3);
  c4->Print(outdir+"/Qual4.gif");
  c4->Print(outdir+"/Qual4.pdf");
	
  TCanvas * c5 = new TCanvas("c5","c5",1600,400);
	makeMultiPanelCanvas(c5,5,1,0,0,0.25);
  if (doLog) drawQual(inf,c5,5,1e-5,10,1,-3,3);
	else drawQual(inf,c5,5,1e-5,0.25,0,-3,3);
  c5->Print(outdir+"/Qual5.gif");
  c5->Print(outdir+"/Qual5.pdf");
	
  TCanvas * c6 = new TCanvas("c6","c6",1600,400);
	makeMultiPanelCanvas(c6,5,1,0,0,0.25);
  if (doLog) drawQual(inf,c6,6,1e-5,10,1,0,0,1);
	else drawQual(inf,c6,6,1e-5,0.8,0,0,0,1);
  c6->Print(outdir+"/Qual6.gif");
  c6->Print(outdir+"/Qual6.pdf");
	
  TCanvas * c7 = new TCanvas("c7","c7",1600,400);
	makeMultiPanelCanvas(c7,5,1,0,0,0.25);
  if (doLog) drawQual(inf,c7,7,1e-5,10,1,0,0,1);
	else drawQual(inf,c7,7,1e-5,0.5,0,0,0,1);
  c7->Print(outdir+"/Qual7.gif");
  c7->Print(outdir+"/Qual7.pdf");
	
  TCanvas * c8 = new TCanvas("c8","c8",1600,400);
	makeMultiPanelCanvas(c8,5,1,0,0,0.25);
  if (doLog) drawQual(inf,c8,8,1e-5,10,1,0,0,1);
	else drawQual(inf,c8,8,1e-5,0.6,0,0,0,1);
  c8->Print(outdir+"/Qual8.gif");
  c8->Print(outdir+"/Qual8.pdf");
	
  TCanvas * c9 = new TCanvas("c9","c9",1600,400);
	makeMultiPanelCanvas(c9,5,1,0,0,0.25);
  if (doLog) drawQual(inf,c9,9,1e-5,10,1,-8,8);
	else drawQual(inf,c9,9,1e-5,0.1,0,-8,8);
  c9->Print(outdir+"/Qual9.gif");
  c9->Print(outdir+"/Qual9.pdf");
	
  TCanvas * c10 = new TCanvas("c10","c10",1600,400);
	makeMultiPanelCanvas(c10,5,1,0,0,0.25);
  if (doLog) drawQual(inf,c10,10,1e-5,10,1,-8,8);
  else drawQual(inf,c10,10,1e-5,0.1,0,-8,8);
  c10->Print(outdir+"/Qual10.gif");
  c10->Print(outdir+"/Qual10.pdf");
	
  TCanvas * c13 = new TCanvas("c13","c13",1600,400);
	makeMultiPanelCanvas(c13,5,1,0,0,0.25);
  if (doLog) drawQual(inf,c13,13,1e-5,10,1,-0.2,0.2);
  else drawQual(inf,c13,13,1e-5,0.2,0,-0.1,0.1);
  c13->Print(outdir+"/Qual13.gif");
  c13->Print(outdir+"/Qual13.pdf");

  TCanvas * c14 = new TCanvas("c14","c14",1600,400);
	makeMultiPanelCanvas(c14,5,1,0,0,0.25);
  if (doLog) drawQual(inf,c14,14,1e-5,10,1,0,0.01);
  else drawQual(inf,c14,14,1e-5,0.25,0,0,0.01);
  c14->Print(outdir+"/Qual14.gif");
  c14->Print(outdir+"/Qual14.pdf");
	
	TCanvas * c15 = new TCanvas("c15","c15",1600,400);
	makeMultiPanelCanvas(c15,5,1,0,0,0.25);
  if (doLog) drawQual(inf,c15,15,1e-5,10,1,-0.2,0.2);
  else drawQual(inf,c15,15,1e-5,0.15,0,-0.2,0.2);
  c15->Print(outdir+"/Qual15.gif");
  c15->Print(outdir+"/Qual15.pdf");
	
  TCanvas * c16 = new TCanvas("c16","c16",1600,400);
	makeMultiPanelCanvas(c16,5,1,0,0,0.25);
  if (doLog) drawQual(inf,c16,16,1e-5,10,1,0,0.01);
  else drawQual(inf,c16,16,1e-5,0.1,0,0,0.01);
  c16->Print(outdir+"/Qual16.gif");
  c16->Print(outdir+"/Qual16.pdf");
	
  TCanvas * c17 = new TCanvas("c17","c17",1600,400);
	makeMultiPanelCanvas(c17,5,1,0,0,0.25);
  if (doLog) drawQual(inf,c17,17,1e-5,10,1,-8,8);
  else drawQual(inf,c17,17,1e-5,0.1,0,-8,8);
  c17->Print(outdir+"/Qual17.gif");
  c17->Print(outdir+"/Qual17.pdf");
	
  TCanvas * c18 = new TCanvas("c18","c18",1600,400);
	makeMultiPanelCanvas(c18,5,1,0,0,0.25);
  if (doLog) drawQual(inf,c18,18,1e-5,10,1,-8,8);
  else drawQual(inf,c18,18,1e-5,0.1,0,-8,8);
  c18->Print(outdir+"/Qual18.gif");	
  c18->Print(outdir+"/Qual18.pdf");	
}
