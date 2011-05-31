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
    TString name="j1";
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
	Bool_t doCut=false;
  Bool_t useHiGood = false;
  Bool_t useHiGoodTight = true;
  // higher studies
  Bool_t startBaseCut = false;
  Bool_t noNHitCut = false;
	Bool_t noChi2Cut = false;
  Int_t jetPtMin=80;
  Float_t ptMin=4;
  Int_t etaMax=1;

  name+=Form("_c%dto%d",cbinBeg,cbinEnd);

  // Book Histograms
  const Int_t numPPtBins=18;
  Float_t pptBins[numPPtBins+1] = {0.5,1,1.5,2,2.5,3,4,5,7.5,10,12,15,20,25,30,45,60,90,120};
  vector<Float_t> ptBin(pptBins,pptBins+numPPtBins+1);
  const Int_t numCentBins=6;
  Float_t centBinEdges[numCentBins+1] = {0,2,4,12,20,28,36};
  vector<Float_t> centBin(centBinEdges,centBinEdges+numCentBins+1);
  const Int_t numPtRanges=8;
  Float_t ptRangeEdges[numPtRanges+1] = {0,1,4,8,16,30,60,120,300};
  vector<Float_t> ptRange(ptRangeEdges,ptRangeEdges+numPtRanges+1);
  const Int_t numEtaBins=6;
  Float_t etaBinEdges[numEtaBins+1] = {-2.4,-1.6,-1,0,1,1.6,2.4};

  // jet
  TNtuple * ntjevt = new TNtuple(Form("ntjevt_%s",name.Data()),"nt jet evt","cbin:jtpt0:jteta0:jtphi0");

  // trk
  vector<TH1D*> vhPPt;
  vector<TH3F*> vhPPt3D;
  TH1D * hPPt_HP = new TH1D(Form("hPPt_HP_%s",name.Data()),";p_{T} (GeV/c)",ptBin.size()-1,&ptBin[0]);
  const Int_t ncuts=20;
  for (Int_t icut=0; icut<ncuts; ++icut) {
    vhPPt.push_back(new TH1D(Form("hPPt_cut%d_%s",icut,name.Data()),";p_{T} (GeV/c)",ptBin.size()-1,&ptBin[0]));
    vhPPt3D.push_back(new TH3F(Form("hPPt3D_cut%d_%s",icut,name.Data()),";p_{T};#eta;bin",ptBin.size()-1,&ptBin[0],numEtaBins,etaBinEdges,numCentBins,centBinEdges));
  }
  vector<TH1D*> vhTrkQualPreCut(20);
  vector<TH1D*> vhTrkQualPostCut(20);
  vector<TH3F*> vhTrkQualPreCut3D(20);
  vector<TH3F*> vhTrkQualPostCut3D(20);
	const Int_t numd0bins = 8000; Float_t d0bins[numd0bins+1]; for (Int_t i=0;i<=numd0bins;++i) d0bins[i]=-5+i*10./numd0bins; // numd0bins,-5,5
	const Int_t numd0errbins = 8000; Float_t d0errbins[numd0errbins+1]; for (Int_t i=0;i<=numd0errbins;++i) d0errbins[i]=i*0.5/numd0errbins; // numd0errbins,0,2
	const Int_t numd0d0errbins = 200; Float_t d0d0errbins[numd0d0errbins+1]; for (Int_t i=0;i<=numd0d0errbins;++i) d0d0errbins[i]=-8+i*16./numd0d0errbins; // 100,-8,8
	const Int_t numpterrbins = 100; Float_t pterrbins[numpterrbins+1]; for (Int_t i=0; i<=numpterrbins;++i) pterrbins[i]=i*0.5/numpterrbins; // 50, 0, 0.5
	const Int_t numnhitbins = 30; Float_t nhitbins[numnhitbins+1]; for (Int_t i=0; i<=numnhitbins;++i) nhitbins[i]=i; // 30, 0, 30
  const Int_t numchi2bins = 50; Float_t chi2perlayerBins[numchi2bins+1]; for (Int_t i=0;i<=numchi2bins;++i) chi2perlayerBins[i]=i*2./numchi2bins; // 50,0,2
  // hiSeletectedTracks
  // 0,1
  vhTrkQualPreCut[0] = new TH1D(Form("hTrkQual1PreCut_%s",name.Data()),";N Layers;",20,0,20);
  vhTrkQualPostCut[0] = new TH1D(Form("hTrkQual1PostCut_%s",name.Data()),";N Layers;",20,0,20);
  vhTrkQualPreCut[1] = new TH1D(Form("hTrkQual2PreCut_%s",name.Data()),";N Layers3D;",20,0,20);
  vhTrkQualPostCut[1] = new TH1D(Form("hTrkQual2PostCut_%s",name.Data()),";N Layers3D;",20,0,20);

  // 2
  vhTrkQualPreCut[2] = new TH1D(Form("hTrkQual3PreCut_%s",name.Data()),";trk #chi_{1d,2d}^{2}/nlayers;",50,0,2);
  vhTrkQualPostCut[2] = new TH1D(Form("hTrkQual3PostCut_%s",name.Data()),";trk #chi_{1d,2d}^{2}/nlayers;",50,0,2);

  // 3,4
  vhTrkQualPreCut[3] = new TH1D(Form("hTrkQual4PreCut_%s",name.Data()),";Trk dz(vtx)/dzErrCut^{Trk};",numd0d0errbins,d0d0errbins);
  vhTrkQualPostCut[3] = new TH1D(Form("hTrkQual4PostCut_%s",name.Data()),";Trk dz(vtx)/dzErrCut^{Trk};",numd0d0errbins,d0d0errbins);
  vhTrkQualPreCut[4] = new TH1D(Form("hTrkQual5PreCut_%s",name.Data()),";Trk d0(vtx)/d0ErrCut^{Trk};",numd0d0errbins,d0d0errbins);
  vhTrkQualPostCut[4] = new TH1D(Form("hTrkQual5PostCut_%s",name.Data()),";Trk d0(vtx)/d0ErrCut^{Trk};",numd0d0errbins,d0d0errbins);

  vhTrkQualPreCut3D[3] = new TH3F(Form("hTrkQual4PreCut3D_%s",name.Data()),";Trk dz(vtx)/dzErrCut^{Trk};",numd0d0errbins,d0d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPostCut3D[3] = new TH3F(Form("hTrkQual4PostCut3D_%s",name.Data()),";Trk dz(vtx)/dzErrCut^{Trk};",numd0d0errbins,d0d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPreCut3D[4] = new TH3F(Form("hTrkQual5PreCut3D_%s",name.Data()),";Trk d0(vtx)/d0ErrCut^{Trk};",numd0d0errbins,d0d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPostCut3D[4] = new TH3F(Form("hTrkQual5PostCut3D_%s",name.Data()),";Trk d0(vtx)/d0ErrCut^{Trk};",numd0d0errbins,d0d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);

  // hackedAnaSel
  // 5,6
  vhTrkQualPreCut[5] = new TH1D(Form("hTrkQual6PreCut_%s",name.Data()),";#sigma(p_{T})/p_{T};",numpterrbins,pterrbins);
  vhTrkQualPostCut[5] = new TH1D(Form("hTrkQual6PostCut_%s",name.Data()),";#sigma(p_{T})/p_{T};",numpterrbins,pterrbins);
  vhTrkQualPreCut[6] = new TH1D(Form("hTrkQual7PreCut_%s",name.Data()),";# valid hits;",numnhitbins,nhitbins);
  vhTrkQualPostCut[6] = new TH1D(Form("hTrkQual7PostCut_%s",name.Data()),";# valid hits;",numnhitbins,nhitbins);
	
  vhTrkQualPreCut3D[5] = new TH3F(Form("hTrkQual6PreCut3D_%s",name.Data()),";#sigma(p_{T})/p_{T};",numpterrbins,pterrbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPostCut3D[5] = new TH3F(Form("hTrkQual6PostCut3D_%s",name.Data()),";#sigma(p_{T})/p_{T};",numpterrbins,pterrbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPreCut3D[6] = new TH3F(Form("hTrkQual7PreCut3D_%s",name.Data()),";# valid hits;",numnhitbins,nhitbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPostCut3D[6] = new TH3F(Form("hTrkQual7PostCut3D_%s",name.Data()),";# valid hits;",numnhitbins,nhitbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
	
  // 7
  vhTrkQualPreCut[7] = new TH1D(Form("hTrkQual8PreCut_%s",name.Data()),";trk #chi^{2}/nlayers;",50,0,2);
  vhTrkQualPostCut[7] = new TH1D(Form("hTrkQual8PostCut_%s",name.Data()),";trk #chi^{2}/nlayers;",50,0,2);
  vhTrkQualPreCut3D[7] = new TH3F(Form("hTrkQual8PreCut3D_%s",name.Data()),";trk #chi^{2}/nlayers;p_{T};bin",numchi2bins,chi2perlayerBins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPostCut3D[7] = new TH3F(Form("hTrkQual8PostCut3D_%s",name.Data()),";trk #chi^{2}/nlayers;p_{T};bin",numchi2bins,chi2perlayerBins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);

  // 8,9
  vhTrkQualPreCut[8] = new TH1D(Form("hTrkQual9PreCut_%s",name.Data()),";Trk dz(vtx)/#sigma(dz)^{vtx,trk};",numd0d0errbins,d0d0errbins);
  vhTrkQualPostCut[8] = new TH1D(Form("hTrkQual9PostCut_%s",name.Data()),";Trk dz(vtx)/#sigma(dz)^{vtx,trk};",numd0d0errbins,d0d0errbins);
  vhTrkQualPreCut[9] = new TH1D(Form("hTrkQual10PreCut_%s",name.Data()),";Trk d0(vtx)/#sigma(d0)^{vtx,trk};",numd0d0errbins,d0d0errbins);
  vhTrkQualPostCut[9] = new TH1D(Form("hTrkQual10PostCut_%s",name.Data()),";Trk d0(vtx)/#sigma(d0)^{vtx,trk};",numd0d0errbins,d0d0errbins);

  vhTrkQualPreCut3D[8] = new TH3F(Form("hTrkQual9PreCut3D_%s",name.Data()),";Trk dz(vtx)/#sigma(dz)^{vtx,trk};pt;cbin",numd0d0errbins,d0d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPostCut3D[8] = new TH3F(Form("hTrkQual9PostCut3D_%s",name.Data()),";Trk dz(vtx)/#sigma(dz)^{vtx,trk};pt;cbin",numd0d0errbins,d0d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPreCut3D[9] = new TH3F(Form("hTrkQual10PreCut3D_%s",name.Data()),";Trk d0(vtx)/#sigma(d0)^{vtx,trk};pt;cbin",numd0d0errbins,d0d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
  vhTrkQualPostCut3D[9] = new TH3F(Form("hTrkQual10PostCut3D_%s",name.Data()),";Trk d0(vtx)/#sigma(d0)^{vtx,trk};pt;cbin",numd0d0errbins,d0d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);

  // 10,11 after all cuts study
  vhTrkQualPreCut[10] = new TH1D(Form("hTrkQual11PreCut_%s",name.Data()),";# valid hits;",30,0,30);
  vhTrkQualPostCut[10] = new TH1D(Form("hTrkQual11PostCut_%s",name.Data()),";# valid hits;",30,0,30);
  vhTrkQualPreCut[11] = new TH1D(Form("hTrkQual12PreCut_%s",name.Data()),";trk #chi^{2}/nlayers;",50,0,2);
  vhTrkQualPostCut[11] = new TH1D(Form("hTrkQual12PostCut_%s",name.Data()),";trk #chi^{2}/nlayers;",50,0,2);
	
	// 12 d0, 13 d0err, 14 dz, 15 dzerr, 16 d0/d0err, 17 dz/dzerr
	vhTrkQualPreCut[12] = new TH1D("hTrkQual13PreCut_"+name,";d0;",numd0bins,d0bins);
	vhTrkQualPreCut3D[12] = new TH3F("hTrkQual13PreCut3D_"+name,";d0;",numd0bins,d0bins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
	vhTrkQualPostCut[12] = new TH1D("hTrkQual13PostCut_"+name,";d0;",numd0bins,d0bins);
	vhTrkQualPostCut3D[12] = new TH3F("hTrkQual13PostCut3D_"+name,";d0;",numd0bins,d0bins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
															
	vhTrkQualPreCut[13] = new TH1D("hTrkQual14PreCut_"+name,";#sigma(d0)^{trk};",numd0errbins,d0errbins);
	vhTrkQualPreCut3D[13] = new TH3F("hTrkQual14PreCut3D_"+name,";#sigma(d0)^{trk};p_{T};cbin",numd0errbins,d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
	vhTrkQualPostCut[13] = new TH1D("hTrkQual14PostCut_"+name,";#sigma(d0)^{trk};",numd0errbins,d0errbins);
	vhTrkQualPostCut3D[13] = new TH3F("hTrkQual14PostCut3D_"+name,";#sigma(d0)^{trk};p_{T};cbin",numd0errbins,d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
	
	vhTrkQualPreCut[14] = new TH1D("hTrkQual15PreCut_"+name,";dz;",numd0bins,d0bins);
	vhTrkQualPreCut3D[14] = new TH3F("hTrkQual15PreCut3D_"+name,";dz;p_{T};cbin",numd0bins,d0bins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
	vhTrkQualPostCut[14] = new TH1D("hTrkQual15PostCut_"+name,";dz;",numd0bins,d0bins);
	vhTrkQualPostCut3D[14] = new TH3F("hTrkQual15PostCut3D_"+name,";dz;p_{T};cbin",numd0bins,d0bins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
	
	vhTrkQualPreCut[15] = new TH1D("hTrkQual16PreCut_"+name,";#sigma(dz)^{trk};",numd0errbins,d0errbins);
	vhTrkQualPreCut3D[15] = new TH3F("hTrkQual16PreCut3D_"+name,";#sigma(dz)^{trk};p_{T};cbin",numd0errbins,d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
	vhTrkQualPostCut[15] = new TH1D("hTrkQual16PostCut_"+name,";#sigma(dz)^{trk};",numd0errbins,d0errbins);
	vhTrkQualPostCut3D[15] = new TH3F("hTrkQual16PostCut3D_"+name,";#sigma(dz)^{trk};p_{T};cbin",numd0errbins,d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
	
	vhTrkQualPreCut[16] = new TH1D("hTrkQual17PreCut_"+name,";d0/#sigma(d0)^{trk};",numd0d0errbins,d0d0errbins);
	vhTrkQualPreCut3D[16] = new TH3F("hTrkQual17PreCut3D_"+name,";d0/#sigma(d0)^{trk};",numd0d0errbins,d0d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
	vhTrkQualPostCut[16] = new TH1D("hTrkQual17PostCut_"+name,";d0/#sigma(d0)^{trk};",numd0d0errbins,d0d0errbins);
	vhTrkQualPostCut3D[16] = new TH3F("hTrkQual17PostCut3D_"+name,";d0/#sigma(d0)^{trk};",numd0d0errbins,d0d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
	
	vhTrkQualPreCut[17] = new TH1D("hTrkQual18PreCut_"+name,";dz/#sigma(dz)^{trk};",numd0d0errbins,d0d0errbins);
	vhTrkQualPreCut3D[17] = new TH3F("hTrkQual18PreCut3D_"+name,";dz/#sigma(dz)^{trk};p_{T};cbin",numd0d0errbins,d0d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
	vhTrkQualPostCut[17] = new TH1D("hTrkQual18PostCut_"+name,";dz/#sigma(dz)^{trk};",numd0d0errbins,d0d0errbins);
	vhTrkQualPostCut3D[17] = new TH3F("hTrkQual18PostCut3D_"+name,";dz/#sigma(dz)^{trk};p_{T};cbin",numd0d0errbins,d0d0errbins,ptRange.size()-1,&ptRange[0],centBin.size()-1,&centBin[0]);
	
  // Loop Trees
  Int_t numEnt=t->GetEntries();
  cout << "Tree: " << t->GetName() << " " << numEnt << endl;
  jevt.LoadBranches(t,1,2);

  Int_t numSelEvt=0;
  for (Int_t ient=0; ient<numEnt; ++ient) {
    t->GetEntry(ient);
    // presel
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
\
    Int_t printnevt = 500;
    if (!isMC) printnevt = 100;
    if (numSelEvt%printnevt==0) cout << "Entry: " << ient << "("<< ient/(Float_t)numEnt << "%) cbin: " << jevt.cbin << " sorted jet0 pt|eta: " << jevt.jtpt[imax] << "|" << jevt.jteta[imax] 
      << "  trk pt|eta|qual: " << jevt.ppt[0] << "|" << jevt.peta[0] << "|" << jevt.trackqual[0] << endl;

    ++numSelEvt;

    // tracks
    for (Int_t ip=0; ip<jevt.np; ++ip) {
      Float_t trkEnergy = jevt.ppt[ip];
      Float_t trkEta = jevt.peta[ip];
      if (trkEnergy<ptMin||fabs(trkEta)>etaMax) continue;

      // use base: hiGlobalPrimTracks with nhit,chi2 cut
      if (startBaseCut && (jevt.tracknhits[ip]<13 || jevt.trackchi2[ip]/jevt.trackNlayer[ip]>0.15)) continue;
      //if (startBaseCut && (jevt.tracknhits[ip]<10||jevt.trackchi2[ip]/jevt.trackNlayer[ip]>0.15)) continue;

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
      if (doCut && jevt.trackNlayer[ip]<minNLayer) continue;
      vhTrkQualPostCut[0]->Fill(jevt.trackNlayer[ip]);
      vhPPt[1]->Fill(trkEnergy);
      vhPPt3D[1]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut2
      Int_t minNLayer3D=0;
      if (useHiGood) minNLayer3D = 3;
      vhTrkQualPreCut[1]->Fill(jevt.trackNlayer3D[ip]);
      if (doCut && jevt.trackNlayer3D[ip]<minNLayer3D) continue;
      vhTrkQualPostCut[1]->Fill(jevt.trackNlayer3D[ip]);
      vhPPt[2]->Fill(trkEnergy);
      vhPPt3D[2]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut3
      vhTrkQualPreCut[2]->Fill(jevt.trackchi2hit1D[ip]/jevt.trackNlayer[ip]);
      if (doCut && !noChi2Cut && useHiGood && jevt.trackchi2hit1D[ip]/jevt.trackNlayer[ip]>0.4) continue;
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
      vhTrkQualPreCut3D[3]->Fill(jevt.trackdz[ip]/dzCut,trkEnergy,jevt.cbin);
      if (doCut && useHiGood && fabs(jevt.trackdz[ip])/dzCut>=1) continue;
      vhTrkQualPostCut[3]->Fill(jevt.trackdz[ip]/dzCut);
      vhTrkQualPostCut3D[3]->Fill(jevt.trackdz[ip]/dzCut,trkEnergy,jevt.cbin);
      vhPPt[4]->Fill(trkEnergy);
      vhPPt3D[4]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut5
      vhTrkQualPreCut[4]->Fill(jevt.trackd0[ip]/d0Cut);
      vhTrkQualPreCut3D[4]->Fill(jevt.trackd0[ip]/d0Cut,trkEnergy,jevt.cbin);
      if (doCut && useHiGood && fabs(jevt.trackd0[ip])/d0Cut>=1) continue;
      vhTrkQualPostCut[4]->Fill(jevt.trackd0[ip]/d0Cut);
      vhTrkQualPostCut3D[4]->Fill(jevt.trackd0[ip]/d0Cut,trkEnergy,jevt.cbin);
      vhPPt[5]->Fill(trkEnergy);
      vhPPt3D[5]->Fill(trkEnergy,trkEta,jevt.cbin);

      // HackedAnaSel
      // cut6
      vhTrkQualPreCut[5]->Fill(jevt.trackptErr[ip]/trkEnergy);
			vhTrkQualPreCut3D[5]->Fill(jevt.trackptErr[ip]/trkEnergy,trkEnergy,jevt.cbin);
      if (doCut && jevt.trackptErr[ip]/trkEnergy>0.05) continue;
      vhTrkQualPostCut[5]->Fill(jevt.trackptErr[ip]/trkEnergy);
      vhPPt[6]->Fill(trkEnergy);
      vhPPt3D[6]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut7
      Int_t min_nhits;
      if (useHiGood) min_nhits = 12;
      if (useHiGoodTight)min_nhits = 13;
      if (noNHitCut) min_nhits = 0;
      vhTrkQualPreCut[6]->Fill(jevt.tracknhits[ip]);
      vhTrkQualPreCut3D[6]->Fill(jevt.tracknhits[ip],trkEnergy,jevt.cbin);
      if (doCut && jevt.tracknhits[ip]<min_nhits) continue;
      vhTrkQualPostCut[6]->Fill(jevt.tracknhits[ip]);
      vhPPt[7]->Fill(trkEnergy);
      vhPPt3D[7]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut8
      Float_t chi2n_par = 9999;
      if (useHiGoodTight) chi2n_par=0.15;
      vhTrkQualPreCut[7]->Fill(jevt.trackchi2[ip]/jevt.trackNlayer[ip]);
      vhTrkQualPreCut3D[7]->Fill(jevt.trackchi2[ip]/jevt.trackNlayer[ip],jevt.ppt[ip],jevt.cbin);
      if (doCut && !noChi2Cut && jevt.trackchi2[ip]/jevt.trackNlayer[ip]>chi2n_par) continue;
      vhTrkQualPostCut[7]->Fill(jevt.trackchi2[ip]/jevt.trackNlayer[ip]);
      vhTrkQualPostCut3D[7]->Fill(jevt.trackchi2[ip]/jevt.trackNlayer[ip],jevt.ppt[ip],jevt.cbin);
      vhPPt[8]->Fill(trkEnergy);
      vhPPt3D[8]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut9
      vhTrkQualPreCut[8]->Fill(jevt.trackdz[ip]/jevt.trackdzErr[ip]);
      vhTrkQualPreCut3D[8]->Fill(jevt.trackdz[ip]/jevt.trackdzErr[ip],jevt.ppt[ip],jevt.cbin);
      if (doCut && fabs(jevt.trackdz[ip])/jevt.trackdzErr[ip]>=3) continue;
      vhTrkQualPostCut[8]->Fill(jevt.trackdz[ip]/jevt.trackdzErr[ip]);
      vhPPt[9]->Fill(trkEnergy);
      vhPPt3D[9]->Fill(trkEnergy,trkEta,jevt.cbin);

      // cut10
      vhTrkQualPreCut[9]->Fill(jevt.trackd0[ip]/jevt.trackd0Err[ip]);
      vhTrkQualPreCut3D[9]->Fill(jevt.trackd0[ip]/jevt.trackd0Err[ip],jevt.ppt[ip],jevt.cbin);
      if (doCut && fabs(jevt.trackd0[ip])/jevt.trackd0Err[ip]>=3) continue;
      vhTrkQualPostCut[9]->Fill(jevt.trackd0[ip]/jevt.trackd0Err[ip]);
      vhPPt[10]->Fill(trkEnergy);
      vhPPt3D[10]->Fill(trkEnergy,trkEta,jevt.cbin);

      // after cut studies
			// qual 11
      vhTrkQualPreCut[10]->Fill(jevt.tracknhits[ip]);
      if (jevt.tracknhits[ip]>=min_nhits) {
				vhTrkQualPostCut[10]->Fill(jevt.tracknhits[ip]);
				vhPPt[11]->Fill(trkEnergy);
				vhPPt3D[11]->Fill(trkEnergy,trkEta,jevt.cbin);
			}
			
			// qual 12
			vhTrkQualPreCut[11]->Fill(jevt.trackchi2[ip]/jevt.trackNlayer[ip]);
			if (jevt.trackchi2[ip]/jevt.trackNlayer[ip]<chi2n_par){
				vhTrkQualPostCut[11]->Fill(jevt.trackchi2[ip]/jevt.trackNlayer[ip]);
				vhPPt[12]->Fill(trkEnergy);
				vhPPt3D[12]->Fill(trkEnergy,trkEta,jevt.cbin);
			}
			
			// qual 13
			vhTrkQualPreCut[12]->Fill(jevt.trackd0[ip]);
			vhTrkQualPreCut3D[12]->Fill(jevt.trackd0[ip],jevt.ppt[ip],jevt.cbin);

			// qual 14
			vhTrkQualPreCut[13]->Fill(jevt.trackd0ErrTrk[ip]);
			vhTrkQualPreCut3D[13]->Fill(jevt.trackd0ErrTrk[ip],jevt.ppt[ip],jevt.cbin);

			// qual 15
			vhTrkQualPreCut[14]->Fill(jevt.trackdz[ip]);
			vhTrkQualPreCut3D[14]->Fill(jevt.trackdz[ip],jevt.ppt[ip],jevt.cbin);
			
			// qual 16
			vhTrkQualPreCut[15]->Fill(jevt.trackdzErrTrk[ip]);
			vhTrkQualPreCut3D[15]->Fill(jevt.trackdzErrTrk[ip],jevt.ppt[ip],jevt.cbin);
			
			// qual 17
			vhTrkQualPreCut[16]->Fill(jevt.trackd0[ip]/jevt.trackd0ErrTrk[ip]);
			vhTrkQualPreCut3D[16]->Fill(jevt.trackd0[ip]/jevt.trackd0ErrTrk[ip],jevt.ppt[ip],jevt.cbin);
			
			// qual 18
			vhTrkQualPreCut[17]->Fill(jevt.trackd0[ip]/jevt.trackdzErrTrk[ip]);
			vhTrkQualPreCut3D[17]->Fill(jevt.trackd0[ip]/jevt.trackdzErrTrk[ip],jevt.ppt[ip],jevt.cbin);
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
  cout << "Begin v11" << endl;
  TChain * tmc = new TChain("PFJetAnalyzer/t");
  tmc->Add("/net/hisrv0001/home/frankma/scratch01/mc/Hydjet_Bass_MinBias_2760GeV/Pyquen_UnquenchedDiJet_393-v1/all_pt80set1-v1.root");
  tmc->Add("/net/hisrv0001/home/frankma/scratch01/mc/Hydjet_Bass_MinBias_2760GeV/Pyquen_UnquenchedDiJet_393-v1/all_pt80set2-v1.root");
  tmc->Add("/net/hisrv0001/home/frankma/scratch01/mc/Hydjet_Bass_MinBias_2760GeV/Pyquen_UnquenchedDiJet_393-v1/all_pt80set3-v1.root");
  tmc->Add("/net/hisrv0001/home/frankma/scratch01/mc/Hydjet_Bass_MinBias_2760GeV/Pyquen_UnquenchedDiJet_393-v1/all_pt80set4-v1.root");
  TChain * tdata = new TChain("PFJetAnalyzer/t");
  tdata->Add("../trees/HIData_Jet35U_hiGoodTightTracks_extraTrackInfo_full.root");

  Int_t cbinBeg=0,cbinEnd=36;
  TFile * outf = new TFile(Form("trkqualhists_dataj35_mc80j1_80_v12_hiGlobPrim_allcent_unCut_pt4.root",cbinBeg,cbinEnd),"RECREATE");
  //TFile * outf = new TFile(Form("trkqualhists_dataj35_mc80j1_80_v12_hiGlobPrim_allcent_startBaseCut_pt4.root",cbinBeg,cbinEnd),"RECREATE");
	cout << "Output: " << outf->GetName() << endl;
  Loop(tmc,"mc80",1,cbinBeg,cbinEnd);
  Loop(tdata,"dataj35",0,cbinBeg,cbinEnd);
  outf->Write();
}
