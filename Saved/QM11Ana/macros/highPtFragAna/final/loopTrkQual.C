#include <iostream>
#include <cmath>
#include <algorithm>
#include "TChain.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
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

void Loop(TChain * t, TString name, bool isMC=true)
{
  AnaJetEvt jevt;
  Bool_t useHiGood = true;
  Bool_t useHiGoodTight = false;

  // Book Histograms
  const Int_t numPPtBins=18;
  Float_t pptBins[numPPtBins+1] = {0.5,1,1.5,2,2.5,3,4,5,7.5,10,12,15,20,25,30,45,60,90,120};
  vector<Double_t> ptBin(pptBins,pptBins+numPPtBins+1);

  vector<TH1D*> vhPPt;
  TH1D * hPPt_HP = new TH1D(Form("hPPt_HP_%s",name.Data()),";p_{T} (GeV/c)",ptBin.size()-1,&ptBin[0]);
  const Int_t ncuts=20;
  for (Int_t icut=0; icut<ncuts; ++icut) {
    vhPPt.push_back(new TH1D(Form("hPPt_cut%d_%s",icut,name.Data()),";p_{T} (GeV/c)",ptBin.size()-1,&ptBin[0]));
  }
  vector<TH1D*> vhTrkQualPreCut;
  vector<TH1D*> vhTrkQualPostCut;
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

  // hackedAnaSel
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual6PreCut_%s",name.Data()),";#sigma(p_{T})/p_{T};",50,0,0.5));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual6PostCut_%s",name.Data()),";#sigma(p_{T})/p_{T};",50,0,0.5));
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual7PreCut_%s",name.Data()),";# valid hits;",30,0,30));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual7PostCut_%s",name.Data()),";# valid hits;",30,0,30));

  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual8PreCut_%s",name.Data()),";trk #chi^{2}/nlayers;",50,0,2));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual8PostCut_%s",name.Data()),";trk #chi^{2}/nlayers;",50,0,2));

  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual9PreCut_%s",name.Data()),";Trk dz(vtx)/#sigma(dz)^{vtx,trk};",100,-8,8));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual9PostCut_%s",name.Data()),";Trk dz(vtx)/#sigma(dz)^{vtx,trk};",100,-8,8));
  vhTrkQualPreCut.push_back(new TH1D(Form("hTrkQual10PreCut_%s",name.Data()),";Trk d0(vtx)/#sigma(d0)^{vtx,trk};",100,-8,8));
  vhTrkQualPostCut.push_back(new TH1D(Form("hTrkQual10PostCut_%s",name.Data()),";Trk d0(vtx)/#sigma(d0)^{vtx,trk};",100,-8,8));

  // Loop Trees
  Int_t numEnt=t->GetEntries();
  cout << "Tree: " << t->GetName() << " " << numEnt << endl;
  jevt.LoadBranches(t,1,2);

  Int_t numSelEvt=0;
  for (Int_t ient=0; ient<numEnt; ++ient) {
    t->GetEntry(ient);
    // presel
    if (!(jevt.cbin>=0&&jevt.cbin<12)) continue;

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
    if (maxj0pt<100) continue;
    ++numSelEvt;

    Int_t printnevt = 500;
    if (!isMC) printnevt = 100;
    if (numSelEvt%printnevt==0) cout << "Entry: " << ient << "("<< ient/(Float_t)numEnt << "%) cbin: " << jevt.cbin << " sorted jet0 pt|eta: " << jevt.jtpt[imax] << "|" << jevt.jteta[imax] 
      << "  trk pt|eta|qual: " << jevt.ppt[0] << "|" << jevt.peta[0] << "|" << jevt.trackqual[0] << endl;

    // tracks
    for (Int_t ip=0; ip<jevt.np; ++ip) {
      Float_t trkEnergy = jevt.ppt[ip];
      Float_t trkEta = jevt.peta[ip];
      if (trkEnergy<8) continue;

      // base
      vhPPt[0]->Fill(trkEnergy);
      if (jevt.trackqual[ip]==1) hPPt_HP->Fill(trkEnergy);

      // AnalyticalTrkSel
      // cut1
      Int_t minNLayer=0;
      if (useHiGood) minNLayer = 7;
      vhTrkQualPreCut[0]->Fill(jevt.trackNlayer[ip]);
      if (jevt.trackNlayer[ip]<minNLayer) continue;
      vhTrkQualPostCut[0]->Fill(jevt.trackNlayer[ip]);
      vhPPt[1]->Fill(trkEnergy);

      // cut2
      Int_t minNLayer3D=0;
      if (useHiGood) minNLayer3D = 3;
      vhTrkQualPreCut[1]->Fill(jevt.trackNlayer3D[ip]);
      if (jevt.trackNlayer3D[ip]<minNLayer3D) continue;
      vhTrkQualPostCut[1]->Fill(jevt.trackNlayer3D[ip]);
      vhPPt[2]->Fill(trkEnergy);

      // cut3
      vhTrkQualPreCut[2]->Fill(jevt.trackchi2hit1D[ip]/jevt.trackNlayer[ip]);
      if (useHiGood && jevt.trackchi2hit1D[ip]/jevt.trackNlayer[ip]>0.4) continue;
      vhTrkQualPostCut[2]->Fill(jevt.trackchi2hit1D[ip]/jevt.trackNlayer[ip]);
      vhPPt[3]->Fill(trkEnergy);

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

      // cut5
      vhTrkQualPreCut[4]->Fill(jevt.trackd0[ip]/d0Cut);
      if (useHiGood && fabs(jevt.trackd0[ip])/d0Cut>=1) continue;
      vhTrkQualPostCut[4]->Fill(jevt.trackd0[ip]/d0Cut);
      vhPPt[5]->Fill(trkEnergy);

      // HackedAnaSel
      // cut6
      vhTrkQualPreCut[5]->Fill(jevt.trackptErr[ip]/trkEnergy);
      if (jevt.trackptErr[ip]/trkEnergy>0.05) continue;
      vhTrkQualPostCut[5]->Fill(jevt.trackptErr[ip]/trkEnergy);
      vhPPt[6]->Fill(trkEnergy);

      // cut7
      Int_t min_nhits;
      if (useHiGood) min_nhits = 12;
      else if (useHiGoodTight)min_nhits = 13;
      vhTrkQualPreCut[6]->Fill(jevt.tracknhits[ip]);
      if (jevt.tracknhits[ip]<min_nhits) continue;
      vhTrkQualPostCut[6]->Fill(jevt.tracknhits[ip]);
      vhPPt[7]->Fill(trkEnergy);

      // cut8
      Float_t chi2n_par = 9999;
      if (useHiGoodTight) chi2n_par=0.15;
      vhTrkQualPreCut[7]->Fill(jevt.trackchi2[ip]/jevt.trackNlayer[ip]);
      if (jevt.trackchi2[ip]/jevt.trackNlayer[ip]>chi2n_par) continue;
      vhTrkQualPostCut[7]->Fill(jevt.trackchi2[ip]/jevt.trackNlayer[ip]);
      vhPPt[8]->Fill(trkEnergy);

      // cut9
      vhTrkQualPreCut[8]->Fill(jevt.trackdz[ip]/jevt.trackdzErr[ip]);
      if (fabs(jevt.trackdz[ip])/jevt.trackdzErr[ip]>=3) continue;
      vhTrkQualPostCut[8]->Fill(jevt.trackdz[ip]/jevt.trackdzErr[ip]);
      vhPPt[9]->Fill(trkEnergy);

      // cut10
      vhTrkQualPreCut[9]->Fill(jevt.trackd0[ip]/jevt.trackd0Err[ip]);
      if (fabs(jevt.trackd0[ip])/jevt.trackd0Err[ip]>=3) continue;
      vhTrkQualPostCut[9]->Fill(jevt.trackd0[ip]/jevt.trackd0Err[ip]);
      vhPPt[10]->Fill(trkEnergy);
    }
  }

  // normalize
  cout << "Sel Evt: " << numSelEvt << endl;
  normHist(hPPt_HP,0,true,1./numSelEvt);
  for (Int_t icut=0; icut<ncuts; ++icut) {
    normHist(vhPPt[icut],0,true,1./numSelEvt);
  }
}

void loopTrkQual()
{
  TH1::SetDefaultSumw2();
  cout << "Begin" << endl;
  TChain * tmc = new TChain("PFJetAnalyzer/t");
  tmc->Add("/net/hisrv0001/home/frankma/scratch01/mc/Hydjet_Bass_MinBias_2760GeV/HICorrJetTuples_hiGoodTightTracks_Pyquen_UnquenchedDiJet-v1/Pt80/all_set1.root");
  TChain * tdata = new TChain("PFJetAnalyzer/t");
  tdata->Add("../trees/HIData_Jet35U_hiGoodTightTracks_extraTrackInfo.root");

  TFile * outf = new TFile("trkqualhists_dataj35_mc80_v5_hiGood.root","RECREATE");
  Loop(tmc,"mc80",1);
  Loop(tdata,"dataj35",0);
  outf->Write();
}
