#ifndef TrkCorrHisAna_h
#define TrkCorrHisAna_h
#include "TChain.h"
#include "TFile.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TString.h"

#include <iostream>
#include <cmath>
using namespace std;

typedef struct
{
  Int_t ids;
  Float_t etas;
  Float_t pts;
  Int_t hits;
  Int_t status;
  Int_t acc;
  Int_t nrec;
  Float_t ptr;
  Float_t dz;
  Float_t d0;
  Float_t pterr;
  Float_t d0err;
  Float_t dzerr;
  Int_t hitr;
  Int_t algo;
  Float_t jetr;
  Int_t cbin;
  Float_t jetar;
  Float_t jrdr;
  Int_t jrind;
} SimTrack_t;

typedef struct
{
  Int_t charge;
  Float_t etar;
  Float_t ptr;
  Float_t phir;
  Float_t dz;
  Float_t d0;
  Float_t pterr;
  Float_t d0err;
  Float_t dzerr;
  Int_t hitr; 
  Int_t algo;
  Int_t nsim;
  Int_t status;
  Int_t ids;
  Int_t parids;
  Float_t etas;
  Float_t pts;
  Float_t jetr;
  Int_t cbin;
  Float_t jetar;
  Float_t jrdr;
  Int_t jrind;
} RecTrack_t;

class TrkCorrHisAna
{
  public:
    TChain * tsim_;
    TChain * trec_;
    vector<Double_t> ptBins;
    vector<Double_t> etaBins;
    vector<Double_t> jetBins;
    vector<Int_t> centBins;

    TString name_;
    TFile * outFile_;
    Int_t selMode_;
    Float_t ConeRadius_;

    // SimTrack
    TH2F* hsim;
    TH2F* hacc;
    TH2F* heff;
    TH2F* hmul;

    TH3F* hsim3D;
    TH3F* heff3D;
    TH3F* hmul3D;
    TH3F* hresStoR3D;

    // RecTrack
    TH2F* hrec;
    TH2F* hfak;
    TH2F* hsec;

    TH3F* hrec3D;
    TH3F* hfak3D;
    TH3F* hsec3D;

    // vector of histograms
    std::vector<TH3F*> vhsim3D;
    std::vector<TH3F*> vheff3D;
    std::vector<TH3F*> vhmul3D;

    std::vector<TH3F*> vhrec3D;
    std::vector<TH3F*> vhfak3D;
    std::vector<TH3F*> vhsec3D;

    std::vector<TH3F*> vhresStoR3D;

    // monitors
    TH2F * hPJDRJIndRec;
    TH2F * hPJDRJIndSim;

    // methods
    TrkCorrHisAna(TString name);
    TrkCorrHisAna(TrkCorrHisAna & orig) {
      tsim_ = orig.tsim_;
      trec_ = orig.trec_;
      ptBins = orig.ptBins;
      etaBins = orig.etaBins;
      jetBins = orig.jetBins;
      centBins = orig.centBins;
      outFile_ = orig.outFile_;
      selMode_ = orig.selMode_;
      ConeRadius_ = orig.ConeRadius_;
    };
    void DeclareHistograms();
    void FillSimHistograms(const SimTrack_t & s);
    void FillRecHistograms(const RecTrack_t & r);
    void LoopSim();
    void LoopRec();
    void WriteHistograms();
};


TrkCorrHisAna::TrkCorrHisAna(TString name) :
  name_(name),
  selMode_(0),
  ConeRadius_(0.5)
{
}

void TrkCorrHisAna::DeclareHistograms()
{
  cout << "===== " << name_ << " =====" << endl;
  cout << " selMode: " << selMode_ << " ConeRadius: " << ConeRadius_ << endl;
  // Setup output dir
  outFile_->mkdir(name_);
  outFile_->cd(name_);

  // simulated
  hsim = new TH2F("hsim","Sim Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);

  // accepted
  hacc = new TH2F("hacc","Accepted Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);

  // efficiency
  heff = new TH2F("heff","Effic Rec Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);

  // multiply reconstructed
  hmul = new TH2F("hmul","Mult Rec Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);

  // reconstructed
  hrec = new TH2F("hrec","Rec Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);

  // fakes
  hfak = new TH2F("hfak","Fake Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);

  // secondary
  hsec = new TH2F("hsec","Secondary Tracks;#eta;p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0]);


  // simulated 3D 
  hsim3D = new TH3F("hsim3D","Sim Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      jetBins.size()-1, &jetBins[0]);

  // efficiency  3D 
  heff3D = new TH3F("heff3D","Effic Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      jetBins.size()-1, &jetBins[0]);

  // multiply reconstructed 3D 
  hmul3D = new TH3F("hmul3D","Mult Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      jetBins.size()-1, &jetBins[0]);


  // reconstructed 3D 
  hrec3D = new TH3F("hrec3D","Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      jetBins.size()-1, &jetBins[0]);

  // fakes 3D 
  hfak3D = new TH3F("hfak3D","Fake Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      jetBins.size()-1, &jetBins[0]);

  // secondary
  hsec3D = new TH3F("hsec3D","Secondary Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      jetBins.size()-1, &jetBins[0]);

  // mom resolution (Sim to Rec) 
  hresStoR3D = new TH3F("hresStoR3D","Momentum resolution (sim to rec);#eta;sim p_{T} (GeV/c);rec p_{T} (GeV/c)",
      etaBins.size()-1, &etaBins[0],
      ptBins.size()-1, &ptBins[0],
      ptBins.size()-1, &ptBins[0]);

  for(unsigned i=0;i<centBins.size()-1;i++){
    vhsim3D.push_back(new TH3F("","Sim Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], jetBins.size()-1, &jetBins[0]) );
    vheff3D.push_back(new TH3F("","Effic Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], jetBins.size()-1, &jetBins[0]) );
    vhmul3D.push_back(new TH3F("","Mult Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], jetBins.size()-1, &jetBins[0]) );
    vhrec3D.push_back(new TH3F("","Rec Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], jetBins.size()-1, &jetBins[0]) );
    vhfak3D.push_back(new TH3F("","Fake Rec Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], jetBins.size()-1, &jetBins[0]) );
    vhsec3D.push_back(new TH3F("","Secondary Tracks;#eta;p_{T} (GeV/c);jet E_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], jetBins.size()-1, &jetBins[0]) );
    vhresStoR3D.push_back(new TH3F("","Momentum resolution (sim to rec);#eta;sim p_{T} (GeV/c);rec p_{T} (GeV/c)",
	  etaBins.size()-1, &etaBins[0], ptBins.size()-1, &ptBins[0], ptBins.size()-1, &ptBins[0]) );
    if(i==0) {
      vhsim3D[i]->SetName(Form("hsim3D_cbin%dto%d",centBins[i],centBins[i+1]));
      vheff3D[i]->SetName(Form("heff3D_cbin%dto%d",centBins[i],centBins[i+1]));
      vhrec3D[i]->SetName(Form("hrec3D_cbin%dto%d",centBins[i],centBins[i+1]));
      vhfak3D[i]->SetName(Form("hfak3D_cbin%dto%d",centBins[i],centBins[i+1]));
      vhmul3D[i]->SetName(Form("hmul3D_cbin%dto%d",centBins[i],centBins[i+1]));
      vhsec3D[i]->SetName(Form("hsec3D_cbin%dto%d",centBins[i],centBins[i+1]));
      vhresStoR3D[i]->SetName(Form("hresStoR3D_cbin%dto%d",centBins[i],centBins[i+1]));
    }else{
      vhsim3D[i]->SetName(Form("hsim3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
      vheff3D[i]->SetName(Form("heff3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
      vhrec3D[i]->SetName(Form("hrec3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
      vhfak3D[i]->SetName(Form("hfak3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
      vhmul3D[i]->SetName(Form("hmul3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
      vhsec3D[i]->SetName(Form("hsec3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
      vhresStoR3D[i]->SetName(Form("hresStoR3D_cbin%dto%d",centBins[i]+1,centBins[i+1]));
    }
  }

  // monitors
  hPJDRJIndRec = new TH2F("hPJDRJIndRec",";Jet Index;dR;",150,-100,50,50,0,1.6);
  hPJDRJIndSim = new TH2F("hPJDRJIndSim",";Jet Index;dR;",150,-100,50,50,0,1.6);
}

void TrkCorrHisAna::FillSimHistograms(const SimTrack_t & s)
{
  //cout << "not in cone(" << ConeRadius_ << ")? " << (s.jrdr>ConeRadius_) << " not j2? " << (s.jrind!=1) << endl;
  //cout << "not in cone2? " << (s.jrdr>ConeRadius_||s.jrind!=1) << endl;
  if (selMode_==1 && s.jrdr>ConeRadius_) return;
  else if (selMode_==2 && (s.jrdr>ConeRadius_||s.jrind!=0)) return;
  else if (selMode_==3 && (s.jrdr>ConeRadius_||s.jrind!=1)) return;
  else if (selMode_==11 && (s.jrind==0||s.jrind==1)) return;
  //if (selMode_==0) cout << "selMode: " << selMode_ << " jrdr: " << s.jrdr << " ind: " << s.jrind << endl;
  //if (selMode_==2 && s.pts>10) cout << "lead " << s.pts << " " << s.jrind << endl;
  //if (selMode_==3 && s.pts>10) cout << "slead " << s.pts << " " << s.jrind << endl;
  if(s.status>0) {
    // monitor
    hPJDRJIndSim->Fill(s.jrind,s.jrdr);
    // corrections
    hsim->Fill(s.etas, s.pts);
    hsim3D->Fill(s.etas, s.pts, s.jetr);
    if(s.acc)    hacc->Fill(s.etas, s.pts);
    if(s.nrec==1) {
       hresStoR3D->Fill(s.etas, s.pts, s.ptr);
    }
    if(s.nrec>0) heff->Fill(s.etas, s.pts), heff3D->Fill(s.etas, s.pts, s.jetr);
    if(s.nrec>1) hmul->Fill(s.etas, s.pts), hmul3D->Fill(s.etas, s.pts, s.jetr);


    // filling histogram in vector 
    for(unsigned i=0;i<centBins.size()-1;i++){
       if(i==0){
          if(s.cbin<=centBins[i+1]){
             vhsim3D[i]->Fill(s.etas, s.pts, s.jetr);
	     if(s.nrec>0) vheff3D[i]->Fill(s.etas, s.pts, s.jetr);
	     if(s.nrec==1) vhresStoR3D[i]->Fill(s.etas, s.pts, s.ptr);
	     if(s.nrec>1) vhmul3D[i]->Fill(s.etas, s.pts, s.jetr);
          }
       }else{
          if(s.cbin>centBins[i] && s.cbin<=centBins[i+1]){
	     vhsim3D[i]->Fill(s.etas, s.pts, s.jetr);
             if(s.nrec>0) vheff3D[i]->Fill(s.etas, s.pts, s.jetr);
	     if(s.nrec==1) vhresStoR3D[i]->Fill(s.etas, s.pts, s.ptr);
             if(s.nrec>1) vhmul3D[i]->Fill(s.etas, s.pts, s.jetr);
          }
       }
    } // end of vector loop 
  } // end of (s.status) loop 
}

void TrkCorrHisAna::FillRecHistograms(const RecTrack_t & r)
{
  if (selMode_==1 && r.jrdr>ConeRadius_) return;
  else if (selMode_==2 && (r.jrdr>ConeRadius_||r.jrind!=0)) return;
  else if (selMode_==3 && (r.jrdr>ConeRadius_||r.jrind!=1)) return;
  else if (selMode_==11 && (r.jrind==0||r.jrind==1)) return;
  //if (selMode_==2) cout << "lead " << r.pts << " " << r.jrind << endl;
  //if (selMode_==3) cout << "slead " << r.pts << " " << r.jrind << endl;
  // monitor
  hPJDRJIndRec->Fill(r.jrind,r.jrdr);
  // corrections
  hrec->Fill(r.etar, r.ptr);
  hrec3D->Fill(r.etar, r.ptr, r.jetr);
  if(!r.nsim) hfak->Fill(r.etar, r.ptr), hfak3D->Fill(r.etar, r.ptr, r.jetr);
  if(r.nsim>0 && r.status<0) hsec->Fill(r.etar, r.ptr), hsec3D->Fill(r.etar, r.ptr, r.jetr); // nsim>0 redudant?

  // filling histogram in vector
  for(unsigned i=0;i<centBins.size()-1;i++){
    if(i==0){
      if(r.cbin<=centBins[i+1]){
	vhrec3D[i]->Fill(r.etar, r.ptr, r.jetr);
	if(!r.nsim) vhfak3D[i]->Fill(r.etar, r.ptr, r.jetr);
	if(r.nsim>0 && r.status<0) vhsec3D[i]->Fill(r.etar, r.ptr, r.jetr);
      }
    }else{
      if(r.cbin>centBins[i] && r.cbin<=centBins[i+1]){
	vhrec3D[i]->Fill(r.etar, r.ptr, r.jetr);
	if(!r.nsim) vhfak3D[i]->Fill(r.etar, r.ptr, r.jetr);
	if(r.nsim>0 && r.status<0) vhsec3D[i]->Fill(r.etar, r.ptr, r.jetr);
      }
    }
  } // end of vector loop
}

void TrkCorrHisAna::LoopSim()
{
  SimTrack_t s;
  tsim_->SetBranchAddress("simTrackValues",&s.ids);
  cout << name_ << " Sim Trk Loop" << endl;
  for (Long_t i=0; i<tsim_->GetEntries(); ++i) {
    tsim_->GetEntry(i);
    if (i%1000000==0) cout << i/1000 << "k: " << s.ids << " " << s.etas << " " << s.pts << " " << s.jetr << " " << s.cbin << endl;
    //if (i%1000000==0) cout << i/1000 << "k: " << s.ids << " " << s.etas << " " << s.pts << " " << s.jetr << " " << s.cbin << endl;
    FillSimHistograms(s);
  }
}

void TrkCorrHisAna::LoopRec()
{
  RecTrack_t r;
  trec_->SetBranchAddress("recTrackValues",&r.charge);
  cout << name_ << " Rec Trk Loop" << endl;
  for (Long_t i=0; i<trec_->GetEntries(); ++i) {
    trec_->GetEntry(i);
    if (i%1000000==0) cout << i/1000 << "k: " << r.charge << " " << r.etar << " " << r.ptr << " " << r.jetr << " " << r.cbin << endl;
    FillRecHistograms(r);
  }
}

void TrkCorrHisAna::WriteHistograms()
{
  outFile_->cd(name_);

  // SimTrack
  hsim->Write();
  hacc->Write();
  heff->Write();
  hmul->Write();

  hsim3D->Write();
  heff3D->Write();
  hmul3D->Write();
  hresStoR3D->Write();

  // RecTrack;
  hrec->Write();
  hfak->Write();
  hsec->Write();

  hrec3D->Write();
  hfak3D->Write();
  hsec3D->Write();

  // vector of histograms
  for(UInt_t i=0;i<centBins.size()-1;i++){
    vhsim3D[i]->Write();
    vheff3D[i]->Write();
    vhmul3D[i]->Write();

    vhrec3D[i]->Write();
    vhfak3D[i]->Write();
    vhsec3D[i]->Write();

    vhresStoR3D[i]->Write();
  }

  // monitors
  hPJDRJIndSim->Write();
  hPJDRJIndRec->Write();
  outFile_->cd("");
}
#endif //TrkCorrHisAna_h