#if !defined(__CINT__) || defined(__MAKECINT__)

//#include "HiCorrectType.h"

//#include "utilities.h"
//#include "commonStyle.h"

#include "utilities_v2.h"
#include "commonStyle_v2.h"


#include "TLine.h"
#include "TLegend.h"
#include "TPaveStats.h"
#endif


int rebinF = 4;
bool rcp = false;
bool highpt = false;

TH1D *hdndpt_peri_mb_all=0;
TH1D *hdndpt_peri_merged=0;

float vpti[5] = {0.0, 2.0, 10.0, 20.0, 30.0};

//--------------------------------------------------------------------------------
void plotTrkQualityVar(TCanvas *can, TString ifile1, TString ifile2, int whichcan, int whichvar, 
		       int ci, int cf, TString legend);
void plotTrkQualityVarProject(TCanvas *can, TString ifile1, TString ifile2, int whichcan, int whichvar,
			      float pti, float ptf, int ci, int cf, TString legend);
void setUpperPadHist(TH1D *dum_up);
void setLowerPadHist(TH1D *dum_low);
void drawText(const char *text, float xp, float yp);

//---------------------------------------------------------------------------------


void PlotTrkQualityVarMatrix(bool save=false, int whichvar=0, bool centmode=true){

  bool modeDM=false;

  highpt=true;
  
  int cbini = 0;
  int cbinf = 1;

  float pti = 0.0;
  float ptf = 200.0;

  float xmin = 0.0;
  float xmax = 400;
  float ymin = 0.0;
  float ymax = 100.0;

  float ymin_r = 0.42;
  float ymax_r = 1.58;

  TString dir1 = "inputs/data";
  //dir1 = "../root_files/mc";

  //TString dir2 = "../inputs/mc/trkhist_apr182011";
  //TString dir2 = "../inputs/data/proc_apr192011";
  TString dir2 = "inputs/mc";

  if(modeDM) dir2 = "../root_files/data_mix_mc";

  TString lgd = "PbPb #rightarrow h+X, 2.76 TeV, Pt80, |#eta|<1.0";
  TString outf;

  outf.Append(Form("trk_quality_var%d_matrix_preapproval",whichvar));
  if(highpt) outf.Append("_highpt");

  TString xaxis;

  bool logy = false;

  if(whichvar==0){
    xaxis = "N_{hits}";
    xmin = 10, xmax = 32, ymin = 0.0, ymax = 0.5;
  }else if(whichvar==1){
    xaxis = "Normalized #chi^{2}";
    xmin = -0.1, xmax = 5.01, ymin = 1E-4, ymax = 0.59;
    logy = true;
  }else if(whichvar==2){
    xaxis = "#Delta p_{T}/p_{T} (%)";
    xmin = -0.001, xmax = 0.059, ymin = 1E-6, ymax = 4.1;
    logy = true;
  }else if(whichvar==3){
    //dzerr
    xaxis = "#sigma_{dz}";
    xmin = -0.01, xmax = 0.45, ymin = 1E-7, ymax = 2.1;
    logy = true;
  }else if(whichvar==4){
    //d0err
    xaxis = "#sigma_{d0}";
    xmin = 0, xmax = 0.20, ymin = 1E-6, ymax = 2.1;
    logy = true;
  }else if(whichvar==5){
    // dz/dzerr
    xaxis = "dz/#sigma_{dz}";
    //xmin = -3.5, xmax = 3.5, ymin = 1E-3, ymax = 5.1;
    //xmin = -10.5, xmax = 10.5, ymin = 1E-6, ymax = 5.1;     
    xmin = -3.5, xmax = 3.5, ymin = 1E-5, ymax = 0.18;
    logy = false;
  }else if(whichvar==6){
    xaxis = "d0/#sigma_{d0}";
    //xmin = -3.5, xmax = 3.5, ymin = 1E-3, ymax = 5.1;
    //xmin = -10.5, xmax = 10.5, ymin = 1E-6, ymax = 5.1;
    //xmin = -3.5, xmax = 3.5, ymin = 1E-5, ymax = 0.18; 
    //xmin = -10.5, xmax = 10.5, ymin = 1E-5, ymax = 0.18;      
    xmin = -5.5, xmax = 5.5, ymin = 1E-5, ymax = 0.16;      
    logy = false;
  }else if(whichvar==7){
    xaxis = "#eta";
    xmin = -2.5, xmax = 2.5, ymin = 1E-3, ymax = 0.03;
  }else if(whichvar==8){
    xaxis = "#phi";
    xmin = -3.2, xmax = 3.2, ymin = 1E-3, ymax = 0.03;
  }else if(whichvar==9){
    //dz
    xaxis = "dz";
    xmin = -0.01, xmax = 1.45, ymin = 1E-7, ymax = 2.1;
    logy = true;
  }else if(whichvar==10){
    // d0
    xaxis = "d0";
    xmin = -0.01, xmax = 1.45, ymin = 1E-7, ymax = 2.1;
    logy = true;
  }else if(whichvar==11){
    // vz error
    xaxis = "#sigma_{vz}";
    xmin = 0.00, xmax = 0.08, ymin = 4E-8, ymax = 5.1;
    logy = true;
  }else if(whichvar==12){
    // vt error
    xaxis = "#sigma_{vt}";
    xmin = 0.00, xmax = 0.008, ymin = 4E-5, ymax = 10.1;
    logy = true;
  }


  // canvas setting
  //TCanvas *c1 = new TCanvas("c1","",1250,400);
  //makeMultiPanelCanvas(c1,5,1,0.0,0.0,0.2,0.15,0.02,0.24);

  TCanvas *c1;
  if (centmode) {
    c1 = new TCanvas("c1","",1250,1200);
    makeMultiPanelCanvas(c1,5,5,0.0,0.0,0.22,0.22,0.02,0.24);
  } else {
    //c1 = new TCanvas("c1","",1250,300);
    //makeMultiPanelCanvas(c1,5,1,0.0,0.0,0.22,0.22,0.02,0.);
    c1 = new TCanvas("c1","",1250,1200);
    makeMultiPanelCanvas(c1,5,5,0.0,0.0,0.22,0.22,0.02,0.24);
  }

  int offset = 5;
  
  // 1st pannel
  TH1D *dum1 = GetDummyHist(xmin,xmax,ymin,ymax,Form("%s",xaxis.Data()),"Normalized to unity",false);
  TH1D *dum1_low = GetDummyHist(xmin,xmax,ymin_r,ymax_r,"p_{T}","Ratio",false);

  TLine *line = new TLine(xmin,1,xmax,1);
  line->SetLineStyle(2), line->SetLineWidth(2);


  // 1 = data, 2 = mc
  TString f1_1, f1_2;


  //f1_1 = "hcpr_spectra_v4_mb_all_sub1_HCPR_apr042011";
  //f1_1 = "hcpr_spectra_v4_J35U_all_sub0_HCPR_apr042011";
  //f1_1 = "hcpr_spectra_v4_J50U_all_sub0_HCPR_apr042011";
  //f1_1 = "hazsv2-spectra-v1-Jet35U-c0_sub1_HAZS_all_hist_apr192011";
  //f1_2 = "trkhist_apr032011_hydjetBass_off393";
  //f1_2 = "trkhist_apr032011_hydjetBass_dijet170";
  //f1_1 = "trkhist_apr262011_hydjetBass_dijet80_LargeSet1_1k";
  f1_1 = "HIDataJ35U_hiHighPtTracks_v2";
  //f1_1 = "HIDataJ35U_hiGoodTightTracks_v1";
  //f1_1 = "pp_2760_jet20_40_60";
  //f1_1 = "pp_2760_jet20_40_60_hiGoodTightTracks_v1";

  //f1_2 = "trkhist_apr262011_hydjetBass_dijet80_LargeSet1_1k";
  //f1_2 = "trkhist_apr262011_hydjetBass_dijet80_LargeSet1_1k_rereco";
  //f1_2 = "trkhist_v1_apr182011_hydjetBass_dijet50";
  //f1_2 = "hcpr_spectra_v4_J35U_all_sub1_HCPR_apr182011";
  f1_2 = "HyDJUQ50_hiHighPtTracks_v1";
  //f1_2 = "DJUQ50";
  //f1_2 = "trkhist_apr262011_hydjetBass_dijet170_LargeSet1_1k_rereco"; // andre
  //f1_2 = "trkCorrHisAna_djuq80_ppz2gtv1"; // andre

  // drawing
  TString ltitle;
  //cbini = 20, cbinf = 35;
  cbini = 20, cbinf = 39;
  
  for(int i=0;i<5;i++){
    c1->cd(i+1);
    setUpperPadHist(dum1);
    pti = vpti[i];
    ptf = 200.0;

    ltitle = "";
    if(centmode && i==0) drawText("50~90%",0.3,0.8);
    
    drawText(Form("p_{T}> %1.1f",pti),0.7,0.8);

    if(logy) gPad->SetLogy();

    plotTrkQualityVarProject(c1,Form("%s/%s.root",dir1.Data(),f1_1.Data()),Form("%s/%s.root",dir2.Data(),f1_2.Data()),
			     i+1,whichvar,pti,ptf,cbini,cbinf,ltitle.Data());
  }
  
  // 2nd
  cbini = 12, cbinf = 19;
  for(int i=0;i<5;i++){
    c1->cd(i+1+offset);
    setUpperPadHist(dum1);
    pti = vpti[i];
    ptf = 200.0;
    ltitle= "";
    if(i==0) drawText("30~50%",0.3,0.8);
    
    drawText(Form("p_{T}> %1.1f",pti),0.7,0.8);
    if (centmode) plotTrkQualityVarProject(c1,Form("%s/%s.root",dir1.Data(),f1_1.Data()),Form("%s/%s.root",dir2.Data(),f1_2.Data()),
                             i+1+offset,whichvar,pti,ptf,cbini,cbinf,ltitle.Data());
  }  


  // 3rd
  cbini = 4, cbinf = 11;
  for(int i=0;i<5;i++){
    c1->cd(i+1+offset+offset);
    setUpperPadHist(dum1);
    pti = vpti[i];
    ptf = 200.0;
    ltitle= "";
    if(i==0) drawText("10~30%",0.3,0.8);

    drawText(Form("p_{T}> %1.1f",pti),0.7,0.8);

    if (centmode) plotTrkQualityVarProject(c1,Form("%s/%s.root",dir1.Data(),f1_1.Data()),Form("%s/%s.root",dir2.Data(),f1_2.Data()),
                             i+1+offset+offset,whichvar,pti,ptf,cbini,cbinf,ltitle.Data());
  }

  // 4rd
  cbini = 2, cbinf = 3;
  for(int i=0;i<5;i++){
    c1->cd(i+1+offset+offset+offset);
    setUpperPadHist(dum1);
    pti = vpti[i];
    ptf = 200.0;
    ltitle= "";

    if(i==0) drawText("5~10%",0.3,0.8);

    drawText(Form("p_{T}> %1.1f",pti),0.7,0.8);

    if (centmode) plotTrkQualityVarProject(c1,Form("%s/%s.root",dir1.Data(),f1_1.Data()),Form("%s/%s.root",dir2.Data(),f1_2.Data()),
                             i+1+offset+offset+offset,whichvar,pti,ptf,cbini,cbinf,ltitle.Data());
  }


  //5th
  cbini = 0, cbinf = 1;
  for(int i=0;i<5;i++){
    c1->cd(i+1+offset+offset+offset+offset);
    setUpperPadHist(dum1);
    pti = vpti[i];
    ptf = 200.0;
    ltitle= "";

    if(i==0) drawText("0~5%",0.3,0.8);

    drawText(Form("p_{T}> %1.1f",pti),0.7,0.8);

    if (centmode) plotTrkQualityVarProject(c1,Form("%s/%s.root",dir1.Data(),f1_1.Data()),Form("%s/%s.root",dir2.Data(),f1_2.Data()),
                             i+1+offset+offset+offset+offset,whichvar,pti,ptf,cbini,cbinf,ltitle.Data());
  }


  
  
  // save output 
  if(save){
    cout<<"Output file = "<<outf.Data()<<endl;
    printCanvases(c1,outf.Data(),0,1);
  }

}


void plotTrkQualityVar(TCanvas *can, TString ifile1, TString ifile2, int whichcan, int whichvar, int ci, int cf, TString legend){
  can->cd(whichcan);
  cout << ifile1 << " " << ifile2 << endl;

  TFile *efile1 = new TFile(ifile1.Data());
  if(efile1->IsOpen()==false) {
    Error("[FileLoading]","could not open efficiency file 1 [%s].",ifile1.Data());
    return;
  }

  TFile *efile2 = new TFile(ifile2.Data());
  if(efile2->IsOpen()==false) {
    Error("[FileLoading]","could not open efficiency file 2 [%s].",ifile2.Data());
    return;
  }

  TString histtype;
  
  if(whichvar==0) histtype = "hNvalidHits";
  else if(whichvar==1) histtype = "hChi2n";
  else if(whichvar==2) histtype = "hRelPtErr";
  else if(whichvar==3) histtype = "hdzErrPV";
  else if(whichvar==4) histtype = "hd0ErrPV";
  else if(whichvar==5) histtype = "hdzOverdzErrPV";
  else if(whichvar==6) histtype = "hd0Overd0ErrPV";
  //else if(whichvar==5) histtype = "hdzOverdzErr";
  //else if(whichvar==6) histtype = "hd0Overd0Err";
  else if(whichvar==7) histtype = "hEta";
  else if(whichvar==8) histtype = "hPhi";
  else if(whichvar==9) histtype = "hdzPV";
  else if(whichvar==10) histtype = "hd0PV";
  else if(whichvar==11) histtype = "hvzErr";
  else if(whichvar==12) histtype = "hvtErr";


  if(ci!=cf){
    cout<<"for histogram binned in centrality: "<<ci<<" to "<<cf<<endl;
    histtype.Append(Form("_cbin%dto%d",ci,cf));
  }

  TH1D *hTrkQV_1 = 0;
  TH1D *hTrkQV_2 = 0;
  //TH1D *hTrkQV_1 = (TH1D*) efile1->Get(Form("higoodtrkval/%s",histtype.Data()));
  //TH1D *hTrkQV_2 = (TH1D*) efile2->Get(Form("higoodtrkval/%s",histtype.Data()));
  
  //TH1D *hTrkQV_1 = (TH1D*) efile1->Get(Form("hitrkvalidator_hiHighPt/%s",histtype.Data()));
  //TH1D *hTrkQV_2 = (TH1D*) efile2->Get(Form("hitrkvalidator_hiHighPt/%s",histtype.Data()));

  //TH1D *hTrkQV_1 = (TH1D*) efile1->Get(Form("hitrkvalidator_hiGood/%s",histtype.Data()));
  //TH1D *hTrkQV_2 = (TH1D*) efile2->Get(Form("hitrkvalidator_hiGood/%s",histtype.Data()));


  hTrkQV_1->Scale(1./hTrkQV_1->Integral(1,hTrkQV_1->GetNbinsX()));
  hTrkQV_2->Scale(1./hTrkQV_2->Integral(1,hTrkQV_2->GetNbinsX()));

  hTrkQV_2->SetFillStyle(1001);
  hTrkQV_2->SetFillColor(kRed-10);
  hTrkQV_2->SetLineColor(kRed+1);
  hTrkQV_2->SetLineWidth(0);

  //gStyle->SetOptStat(1111);  
  hTrkQV_2->SetStats(false);
  hTrkQV_2->SetName("MC");
  hTrkQV_2->Draw("histsame"); // sames -> draw stat box 
  
  /*
  gPad->Update();
  TPaveStats *st2 = (TPaveStats*)hTrkQV_2->FindObject("stats");
  st2->SetX1NDC(0.63);
  st2->SetY1NDC(0.80);
  st2->SetX2NDC(0.998);
  st2->SetY2NDC(0.98);
  st2->Draw();
  */

  hTrkQV_1->SetName("DATA MIX");
  th1Style2(hTrkQV_1,1,20,0.8,1,1.0,1,1,false);
  
  /*
  gPad->Update();
  TPaveStats *st1 = (TPaveStats*)hTrkQV_1->FindObject("stats");
  st1->SetX1NDC(0.63);
  st1->SetY1NDC(0.59);
  st1->SetX2NDC(0.998);
  st1->SetY2NDC(0.80);
  //st1->Draw();
  */

  if(legend.Length()>3){
    TLegend *legd = new TLegend(0.55,0.77,0.99,0.95);
    legd->SetMargin(0.3);
    legd->SetBorderSize(0);
    legd->SetFillColor(kWhite);
    legd->SetFillStyle(0);
    legd->SetTextSize(0.09);
    legd->SetMargin(0.3);
    legd->SetHeader(legend.Data());
    legd->AddEntry(hTrkQV_1,"Data","pl");
    legd->AddEntry(hTrkQV_2,"HYDJET MC","lf");
    if(highpt) legd->AddEntry("","+ QCD Dijet","");
    legd->Draw();
  }

}




void plotTrkQualityVarProject(TCanvas *can, TString ifile1, TString ifile2, int whichcan, int whichvar, 
			      float pti, float ptf, int ci, int cf, TString legend){

  can->cd(whichcan);

  TFile *efile1 = new TFile(ifile1.Data());
  if(efile1->IsOpen()==false) {
    Error("[FileLoading]","could not open efficiency file 1 [%s].",ifile1.Data());
    return;
  }

  TFile *efile2 = new TFile(ifile2.Data());
  if(efile2->IsOpen()==false) {
    Error("[FileLoading]","could not open efficiency file 2 [%s].",ifile2.Data());
    return;
  }

  TString histtype;

  if(whichvar==0) histtype = "hNvalidHitsdPt";
  else if(whichvar==1) histtype = "hChi2ndPt";
  else if(whichvar==2) histtype = "hRelPtErrdPt";
  else if(whichvar==3) histtype = "hdzErrdPt";
  else if(whichvar==4) histtype = "hd0ErrdPt";
  else if(whichvar==5) histtype = "hdzOverdzErrPVdPt";
  else if(whichvar==6) histtype = "hd0Overd0ErrPVdPt";
  else if(whichvar==7) histtype = "hEta";
  else if(whichvar==8) histtype = "hPhi";
  else if(whichvar==9) histtype= "hdzPVdPt";
  else if(whichvar==10) histtype = "hd0PVdPt";
  else if(whichvar==11) histtype = "hvzErrdPt";
  else if(whichvar==12) histtype = "hvtErrdPt";

  if(ci!=cf){
    cout<<"for histogram binned in centrality: "<<ci<<" to "<<cf<<endl;
    histtype.Append(Form("_cbin%dto%d",ci,cf));
    cout<<"histogram to be plotted is = "<<histtype.Data()<<endl;
  }
  

  //TH2D *hTrkQVdPt_1 = (TH2D*) efile1->Get(Form("higoodtrkval/%s",histtype.Data()));
  //TH2D *hTrkQVdPt_2 = (TH2D*) efile2->Get(Form("higoodtrkval/%s",histtype.Data()));

  //TH2D *hTrkQVdPt_1 = (TH2D*) efile1->Get(Form("hihightrkval/%s",histtype.Data()));
  //TH2D *hTrkQVdPt_2 = (TH2D*) efile2->Get(Form("hihightrkval/%s",histtype.Data()));

  TH2D *hTrkQVdPt_1 = (TH2D*) efile1->Get(Form("hitrkvalidator/%s",histtype.Data()));
  TH2D *hTrkQVdPt_2 = (TH2D*) efile2->Get(Form("hitrkvalidator/%s",histtype.Data()));

  //TH2D *hTrkQVdPt_1 = (TH2D*) efile1->Get(Form("hitrkvalidator_hiHighPt/%s",histtype.Data()));
  //TH2D *hTrkQVdPt_2 = (TH2D*) efile2->Get(Form("hitrkvalidator_hiHighPt/%s",histtype.Data()));

  //TH2D *hTrkQVdPt_1 = (TH2D*) efile1->Get(Form("hitrkvalidator_hiGood/%s",histtype.Data()));
  //TH2D *hTrkQVdPt_2 = (TH2D*) efile2->Get(Form("hitrkvalidator_hiGood/%s",histtype.Data()));

  //TH2D *hTrkQVdPt_1 = (TH2D*) efile1->Get(Form("higoodtightTrkVal/%s",histtype.Data()));

  //TH2D *hTrkQVdPt_1 = (TH2D*) efile1->Get(Form("hihighTrkVal/%s",histtype.Data()));
  //TH2D *hTrkQVdPt_2 = (TH2D*) efile2->Get(Form("hihighTrkVal/%s",histtype.Data()));

  cout<<"check the histograms = "<<hTrkQVdPt_1<<" and = "<<hTrkQVdPt_2<<endl;
  
  int ptbini = hTrkQVdPt_1->GetXaxis()->FindBin(pti);
  int ptbinf = (ptf>=hTrkQVdPt_1->GetXaxis()->GetXmax()) ?
    hTrkQVdPt_1->GetXaxis()->GetLast() : hTrkQVdPt_1->GetXaxis()->FindBin(ptf);

  cout<<"check pt range = "<<hTrkQVdPt_1->GetXaxis()->GetBinLowEdge(ptbini)
      <<" to "<<hTrkQVdPt_1->GetXaxis()->GetBinUpEdge(ptbinf)<<endl;

  //TH1D *hTrkQV_1 = (TH1D*) hTrkQVdPt_1->ProjectionX(Form("hTrkQVdPt_1_%d",whichcan),ptbini,ptbinf,"e");
  //TH1D *hTrkQV_2 = (TH1D*) hTrkQVdPt_2->ProjectionX(Form("hTrkQVdPt_1_%d",whichcan),ptbini,ptbinf,"e");

  TH1D *hTrkQV_1 = (TH1D*) hTrkQVdPt_1->ProjectionY("hTrkQV_1",ptbini,ptbinf,"e");
  TH1D *hTrkQV_2 = (TH1D*) hTrkQVdPt_2->ProjectionY("hTrkQV_2",ptbini,ptbinf,"e");

  hTrkQV_1->Scale(1./hTrkQV_1->Integral(1,hTrkQV_1->GetNbinsX()));
  hTrkQV_2->Scale(1./hTrkQV_2->Integral(1,hTrkQV_2->GetNbinsX()));

  double fArea_1 = hTrkQV_1->Integral();
  double fArea_2 = hTrkQV_2->Integral();

  int lcut = hTrkQV_1->GetXaxis()->FindBin(-3.0);
  int hcut = hTrkQV_1->GetXaxis()->FindBin(3.0-0.001);

  double pArea_1 = hTrkQV_1->Integral(lcut,hcut);
  double pArea_2 = hTrkQV_2->Integral(lcut,hcut);
  
  double lostfrac_1 = fabs(fArea_1-pArea_1)/fArea_1;
  double lostfrac_2 = fabs(fArea_2-pArea_2)/fArea_2;

  
  cout<<"\n===================== fractions ========================="<<endl;
  cout<<"   for centrality range = "<<ci<<" to "<<cf<<endl;
  cout<<"   for low pt cut at "<<pti<<endl;
  cout<<"   cut range = "<<hTrkQV_1->GetXaxis()->GetBinLowEdge(lcut)
      <<" to "<<hTrkQV_1->GetXaxis()->GetBinUpEdge(hcut)<<endl;
  cout<<"   full area (data) = "<<fArea_1<<" (mc) = "<<fArea_2<<endl;
  cout<<"   partial area (data) = "<<pArea_1<<" (mc) = "<<pArea_2<<endl;
  cout<<"   lost fraction (data) = "<<lostfrac_1<<" (mc) = "<<lostfrac_2<<endl;
  cout<<"   difference in lost fraction (data-mc) = "<<lostfrac_1-lostfrac_2<<endl;
  cout<<"===================== fractions =========================\n"<<endl;

  hTrkQV_2->SetFillStyle(1001);
  hTrkQV_2->SetFillColor(kRed-10);
  hTrkQV_2->SetLineColor(kRed+1);
  hTrkQV_2->SetLineWidth(0);
  
  //hTrkQV_2->SetStats(true);
  hTrkQV_2->SetName("MC");
  hTrkQV_2->Draw("histsame"); // sames -> draw stat box                                                                                                 

  /*
  gPad->Update();
  TPaveStats *st2 = (TPaveStats*)hTrkQV_2->FindObject("stats");
  st2->SetX1NDC(0.63);
  st2->SetY1NDC(0.80);
  st2->SetX2NDC(0.998);
  st2->SetY2NDC(0.98);
  st2->Draw();
  */

  hTrkQV_1->SetName("DATA MIX");
  th1Style2(hTrkQV_1,1,20,1.0,1,1.0,1,1,false);

  /*
  gPad->Update();
  TPaveStats *st1 = (TPaveStats*)hTrkQV_1->FindObject("stats");
  st1->SetX1NDC(0.63);
  st1->SetY1NDC(0.59);
  st1->SetX2NDC(0.998);
  st1->SetY2NDC(0.80);
  st1->Draw();
  */

  if(legend.Length()>3){
    TLegend *legd = new TLegend(0.55,0.77,0.99,0.95);
    legd->SetMargin(0.3);
    legd->SetBorderSize(0);
    legd->SetFillColor(kWhite);
    legd->SetFillStyle(0);
    legd->SetTextSize(0.04);
    legd->SetMargin(0.3);
    legd->SetHeader(legend.Data());
    legd->AddEntry(hTrkQV_1,"Data","pl");
    legd->AddEntry(hTrkQV_2,"HYDJET MC","lf");
    if(highpt) legd->AddEntry("","+ QCD Dijet","");
    legd->Draw();
  }

}



void setUpperPadHist(TH1D *dum_up){

  dum_up->GetYaxis()->SetLabelFont(43);
  dum_up->GetYaxis()->SetTitleFont(43);
  dum_up->GetYaxis()->SetLabelSize(17);
  dum_up->GetYaxis()->SetTitleSize(15);
  dum_up->GetYaxis()->SetTitleOffset(4.9);

  dum_up->GetXaxis()->SetLabelFont(43);
  dum_up->GetXaxis()->SetTitleFont(43);
  dum_up->GetXaxis()->SetLabelSize(17);
  dum_up->GetXaxis()->SetTitleSize(18);
  dum_up->GetXaxis()->SetTitleOffset(3.8);

  dum_up->GetXaxis()->SetNdivisions(405);

  dum_up->GetYaxis()->SetNdivisions(405);
  dum_up->GetYaxis()->SetDecimals(true);

  //dum_up->SetStats(0);
  dum_up->Draw();

}

void setLowerPadHist(TH1D *dum_low){

  //Just to make lower pannel lable appearing as similar size as upper one 
  dum_low->GetYaxis()->SetLabelSize(0.1);
  dum_low->GetYaxis()->SetTitleSize(0.105);
  dum_low->GetYaxis()->SetTitleOffset(0.95);

  dum_low->GetXaxis()->SetLabelSize(0.09);
  dum_low->GetXaxis()->SetLabelOffset(0.05);
  dum_low->GetXaxis()->SetTitleSize(0.11);
  dum_low->GetXaxis()->SetTitleOffset(1.47);

  dum_low->GetYaxis()->SetNdivisions(405);
  dum_low->GetYaxis()-> SetDecimals(true);
  dum_low->Draw();

}

void drawText(const char *text, float xp, float yp){
  TLatex *tex = new TLatex(xp,yp,text);
  tex->SetTextFont(63);
  tex->SetTextSize(16);
  tex->SetTextColor(kBlack);
  tex->SetLineWidth(1);
  tex->SetNDC();
  tex->Draw();
}

