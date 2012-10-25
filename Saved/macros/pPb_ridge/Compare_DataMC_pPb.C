#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TString.h"
#include "TCut.h"
#include "TLegend.h"
#include "TLine.h"
#include "commonUtility.h"
#include "compare.h"
using namespace std;
const float PI = 3.1415926535857932;

void Compare_DataMC_pPb(
  bool doMulPan = true,
  float trkPtMin=0.4
  )
{
  TH1::SetDefaultSumw2();
  TString tag = "compPPb", reftag;

  int iRef = 2;
  if (iRef == 0) reftag = "Pythia";
  if (iRef == 2) reftag = "AMPT";
  
  float trkEtaMax=2.4;
  TString outdir = "fig";
//   bool drawTracks=false;
  
  // inputs
  TString infdataname="/net/hisrv0001/home/yenjie/slocal/production/CMSSW_5_3_3_patch3/test/PAPhysics-pt01-full.root";
  TFile * infdata = new TFile(infdataname);
  TTree * tdata = (TTree*)infdata->Get("ppTrack/trackTree");
//   tdata->AddFriend("trevt.hiEvtAnalyzer/HiTree",infdataname);
  tdata->AddFriend("skimanalysis/HltTree",infdataname);
  tdata->AddFriend("hltanalysis/HltTree",infdataname);
  cout << "Data: " << infdataname << endl;
  
  TString infrefname;
  if (iRef==0) infrefname = "/mnt/hadoop/cms/store/user/frankmalocal/forest/HiForest_pythia533.root";
  if (iRef==2) infrefname = "/mnt/hadoop/cms/store/user/frankmalocal/forest/HiForest_AMPT_Evening_5_3_3_v1.root";
  TFile * infref = new TFile(infrefname);
  TTree * tref = (TTree*)infref->Get("ppTrack/trackTree");
//   tref->AddFriend("hiEvtAnalyzer/HiTree",infrefname);
  tref->AddFriend("skimanalysis/HltTree",infrefname);
  tref->AddFriend("hltanalysis/HltTree",infrefname);
  cout << "Ref: " << infrefname << endl;
  
  TCut evtSel = "phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&pprimaryvertexFilter&&abs(vz[1])<15&&HLT_PAZeroBiasPixel_SingleTrack_v1";
  TString trkQual = "highPurity&&abs(trkDz1/trkDzError1)<3&&abs(trkDxy1/trkDxyError1)<3&&(trkPtError/trkPt)<0.1";
  TCut trkSel = Form("trkPt>%.2f&&abs(trkEta)<%.2f&&%s",trkPtMin,trkEtaMax,trkQual.Data());
  
  tdata->SetAlias("N",Form("Sum$(trkPt>0.4&&abs(trkEta)<2.4&&%s)",trkQual.Data()));
  tref->SetAlias("N",Form("Sum$(trkPt>0.4&&abs(trkEta)<2.4&&%s)",trkQual.Data()));
  cout << "N: " << tdata->GetAlias("N") << endl;
  cout << "evtSel: " << evtSel << ": " << tdata->GetEntries(evtSel) << endl;
  
  // histograms
  for (int ic=0; ic<3; ++ic) {
    int nbin;
    float xmin,xmax,ymin=0,ymax=0;
    TString var,title;
    float lx1=0.46,ly1=0.67,lx2=0.79,ly2=0.92;
//     bool doMulPan = true;
    bool doLog=false, doTrack=false;
    
    TCut mysel[4] = {"N>0&&N<35","N>=35&&N<90","N>=90&&N<110","N>=110"};
    TString mytitle[4] = {"N<35","35#leqN<90","90#leqN<110","N>=110"};

    if (ic==0) { // vz
      nbin=120; xmin=-15; xmax=15;
      var = "vz[1]";
      title = ";vertex z (cm); u.n.";
//       doMulPan = false;
    } else if (ic == 1) { // multiplicity
      nbin=50; xmin=0; xmax=200; ymin=1e-4; ymax=1;
      var = "N";
      title = ";N; u.n.";
//       doMulPan = false;
      doLog = true;
    } else if (ic == 2) { // dndeta
      nbin=12; xmin=-2.4; xmax=2.4; ymin=1e-3; ymax=0.15;
      var = "trkEta";
      title = ";Track #eta;u.n.";
//       doMulPan = false;
      doTrack = true;
    }

    for (int i=0; i<4; ++i) {
      if (!doMulPan) {
        mysel[i] = "";
        mytitle[i] = "";    
      }
      if (doTrack) {
          mysel[i] = mysel[i]&&trkSel;
      }
    }

    TCanvas * ca;
    if (doMulPan) {
      ca = new TCanvas(Form("cPPb_%d",ic),Form("cPPb_%d",ic),1200,300);
      ca->Divide(4,1);
    } else ca = new TCanvas(Form("cPPb_%d",ic),Form("cPPb_%d",ic),400,400);
    
    for (int i=0; i<4; ++i) {
      if (doMulPan) ca->cd(i+1);
      else {
        if (i>0) continue;
      }
      if (doLog) gPad->SetLogy();
      Compare cmp(Form("cmp_%d_%d",ic,i),evtSel,1);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(mysel[i]);
      cmp.vt.push_back(tref);
      cmp.vsel.push_back(mysel[i]);
      cmp.Init(nbin,xmin,xmax);
      cmp.Project(var+">>");
      cmp.Draw(title,ymin,ymax);
      cmp.Legend(lx1,ly1,lx2,ly2);
      cmp.leg->AddEntry(cmp.vh[0],mytitle[i],"");
      if (i==0) {
        if (doTrack) cmp.leg->AddEntry(cmp.vh[0],Form("p_{T} > %.1f GeV/c",trkPtMin),"");
        cmp.leg->AddEntry(cmp.vh[0],"pPb Data","p");
        cmp.leg->AddEntry(cmp.vh[1],reftag,"p");
      }
      cmp.leg->Draw();
    }
    ca->Print(Form("%s/%s_%s_%d_mul%d.gif",outdir.Data(),tag.Data(),reftag.Data(),ic,doMulPan));
    ca->Print(Form("%s/%s_%s_%d_mul%d.pdf",outdir.Data(),tag.Data(),reftag.Data(),ic,doMulPan));
  }
}
