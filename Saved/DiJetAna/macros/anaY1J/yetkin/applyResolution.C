#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TNtuple.h"
#include "TH1D.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"

using namespace std;



class Jet{
public:
  double pt;
  double eta;
  double phi;

  double ptt;

  bool operator >(Jet b) const{ return pt > b.pt;}
  bool operator <(Jet b) const{ return pt < b.pt;}
 
};

bool JetComparison (Jet a,Jet b) { return (a.pt>b.pt); }

class JetAnalyzer{
public:

  void apply();
  void load();
  void print();
  void Draw(const char* arg){ output->Draw(arg);}

  double resolve(double pt);
  double resolveGaus(double pt);
  double resolveUser(double pt);
  double resolveHist(double pt);

  double getMonoRate(double resol, double ton);

  //  TTree* input;
  TChain* input;

  TNtuple* output;
  TFile* outf;

  TF1* efficiency;
  TF1* resolution;
  TF1* gaussian;

  TF1* userFunc;
   TH1* userHist;
   double histMean;

  double turnOn;
  double analysisCutLead;
  double inputResolution;

  int maxJets;

  bool relative_;
  bool constant_;
  float jtpt[1000],jteta[1000],jtphi[1000];
  int nref;

  std::vector<Jet> vj;

  string fileName_;
  string outName_;

};

void applyResolution(const char* file = "ALL",const char* of = "output.root"){
  JetAnalyzer ana;
  ana.fileName_ = file;
  ana.outName_ = of;

  ana.inputResolution = 20;
  ana.load();

  ana.apply();
  ana.Draw("(pt1-pt2)/pt1");
  
}


double JetAnalyzer::getMonoRate(double resol, double ton){
  turnOn = ton;
  resolution->SetParameter(0,resol);
  apply();

  TH1D h("h","",100,0,1);
  Draw("(pt1-pt2)/pt1>>h");
  return h.GetBinContent(101)/h.Integral();

}

double JetAnalyzer::resolve(double pt){
  return resolveGaus(pt);
}

double JetAnalyzer::resolveGaus(double pt){
  if(!constant_){
    if(relative_){
      gaussian->SetParameter(2,resolution->Eval(pt)*pt);
    }else{
      gaussian->SetParameter(2,resolution->Eval(pt));
    }
  }
  return gaussian->GetRandom();
}

double JetAnalyzer::resolveUser(double pt){
  return userFunc->GetRandom();
 
}

double JetAnalyzer::resolveHist(double pt){
  return userHist->GetRandom()-histMean;
}

void JetAnalyzer::load(){
  bool print = true;

  constant_ = true;

  relative_ = false;

  TH1::SetDefaultSumw2();
  TFile* inf = new TFile("resolutions_randomCones_eta0_ref1_jet0.root");
  userHist = (TH1D*)inf->Get("hRes0_0");
  histMean = userHist->GetMean();

  outf = new TFile(outName_.data(),"recreate");
    output = new TNtuple("nt","","pt1:eta1:phi1:ptt1:pt2:eta2:phi2:ptt2");

    maxJets = -10000;
    turnOn = 0;
    analysisCutLead = 0;

    efficiency = new TF1("efficiency","1./(exp([1]*(x-[0]))+1)",0,300);
    resolution = new TF1("resolution","[0]",0,300);
    gaussian = new TF1("gaussian","gaus",-200,200);
    gaussian->SetParameter(0,1);
    gaussian->SetParameter(1,0);
    resolution->SetParameter(0,inputResolution);

    if(constant_){
      gaussian->SetParameter(2,resolution->Eval(100));
    }

    userFunc = new TF1("userFunc","gaus(0)*pol3(3)",0,300);
    userFunc->SetParameter(0,1.89408e-04);
    userFunc->SetParameter(1,5.24246e+00);
    userFunc->SetParameter(2,1.36120e+01);
    userFunc->SetParameter(3,1.80776e+02);
    userFunc->SetParameter(4,-3.65486e+00);
    userFunc->SetParameter(5,-1.48611e-01);
    userFunc->SetParameter(6,6.88175e-03);

    TH1D* h = new TH1D(Form("h%d",0),"",500,0,500);
    //    input = (TTree*)inf->Get("jets/t");
    input = new TChain("jets/t");
    if(fileName_ == "ALL"){
      input->Add("test1/treefile*.root");
      input->Add("treefile*.root");
    }else{
      input->Add(fileName_.data());
    }

    if(maxJets > 0 && maxJets < input->GetEntries()) input->SetEntries(maxJets);

    double Nev = input->GetEntries();
    double A = 208;
    double AA = A*A;
    double bin = h->GetBinWidth(1); 
    
    double xs = 0.06608; // mb
    double intlum = 5000; // invmb
    double s = xs*intlum*AA/Nev/bin;
    
    h->Scale(s);

    cout<<"h integral : "<<h->Integral()<<endl;
    
    input->SetBranchAddress("jtpt",jtpt);
    input->SetBranchAddress("jteta",jteta);
    input->SetBranchAddress("jtphi",jtphi);
    input->SetBranchAddress("nref",&nref);

}


void JetAnalyzer::apply(){
  
  vector<Jet> vj;
  for(int i = 0; i < input->GetEntries(); ++i){
    if(i% 100 == 0)cout<<"Processing : "<<i<<endl;
    input->GetEntry(i);

    vj.clear();
    for(int it = 0; it < nref; ++it){
      Jet j;
      j.ptt = jtpt[it];
      j.pt = jtpt[it] + resolve(jtpt[it]);
      if(j.pt < turnOn) continue;
      j.eta = jteta[it];
      j.phi = jtphi[it];
      vj.push_back(j);
    }

    sort(vj.begin(),vj.end(),JetComparison);
    
    double pt1 = 0, eta1 = -99, phi1 = -99, ptt1 = 0;
    double pt2 = 0, eta2 = 99, phi2 = 99, ptt2 = 0;
    
    if(vj.size() > 0){
      ptt1 = vj[0].ptt;
      pt1 = vj[0].pt;
      eta1 = vj[0].eta;
      phi1 = vj[0].phi;
    }
    
    if(vj.size() > 1){
      ptt2 = vj[1].ptt;
      pt2 = vj[1].pt;
      eta2 = vj[1].eta;
      phi2 = vj[1].phi;
    }
    
    if(nref > 0 && pt1 > analysisCutLead){
      output->Fill(pt1,eta1,phi1,ptt1,pt2,eta2,phi2,ptt2);		    
    }
    
  }
  outf->Write();
}

void JetAnalyzer::print(){

  for(int i = 0; i < input->GetEntries(); ++i){
    input->GetEntry(i);
    if(nref > 0 && jtpt[0] > 180){
      cout<<"N "<<nref<<endl;
      for(int it = 0; it < nref; ++it){
        cout<<jtpt[it]<<" "<<jteta[it]<<" "<<jtphi[it]<<endl;
      }
    }
  }
}


  
