#include "../../HisMath.C"

TH1D * drawQual(TFile * inf, TCanvas * c, TString name1, TString name2, Int_t ic, Float_t ymin, Float_t ymax, Int_t doLog)
{
  c->Divide(3,1);
  c->cd(1);
  if (doLog==1) gPad->SetLogy();
  TH1D * hQual_precut_mc = (TH1D*)inf->FindObjectAny(Form("hTrkQual%dPreCut_%s",ic,name1.Data()));
  TH1D * hQual_precut_data = (TH1D*)inf->FindObjectAny(Form("hTrkQual%dPreCut_%s",ic,name2.Data()));
  TH1D * hQual_mc = (TH1D*)inf->FindObjectAny(Form("hTrkQual%dPostCut_%s",ic,name1.Data()));
  TH1D * hQual_data = (TH1D*)inf->FindObjectAny(Form("hTrkQual%dPostCut_%s",ic,name2.Data()));
  if (!hQual_precut_mc) cout << Form("hTrkQual%dPreCut_%s not found",ic,name1.Data()) << endl;
  if (!hQual_precut_data) cout << Form("hTrkQual%dPreCut_%s not found",ic,name2.Data()) << endl;
  hQual_precut_mc->Scale(1./hQual_precut_mc->GetEntries());
  hQual_precut_data->Scale(1./hQual_precut_data->GetEntries());
  hQual_mc->Scale(1./hQual_precut_mc->GetEntries());
  hQual_data->Scale(1./hQual_precut_data->GetEntries());

  hQual_precut_mc->SetAxisRange(ymin,ymax,"Y");
  hQual_precut_mc->SetLineColor(2);
  hQual_precut_mc->SetLineStyle(2);
  hQual_mc->SetMarkerStyle(kOpenCircle);
  hQual_mc->SetMarkerColor(2);
  hQual_mc->SetLineColor(2);
  hQual_precut_data->SetLineStyle(2);
  hQual_precut_mc->Draw("hist");
  hQual_precut_data->Draw("same hist");
  hQual_mc->Draw("same E");
  hQual_data->Draw("same E");

  c->cd(2);
  gPad->SetLogy();
  TH1D * hPPt_precut_mc = inf->FindObjectAny(Form("hPPt_cut%d_%s",ic-1,name1.Data()));
  TH1D * hPPt_precut_data = inf->FindObjectAny(Form("hPPt_cut%d_%s",ic-1,name2.Data()));
  TH1D * hPPt_mc = inf->FindObjectAny(Form("hPPt_cut%d_%s",ic,name1.Data()));
  TH1D * hPPt_data = inf->FindObjectAny(Form("hPPt_cut%d_%s",ic,name2.Data()));

  hPPt_precut_mc->SetAxisRange(5e-5,2,"Y");
  hPPt_precut_mc->SetMarkerColor(2);
  hPPt_precut_mc->SetLineColor(2);
  hPPt_precut_mc->SetLineStyle(2);
  hPPt_precut_data->SetLineStyle(2);
  hPPt_mc->SetMarkerStyle(kOpenCircle);
  hPPt_mc->SetLineColor(2);
  hPPt_mc->SetMarkerColor(2);
  hPPt_precut_mc->Draw("hist");
  hPPt_precut_data->Draw("samehist");
  hPPt_mc->Draw("sameE");
  hPPt_data->Draw("sameE");

  c->cd(3);
  TH1D * hRatMc = hPPt_mc->Clone(Form("%s_rat",hPPt_mc->GetName()));
  hRatMc->Divide(hPPt_precut_mc);
  TH1D * hRatData = hPPt_data->Clone(Form("%s_rat",hPPt_data->GetName()));
  hRatData->Divide(hPPt_precut_data);
  hRatMc->SetAxisRange(0,1.1,"Y");
  hRatMc->SetMarkerStyle(kOpenCircle);
  hRatMc->SetMarkerColor(2);
  hRatMc->SetLineColor(2);
	hRatMc->SetLineStyle(1);
  hRatData->SetLineStyle(1);
	hRatMc->SetYTitle("(After Cut)/(Before Cut)");
  hRatMc->Draw("E");
  hRatData->Draw("sameE");

  return hPPt_data;
}

void drawNt(TFile * inf, TCanvas * c, TString title, TString name1, TString name2, TString var, TCut cut, Float_t ymin, Float_t ymax, Int_t doLog, Float_t xmin=0, Float_t xmax=0)
{
	TNtuple * nt1 = (TNtuple*)inf->Get("ntjevt_"+name1);
	TNtuple * nt2 = (TNtuple*)inf->Get("ntjevt_"+name2);
	
	TString hname1 = "h"+name1+var;
	TString hname2 = "h"+name2+var;
	TH1D * h1 = new TH1D(hname1,title,100,xmin,xmax);
	TH1D * h2 = new TH1D(hname2,title,100,xmin,xmax);
	Float_t num1 = nt1->Project(hname1,var,cut);
	Float_t num2 = nt2->Project(hname2,var,cut);
	cout << name1 << ": " << num1 << " " << name2 << ": " << num2 << endl;

	h1->Sumw2();
	h2->Sumw2();
	h1->Scale(1./num1);
	h2->Scale(1./num2);

	if (doLog) c->SetLogy();
	h1->SetAxisRange(ymin,ymax,"Y");
	h1->SetLineColor(2);
	h1->Draw("hist");
	h2->Draw("sameE");
}

void drawTrkQualityMany(
    TString infname="trkqualhists_dataj35_mc80j1_80_v10_hiGoodTight_allcent_noChi2Cut_pt30.root"
    )
{
	TString input1 = "mc80_c0to2";
	TString input2 = "dataj35_c0to2";
  cout << "input: " << infname << endl;
  TFile * inf = new TFile(infname);
  TString outdir = "out/20110518/"+infname+"/"+input1+"_"+input2;
	outdir.ReplaceAll(".root","");
	gSystem->mkdir(outdir,kTRUE);
  cout << "output: " << outdir << endl;


	
	TCanvas * ck1 = new TCanvas("ck1","ck1",400,400);
	drawNt(inf,ck1,";Leading Jet p_{T} (GeV/c);",input1,input2,"jtpt0","",1e-5,1,1,0,300);
	
  TCanvas * c1 = new TCanvas("c1","c1",1200,400);
  drawQual(inf,c1,input1,input2,1,0,0.6,0);
  c1->Print(outdir+"/PPtcut1.gif");

  TCanvas * c2 = new TCanvas("c2","c2",1200,400);
  drawQual(inf,c2,input1,input2,2,0,0.9,0);
  c2->Print(outdir+"/PPtcut2.gif");

  TCanvas * c3 = new TCanvas("c3","c3",1200,400);
  drawQual(inf,c3,input1,input2,3,1e-3,1,0);
  c3->Print(outdir+"/PPtcut3.gif");

  TCanvas * c4 = new TCanvas("c4","c4",1200,400);
  drawQual(inf,c4,input1,input2,4,1e-4,4e-1,0);
  c4->Print(outdir+"/PPtcut4.gif");

  TCanvas * c5 = new TCanvas("c5","c5",1200,400);
  drawQual(inf,c5,input1,input2,5,1e-4,1.4e-1,0);
  c5->Print(outdir+"/PPtcut5.gif");

  TCanvas * c6 = new TCanvas("c6","c6",1200,400);
  drawQual(inf,c6,input1,input2,6,1e-4,1,0);
  c6->Print(outdir+"/PPtcut6.gif");

  TCanvas * c7 = new TCanvas("c7","c7",1200,400);
  drawQual(inf,c7,input1,input2,7,0,0.5,0);
  c7->Print(outdir+"/PPtcut7.gif");

  TCanvas * c8 = new TCanvas("c8","c8",1200,400);
  drawQual(inf,c8,input1,input2,8,1e-3,1,0);
  c8->Print(outdir+"/PPtcut8.gif");

  TCanvas * c9 = new TCanvas("c9","c9",1200,400);
  drawQual(inf,c9,input1,input2,9,0,0.1,0);
  c9->Print(outdir+"/PPtcut9.gif");

  TCanvas * c10 = new TCanvas("c10","c10",1200,400);
  drawQual(inf,c10,input1,input2,10,0,0.1,0);
  c10->Print(outdir+"/PPtcut10.gif");

	
  TCanvas * c11 = new TCanvas("c11","c11",1200,400);
  drawQual(inf,c11,input1,input2,11,0,0.5,0);
  c11->Print(outdir+"/PPtcut11.gif");
  
	TCanvas * c12 = new TCanvas("c12","c12",1200,400);
  drawQual(inf,c12,input1,input2,12,0,0.6,0);
  c12->Print(outdir+"/PPtcut12.gif");
	
	TCanvas * cFin = new TCanvas("cFin","cFin",500,500);
	cFin->SetLogy();
  TH1D * hPPt_cut10_data = (TH1D*)inf->FindObjectAny("hPPt_cut10_"+input1);
  TH1D * hPPt_cut10_mc = (TH1D*)inf->FindObjectAny("hPPt_cut10_"+input2);
  TH1D * hPPt_HP_data = (TH1D*)inf->FindObjectAny("hPPt_HP_"+input1);
  TH1D * hPPt_HP_mc = (TH1D*)inf->FindObjectAny("hPPt_HP_"+input2);
	hPPt_cut10_mc->SetLineColor(2);
	hPPt_cut10_mc->SetMarkerColor(2);
	hPPt_cut10_mc->SetMarkerStyle(kOpenCircle);
	hPPt_HP_mc->SetLineColor(2);
	hPPt_HP_mc->SetAxisRange(5e-5,2,"Y");
	hPPt_HP_mc->Draw("hist");
	hPPt_cut10_mc->Draw("sameE");
  hPPt_HP_data->Draw("hist same");
  hPPt_cut10_data->Draw("sameE");
  cFin->Print(outdir+"/PPtHP.gif");
}
