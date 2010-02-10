// Implementation of template class: JetPlotsExample
// Description:  Example of simple EDAnalyzer for jets.
// Author: K. Kousouris
// Date:  25 - August - 2008
#include "RecoJets/JetAnalyzers/interface/JetPlotsExample.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "HepMC/HeavyIon.h"
#include "HepMC/GenEvent.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include <TFile.h>
#include <cmath>

using namespace edm;
using namespace reco;
using namespace std;
////////////////////////////////////////////////////////////////////////////////////////
template<class Jet>
JetPlotsExample<Jet>::JetPlotsExample(edm::ParameterSet const& cfg)
{
  JetAlgorithm  = cfg.getParameter<std::string> ("JetAlgorithm");
  HistoFileName = cfg.getParameter<std::string> ("HistoFileName");
  NJets         = cfg.getParameter<int> ("NJets");
}
////////////////////////////////////////////////////////////////////////////////////////
template<class Jet>
void JetPlotsExample<Jet>::beginJob() 
{
  TString hname;
  m_file = new TFile(HistoFileName.c_str(),"RECREATE"); 
  /////////// Booking histograms //////////////////////////
  hname = "JetPt";
  m_HistNames1D[hname] = new TH1F(hname,hname,200,0,200);
  hname = "JetEta";
  m_HistNames1D[hname] = new TH1F(hname,hname,120,-6,6);
  hname = "JetPhi";
  m_HistNames1D[hname] = new TH1F(hname,hname,100,-M_PI,M_PI);
  hname = "NumberOfJets";
  m_HistNames1D[hname] = new TH1F(hname,hname,100,0,100);
  
  // write tree
  ntjets = fs->make<TNtuple>("jets","leading jets in event","evt:b:npart:dphi:njet:njeta:ajet:ajeta:npjet:npjid:apjet:apjid:ndR:adR");

}
////////////////////////////////////////////////////////////////////////////////////////
template<class Jet>
void JetPlotsExample<Jet>::analyze(edm::Event const& evt, edm::EventSetup const& iSetup) 
{
  // === HI Event info ===
  Handle<HepMCProduct> MCevt;
  evt.getByLabel("generator",MCevt);
  HepMC::GenEvent * myGenEvent = new HepMC::GenEvent(*(MCevt->GetEvent()));
  HepMC::HeavyIon * hi = new HepMC::HeavyIon(*(myGenEvent->heavy_ion()));

  double b,Phi0;
  int Npart,Ncoll,Nhard;
  if(hi){
    b = hi->impact_parameter();
    Npart = hi->Npart_proj()+hi->Npart_targ();
    Ncoll = hi->Ncoll();
    Nhard = hi->Ncoll_hard();
    Phi0 = hi->event_plane_angle();
  }
  delete myGenEvent;
  delete hi;

  /////////// Get the jet collection //////////////////////
  Handle<JetCollection> jets;
  evt.getByLabel(JetAlgorithm,jets);
  typename JetCollection::const_iterator i_jet;
  int index = 0;
  TString hname; 
  /////////// Count the jets in the event /////////////////
  hname = "NumberOfJets";
  FillHist1D(hname,jets->size()); 
  cout << "N jets: " << jets->size() << endl;

  // ***add important safe guard***
  if (jets->size()<2) return;

  /////////// Fill Histograms for the leading NJet jets ///
  math::XYZTLorentzVector p4jet[2];
  math::XYZTLorentzVector p4parton[2];
  double dRMat[2] = {-1, -1};
  int idMat[2] = {-1, -1};
  typename JetCollection::const_iterator i_njet;
  typename JetCollection::const_iterator i_ajet;
  // get gen particles
  edm::Handle<std::vector<reco::GenParticle> > genParticlesHandle_;
  evt.getByLabel("hiGenParticles",genParticlesHandle_);
  /*
  for( size_t ip = 0; ip < genParticlesHandle_->size(); ++ ip ) {
    const reco::GenParticle & p = (*genParticlesHandle_)[ip];
    int id = p.pdgId();
    int st = p.status();
    math::XYZTLorentzVector genP4 = p.p4();
    if ((abs(id)>6 && id!=21) || st!=3) continue;
    std::cout << "particle " << ip << ": id=" << id << ", status=" << st << ", mass=" << genP4.mass() << ", pt=" <<  genP4.pt() << ", eta=" << genP4.eta() << std::endl; 
  }
  */
  cout << "check jet et" << endl;
  double lptMax=-1;
  for(i_jet = jets->begin(); i_jet != jets->end(); ++i_jet) {
    double lpt=i_jet->pt();
    cout << "jet et: " << lpt << endl;
    if (lpt>lptMax) {
      lptMax=lpt;
      i_njet=i_jet;
    }
  }
  // get away jet
  double aptMax=-1;
  for(i_jet = jets->begin(); i_jet != jets->end(); ++i_jet) {
    double dphi = fabs(reco::deltaPhi(i_njet->phi(),i_jet->phi()));
    if (dphi>2.0) {
      double apt=i_jet->pt();
      if (apt>aptMax) {
	aptMax=apt;
	i_ajet=i_jet;
      }
    }
  }

  // ***add important safe guard***
  if (aptMax<0) return;

  // summarize
  //cout << "near jet: " << lptMax << "  away jet: " << aptMax << endl;
  cout << "near jet et|eta|phi: " << i_njet->pt() << "|" << i_njet->eta() << "|" << i_njet->phi()
    << "  away jet: " << i_ajet->pt() << "|" << i_ajet->eta() << "|" << i_ajet->phi() 
    << "  dphi: " << fabs(reco::deltaPhi(i_njet->phi(),i_ajet->phi())) << endl << endl;
  for(i_jet = jets->begin(); i_jet != jets->end() && index < NJets; ++i_jet) 
    {
      hname = "JetPt";
      FillHist1D(hname,i_jet->pt());
      hname = "JetEta";
      FillHist1D(hname,i_jet->eta());
      hname = "JetPhi";
      FillHist1D(hname,i_jet->phi());

      // get lead jets
      p4jet[index] = i_jet->p4();
      //cout << "jet " << index << ": " << p4jet[index] << endl;

      // get jet partons
      //cout << "=== parton match " << endl;
      int ip_max = 0;
      double pt_max= 0;
      for( size_t ip = 0; ip < genParticlesHandle_->size(); ++ ip ) {
	const reco::GenParticle & p = (*genParticlesHandle_)[ip];
	// first check status
	int id = p.pdgId();
	int st = p.status();
	math::XYZTLorentzVector genP4 = p.p4();
	if ((abs(id)>6 && id!=21) || st!=3) continue;
	// now check dR
	double dR = reco::deltaR(i_jet->p4(),p.p4());
	if (dR<0.15) {
	  if ( p.pt()>pt_max) {
	    pt_max=p.pt();
	    ip_max=ip;
	    p4parton[index] = p.p4();
	    dRMat[index] = dR;
	    idMat[index] = id;
	  }
	  //cout << "particle " << ip << ": id=" << id << ", status=" << st << ", mass=" << genP4.mass() << ", pt=" <<  genP4.pt() << ", eta=" << genP4.eta() << std::endl; 
	}
      }
      /*
      if (pt_max>7) {
	const reco::GenParticle & p = (*genParticlesHandle_)[ip_max];
	math::XYZTLorentzVector genP4 = p.p4();
	cout << " --- matched ---" << endl;
	std::cout << "particle " << ip_max << " in jet: id=" << p.pdgId() << ", status=" << p.status() << ", mass=" << genP4.mass() << ", pt=" <<  genP4.pt() << ", eta=" << genP4.eta() << std::endl; 
	cout << "dR " << dRMat[index] << " - jet eta|phi: " << p4jet[index].eta() << "|" << p4jet[index].phi() << "  parton eta|phi: " << p4parton[index].eta() << "|" << p4parton[index].phi() << endl;
      }
      */
      index++;
    }
  cout << endl;

  // fill tree
  double dphi = fabs(reco::deltaPhi(p4jet[0].phi(),p4jet[1].phi()));
  ntjets->Fill(evt.id().event(),
      b,Npart,//Ncoll,
      //(p4jet[0]+p4jet[1]).mass(),
      dphi,
      p4jet[0].pt(),p4jet[0].eta(),//p4jet[0].phi(),
      p4jet[1].pt(),p4jet[1].eta(),//p4jet[1].phi(),
      p4parton[0].pt(), idMat[0],
      p4parton[1].pt(), idMat[1],
      dRMat[0],dRMat[1]
      );
}
////////////////////////////////////////////////////////////////////////////////////////
template<class Jet>
void JetPlotsExample<Jet>::endJob() 
{
  /////////// Write Histograms in output ROOT file ////////
  if (m_file !=0) 
    {
      m_file->cd();
      for (std::map<TString, TH1*>::iterator hid = m_HistNames1D.begin(); hid != m_HistNames1D.end(); hid++)
        hid->second->Write();
      delete m_file;
      m_file = 0;      
    }
}
////////////////////////////////////////////////////////////////////////////////////////
template<class Jet>
void JetPlotsExample<Jet>::FillHist1D(const TString& histName,const Double_t& value) 
{
  std::map<TString, TH1*>::iterator hid=m_HistNames1D.find(histName);
  if (hid==m_HistNames1D.end())
    std::cout << "%fillHist -- Could not find histogram with name: " << histName << std::endl;
  else
    hid->second->Fill(value);
}
/////////// Register Modules ////////
#include "FWCore/Framework/interface/MakerMacros.h"
/////////// Calo Jet Instance ////////
typedef JetPlotsExample<CaloJet> CaloJetPlotsExample;
DEFINE_FWK_MODULE(CaloJetPlotsExample);
/////////// Cen Jet Instance ////////
typedef JetPlotsExample<GenJet> GenJetPlotsExample;
DEFINE_ANOTHER_FWK_MODULE(GenJetPlotsExample);
/////////// PF Jet Instance ////////
typedef JetPlotsExample<PFJet> PFJetPlotsExample;
DEFINE_ANOTHER_FWK_MODULE(PFJetPlotsExample);
