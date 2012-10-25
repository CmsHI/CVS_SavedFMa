#include <iostream>
#include <vector>
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

void Compare_DataMC_pPb_many(
  float trkPtMin=0.4
  )
{
  TH1::SetDefaultSumw2();
  TString tag = "compPPb";

  float trkEtaMax=2.4;
  TString outdir = "fig/2012.10.13";
  
  // inputs
  TString infdataname="/net/hisrv0001/home/yenjie/slocal/production/CMSSW_5_3_3_patch3/test/PAPhysics-pt01-full.root";
  TFile * infdata = new TFile(infdataname);
  TTree * tdata = (TTree*)infdata->Get("ppTrack/trackTree");
  tdata->AddFriend("skimanalysis/HltTree",infdataname);
  tdata->AddFriend("hltanalysis/HltTree",infdataname);
  cout << "Data: " << infdataname << endl;
  
  vector<TString> vinfrefname,vreftag;
  vinfrefname.push_back("/mnt/hadoop/cms/store/user/frankmalocal/forest/HiForest_AMPT_Evening_5_3_3_v1.root");
  vreftag.push_back("AMPT");
//   vinfrefname.push_back("/mnt/hadoop/cms/store/user/tuos/pPb/HiForest/5_3_3_patch3/pPbHijing_5_3_3_v0_HiForest2_v07.root");
//   vreftag.push_back("HIJING");
//   vinfrefname.push_back("/mnt/hadoop/cms/store/user/frankmalocal/forest/HiForest_pythia533.root");
//   vreftag.push_back("PYTHIA");
  vector<TTree*> vtref;
  for (int i=0; i<vinfrefname.size(); ++i) {
    TFile * infref = new TFile(vinfrefname[i]);
    vtref.push_back((TTree*)infref->Get("ppTrack/trackTree"));
    vtref[i]->AddFriend("skimanalysis/HltTree",vinfrefname[i]);
    vtref[i]->AddFriend("hltanalysis/HltTree",vinfrefname[i]);
    cout << vreftag[i] << ": " << vinfrefname[i] << endl;
  }
  
  TCut evtSel = "phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&pprimaryvertexFilter&&abs(vz[1])<15&&HLT_PAZeroBiasPixel_SingleTrack_v1";
  TString trkQual = "highPurity&&abs(trkDz1/trkDzError1)<3&&abs(trkDxy1/trkDxyError1)<3&&(trkPtError/trkPt)<0.1";
  TCut trkSel = Form("trkPt>%.2f&&abs(trkEta)<%.2f&&%s",trkPtMin,trkEtaMax,trkQual.Data());
  
  tdata->SetAlias("N",Form("Sum$(trkPt>0.4&&abs(trkEta)<2.4&&%s)",trkQual.Data()));
  for (int i=0; i<vtref.size(); ++i) {
    vtref[i]->SetAlias("N",Form("Sum$(trkPt>0.4&&abs(trkEta)<2.4&&%s)",trkQual.Data()));
  }
  cout << "N: " << tdata->GetAlias("N") << endl;
  cout << "evtSel: " << evtSel << ": " << tdata->GetEntries(evtSel) << endl;
  
  // output
  TFile * outf = new TFile(outdir+"/comparisons_trk100k.root","update");
  
  // histograms
//   for (int ic=3; ic<8; ++ic) {
//   for (int ic=0; ic<2; ++ic) { // evt only
  for (int ic=5; ic<8; ++ic) { // trk qual only
    int nbin;
    float xmin,xmax,ymin=0,ymax=0;
    TString var,title;
    float lx1=0.55,ly1=0.68,lx2=0.92,ly2=0.92;
    bool doMulPan = true;
    bool doLog=false, doTrack=false;
    
    TCut mysel[4] = {"N>0&&N<35","N>=35&&N<90","N>=90&&N<110","N>=110"};
    TString mytitle[4] = {"N<35","35#leqN<90","90#leqN<110","N>=110"};

    if (ic==0) { // vz
//       nbin=120; xmin=-15; xmax=15;
      nbin=40; xmin=-15; xmax=15;
      var = "vz[1]";
      title = ";vertex z (cm); u.n.";
//       doMulPan = false;
    } else if (ic == 1) { // multiplicity
      nbin=50; xmin=0; xmax=200; ymin=1e-4; ymax=1;
      var = "N";
      title = ";N; u.n.";
      doMulPan = false;
      doLog = true;
    } else if (ic == 2) { // dndeta
      nbin=12; xmin=-2.4; xmax=2.4; ymin=1e-3; ymax=0.15;
      var = "trkEta";
      title = ";Track #eta;u.n.";
      lx1=0.2;ly1=0.68;lx2=0.53;ly2=0.92;
//       doMulPan = false;
      doTrack = true;
    } else if (ic == 3) { // dndphi
      nbin=20; xmin=-PI; xmax=PI; ymin=1e-3; ymax=0.1;
      var = "trkPhi";
      title = ";Track #phi;u.n.";
      doTrack = true;
    } else if (ic == 4) { // dndpt
      nbin=20; xmin=0.5; xmax=10.5; ymin=1e-6; ymax=1e1;
      var = "trkPt";
      title = ";Track p_{T} (GeV/c);u.n.";
      doLog = true;
      doTrack = true;
    } else if (ic == 5) { // dxy
      nbin=50; xmin=-3; xmax=3; ymin=1e-3; ymax=1;
      var = "trkDxy1/trkDxyError1";
      title = ";Dxy/#sigma(Dxy);u.n.";
      doLog = true;
      doTrack = true;
    } else if (ic == 6) { // dz
      nbin=50; xmin=-3; xmax=3; ymin=1e-3; ymax=1;
      var = "trkDz1/trkDzError1";
      title = ";Dz/#sigma(Dz);u.n.";
      doLog = true;
      doTrack = true;
    } else if (ic == 7) { // pterror
      nbin=40; xmin=0; xmax=0.1; ymin=1e-3; ymax=1;
      var = "trkPtError/trkPt";
      title = ";#sigma(p_{T})/p_{T};u.n.";
      doLog = true;
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
      Compare cmp(Form("cmp_%d_%d",ic,i),evtSel,0);
      cmp.vt.push_back(tdata);
      cmp.vsel.push_back(mysel[i]);
      for (int k=0; k<vtref.size(); ++k) {
        cmp.vt.push_back(vtref[k]);
        cmp.vsel.push_back(mysel[i]);
      }
      cmp.Init(nbin,xmin,xmax);
      cmp.Project(var+">>");
      cmp.Draw(title,ymin,ymax);
      cmp.Legend(lx1,ly1,lx2,ly2);
      cmp.leg->AddEntry(cmp.vh[0],mytitle[i],"");
      if (i==0) {
        if (doTrack) cmp.leg->AddEntry(cmp.vh[0],Form("p_{T} > %.1f GeV/c",trkPtMin),"");
        cmp.leg->AddEntry(cmp.vh[0],"pPb Data","p");
        for (int k=0; k<vtref.size(); ++k) {
          cmp.leg->AddEntry(cmp.vh[k+1],vreftag[k],"p");
        }
      }
      cmp.leg->Draw();
    }
    ca->Print(Form("%s/%s_%s_%d_mul%d.gif",outdir.Data(),tag.Data(),vreftag[0].Data(),ic,doMulPan));
    ca->Print(Form("%s/%s_%s_%d_mul%d.pdf",outdir.Data(),tag.Data(),vreftag[0].Data(),ic,doMulPan));
  }
  
  outf->Write();
}
