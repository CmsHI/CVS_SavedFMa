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
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "HepMC/HeavyIon.h"
#include "HepMC/GenEvent.h"
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
  m_HistNames1D[hname] = new TH1F(hname,hname,100,0,1000);
  hname = "JetEta";
  m_HistNames1D[hname] = new TH1F(hname,hname,120,-6,6);
  hname = "JetPhi";
  m_HistNames1D[hname] = new TH1F(hname,hname,100,-M_PI,M_PI);
  hname = "NumberOfJets";
  m_HistNames1D[hname] = new TH1F(hname,hname,100,0,100);
  
  // write tree
  ntjets = fs->make<TNtuple>("jets","leading jets in event","evt:b:npart:ncoll:mass:dphi:njet:njeta:njphi:ajet:ajeta:ajphi");

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
  /////////// Fill Histograms for the leading NJet jets ///
  math::XYZTLorentzVector p4jet[2];
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
      index++;
    }

  // fill tree
  double dphi = fabs(reco::deltaPhi(p4jet[0].phi(),p4jet[1].phi()));
  ntjets->Fill(evt.id().event(),
      b,Npart,Ncoll,
      (p4jet[0]+p4jet[1]).mass(),dphi,
      p4jet[0].pt(),p4jet[0].eta(),p4jet[0].phi(),
      p4jet[1].pt(),p4jet[1].eta(),p4jet[1].phi()
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
