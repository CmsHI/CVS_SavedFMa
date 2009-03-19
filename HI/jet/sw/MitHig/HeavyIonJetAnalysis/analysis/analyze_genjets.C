#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TNtuple.h"
#include "TGraphErrors.h"
#include "MitHigFunctions.h"

#define MAXPARTICLES 50000
#define MAXJETS 5000
#define MAXHITS 5000
#define MAXVTX 100
#define ETABINS 3 

using namespace std;

struct HydjetEvent{

   int event;
   float b;
   float npart;
   float ncoll;
   float nhard;

   int n[ETABINS];
   float ptav[ETABINS];

   int np;
   float par_pt[MAXPARTICLES];
   float par_eta[MAXPARTICLES];
   float par_phi[MAXPARTICLES];
   int pdg[MAXPARTICLES];
   int chg[MAXPARTICLES];

   int algos;
   int njet;

   float et[MAXJETS];
   float eta[MAXJETS];
   float phi[MAXJETS];
   float area[MAXJETS];

   float vx;
   float vy;
   float vz;
   float vr;

};

struct GenParticle{
   float pid;
   float status;
   float pt;
   float eta;
   float phi;
   float charge;
   float subeid;
   float ishydro;
};

double analyze_with_cut(TTree* tsub, TTree* tall, double jetEtCut = 20);

void analyze_genjets(char * infile = "genjets.root", char * outfile = "output.root"){
//void analyze_genjets(char * infile = "../../producer/OoB/genjets.root", char * outfile = "output.root"){

  TFile* inf = new TFile(infile);
  inf->ls();
  TTree* tsub = dynamic_cast<TTree*>(inf->Get("subevent/hi"));
  //TTree* tall = dynamic_cast<TTree*>(inf->Get("allevent/hi"));
  TNtuple * nt2 = dynamic_cast<TNtuple*>(inf->Get("subevent/nt2"));
  TNtuple * nt3 = dynamic_cast<TNtuple*>(inf->Get("subevent/nt3"));

  TFile* outf = new TFile(outfile,"recreate");

  double x[6];
  double y[6];

  for(int i=0; i < 1; ++i){
    x[i] = 5*i;
    cout<<"Et Cut  : "<<x[i]<<endl;

    y[i] = analyze_with_cut(tsub,nt2,5*i);
    cout<<"Overlap : "<<y[i]<<endl;

  }  
//
//  TGraph* g1 = new TGraph(6,x,y);
//
//  TCanvas* c1 = new TCanvas();
//  g1->Draw("a*");
//
//  g1->Write();
//  c1->Write();

  TCanvas* c5 = new TCanvas();
  tsub->Draw("njet");
  c5->Print("nsubjets.gif");

//  TCanvas* c6 = new TCanvas();
//  tall->Draw("njet");
//  c6->Print("nglobjets.gif");

  TCanvas* c7 = new TCanvas();
  tsub->Draw("et");
  gPad->SetLogy();
  c7->Print("subjet_et.gif");

//  TCanvas* c8 = new TCanvas();
//  tall->Draw("et");
//  c8->Print("globjet_et.gif");


  // Check frankma ntuples
  TCanvas* c9 = new TCanvas();
  TH1F * hcand = new TH1F("subevent_mul", "pyquen subevents multiplicity", 100, 0, 250);
  nt3->Draw("ncands>>subevent_mul");
  c9->Print("subevent_multiplicity.gif");
  TCanvas* c10 = new TCanvas();
  TH2F * hcandvb = new TH2F("submul_vs_b", "all subevents multiplicity vs b", 100,0,14, 1000, 0, 30000);
  nt3->Draw("ncands:b>>submul_vs_b","","colz");
  c10->Print("submul_vs_b.gif");
  //-- Check gen particle ntuple: HepMC vs CMSSW GenParticle --
  // check pt
  TH1F * hpt = new TH1F("genp_pt","HepMC particles pt vs CMSSW GenParticles pt", 100, 0, 70);
  hpt->SetLineColor(2);
  hpt->SetLineWidth(3);
  TCanvas* c11 = new TCanvas();
  gPad->SetLogy();
  tsub->Draw("par_pt>>genp_pt");
  nt2->Draw("pt","status==1","same");
  // check eta
  TH1F * heta = new TH1F("genp_eta","HepMC particles eta vs CMSSW GenParticles eta", 100, -10, 10);
  heta->SetLineColor(2);
  heta->SetLineWidth(3);
  TCanvas* c12 = new TCanvas();
  tsub->Draw("par_eta>>genp_eta");
  nt2->Draw("eta","status==1","same");


}

double analyze_with_cut(TTree* tsub, TTree* ntgenp, double jetEtCut){
  cout<<"Begin"<<endl;

   TH1F* h1 = new TH1F(Form("h1_et%02d",(int)jetEtCut),"Self Correlation;#Delta R;jets",200,0,6);
   TH1F* h2 = new TH1F(Form("h2_et%02d",(int)jetEtCut),"Relation between Globally reconstructed and Sub-Event based GenJets;#Delta R;jets",200,0,6);
   TH2F* het = new TH2F(Form("het_et%02d",(int)jetEtCut),";E_{T}^{sub-event genjet};E_{T}^{global genjet}",50,0,200,50,0,200);

   double cone = 0.5;
   double match = cone/2.;

   int maxEvents = 100000;

   cout<<"A"<<endl;

   HydjetEvent jet;
   GenParticle genp;

   //-- Set Jet tree branches --
   tsub->SetBranchAddress("b",&jet.b);
 
   tsub->SetBranchAddress("npart",&jet.npart);
   tsub->SetBranchAddress("ncoll",&jet.ncoll);
   tsub->SetBranchAddress("nhard",&jet.nhard);

   tsub->SetBranchAddress("n",jet.n);
   tsub->SetBranchAddress("ptav",jet.ptav);
   tsub->SetBranchAddress("np",&jet.np);
   tsub->SetBranchAddress("par_pt",jet.par_pt);
   tsub->SetBranchAddress("par_eta",jet.par_eta);
   tsub->SetBranchAddress("par_phi",jet.par_phi);
   tsub->SetBranchAddress("pdg",jet.pdg);
   tsub->SetBranchAddress("chg",jet.chg);
   tsub->SetBranchAddress("njet",&jet.njet);
   tsub->SetBranchAddress("et",jet.et);
   tsub->SetBranchAddress("eta",jet.eta);
   tsub->SetBranchAddress("phi",jet.phi);
   tsub->SetBranchAddress("area",jet.area);
   tsub->SetBranchAddress("vx",&jet.vx);
   tsub->SetBranchAddress("vy",&jet.vy);
   tsub->SetBranchAddress("vz",&jet.vz);
   tsub->SetBranchAddress("vr",&jet.vr);

//   tall->SetBranchAddress("njet",&jet2.njet);
//   tall->SetBranchAddress("et",jet2.et);
//   tall->SetBranchAddress("eta",jet2.eta);
//   tall->SetBranchAddress("phi",jet2.phi);
//   tall->SetBranchAddress("area",jet2.area);

   //-- Set Gen particle branches --
   ntgenp->Print();
   ntgenp->SetBranchAddress("pid",&genp.pid);
   ntgenp->SetBranchAddress("status",&genp.status);
   ntgenp->SetBranchAddress("pt",&genp.pt);
   ntgenp->SetBranchAddress("eta",&genp.eta);
   ntgenp->SetBranchAddress("phi",&genp.phi);
   ntgenp->SetBranchAddress("charge",&genp.charge);
   ntgenp->SetBranchAddress("subeid",&genp.subeid);
   ntgenp->SetBranchAddress("ishydro",&genp.ishydro);


   cout<<"B"<<endl;


   // Event Loop
   for(int i = 0; i< tsub->GetEntries() && i < maxEvents; ++i){    
     tsub->GetEntry(i);
//     tall->GetEntry(i);
     if (i % 1000 == 0) cout <<"Event "<<i<<endl;    
     // Selection on Events

     // Loop over GenJets

//     for(int j = 0; j < jet.njet; ++j){
//       if(jet.et[j] < jetEtCut) continue;
//
//       for(int j1 = 0; j1< jet.njet; ++j1){
//         if(jet.et[j1] < jetEtCut) continue;
//         double dR = deltaR(jet.eta[j],jet.phi[j],jet.eta[j1],jet.phi[j1]);
//
//	 if(dR != 0) 
//	   h1->Fill(dR);
//         if(dR < cone){
//
//         }
//       }
//
//       for(int j2 = 0; j2< jet2.njet; ++j2){
//	 if(jet2.et[j2] < jetEtCut) continue;
//	 double dR = deltaR(jet.eta[j],jet.phi[j],jet2.eta[j2],jet2.phi[j2]);
//
//         h2->Fill(dR);
//	 if(dR < match){
//	   het->Fill(jet.et[j],jet2.et[j2]);
//	 }
//       }
//      
//     }

   // Gen particle loop
     int topn = 20;
     float tmp =0;
     for(int ip = 0; ip < ntgenp->GetEntries(); ++ip) {
	ntgenp->GetEntry(ip);

	// find the incoming proton, start from there and count topn
	if ( genp.eta > 10000 ) {
	   float maxpt1 = 0;
	   float maxpt2 = 0;
	   for (int jp = 0; jp < topn; ++jp) {
	      ntgenp->GetEntry(ip+jp);
	      cout << ip+jp << "  " << genp.pid << "  " << genp.pt << "  " << genp.eta << endl;
	      if (genp.pt > maxpt1) {
		 maxpt1 = genp.pt;
	      }
	      if (genp.pt > maxpt2 && genp.pt < maxpt1)
		 maxpt2 = genp.pt;
	      //cout << "maxpt1: " << maxpt1 << "  maxpt2: " << maxpt2 << endl;
	   }
	   ip+=topn;
	   if (maxpt1>20)
	      cout << "maxpt1: " << maxpt1 << "  maxpt2: " << maxpt2 << endl;
	   //cin >>tmp;
	   if (tmp==0)
	      return 0;

	}
     }





   }


   cout<<"End."<<endl;

   TCanvas* c1 = new TCanvas();
   h1->Draw();

   TCanvas* c2 = new TCanvas();
   h2->Draw();

   TCanvas* c3 = new TCanvas();
   het->Draw("colz");


   c1->Write();
   c2->Write();
   c3->Write();


   h1->Write();
   h2->Write();
   het->Write();

   double overlap = h1->Integral(0,cone/(h1->GetBinWidth(1)));
   overlap /= h1->Integral();

   return overlap;
}
