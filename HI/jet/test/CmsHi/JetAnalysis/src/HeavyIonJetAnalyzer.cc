// -*- C++ -*-
//
// Package:    HeavyIonJetAnalyzer
// Class:      HeavyIonJetAnalyzer
// 
/**\class HeavyIonJetAnalyzer HeavyIonJetAnalyzer.cc yetkin/HeavyIonJetAnalyzer/src/HeavyIonJetAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Yetkin Yilmaz
//         Created:  Tue Dec 18 09:44:41 EST 2007
// $Id: HeavyIonJetAnalyzer.cc,v 1.7 2009/06/17 16:08:19 yilmaz Exp $
//
//


// system include files
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"

#include "DataFormats/JetReco/interface/JetCollection.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"


#include "HepMC/GenEvent.h"
#include "HepMC/HeavyIon.h"

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

// root include file
#include "TFile.h"
#include "TNtuple.h"

#include "CmsHi/JetAnalysis/macros/CmsHiFunctions.h"

using namespace std;

//static const int PI = 3.14159265358979;

static const int MAXPARTICLES = 5000000;
static const int MAXJETS = 500000;
static const int MAXCONS = 200;

static const int MAXHITS = 50000;
static const int MAXVTX = 1000;
static const int ETABINS = 3; // Fix also in branch string

//
// class decleration
//

struct JetAxis{
  const reco::Jet* axis_;
  bool operator() (const reco::Candidate * cand1,const reco::Candidate * cand2){ 
    double dr1 = deltaR(axis_->eta(),axis_->phi(),cand1->eta(),cand1->phi());
    double dr2 = deltaR(axis_->eta(),axis_->phi(),cand2->eta(),cand2->phi());

    return dr1 < dr2;
  }
};

struct MyJet{

  float etjet;
  float etajet;
  float phijet;
  float area;
  int ncons;

  float r20;
  float r50;
  float r90;

  float e01;
  float e02;
  float e03;
  float e04;
  float e05;

  float et[MAXCONS];
  float eta[MAXCONS];
  float phi[MAXCONS];

  float dr[MAXCONS];

  int event;
  float b;

};


struct HydjetEvent{

   int event;
   float b;
   float npart;
   float ncoll;
   float nhard;
   float phi0;

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

  int ncons[MAXJETS];

  float r20[MAXJETS];
  float r50[MAXJETS];
  float r90[MAXJETS];

  float e01[MAXJETS];
  float e02[MAXJETS];
  float e03[MAXJETS];
  float e04[MAXJETS];
  float e05[MAXJETS];
 
   float vx;
   float vy;
   float vz;
   float vr;

};

class HeavyIonJetAnalyzer : public edm::EDAnalyzer {
   public:
      explicit HeavyIonJetAnalyzer(const edm::ParameterSet&);
      ~HeavyIonJetAnalyzer();
    bool sortDeltaR(const reco::Candidate * cand1,const reco::Candidate * cand2);

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

   std::ofstream out_b;
   std::string fBFileName;

   std::ofstream out_n;
   std::string fNFileName;

   std::ofstream out_m;
   std::string fMFileName;

  
   TTree* hydjetTree_;
  TTree* jetTree_;

   HydjetEvent hev_;
  MyJet yet_;
   
   // count # of events
   int nEvt;

   TNtuple *ntpart;

   std::string output;           // Output filename
 
   bool doAnalysis_;
   bool doParticles_;
   bool printLists_;
   bool doCF_;
   bool doVertices_;
   double etaMax_;
   double ptMin_;

   vector<string> jetSrc_;
   string hepmcSrc_;
   edm::ESHandle < ParticleDataTable > pdt;
   edm::Service<TFileService> f;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HeavyIonJetAnalyzer::HeavyIonJetAnalyzer(const edm::ParameterSet& iConfig) :
   nEvt(0)
{
   //now do what ever initialization is needed
   fBFileName = iConfig.getUntrackedParameter<std::string>("output_b", "b_values.txt");
   fNFileName = iConfig.getUntrackedParameter<std::string>("output_n", "n_values.txt");
   fMFileName = iConfig.getUntrackedParameter<std::string>("output_m", "m_values.txt");
   doAnalysis_ = iConfig.getUntrackedParameter<bool>("doAnalysis", true);
   doParticles_ = iConfig.getUntrackedParameter<bool>("doParticles", true);
   printLists_ = iConfig.getUntrackedParameter<bool>("printLists", false);
   doCF_ = iConfig.getUntrackedParameter<bool>("doMixed", false);
   doVertices_ = iConfig.getUntrackedParameter<bool>("doVertices", false);
   jetSrc_ = iConfig.getParameter<vector<string> >("jetSrc");
   hepmcSrc_ = iConfig.getUntrackedParameter<string>("hepmcSrc","generator");

   etaMax_ = iConfig.getUntrackedParameter<double>("etaMax", 2);
   ptMin_ = iConfig.getUntrackedParameter<double>("ptMin", 0);

   // Output

}


HeavyIonJetAnalyzer::~HeavyIonJetAnalyzer()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ method called to for each event  ------------
void
HeavyIonJetAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  cout<<"Analyze"<<" " << jetSrc_[0].c_str() << "  Event: " << nEvt <<endl;

   using namespace edm;
   using namespace HepMC;
  
//   hev_.event = iEvent.id().event();
   hev_.event = nEvt;
   for(int ieta = 0; ieta < ETABINS; ++ieta){
      hev_.n[ieta] = 0;
      hev_.ptav[ieta] = 0;
   }

   hev_.njet = 0;
   hev_.np = 0;

   double b = -1;
   int npart = -1;
   int ncoll = -1;
   int nhard = -1;
   double phi0 = -99;

   double vx = -99;
   double vy = -99;
   double vz = -99;
   double vr = -99;
   const GenEvent* evt;
   
   if(doCF_){

     Handle<CrossingFrame<HepMCProduct> > cf;
     iEvent.getByLabel(InputTag("mix","source"),cf);

     /*

     MixCollection<HepMCProduct> mix(cf.product());

     int mixsize = mix.size();

     cout<<"Mix Collection Size: "<<mixsize<<endl;
     evt = mix.getObject(mixsize-1).GetEvent();

     MixCollection<HepMCProduct>::iterator begin = mix.begin();
     MixCollection<HepMCProduct>::iterator end = mix.end();
     
     for(MixCollection<HepMCProduct>::iterator mixit = begin; mixit != end; ++mixit){

       const GenEvent* subevt = (*mixit).GetEvent();
       int all = subevt->particles_size();
       HepMC::GenEvent::particle_const_iterator begin = subevt->particles_begin();
       HepMC::GenEvent::particle_const_iterator end = subevt->particles_end();
       for(HepMC::GenEvent::particle_const_iterator it = begin; it != end; ++it){
	 if((*it)->status() == 1){
           float pdg_id = (*it)->pdg_id();
           float eta = (*it)->momentum().eta();
           float pt = (*it)->momentum().perp();
           const ParticleData * part = pdt->particle(pdg_id );
           float charge = part->charge();
	   }
	 }
       }
       
     }     

     */

   }else{
      
      Handle<HepMCProduct> mc;
      iEvent.getByLabel(hepmcSrc_,mc);
      evt = mc->GetEvent();

      if(doParticles_){
      int all = evt->particles_size();
      HepMC::GenEvent::particle_const_iterator begin = evt->particles_begin();
      HepMC::GenEvent::particle_const_iterator end = evt->particles_end();
      for(HepMC::GenEvent::particle_const_iterator it = begin; it != end; ++it){
	if((*it)->status() == 1){
	   int pdg_id = (*it)->pdg_id();
	   float eta = (*it)->momentum().eta();
           float phi = (*it)->momentum().phi();
	   float pt = (*it)->momentum().perp();
	  const ParticleData * part = pdt->particle(pdg_id );
	  int charge = part->charge();

	  hev_.par_pt[hev_.np] = pt;
          hev_.par_eta[hev_.np] = eta;
          hev_.par_phi[hev_.np] = phi;
          hev_.pdg[hev_.np] = pdg_id;
          hev_.chg[hev_.np] = charge;

	  eta = fabs(eta);
	  int etabin = 0;
	  if(eta > 0.5) etabin = 1; 
	  if(eta > 1.) etabin = 2;
	  if(eta < 2.){
	     hev_.ptav[etabin] += pt;
	     ++(hev_.n[etabin]);
	  }
	  ++(hev_.np);
	}
      }
      }

   }

   const HeavyIon* hi = evt->heavy_ion();
   if(hi){
      b = hi->impact_parameter();
      npart = hi->Npart_proj()+hi->Npart_targ();
      ncoll = hi->Ncoll();
      nhard = hi->Ncoll_hard();
      phi0 = hi->event_plane_angle();

      if(printLists_){
	 out_b<<b<<endl;
	 out_n<<npart<<endl;
      }

   }
   
   //   edm::Handle<reco::GenJetCollection> genjets;
   edm::Handle<reco::JetView> genjets;
   //   edm::Handle<vector<reco::Jet> > genjets;
   iEvent.getByLabel(jetSrc_[0],genjets);
   for(int ijet = 0; ijet < genjets->size(); ++ijet){

      const reco::Jet* jet = &((*genjets)[ijet]);
      //	 const reco::GenJet* jet = dynamic_cast<const reco::GenJet*>(&((*genjets)[ijet]));

      //cout<<"Jet Quantities : "<<jet->et()<<" "<<jet->eta()<<" "<<jet->phi()<<" "<<jet->jetArea()<<endl;
      printf("**%s Info: evtn|et|eta|phi: %d | %f | %f | %f**\n", jetSrc_[0].c_str(),nEvt, jet->et(),jet->eta(),jet->phi());

      double phi = jet->phi()-phi0;
      if(phi > PI) phi = phi - 2*PI;
      if(phi < -PI) phi = phi + 2*PI;
      
      hev_.et[hev_.njet] = jet->et();
      hev_.eta[hev_.njet] = jet->eta();
      hev_.phi[hev_.njet] = phi;

      hev_.area[hev_.njet] = jet->jetArea();
      printf("                     Fill tree: evtn|jetn|et|eta|phi: %d | %d | %f | %f | %f**\n", hev_.event,hev_.njet,hev_.et[hev_.njet],hev_.eta[hev_.njet],hev_.phi[hev_.njet]);

      // Constituents
      std::vector<const reco::Candidate*> members = jet->getJetConstituentsQuick();

      int nm = members.size();

      hev_.ncons[hev_.njet] = nm;
      yet_.ncons = nm;

      cout<<"Jet has "<<nm<<" constituents."<<endl;
      
      JetAxis jaxis;
      jaxis.axis_ = jet;
      
      //      cout<<"Jet axis is at : "<<jaxis.axis_->eta()<<"  "<<jaxis.axis_->phi()<<endl;

      sort(members.begin(),members.end(),jaxis);

      double sum = 0;
      double et = jet->et();
      double r20 = -99;
      double r50 = -99;
      double r90 = -99;

      double e0[6] = {0,0,0,0,0,0};

      for(int im = 0; im < nm; ++im){
	const reco::Candidate* candi = members[im];
	double dr = deltaR(jet->eta(),jet->phi(),candi->eta(),candi->phi());
//	cout<<"Constiutuent's Delta R = "<<dr<<endl;

	double phicon = candi->phi()-phi0;
	if(phicon > PI) phicon = phicon - 2*PI;
	if(phicon < -PI) phicon = phicon + 2*PI;


        yet_.et[im] = candi->et();
        yet_.eta[im] = candi->eta();
	yet_.phi[im] = phicon;	
	yet_.dr[im] = dr;

	for(int ir = 1; ir <6; ++ir){
	  if(dr < 0.1*ir){
	    e0[ir] += candi->et();	  
	  }
	}
	
	sum += candi->et();
	
	if(sum > et*0.2 && r20 == -99) r20 = dr;
	if(sum > et*0.5 && r50 == -99) r50 = dr;
        if(sum > et*0.9 && r90 == -99) r90 = dr;
	
      }

      hev_.r20[hev_.njet] = r20;
      hev_.r50[hev_.njet] = r50;
      hev_.r90[hev_.njet] = r90;

      hev_.e01[hev_.njet] = e0[1]/et;
      hev_.e02[hev_.njet] = e0[2]/et;
      hev_.e03[hev_.njet] = e0[3]/et;
      hev_.e04[hev_.njet] = e0[4]/et;
      hev_.e05[hev_.njet] = e0[5]/et;

      yet_.e01 = e0[1]/et;
      yet_.e02 = e0[2]/et;
      yet_.e03 = e0[3]/et;
      yet_.e04 = e0[4]/et;
      yet_.e05 = e0[5]/et;

      yet_.etjet = jet->et();
      yet_.etajet = jet->eta();
      yet_.phijet = phi;
      yet_.area = jet->jetArea();

      yet_.event = hev_.event;
      yet_.b = hev_.b;

      jetTree_->Fill();

      ++(hev_.njet);
      
   }
   
   if(doVertices_){   

     HepMC::GenVertex* genvtx = evt->signal_process_vertex();
     if(!genvtx){
       cout<<"No Signal Process Vertex!"<<endl;
       HepMC::GenEvent::particle_const_iterator pit=evt->particles_begin();
       HepMC::GenEvent::particle_const_iterator ptend=evt->particles_end();
       while(!genvtx || ( genvtx->particles_in_size() == 1 && pit != ptend ) ){
	 if(!genvtx) cout<<"No Gen Vertex!"<<endl;
	 ++pit;
	 if(pit == ptend) cout<<"End reached!"<<endl;
	 genvtx = (*pit)->production_vertex();
       }
     }

   vx = genvtx->position().x()/10;
   vy = genvtx->position().y()/10;
   vz = genvtx->position().z()/10;
   vr = genvtx->position().rho()/10;

   }

   hev_.b = b;
   hev_.npart = npart;
   hev_.ncoll = ncoll;
   hev_.nhard = nhard;
   hev_.phi0 = phi0;

   hev_.vx = vx;
   hev_.vy = vy;
   hev_.vz = vz;
   hev_.vr = vr;

   hydjetTree_->Fill();
   ++nEvt;
}


// ------------ method called once each job just before starting event loop  ------------
void 
HeavyIonJetAnalyzer::beginJob(const edm::EventSetup& iSetup)
{
  cout<<"Begin"<<endl;

   iSetup.getData(pdt);

   if(printLists_){
      out_b.open(fBFileName.c_str());
      if(out_b.good() == false)
	 throw cms::Exception("BadFile") << "Can\'t open file " << fBFileName;
      out_n.open(fNFileName.c_str());
      if(out_n.good() == false)
	 throw cms::Exception("BadFile") << "Can\'t open file " << fNFileName;
      out_m.open(fMFileName.c_str());
      if(out_m.good() == false)
	 throw cms::Exception("BadFile") << "Can\'t open file " << fMFileName;
   }   
   
   if(doAnalysis_){
      hydjetTree_ = f->make<TTree>("hi","Tree of Hydjet Events");
      jetTree_ = f->make<TTree>("jet","Tree of Jet Constituents");

      hydjetTree_->Branch("event",&hev_.event,"event/I");
      hydjetTree_->Branch("b",&hev_.b,"b/F");
      hydjetTree_->Branch("npart",&hev_.npart,"npart/F");
      hydjetTree_->Branch("ncoll",&hev_.ncoll,"ncoll/F");
      hydjetTree_->Branch("nhard",&hev_.nhard,"nhard/F");
      hydjetTree_->Branch("phi0",&hev_.phi0,"phi0/F");

      if(doParticles_){
	 hydjetTree_->Branch("n",hev_.n,"n[3]/I");
	 hydjetTree_->Branch("ptav",hev_.ptav,"ptav[3]/F");
	 hydjetTree_->Branch("np",&hev_.np,"np/I");
	 hydjetTree_->Branch("par_pt",hev_.par_pt,"par_pt[np]/F");
	 hydjetTree_->Branch("par_eta",hev_.par_eta,"par_eta[np]/F");
	 hydjetTree_->Branch("par_phi",hev_.par_phi,"par_phi[np]/F");
	 hydjetTree_->Branch("pdg",hev_.pdg,"pdg[np]/I");
	 hydjetTree_->Branch("chg",hev_.chg,"chg[np]/I");
      }

      hydjetTree_->Branch("vx",&hev_.vx,"vx/F");
      hydjetTree_->Branch("vy",&hev_.vy,"vy/F");
      hydjetTree_->Branch("vz",&hev_.vz,"vz/F");
      hydjetTree_->Branch("vr",&hev_.vr,"vr/F");

      hydjetTree_->Branch("njet",&hev_.njet,"njet/I");
      hydjetTree_->Branch("et",hev_.et,"et[njet]/F");
      hydjetTree_->Branch("eta",hev_.eta,"eta[njet]/F");
      hydjetTree_->Branch("phi",hev_.phi,"phi[njet]/F");
      hydjetTree_->Branch("area",hev_.area,"area[njet]/F");

      hydjetTree_->Branch("r20",hev_.r20,"r20[njet]/F");
      hydjetTree_->Branch("r50",hev_.r50,"r50[njet]/F");
      hydjetTree_->Branch("r90",hev_.r90,"r90[njet]/F");

      hydjetTree_->Branch("e01",hev_.e01,"e01[njet]/F");
      hydjetTree_->Branch("e02",hev_.e02,"e02[njet]/F");
      hydjetTree_->Branch("e03",hev_.e03,"e03[njet]/F");
      hydjetTree_->Branch("e04",hev_.e04,"e04[njet]/F");
      hydjetTree_->Branch("e05",hev_.e05,"e05[njet]/F");

      hydjetTree_->Branch("ncons",hev_.ncons,"ncons[njet]/I");

      jetTree_->Branch("event",&yet_.event,"event/I");
      jetTree_->Branch("b",&hev_.b,"b/F");

      jetTree_->Branch("ncons",&yet_.ncons,"ncons/I");
      jetTree_->Branch("et",yet_.et,"et[ncons]/F");
      jetTree_->Branch("eta",yet_.eta,"eta[ncons]/F");
      jetTree_->Branch("phi",yet_.phi,"phi[ncons]/F");
      jetTree_->Branch("dr",yet_.dr,"dr[ncons]/F");

      jetTree_->Branch("etjet",&yet_.etjet,"etjet/F");
      jetTree_->Branch("etajet",&yet_.etajet,"etajet/F");
      jetTree_->Branch("phijet",&yet_.phijet,"phijet/F");

      jetTree_->Branch("area",&yet_.area,"area/F");
      jetTree_->Branch("r20",&yet_.r20,"r20/F");
      jetTree_->Branch("r50",&yet_.r50,"r50/F");
      jetTree_->Branch("r90",&yet_.r90,"r90/F");

      jetTree_->Branch("e01",&yet_.e01,"e01/F");
      jetTree_->Branch("e02",&yet_.e02,"e02/F");
      jetTree_->Branch("e03",&yet_.e03,"e03/F");
      jetTree_->Branch("e04",&yet_.e04,"e04/F");
      jetTree_->Branch("e05",&yet_.e05,"e05/F");



   }
  
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HeavyIonJetAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HeavyIonJetAnalyzer);
