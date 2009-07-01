#include <iostream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TProfile.h"
#include "TNtuple.h"
#include "TLegend.h"
#include "TDirectory.h"

#include "TGraphAsymmErrors.h"
#include "CmsHiFunctions.h"

#define MAXPARTICLES 50000
#define MAXJETS 5000
#define MAXHITS 5000
#define MAXVTX 100
#define ETABINS 3 

using namespace std;


/*
   void shiftHistCenter(TH1* h){
   int nbins = h->GetNbinsX();
   for(){


   }
   }

 */

int getBin(double input, int size, const double* edges){

   int bin = -9;

   for(int i = 0; i< size; i++){
      double min = edges[i];

      if(input >= min) bin  = i;
      if(input < min ) break;    
   }

   if(bin < 0) cout<<"Warning : Bin not determined correctly - "<<bin<<endl;
   return bin;
}


struct Event{

   int event;
   float b;
   float npart;
   float ncoll;
   float nhard;

   int n[ETABINS];
   float ptav[ETABINS];

   int algos;

   float vx;
   float vy;
   float vz;
   float vr;



};

struct Particles{

   int np;
   float pt[MAXPARTICLES];
   float eta[MAXPARTICLES];
   float phi[MAXPARTICLES];
   int pdg[MAXPARTICLES];
   int chg[MAXPARTICLES];

};

struct Jets{

   int njet;

   float et[MAXJETS];
   float eta[MAXJETS];
   float phi[MAXJETS];
   float area[MAXJETS];

   float r20[MAXJETS];
   float r50[MAXJETS];
   float r90[MAXJETS];

   float e02[MAXJETS];

   int ncons[MAXJETS];

};

// void analyze_jets(char * infile = "/home/yilmaz/analysis/jets/4TeV/uncorrected/pythia_mixed_d20090513.root"){
//  void analyze_jets(char * infile = "/Users/yetkinyilmaz/data/jets/pythia_dijet_pt80to120_hydjet_mb_4TeV_d20090513_run2.root"){

//void analyze_jets(char * infile = "/home/yilmaz/analysis/jets/4TeV/corrected/pythia_dijet_pt80to120_hydjet_mb_4TeV_d20090612/jet234.root"){

void analyze_jets(char * infile = "/home/yilmaz/analysis/jets/4TeV/corrected/pythia_dijet_pt80to120_hydjet_mb_4TeV_d20090615/run2/pre.root"){

   double r50max = 1000.15;
   double r50min = 0.;

   double e02max = 1000.15;
   double e02min = 0.;

   int version = 0;
   double match = 0.15;
   double genJetEt = 20;

   TH1::SetDefaultSumw2();

   TFile* inf = new TFile(infile);
   TTree* tsub = dynamic_cast<TTree*>(inf->Get("genevent/hi"));
   TTree* treco = dynamic_cast<TTree*>(inf->Get("recoevent/hi"));
   //  TTree* treco = dynamic_cast<TTree*>(inf->Get("corrected/hi"));

   bool doEfficiency = true;
   bool doFake = false;
   bool doEnergy = false;
   bool doProfiles = true;
   bool do2D = true;
   bool showFits = false;

   double x[6];
   double y[6];

   double etMax = 150;

   cout<<"Begin"<<endl;

   const int nbBins = 10;
   const int nEtBins = 13;
   const int nEtaBins = 3;

   int color[17] = {1,1,4,2,2,4,4,2,2,1,1,1,2,1,1,4,4};

   int etStep = 3;
   int bStep = 2;

   int bBegin = 0;
   int etBegin = 5;

   double bBins[nbBins + 1] = {0,5.045,7.145,8.755,10.105,11.294,12.373,13.359,14.283,15.202,18};
   double etBins[nEtBins + 1] = {0,20,30,40,45,50,55,60,65,70,75,80,100,200};
   double etaBins[nEtaBins + 1] = {0,1.3,3,5.2};

   const char* det[nEtaBins] = {"Barrel","Endcap","Forward"};

   int doBbins[3] = {0,2,8};

   double cone = 0.5;

   int maxEvents = 10000000;

   double etaMax;
   double etaMin;

   const char* outfile = Form("plots.root");

   TFile* outf = new TFile(outfile,"recreate");

   double binEt = 5;

   int nsize[3] = {20,10,5}; 

   TH1D* h1 = new TH1D(Form("h1_et%02d",(int)(version)),"Di-Jet Correlation;#Delta R;jets",200,0,6);
   TH1D* h2 = new TH1D(Form("h2_et%02d",(int)(version)),"#Delta R between RecoJets and GenJets;#Delta R;jet pairs",100,0,1);

   TH2D* hEtR = new TH2D(Form("hEtR%02d",(int)(version)),"#Delta p_{T} vs #Delta R between RecoJets and GenJets;#Delta R;(p_{T}^{GenJet} - p_{T}^{RecoJet})/p_{T}^{GenJet}",100,0,0.5,100,-2.5,2.5);//-etMax/1.2,etMax/1.2);

   TH1D* hres = new TH1D(Form("hres_et%02d",(int)(version)),"Jet Energy Resolution;p_{T}^{CaloJet}-p_{T}^{GenJet}/p_{T}^{GenJet};jets",200,-2.5,2.5);

   TH1D* hVtx = new TH1D(Form("Vertex",(int)(version)),"Vertex;v_{z}[cm];events",40,-20,20);
   TH1D* hB = new TH1D(Form("ImpactParameter",(int)(version)),"Impact Parameter;b[fm];events",50,0,20);

   TH2D* het[nbBins][nEtaBins];
   TH2D* hetrat[nbBins][nEtaBins];

   TProfile* hetprof[nbBins][nEtaBins];
   TProfile* hetratprof[nbBins][nEtaBins];

   TH2D* hnjet = new TH2D(Form("hnjet_et%02d",(int)(version)),";#_{GenJet};#_{CaloJet} [GeV]",100,0,50,100,0,50);

   TH1D* heff1 = new TH1D(Form("heff1_et%02d",(int)(version)),"Jet Finding Efficiency;GenJet p_{T} [GeV];efficiency",(int)(etMax/binEt),0,etMax);
   TH1D* heff2[nbBins][nEtaBins];

   TH1D* hEffPad = new TH1D(Form("hEffPad_et%02d",(int)(version)),"Jet Finding Efficiency;GenJet p_{T} [GeV];efficiency",nEtBins,etBins);
   TH1D* hFakePad = new TH1D(Form("hFakePad_et%02d",(int)(version)),"Fake Jets;CaloJet p_{T} [GeV];N_{Fake Jets}/N_{CaloJets}",nEtBins,etBins);

   TH1D* hfake1[nbBins][nEtaBins];

   TH1D* hcaloet[nbBins][nEtaBins];
   TH1D* hgenet[nbBins][nEtaBins];

   TH1D* bMatched[nEtBins][nEtaBins];
   TH1D* bAll[nEtBins][nEtaBins];

   TH1D* nPartMatched[nEtBins][nEtaBins];
   TH1D* nPartAll[nEtBins][nEtaBins];
   TH1D* bPad = new TH1D(Form("hbPad_et%02d",(int)(version)),";b [fm];efficiency",nbBins,bBins);
   TH1D* nPartPad= new TH1D(Form("hNpartPad_et%02d",(int)(version)),";N_{part};efficiency",10,0,400);

   TGraphAsymmErrors *gEff[nbBins][nEtaBins];
   TGraphAsymmErrors *gFake[nbBins][nEtaBins];
   TGraphAsymmErrors *gB[nEtBins][nEtaBins];
   TGraphAsymmErrors *gNpart[nEtBins][nEtaBins];


   TH1D* hsca1[nEtBins][nEtaBins];
   TH1D* hsca2[nbBins][nEtaBins];

   for(int ieta = 0; ieta < nEtaBins; ++ieta){

      for(int i1 = 0; i1 < nbBins; ++i1){

	 het[i1][ieta] = new TH2D(Form("het_et%02d_b%01d",i1,ieta),";p_{T}^{GenJet} [GeV];p_{T}^{CaloJet} [GeV]",75,0,etMax,75,0,etMax);
	 hetrat[i1][ieta] = new TH2D(Form("hetrat_b%02d_eta%01d",i1,ieta),";p_{T}^{GenJet} [GeV];p_{T}^{CaloJet}/p_{T}^{GenJet}",75,0,etMax,50,0,2.1);

	 hetprof[i1][ieta] = new TProfile(Form("hetprof_b%02d_eta%01d",i1,ieta),";p_{T}^{GenJet} [GeV];p_{T}^{CaloJet} [GeV]",20,0,etMax,"s");
	 hetratprof[i1][ieta] = new TProfile(Form("hetratprof_b%02d_eta%01d",i1,ieta),";p_{T}^{GenJet} [GeV];p_{T}^{CaloJet}/p_{T}^{GenJet}",20,0,etMax,"s");

	 hsca2[i1][ieta] = new TH1D(Form("hsca2_et%02d_eta%01d",i1,ieta),";p_{T}^{CaloJet}/p_{T}^{GenJet};jets",100,0,1);

	 heff2[i1][ieta] = new TH1D(Form("heff2_et%02d_eta%01d",i1,ieta),"Jet Finding Efficiency;GenJet p_{T} [GeV];efficiency",nEtBins,etBins);
	 hfake1[i1][ieta] = new TH1D(Form("hfake1_et%02d_eta%01d",i1,ieta),"N_{Fake Jets}/N_{CaloJets};CaloJet p_{T} [GeV];fake",nEtBins,etBins);
	 hcaloet[i1][ieta] = new TH1D(Form("hcaloet_et%02d_eta%01d",i1,ieta),";p_{T} [GeV];jets",nEtBins,etBins);
	 hgenet[i1][ieta] = new TH1D(Form("hgenet_et%02d_eta%01d",i1,ieta),";p_{T} [GeV];jets",nEtBins,etBins);

	 gEff[i1][ieta] = new TGraphAsymmErrors();
	 gFake[i1][ieta] = new TGraphAsymmErrors();

      }

      for(int i1 = 0; i1 < nEtBins; ++i1){
	 hsca1[i1][ieta] = new TH1D(Form("hsca1_et%02d_eta%01d",i1,ieta),";p_{T}^{CaloJet}/p_{T}^{GenJet};jets",100,0,1);

	 bMatched[i1][ieta] = new TH1D(Form("hbMatched_%02d_eta%01d",i1,ieta),";b [fm];jets",nbBins,bBins);
	 bAll[i1][ieta] = new TH1D(Form("hbAll_%02d_eta%01d",i1,ieta),";b [fm];jets",nbBins,bBins);
	 nPartMatched[i1][ieta] = new TH1D(Form("hNpartMatched_%02d_eta%01d",i1,ieta),";N_{part};efficiency",10,0,400);
	 nPartAll[i1][ieta] = new TH1D(Form("hNpartAll_%02d_eta%01d",i1,ieta),";N_{part};efficiency",10,0,400);

	 gB[i1][ieta] = new TGraphAsymmErrors();
	 gNpart[i1][ieta] = new TGraphAsymmErrors();
      }

   }

   TH2D* hEff2D = new TH2D(Form("hEff2D_et%02d",(int)(version)),";Npart;p_{T}^{GenJet} [GeV]",10,0,400,10,0,etMax);
   TH2D* hAll2D = new TH2D(Form("hAll2D_et%02d",(int)(version)),";Npart;p_{T}^{GenJet} [GeV]",10,0,400,10,0,etMax);

   TH2D* hFake2D = new TH2D(Form("hFake2D_et%02d",(int)(version)),";Npart;p_{T}^{CaloJet} [GeV]",10,0,400,10,0,etMax);
   TH2D* hCalo2D = new TH2D(Form("hCalo2D_et%02d",(int)(version)),";Npart;p_{T}^{CaloJet} [GeV]",10,0,400,10,0,etMax);


   cout<<"Histograms created."<<endl;

   Jets recojet; //RecoJets
   Jets genjet; //GenJets

   Particles par;

   Event event;

   // Event Info - Same for both jet collections
   treco->SetBranchAddress("b",&event.b);

   treco->SetBranchAddress("npart",&event.npart);
   treco->SetBranchAddress("ncoll",&event.ncoll);
   treco->SetBranchAddress("nhard",&event.nhard);

   treco->SetBranchAddress("vx",&event.vx);
   treco->SetBranchAddress("vy",&event.vy);
   treco->SetBranchAddress("vz",&event.vz);
   treco->SetBranchAddress("vr",&event.vr);

   treco->SetBranchAddress("n",event.n);
   treco->SetBranchAddress("ptav",event.ptav);


   // MC Info - Same for both jet collections 

   treco->SetBranchAddress("np",&par.np);
   treco->SetBranchAddress("par_pt",par.pt);
   treco->SetBranchAddress("par_eta",par.eta);
   treco->SetBranchAddress("par_phi",par.phi);
   treco->SetBranchAddress("pdg",par.pdg);
   treco->SetBranchAddress("chg",par.chg);

   // Reconstructed Jets
   treco->SetBranchAddress("njet",&recojet.njet);
   treco->SetBranchAddress("et",recojet.et);
   treco->SetBranchAddress("eta",recojet.eta);
   treco->SetBranchAddress("phi",recojet.phi);
   treco->SetBranchAddress("area",recojet.area);
   //   treco->SetBranchAddress("ncons",recojet.ncons);
   treco->SetBranchAddress("r20",recojet.r20);
   treco->SetBranchAddress("r50",recojet.r50);
   treco->SetBranchAddress("r90",recojet.r90);
   treco->SetBranchAddress("e02",recojet.e02);

   // Gen Jets
   tsub->SetBranchAddress("njet",&genjet.njet);
   tsub->SetBranchAddress("et",genjet.et);
   tsub->SetBranchAddress("eta",genjet.eta);
   tsub->SetBranchAddress("phi",genjet.phi);
   tsub->SetBranchAddress("area",genjet.area);
   //   tsub->SetBranchAddress("ncons",genjet.ncons);
   tsub->SetBranchAddress("r20",genjet.r20);
   tsub->SetBranchAddress("r50",genjet.r50);
   tsub->SetBranchAddress("r90",genjet.r90);
   tsub->SetBranchAddress("e02",genjet.e02);

   // Event Loop
   int nevents = tsub->GetEntries();
   cout<<"Number of Events : "<<nevents<<endl;
   for(int i = 0; i< nevents && i < maxEvents; ++i){    
      tsub->GetEntry(i);
      treco->GetEntry(i);
      int ngenjet = 0;
      int nrecojet = 0;
      bool counted = false;
      int jetmatch[MAXJETS];

      if (i % 1000 == 0) cout <<"Event "<<i<<endl;    
      // Selection on Events

      //     if(event.b > bMax || event.b < bMin ) continue;

      int bbin = getBin(event.b,nbBins,bBins);

      hVtx->Fill(event.vz);
      hB->Fill(event.b);

      // Loop over RecoJets
      for(int j = 0; j < recojet.njet; ++j){

	 double recojetetaj = etaVertex(recojet.eta[j],event.vz);

	 int etaReco = getBin(fabs(recojet.eta[j]),nEtaBins,etaBins);

	 hcaloet[bbin][etaReco]->Fill(recojet.et[j]);

	 hCalo2D->Fill(event.npart,recojet.et[j]);

	 nrecojet++;

	 for(int j1 = 0; j1< recojet.njet; ++j1){

	    double recojetetaj1 = etaVertex(recojet.eta[j1],event.vz);

	    double dR = deltaR(recojet.eta[j],recojet.phi[j],recojet.eta[j1],recojet.phi[j1]);

	    if(dR != 0) 
	       h1->Fill(dR);
	    if(dR < cone){

	    }
	 }
	 int j2match = -99;
	 double etmatch = 0;
	 for(int j2 = 0; j2< genjet.njet; ++j2){

	    // Ignore genjets below et cut
	    if( genjet.et[j2] < genJetEt ) continue;

	    double genjetetaj2 = etaVertex(genjet.eta[j2],event.vz);

	    double dR = deltaR(recojet.eta[j],recojet.phi[j],genjet.eta[j2],genjet.phi[j2]);
	    h2->Fill(dR);
	    double dET = genjet.et[j2] - recojet.et[j];
	    hEtR->Fill(dR,dET/genjet.et[j2]);

	    if(dR < match && genjet.et[j2] > etmatch){ 
	       j2match = j2;	 
	       etmatch = genjet.et[j2match];
	    }
	 }
	 jetmatch[j] = j2match;
	 if(j2match > -99){

	    int etBin = getBin(genjet.et[j2match], nEtBins, etBins);
	    int etaBin = getBin(fabs(genjet.eta[j2match]), nEtaBins, etaBins);

	    het[bbin][etaBin]->Fill(genjet.et[j2match],recojet.et[j]);
	    hetprof[bbin][etaBin]->Fill(genjet.et[j2match],recojet.et[j]);

	    hetrat[bbin][etaBin]->Fill(genjet.et[j2match],recojet.et[j]/genjet.et[j2match]);
	    hetratprof[bbin][etaBin]->Fill(genjet.et[j2match],recojet.et[j]/genjet.et[j2match]);

	    hres->Fill((recojet.et[j]-genjet.et[j2match])/genjet.et[j2match]);

	    if(bbin == 0)
	       hsca1[etBin][etaBin]->Fill(recojet.et[j]/genjet.et[j2match]);

	    if(etBin > 6)
	       hsca2[bbin][etaBin]->Fill(recojet.et[j]/genjet.et[j2match]);

	 }else{
	    //Jet is a fake

	    hfake1[bbin][etaReco]->Fill(recojet.et[j]);
	    hFake2D->Fill(event.npart,recojet.et[j]);
	 }      
      }

      //Second Loop over GenJets
      for(int j2 = 0; j2< genjet.njet; ++j2){
	 if(genjet.et[j2] < genJetEt) continue;

	 // Shape analysis:

	 if(genjet.r50[j2] > r50max) continue;
	 if(genjet.r50[j2] < r50min) continue;

	 if(genjet.e02[j2] < e02min) continue;
	 if(genjet.e02[j2] > e02max) continue;

	 double genjetetaj2 = etaVertex(genjet.eta[j2],event.vz);

	 int etBin = getBin(genjet.et[j2], nEtBins, etBins);
	 int etaBin = getBin(fabs(genjet.eta[j2]), nEtaBins, etaBins);

	 hAll2D->Fill(event.npart,genjet.et[j2]);
	 bAll[etBin][etaBin]->Fill(event.b);
	 nPartAll[etBin][etaBin]->Fill(event.npart); 

	 hgenet[bbin][etaBin]->Fill(genjet.et[j2]);
	 ngenjet++;
	 int counter  = 0;       
	 int j1match = -99;
	 double etmatch = 0;
	 for(int j1 = 0; j1< recojet.njet; ++j1){

	    double recojetetaj1 = etaVertex(recojet.eta[j1],event.vz);

	    double dR = deltaR(recojet.eta[j1],recojet.phi[j1],genjet.eta[j2],genjet.phi[j2]);
	    if(dR < match && genjet.et[j2] > etmatch){
	       j1match = j1;
	       etmatch = recojet.et[j1match];
	    }

	    // If RecoJet and GenJet match correctly and energy is reasonable                 
	    //         if(jetmatch[j1] == j2 && recojet.et[j1] < genjet.et[j2] )
	    if(jetmatch[j1] == j2 ){ 
	       heff2[bbin][etaBin]->Fill(genjet.et[j2]);
	       hEff2D->Fill(event.npart,genjet.et[j2]);
	       bMatched[etBin][etaBin]->Fill(event.b);
	       nPartMatched[etBin][etaBin]->Fill(event.npart);

	       counter++;
	       if(counter > 1){
		  cout<<"j2 is "<<j2<<endl;
		  cout<<" Jet Energy is "<<genjet.et[j2]<<endl;
	       }
	       break;
	    }
	 }     
	 if(j1match > -99){
	    //	 cout<<"j2 matched is "<<j2<<endl;
	    //         heff1->Fill(genjet.et[j2]);
	 }else{

	 }
      }

      hnjet->Fill(ngenjet,nrecojet);
   }

   hEffPad->SetMaximum(1.1);
   hFakePad->SetMaximum(1.1);

   nPartPad->SetMaximum(1.1);
   bPad->SetMaximum(1.1);

   hEffPad->GetXaxis()->CenterTitle();
   hEffPad->GetYaxis()->CenterTitle();
   hFakePad->GetXaxis()->CenterTitle();
   hFakePad->GetYaxis()->CenterTitle();
   nPartPad->GetXaxis()->CenterTitle();
   nPartPad->GetYaxis()->CenterTitle();
   bPad->GetXaxis()->CenterTitle();
   bPad->GetYaxis()->CenterTitle();

   TLegend *etLeg[nEtaBins];
   TLegend *etLeg2[nEtaBins];
   TLegend *bLeg[nEtaBins];
   TLegend *bLeg2[nbBins][nEtaBins];

   for(int ieta = 0; ieta < nEtaBins; ieta++){

      etaMin = etaBins[ieta];
      etaMax = etaBins[ieta+1];

      etLeg[ieta] = new TLegend(0.37,0.24,0.90,0.51,NULL,"brNDC");
      etLeg[ieta]->SetFillColor(0);
      etLeg[ieta]->SetTextSize(0.04);
      etLeg[ieta]->SetBorderSize(0);
      etLeg[ieta]->AddEntry(hEffPad,Form("%0.1f<|#eta|<%0.1f(%s)",etaMin,etaMax,det[ieta]),"");

      bLeg[ieta] = new TLegend(0.52,0.18,0.92,0.41,NULL,"brNDC");
      bLeg[ieta]->SetFillColor(0);
      bLeg[ieta]->SetTextSize(0.04);
      bLeg[ieta]->SetBorderSize(0);
      bLeg[ieta]->AddEntry(hEffPad,Form("%0.1f<|#eta|<%0.1f(%s)",etaMin,etaMax,det[ieta]),"");

      etLeg2[ieta] = new TLegend(0.46,0.71,0.93,0.92,NULL,"brNDC");
      etLeg2[ieta]->SetFillColor(0);
      etLeg2[ieta]->SetTextSize(0.04);
      etLeg2[ieta]->SetBorderSize(0);
      etLeg2[ieta]->AddEntry(hEffPad,Form("%0.1f<|#eta|<%0.1f(%s)",etaMin,etaMax,det[ieta]),"");

      for(int i1 = 0; i1 < 3; i1++){
	 int i = doBbins[i1];

	 bLeg2[i][ieta] = new TLegend(0.46,0.71,0.93,0.92,NULL,"brNDC");
	 bLeg2[i][ieta]->SetFillColor(0);
	 bLeg2[i][ieta]->SetTextSize(0.04);
	 bLeg2[i][ieta]->SetBorderSize(0);
	 bLeg2[i][ieta]->AddEntry(hEffPad,Form("%0.1f<|#eta|<%0.1f(%s)",etaMin,etaMax,det[ieta]),"");

	 gEff[i][ieta]->SetMarkerSize(1.);
	 gFake[i][ieta]->SetMarkerSize(1.);

	 gEff[i][ieta]->SetMarkerColor(color[i]);
	 gFake[i][ieta]->SetMarkerColor(color[i]);
	 gEff[i][ieta]->SetLineColor(color[i]);
	 gFake[i][ieta]->SetLineColor(color[i]);

	 gFake[i][ieta]->BayesDivide(hfake1[i][ieta],hcaloet[i][ieta]);
	 gEff[i][ieta]->BayesDivide(heff2[i][ieta],hgenet[i][ieta]);
	 bLeg[ieta]->AddEntry(gEff[i][ieta],Form("%0.01f<b<%0.01f [fm]",bBins[i],bBins[i+1]),"PL");
	 bLeg2[i][ieta]->AddEntry(hEffPad,Form("%0.01f<b<%0.01f [fm]",bBins[i],bBins[i+1]),"PL");

      }

      for(int i = etBegin; i < nEtBins; i = i+ etStep){

	 gB[i][ieta]->SetMarkerSize(1.);
	 gNpart[i][ieta]->SetMarkerSize(1.);

	 gB[i][ieta]->SetMarkerColor(color[i]);
	 gNpart[i][ieta]->SetMarkerColor(color[i]);
	 gB[i][ieta]->SetLineColor(color[i]);
	 gNpart[i][ieta]->SetLineColor(color[i]);

	 gB[i][ieta]->BayesDivide(bMatched[i][ieta],bAll[i][ieta]);
	 gNpart[i][ieta]->BayesDivide(nPartMatched[i][ieta],nPartAll[i][ieta]);
	 etLeg[ieta]->AddEntry(gB[i][ieta],Form("%d<p_{T}^{GenJet}<%d [GeV]",(int)etBins[i],(int)etBins[i+1]),"PL");	
	 //        etLeg2[ieta]->AddEntry(gB[i][ieta],Form("%d<p_{T}^{GenJet}<%d [GeV]",(int)etBins[i],(int)etBins[i+1]),"PL");

      }

      etLeg2[ieta]->AddEntry(hEffPad,"0-10% Central","");
      //      bLeg2[ieta]->AddEntry(hEffPad,"p_{T}^{GenJet} > 70 GeV","");

      //      etLeg2[ieta]->AddEntry(hEffPad,"Integral = 1","");
      //      bLeg2[ieta]->AddEntry(hEffPad,"Integral = 1","");

   }

   hEff2D->Divide(hAll2D);
   hFake2D->Divide(hCalo2D);

   cout<<"End."<<endl;

   if(0){
      TCanvas* e1 = new TCanvas(Form("Event Vertex",(int)(version)),Form("Event Vertex",(int)(version)),400,400);
      hVtx->Draw();
      TCanvas* e2 = new TCanvas(Form("Impact Parameter",(int)(version)),Form("Impact Parameter",(int)(version)),400,400);
      hB->Draw();
   }

   if(0){

      TCanvas* c1 = new TCanvas(Form("c1_et%02d",(int)(version)),Form("c1_et%02d",(int)(version)),400,400);
      h1->Draw();

      TCanvas* c2 = new TCanvas(Form("c2_et%02d",(int)(version)),Form("c2_et%02d",(int)(version)),400,400);
      h2->Draw();
   }

   if(0){


      TCanvas* c1_1 = new TCanvas(Form("DeltaR_deltaET_v%02d",(int)(version)),Form("DeltaR_deltaET_v%02d",(int)(version)),400,400);
      hEtR->Draw("colz");
      c1_1->Print(Form("DeltaR_deltaET.gif"));

   }

   for(int ieta = 0; ieta < nEtaBins; ieta++){

      for(int i3 = 0; i3 < 3; i3++){
	 int ib = doBbins[i3];

	 if(doEnergy){

	    het[ib][ieta]->FitSlicesY(0,0,-1,3,"Q R G5");
	    hetrat[ib][ieta]->FitSlicesY(0,0,-1,3,"Q R G5");

	    TH1D *rat_1 = (TH1D*)gDirectory->Get(Form("%s_1",hetrat[ib][ieta]->GetName()));
	    TH1D *rat_2 = (TH1D*)gDirectory->Get(Form("%s_2",hetrat[ib][ieta]->GetName()));
	    TH1D *rat_3 = dynamic_cast<TH1D*>(rat_2->Clone(Form("%s_div",hetrat[ib][ieta]->GetName())));

	    rat_3->Divide(rat_1);

	    rat_1->SetTitle(";p_{T}^{GenJet} [GeV];<p_{T}^{RecoJet}/p_{T}^{GenJet}>");
	    rat_2->SetTitle(";p_{T}^{GenJet} [GeV];#sigma(p_{T}^{RecoJet}/p_{T}^{GenJet})");
	    rat_3->SetTitle(";p_{T}^{GenJet} [GeV];#sigma(p_{T}^{RecoJet}/p_{T}^{GenJet})/<p_{T}^{RecoJet}/p_{T}^{GenJet}>");

	    rat_1->GetXaxis()->CenterTitle();
	    rat_1->GetYaxis()->CenterTitle();
	    rat_2->GetXaxis()->CenterTitle();
	    rat_2->GetYaxis()->CenterTitle();
	    rat_3->GetXaxis()->CenterTitle();
	    rat_3->GetYaxis()->CenterTitle();

	    rat_1->SetMaximum(2.1);
	    rat_1->SetMinimum(0);

	    rat_2->SetMaximum(2.1);
	    rat_2->SetMinimum(0);

	    rat_3->SetMaximum(2.1);
	    rat_3->SetMinimum(0);

	    TCanvas* cc1 = new TCanvas(Form("cEtScaleMean_b%02d_eta%01d",ib,ieta),Form("cEtScaleMean_b%02d_eta%01d",ib,ieta),700,700);
	    rat_1->Draw();
	    bLeg2[ib][ieta]->Draw();

	    cc1->Print(Form("cEtScaleMean_b%02d_eta%01d.gif",ib,ieta));

	    TCanvas* cc2 = new TCanvas(Form("cEtScaleSigma_et%02d_eta%01d",ib,ieta),Form("cEtScaleSigma_b%02d_eta%01d",ib,ieta),700,700);
	    rat_2->Draw();
	    bLeg2[ib][ieta]->Draw();
	    cc2->Print(Form("cEtScaleSigma_b%02d_eta%01d.gif",ib,ieta));

	    TCanvas* cc3 = new TCanvas(Form("cEtScaleSigmaOverMean_b%02d_eta%01d",ib,ieta),Form("cEtScaleSigmaOverMean_b%02d_eta%01d",ib,ieta),700,700);
	    rat_3->Draw();
	    bLeg2[ib][ieta]->Draw();
	    cc3->Print(Form("cEtScaleSigmaOverMean_b%02d_eta%01d.gif",ib,ieta));

	 }

	 if(doEnergy && do2D){

	    TCanvas* c3 = new TCanvas(Form("c3_b%02d_eta%01d",ib,ieta),Form("c3_b%02d_eta%01d",ib,ieta),400,400);
	    het[ib][ieta]->Draw("colz");
	    c3->Print(Form("EnergyScatter_b%02d_eta%01d.gif",ib,ieta));

	    TCanvas* c32 = new TCanvas(Form("c32_b%02d_eta%01d",ib,ieta),Form("c32_b%02d_eta%01d",ib,ieta),400,400);
	    hetprof[ib][ieta]->Draw("");
	    c32->Print(Form("EnergyProfile_b%02d_eta%01d.gif",ib,ieta));

	    TCanvas* c311 = new TCanvas(Form("c311_b%02d_eta%01d",ib,ieta),Form("c311_b%02d_eta%01d",ib,ieta),400,400);
	    hetrat[ib][ieta]->Draw("colz");
	    c311->Print(Form("EnergyScale_b%02d_eta%01d.gif",ib,ieta));

	    TCanvas* c312 = new TCanvas(Form("c312_et%02d_eta%01d",ib,ieta),Form("c312_et%02d_eta%01d",ib,ieta),700,700);

	    hetratprof[0][ieta]->Draw();
	    hetratprof[0][ieta]->GetXaxis()->CenterTitle();
	    hetratprof[0][ieta]->GetYaxis()->CenterTitle();

	    for(int i1 = 0; i1 < 3; i1++){
	       int i = doBbins[i1];
	       hetratprof[i][ieta]->SetMarkerColor(color[i]);
	       hetratprof[i][ieta]->SetLineColor(color[i]);

	       hetratprof[i][ieta]->Draw("same");
	    }

	    bLeg[ieta]->Draw();
	    c312->Print(Form("EnergyScaleProfile_eta%01d.gif",ieta));

	    TCanvas* c4 = new TCanvas(Form("c4_et%02d",(int)(version)),Form("c4_et%01d",ieta),400,400);
	    hres->Draw("");
	    c4->Print(Form("EnergyResolution_et%02d.gif",(int)(version)));

	    if(showFits){

	       TCanvas* c42 = new TCanvas(Form("c42_eta%01d",ieta),Form("c42_et%01d",ieta),400,400);
	       for(int i = etBegin; i < nEtBins; i = i + etStep){ 
		  hsca1[i][ieta]->Scale(1./hsca1[i][ieta]->GetEntries());
	       }

	       hsca1[etBegin][ieta]->Draw("hist");
	       for(int i = etBegin; i < nEtBins; i = i + etStep){
		  hsca1[i][ieta]->SetLineColor(color[i]);
		  hsca1[i][ieta]->Fit("gaus");
		  hsca1[i][ieta]->Draw("hist same");
	       }
	       etLeg2[ieta]->Draw();

	       c42->Print(Form("Energy_et_v%02d.gif",(int)(version)));

	       TCanvas* c422 = new TCanvas(Form("c422_et%02d",(int)(version)),Form("c422_et%02d",(int)(version)),400,400);

	       for(int i1 = 0; i1 < 3; i1++){
		  int i = doBbins[i1];
		  hsca2[i][ieta]->Scale(1./hsca2[i][ieta]->GetEntries());
	       }

	       hsca2[0][ieta]->Draw("hist ");

	       for(int i1 = 0; i1 < 3; i1++){
		  int i = doBbins[i1];
		  hsca2[i][ieta]->SetLineColor(color[i]);
		  hsca2[i][ieta]->Draw("hist same");
	       }

	       //   bLeg2[ib][ieta]->Draw();

	       c422->Print(Form("Energy_b_v%01d.gif",ieta));
	    }

	 }

	 if(do2D){

	    TCanvas* c5 = new TCanvas(Form("c5_et%02d",(int)(version)),Form("NumberOfJets_et%02d",(int)(version)),400,400);
	    hnjet->Draw("colz");
	    c5->Print(Form("NumberOfJets_et%02d.gif",(int)(version)));

	 }

	 if(doFake){

	    TCanvas* c7 = new TCanvas(Form("Fake1_et%01d",ieta),Form("Fake1_et%02d",(int)(version)),700,700);
	    hFakePad->Draw();
	    hFakePad->SetMaximum(0.3);
	    hFakePad->GetXaxis()->CenterTitle();
	    hFakePad->GetYaxis()->CenterTitle();

	    for(int i1 = 0; i1 < 3; i1++){
	       int i = doBbins[i1];
	       gFake[i][ieta]->Draw("P same");
	    }

	    bLeg[ieta]->Draw();
	    c7->Print(Form("JetFakes_eta%01d.gif",ieta));

	 }

	 if(doEfficiency){

	    TCanvas* c8 = new TCanvas(Form("Efficiency_%01d",ieta),Form("Efficiency_v%01d",ieta),700,700);
	    hEffPad->Draw();
	    hEffPad->GetXaxis()->CenterTitle();
	    hEffPad->GetYaxis()->CenterTitle();

	    for(int i1 = 0; i1 < 3; i1++){
	       int i = doBbins[i1];
	       gEff[i][ieta]->Draw("P same");
	    }

	    bLeg[ieta]->Draw();
	    c8->Print(Form("JetEfficiency_v%01d.gif",ieta));

	    TCanvas* c10 = new TCanvas(Form("Efficiency_vs_b_eta%01d",ieta),Form("Efficiency_vs_b%01d",ieta),700,700);
	    bPad->Draw();

	    bPad->GetXaxis()->CenterTitle();
	    bPad->GetYaxis()->CenterTitle();

	    for(int i = etBegin; i < nEtBins; i = i + etStep){
	       gB[i][ieta]->Draw("P same");
	    }

	    etLeg[ieta]->Draw();
	    c10->Print(Form("Efficiency_vs_b_v%02d.gif",ieta));

	    TCanvas* c11 = new TCanvas(Form("Efficiency_vs_Npart_v%01d",ieta),Form("Efficiency_vs_Npart_eta%01d",ieta),400,400);
	    nPartPad->Draw();
	    for(int i = etBegin; i < nEtBins; i = i+ etStep){
	       gNpart[i][ieta]->Draw("P same");
	    }

	    etLeg[ieta]->Draw();

	    c11->Print(Form("Efficiency_vs_Npart_eta%01d.gif",ieta));

	 }

	 if(do2D){

	    TCanvas* c13 = new TCanvas(Form("Efficiency2D_eta%01d",(int)(version)),Form("Efficiency2D",(int)(version)),400,400);
	    hEff2D->Draw("colz");
	    c13->Print(Form("Efficiency_2D_v%02d.gif",(int)(version)));

	    TCanvas* c14 = new TCanvas(Form("Fakes2D_et%02d",(int)(version)),Form("Fake2D",(int)(version)),400,400);
	    hFake2D->Draw("colz");
	    c14->Print(Form("Fakes_2D_v%02d.gif",(int)(version)));
	 }


      }

   }


   outf->Write();

}
