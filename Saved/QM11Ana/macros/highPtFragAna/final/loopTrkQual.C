#include <iostream>
#include <cmath>
#include <algorithm>
#include "TChain.h"
#include "TNtuple.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "../../HisMath.C"
const Int_t MAXNJETS = 2000;
const Int_t MAXNP = 100000;
const Int_t MAXHLTBITS = 1000;
using namespace std;

struct AnaJetEvt
{
  // evt
  Int_t run, evt, lumi, cbin;
  Float_t vz;
  // jet
  Int_t njets;
  Float_t jtpt[MAXNJETS];
  Float_t jteta[MAXNJETS];
  Float_t jtphi[MAXNJETS];
  // particle
  Int_t np;
  Float_t ppt[MAXNP];
  Float_t peta[MAXNP];
  Float_t pphi[MAXNP];
  Int_t trackqual[MAXNP];
  Int_t tracknhits[MAXNP];
  Int_t trackNlayer[MAXNP];
  Int_t trackNlayer3D[MAXNP];
  Float_t trackptErr[MAXNP];
  Float_t trackd0[MAXNP];
  Float_t trackdz[MAXNP];
  Float_t trackd0ErrTrk[MAXNP];
  Float_t trackdzErrTrk[MAXNP];
  Float_t trackd0Err[MAXNP];
  Float_t trackdzErr[MAXNP];
  Float_t trackchi2[MAXNP];
  Float_t trackchi2hit1D[MAXNP];
  void LoadBranches(TChain * t, Int_t treeFormat, Int_t pType) {
    TString tname = t->GetName();
    cout << "load branches for " << tname << endl;
    t->SetBranchAddress("run",&(this->run));
    t->SetBranchAddress("evt",&(this->evt));
    t->SetBranchAddress("lumi",&(this->lumi));
    t->SetBranchAddress("bin",&(this->cbin));
    t->SetBranchAddress("vz",&(this->vz));

    // jet
    TString name="j4";
    t->SetBranchAddress("n"+name,&(this->njets));
    t->SetBranchAddress("jtpt_"+name,this->jtpt);
    t->SetBranchAddress("jteta_"+name,this->jteta);
    t->SetBranchAddress("jtphi_"+name,this->jtphi);

    if (pType==2) { // tracks
	// pf ana
	t->SetBranchAddress("ntrack",&(this->np));
	t->SetBranchAddress("trackpt",this->ppt);
	t->SetBranchAddress("tracketa",this->peta);
	t->SetBranchAddress("trackphi",this->pphi);
	t->SetBranchAddress("trackqual",this->trackqual);
	t->SetBranchAddress("tracknhits",this->tracknhits);
	t->SetBranchAddress("nlayer",this->trackNlayer);
	t->SetBranchAddress("nlayer3D",this->trackNlayer3D);
	t->SetBranchAddress("ptErr",this->trackptErr);
	t->SetBranchAddress("d0",this->trackd0);
	t->SetBranchAddress("dz",this->trackdz);
	t->SetBranchAddress("d0ErrTrk",this->trackd0ErrTrk);
	t->SetBranchAddress("dzErrTrk",this->trackdzErrTrk);
	t->SetBranchAddress("d0Err",this->trackd0Err);
	t->SetBranchAddress("dzErr",this->trackdzErr);
	t->SetBranchAddress("chi2",this->trackchi2);
	t->SetBranchAddress("chi2hit1D",this->trackchi2hit1D);
    }
  }
};

void Loop(TChain * t, TString name, bool isMC=true,
  Int_t cbinBeg=0,
  Int_t cbinEnd=1)
{
  AnaJetEvt jevt;
  Bool_t useHiGood = true;
  Bool_t useHiGoodTight = false;
  // higher studies
  Bool_t startBaseCut = true;
  Bool_t noNHitCut = false;
  Int_t jetPtMin=90;
  Float_t ptMin=30;
  Int_t etaMax=1;

  // Book Histograms
  const Int_t numPPtBins=18;
  Float_t pptBins[numPPtBins+1] = {0.5,1,1.5,2,2.5,3,4,5,7.5,10,12,15,20,25,30,45,60,90,120};
  vector<Float_t> ptBin(pptBins,pptBins+numPPtBins+1);
  const Int_t numCentBins=6;
  Float_t centBinEdges[numCentBins+1] = {0,2,4,12,20,28,36};
  vector<Float_t> centBin(centBinEdges,centBinEdges+numCentBins+1);
  const Int_t numPtRanges=3;
  Float_t ptRangeEdges[numPtRanges+1] = {0,30,60,120};
  vector<Float_t> ptRange(ptRangeEdges,ptRangeEdges+numPtRanges+1);
  const Int_t numEtaBins=6;
  Float_t etaBinEdges[numEtaBins+1] = {-2.4,-1.6,-1,0,1,1.6,2.4};

  // jet
  TNtuple * ntjevt = new TNtuple("ntjevt","nt jet evt","cbin:jtpt0:jteta0:jtphi0");

  // trk
  vector<TH1D*> vhPPt;
  vector<TH3F*> vhPPt3D;
  TH1D * hPPt_HP = new TH1D(Form("hPPt_HP_%s",name.Data()),";p_{T} (GeV/c)",ptBin.size()-1,&ptBin[0]);
  const Int_t ncuts=20;
  for (Int_t icut=0; icut<ncuts; ++icut) {
    vhPPt.push_back(new TH1D(Form("hPPt_cut%d_%s",icut,name.Data()),";p_{T} (GeV/c)",ptBin.size()-1,&ptBin[0]));
    vhPPt3D.push_back(new TH3F(Form("hPPt3D_cut%d_%s",icut,name.Data()),";p_{T};#eta;bin",ptBin.size()-1,&ptBin[0],numEtaBins,etaBinEdges,numCentBins,centBinEdges));
  }
  vector<TH1D*> vhTrkQualPreCut;
  vector<TH1D*> vhTrkQualPostCut;
  vector<TH3F*> vhTrkQualPreCut3D(20);
  vector<TH3F*> vhTrkQualPostCut3D(20);
  // hiSeletectedTracks
  // 0,1
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual1PreCut_%s",name.Data()),";N Layers;",20,0,20));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual1PostCut_%s",name.Data()),";N Layers;",20,0,20));
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual2PreCut_%s",name.Data()),";N Layers3D;",20,0,20));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual2PostCut_%s",name.Data()),";N Layers3D;",20,0,20));

  // 2
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual3PreCut_%s",name.Data()),";trk #chi_{1d,2d}^{2}/nlayers;",50,0,2));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual3PostCut_%s",name.Data()),";trk #chi_{1d,2d}^{2}/nlayers;",50,0,2));

  // 3,4
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual4PreCut_%s",name.Data()),";Trk dz(vtx)/dzErrCut^{Trk};",100,-8,8));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual4PostCut_%s",name.Data()),";Trk dz(vtx)/dzErrCut^{Trk};",100,-8,8));
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual5PreCut_%s",name.Data()),";Trk d0(vtx)/d0ErrCut^{Trk};",100,-8,8));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual5PostCut_%s",name.Data()),";Trk d0(vtx)/d0ErrCut^{Trk};",100,-8,8));

  Float_t d0d0errbins[101];
  for (Int_t i=0;i<=100;++i) d0d0errbins[i]=-8+i*0.16; // 100,-8,8
  vhTrkQualPreCut3D[3] = new TH3F(Form("hTrkQual4PreCut3D_%s",name.Data()),";Trk dz(vtx)/dzErrCut^{Trk};",100,d0d0errbins,ptBin.size()-1,&ptBin[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPostCut3D[3] = new TH3F(Form("hTrkQual4PostCut3D_%s",name.Data()),";Trk dz(vtx)/dzErrCut^{Trk};",100,d0d0errbins,ptBin.size()-1,&ptBin[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPreCut3D[4] = new TH3F(Form("hTrkQual5PreCut3D_%s",name.Data()),";Trk d0(vtx)/d0ErrCut^{Trk};",100,d0d0errbins,ptBin.size()-1,&ptBin[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPostCut3D[4] = new TH3F(Form("hTrkQual5PostCut3D_%s",name.Data()),";Trk d0(vtx)/d0ErrCut^{Trk};",100,d0d0errbins,ptBin.size()-1,&ptBin[0],centBin.size()-1,&centBin[0]);

  // hackedAnaSel
  // 5,6
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual6PreCut_%s",name.Data()),";#sigma(p_{T})/p_{T};",50,0,0.5));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual6PostCut_%s",name.Data()),";#sigma(p_{T})/p_{T};",50,0,0.5));
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual7PreCut_%s",name.Data()),";# valid hits;",30,0,30));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual7PostCut_%s",name.Data()),";# valid hits;",30,0,30));

  // 7
  Float_t chi2perlayerBins[51];
  for (Int_t i=0;i<=50;++i) chi2perlayerBins[i]=i*0.04; // 50,0,2
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual8PreCut_%s",name.Data()),";trk #chi^{2}/nlayers;",50,0,2));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual8PostCut_%s",name.Data()),";trk #chi^{2}/nlayers;",50,0,2));
  vhTrkQualPreCut3D[7] = new TH3F(Form("hTrkQual8PreCut3D_%s",name.Data()),";trk #chi^{2}/nlayers;p_{T};bin",50,chi2perlayerBins,ptBin.size()-1,&ptBin[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPostCut3D[7] = new TH3F(Form("hTrkQual8PostCut3D_%s",name.Data()),";trk #chi^{2}/nlayers;p_{T};bin",50,chi2perlayerBins,ptBin.size()-1,&ptBin[0],centBin.size()-1,&centBin[0]);

  // 8,9
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual9PreCut_%s",name.Data()),";Trk dz(vtx)/#sigma(dz)^{vtx,trk};",100,-8,8));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual9PostCut_%s",name.Data()),";Trk dz(vtx)/#sigma(dz)^{vtx,trk};",100,-8,8));
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual10PreCut_%s",name.Data()),";Trk d0(vtx)/#sigma(d0)^{vtx,trk};",100,-8,8));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual10PostCut_%s",name.Data()),";Trk d0(vtx)/#sigma(d0)^{vtx,trk};",100,-8,8));

  // 10 after all cuts study
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual11PreCut_%s",name.Data()),";# valid hits;",30,0,30));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual11PostCut_%s",name.Data()),";# valid hits;",30,0,30));

  // Loop Trees
  Int_t numEnt=t->GetEntries();
  cout << "Tree: " << t->GetName() << " " << numEnt << endl;
  jevt.LoadBranches(t,1,2);

  Int_t numSelEvt=0;
  for (Int_t ient=0; ient<numEnt; ++ient) {
    t->GetEntry(ient);
    // presel
    //if (!(jevt.cbin>=0&&jevt.cbin<12)) continue;
    if (!(jevt.cbin>=cbinBeg&&jevt.cbin<cbinEnd)) continue;

    // leading jet
    Float_t maxj0pt=0;
    Int_t imax=-1;
    for (Int_t j=0; j<jevt.njets; ++j) {
      if (fabs(jevt.jteta[j])>2) continue;
      if (jevt.jtpt[j]>maxj0pt) {
	maxj0pt=jevt.jtpt[j];
	imax=j;
      }
    }

    // leading jet selection
    if (maxj0pt<jetPtMin) continue;
    ntjevt->Fill(jevt.cbin,jevt.jtpt[imax],jevt.jteta[imax],jevt.jtphi[imax]);
    ++numSelEvt;

    Int_t printnevt = 500;
    if (!isMC) printnevt = 100;
    if (numSelEvt%printnevt==0) cout << "Entry: " << ient << "("<< ient/(Float_t)numEnt << "%) cbin: " << jevt.cbin << " sorted jet0 pt|eta: " << jevt.jtpt[imax] << "|" << jevt.jteta[imax] 
      << "  trk pt|eta|qual: " << jevt.ppt[0] << "|" << jevt.peta[0] << "|" << jevt.trackqual[0] << endl;

    // tracks
    for (Int_t ip=0; ip<jevt.np; ++ip) {
      Float_t trkEnergy = jevt.ppt[ip];
      Float_t trkEta = jevt.peta[ip];
      if (trkEnergy<ptMin||fabs(trkEta)>etaMax) continue;

      // use base: hiGlobalPrimTracks with nhit cut
      if (startBaseCut&&jevt.tracknhits[ip]<13) continue;

      vhPPt[0]->Fill(trkEnergy);
      vhPPt3D[0]->Fill(trkEnergy,trkEta,jevt.cbin);
      if (jevt.trackqual[ip]==1) {
	hPPt_HP->Fill(trkEnergy);
      }

      // AnalyticalTrkSel
      // cut1
      Int_t minNLayer=0;
      if (useHiGood) minNLayer = 7;
      vhTrkQualPreCut[0]->Fill(jevt.trackNlayer[ip]);
      if (jevt.trackNlayer[ip]<minNLayer) continue;
      vhTrkQualPostCut[0]->Fill(jevt.trackNlayer[ip]);
      vhPPt[1]->Fill(trkEnergy);
      vhPPt3D[1]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut2
      Int_t minNLayer3D=0;
      if (useHiGood) minNLayer3D = 3;
      vhTrkQualPreCut[1]->Fill(jevt.trackNlayer3D[ip]);
      if (jevt.trackNlayer3D[ip]<minNLayer3D) continue;
      vhTrkQualPostCut[1]->Fill(jevt.trackNlayer3D[ip]);
      vhPPt[2]->Fill(trkEnergy);
      vhPPt3D[2]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut3
      vhTrkQualPreCut[2]->Fill(jevt.trackchi2hit1D[ip]/jevt.trackNlayer[ip]);
      if (useHiGood && jevt.trackchi2hit1D[ip]/jevt.trackNlayer[ip]>0.4) continue;
      vhTrkQualPostCut[2]->Fill(jevt.trackchi2hit1D[ip]/jevt.trackNlayer[ip]);
      vhPPt[3]->Fill(trkEnergy);
      vhPPt3D[3]->Fill(trkEnergy,trkEta,jevt.cbin);

      // calc vtx compat
      Double_t nomd0E = sqrt(pow(0.003,2)+pow(0.001/trkEnergy,2));
      Double_t nomdzE = nomd0E*(cosh(trkEta));
      Double_t dzCut = min(9999*jevt.trackNlayer[ip]*nomdzE,
			   pow(30*jevt.trackNlayer[ip],0.3)*jevt.trackdzErrTrk[ip]);
      Double_t d0Cut = min(9999*jevt.trackNlayer[ip]*nomd0E,
			   pow(5*jevt.trackNlayer[ip],0.3)*jevt.trackd0ErrTrk[ip]);

      // cut4
      vhTrkQualPreCut[3]->Fill(jevt.trackdz[ip]/dzCut);
      if (useHiGood && fabs(jevt.trackdz[ip])/dzCut>=1) continue;
      vhTrkQualPostCut[3]->Fill(jevt.trackdz[ip]/dzCut);
      vhPPt[4]->Fill(trkEnergy);
      vhPPt3D[4]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut5
      vhTrkQualPreCut[4]->Fill(jevt.trackd0[ip]/d0Cut);
      if (useHiGood && fabs(jevt.trackd0[ip])/d0Cut>=1) continue;
      vhTrkQualPostCut[4]->Fill(jevt.trackd0[ip]/d0Cut);
      vhPPt[5]->Fill(trkEnergy);
      vhPPt3D[5]->Fill(trkEnergy,trkEta,jevt.cbin);

      // HackedAnaSel
      // cut6
      vhTrkQualPreCut[5]->Fill(jevt.trackptErr[ip]/trkEnergy);
      if (jevt.trackptErr[ip]/trkEnergy>0.05) continue;
      vhTrkQualPostCut[5]->Fill(jevt.trackptErr[ip]/trkEnergy);
      vhPPt[6]->Fill(trkEnergy);
      vhPPt3D[6]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut7
      Int_t min_nhits;
      if (useHiGood) min_nhits = 12;
      if (useHiGoodTight)min_nhits = 13;
      if (noNHitCut) min_nhits = 0;
      vhTrkQualPreCut[6]->Fill(jevt.tracknhits[ip]);
      if (jevt.tracknhits[ip]<min_nhits) continue;
      vhTrkQualPostCut[6]->Fill(jevt.tracknhits[ip]);
      vhPPt[7]->Fill(trkEnergy);
      vhPPt3D[7]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut8
      Float_t chi2n_par = 9999;
      if (useHiGoodTight) chi2n_par=0.15;
      vhTrkQualPreCut[7]->Fill(jevt.trackchi2[ip]/jevt.trackNlayer[ip]);
      vhTrkQualPreCut3D[7]->Fill(jevt.trackchi2[ip]/jevt.trackNlayer[ip],jevt.ppt[ip],jevt.cbin);
      if (jevt.trackchi2[ip]/jevt.trackNlayer[ip]>chi2n_par) continue;
      vhTrkQualPostCut[7]->Fill(jevt.trackchi2[ip]/jevt.trackNlayer[ip]);
      vhTrkQualPostCut3D[7]->Fill(jevt.trackchi2[ip]/jevt.trackNlayer[ip],jevt.ppt[ip],jevt.cbin);
      vhPPt[8]->Fill(trkEnergy);
      vhPPt3D[8]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut9
      vhTrkQualPreCut[8]->Fill(jevt.trackdz[ip]/jevt.trackdzErr[ip]);
      if (fabs(jevt.trackdz[ip])/jevt.trackdzErr[ip]>=3) continue;
      vhTrkQualPostCut[8]->Fill(jevt.trackdz[ip]/jevt.trackdzErr[ip]);
      vhPPt[9]->Fill(trkEnergy);
      vhPPt3D[9]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut10
      vhTrkQualPreCut[9]->Fill(jevt.trackd0[ip]/jevt.trackd0Err[ip]);
      if (fabs(jevt.trackd0[ip])/jevt.trackd0Err[ip]>=3) continue;
      vhTrkQualPostCut[9]->Fill(jevt.trackd0[ip]/jevt.trackd0Err[ip]);
      vhPPt[10]->Fill(trkEnergy);
      vhPPt3D[10]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut11
      vhTrkQualPreCut[10]->Fill(jevt.tracknhits[ip]);
      if (jevt.tracknhits[ip]<13) continue;
      vhTrkQualPostCut[10]->Fill(jevt.tracknhits[ip]);
      vhPPt[11]->Fill(trkEnergy);
      vhPPt3D[11]->Fill(trkEnergy,trkEta,jevt.cbin);
    }
  }

  // normalize
  cout << "Sel Evt: " << numSelEvt << endl;
  normHist(hPPt_HP,0,true,1./numSelEvt);
  for (Int_t icut=0; icut<ncuts; ++icut) {
    normHist(vhPPt[icut],0,true,1./numSelEvt);
    vhPPt3D[icut]->Scale(1./numSelEvt);
  }
}

void loopTrkQual()
{
  TH1::SetDefaultSumw2();
  cout << "Begin" << endl;
  TChain * tmc = new TChain("PFJetAnalyzer/t");
  tmc->Add("/net/hisrv0001/home/frankma/scratch01/mc/Hydjet_Bass_MinBias_2760GeV/HICorrJetTuples_hiGoodTightTracks_Pyquen_UnquenchedDiJet-v1/Pt80/all_set1set2.root");
  TChain * tdata = new TChain("PFJetAnalyzer/t");
  tdata->Add("../trees/HIData_Jet35U_hiGoodTightTracks_extraTrackInfo_full.root");

  Int_t cbinBeg=0,cbinEnd=2;
  TFile * outf = new TFile(Form("trkqualhists_dataj35_mc80_v8_hiGood_c%dto%d_StartNHitCut_pt30.root",cbinBeg,cbinEnd),"RECREATE");
  Loop(tmc,"mc80",1,cbinBeg,cbinEnd);
  Loop(tdata,"dataj35",0,cbinBeg,cbinEnd);
  outf->Write();
}
