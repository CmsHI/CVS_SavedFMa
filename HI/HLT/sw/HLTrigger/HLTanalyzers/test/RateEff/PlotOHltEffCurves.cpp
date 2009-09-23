#define PlotOHltEffCurves_cxx

#include "OHltTree.h"
#include <TString.h>
#include "TH1.h"
#include "TH2.h"
#include "TNtuple.h"

using namespace std;
Float_t OHltTree::DeltaR(Float_t eta1, Float_t phi1, Float_t eta2, Float_t phi2)
{
   Float_t deta = eta2 - eta1;
   Float_t dphi = phi2 - phi2;
   Float_t dr = sqrt((deta*deta) + (dphi*dphi));
   return dr;
}

void OHltTree::PlotOHltEffCurves(OHltConfig *cfg,TString hlteffmode,TString ohltobject,TH1F* &h1,TH1F* &h2,TH1F* &h3,TH1F* &h4,TNtuple* ntlead)
{
  // Generic N, pT, eta, phi varibles
  Int_t nhlt=0;
  Float_t hltpt[500];
  Float_t hlteta[500];
  Float_t hltphi[500];
  Int_t nl1=0;
  Float_t l1pt[500];
  Float_t l1eta[500];
  Float_t l1phi[500];
  Int_t nrec=0;
  Float_t recopt[500];
  Float_t recoeta[500];
  Float_t recophi[500];
  Int_t mctruthpid = -1;
  // generic leading varialbes
  Float_t lrecpt=-10, lreceta=-10, lrecphi=-10, lgenpt=-10, lgeneta=-10, lgenphi=-10, ll1pt=-10, ll1eta=-10, ll1phi=-10;
  Float_t l1dr=-10, recdr=-10;
  // other
  Int_t l1bit=-10, hltbit=-10;


  if(ohltobject == "muon")
    {
      nhlt=NohMuL3;
      for(int i=0;i<nhlt;i++){
        hltpt[i] = ohMuL3Pt[i];
        hlteta[i] = ohMuL3Eta[i];
        hltphi[i] = ohMuL3Phi[i];
      }
      if (cfg->selectBranchL1extra) nl1= NL1Mu;
      for(int i=0;i<nl1;i++){
        l1pt[i] = L1MuPt[i];
        l1eta[i] = L1MuEta[i];
        l1phi[i] = L1MuPhi[i];
      }
      if (cfg->selectBranchReco) nrec=NrecoMuon;
      for(int i=0;i<nrec;i++){
        recopt[i] = recoMuonPt[i];
        recoeta[i] = recoMuonEta[i];
        recophi[i] = recoMuonPhi[i];
      }
      mctruthpid = 13;
    }
  if(ohltobject == "ele_mu") //RR fill only the mu leg efficiency
    {
      nhlt=NohMuL3;
      for(int i=0;i<nhlt;i++){
        hltpt[i] = ohMuL3Pt[i];
        hlteta[i] = ohMuL3Eta[i];
        hltphi[i] = ohMuL3Phi[i];
      }
      if (cfg->selectBranchL1extra) nl1= NL1Mu;
      for(int i=0;i<nl1;i++){
        l1pt[i] = L1MuPt[i];
        l1eta[i] = L1MuEta[i];
        l1phi[i] = L1MuPhi[i];
      }
      if (cfg->selectBranchReco) nrec=NrecoMuon;
      for(int i=0;i<nrec;i++){
        recopt[i] = recoMuonPt[i];
        recoeta[i] = recoMuonEta[i];
        recophi[i] = recoMuonPhi[i];
      }
      mctruthpid = 13;
    }
  else if(ohltobject == "photon")
    {
      nhlt=NohPhot;
      for(int i=0;i<nhlt;i++){
        hltpt[i] = ohPhotEt[i];
        hlteta[i] = ohPhotEta[i];
        hltphi[i] = ohPhotPhi[i];
      }
     if (cfg->selectBranchL1extra) nl1= NL1IsolEm;
      for(int i=0;i<nl1;i++){
        l1pt[i] = L1IsolEmEt[i];
        l1eta[i] = L1IsolEmEta[i];
        l1phi[i] = L1IsolEmPhi[i];
      }
      if (cfg->selectBranchReco) nrec=NrecoPhot;
      for(int i=0;i<nrec;i++){
        recopt[i] = recoPhotEt[i];
        recoeta[i] = recoPhotEta[i];
        recophi[i] = recoPhotPhi[i];
      }
      mctruthpid = 22;
    }
  else if(ohltobject == "electron")
    {
      nhlt=NohEle;
      for(int i=0;i<nhlt;i++){
        hltpt[i] = ohEleEt[i];
        hlteta[i] = ohEleEta[i];
        hltphi[i] = ohElePhi[i];
        // No pixel-matching!
      }
      if (cfg->selectBranchL1extra) nl1= NL1IsolEm;
      for(int i=0;i<nl1;i++){
        l1pt[i] = L1IsolEmEt[i];
        l1eta[i] = L1IsolEmEta[i];
        l1phi[i] = L1IsolEmPhi[i];
      }
      if (cfg->selectBranchReco) nrec=NrecoElec;
      for(int i=0;i<nrec;i++){
        recopt[i] = recoElecEt[i];
        recoeta[i] = recoElecEta[i];
        recophi[i] = recoElecPhi[i];
      }
      mctruthpid = 11;
    }
  else if(ohltobject == "jet")
    {
      nhlt=NrecoJetCal;
      l1bit=map_BitOfStandardHLTPath["L1_SingleJet15"];
      for(int i=0;i<nhlt;i++){
        hltpt[i] = recoJetCalPt[i];
        hlteta[i] = recoJetCalEta[i];
        hltphi[i] = recoJetCalPhi[i];
      }
      if (cfg->selectBranchL1extra) nl1= NL1CenJet;
      for(int i=0;i<nl1;i++){
        l1pt[i] = L1CenJetEt[i];
        l1eta[i] = L1CenJetEta[i];
        l1phi[i] = L1CenJetPhi[i];
      }
      if (cfg->selectBranchReco) nrec=NrecoJetCal;
      for(int i=0;i<nrec;i++){
        recopt[i] = recoJetCalPt[i];
        recoeta[i] = recoJetCalEta[i];
        recophi[i] = recoJetCalPhi[i];
        if(ntlead) cout << "EffCurve% jet:" << i << "  pt: " << recopt[i] << "  l1bit: " << l1bit << endl;
      }
      mctruthpid = 21; // gluons - probably should be GenJets or something
    }

  // === fill leading ntuple ===
  if(ntlead) {
     if (NrecoJetGen>0) {
	lgenpt = recoJetGenPt[0];
	lgeneta = recoJetGenEta[0];
	lgenphi = recoJetGenPhi[0];
     }
     if (nrec>0) {
	lrecpt = recopt[0];
	lreceta = recoeta[0];
	lrecphi = recophi[0];
     }
     if (nl1>0) {
	ll1pt = l1pt[0];
	ll1eta = l1eta[0];
	ll1phi = l1phi[0];
     }
     if (NrecoJetGen>0 && nrec>0) {
	l1dr = DeltaR(ll1eta,ll1phi,lgeneta,lgenphi);;
	recdr = DeltaR(lreceta,lrecphi,lgeneta,lgenphi);;
     }
     ntlead->Fill(lgenpt,lgeneta,lgenphi,lrecpt,lreceta,lrecphi,
	   ll1pt,ll1eta,ll1phi,l1bit,hltbit,l1dr,recdr);
  }

  // Now really make efficiency curves

  Int_t pass = 0;
  Float_t etacutforpt = 3.0;
  Float_t ptcutforeta = 5.0;
  Float_t drmatch = 0.5;

  if (ntlead) cout << "Eff mode: " << hlteffmode << endl;
  // Do Efficiency of leading Detector Physics Object Reco/GEN
  if (hlteffmode == "GEN" && ntlead)
  {
     if (lgenpt>0) {
	if(lrecpt>50)
	{
	   /*
	    * Any additional cuts on the numerator for the HLT efficiency (isolation,
	    * matching, tagging, etc.) will go here!
	    */
	   pass = 1;
	}
	cout << "recdr: " << recdr << "  pass? " << pass << endl;
	// Fill histograms for eff vs. pT
	if(fabs(lgeneta) < etacutforpt)
	{
	   h2->Fill(lgenpt);
	   if(pass == 1)
	      h1->Fill(lgenpt);
	}

	// Fill histograms for eff vs. eta
	if(lgenpt > ptcutforeta)
	{
	   h4->Fill(lgeneta);
	   if(pass == 1)
	      h3->Fill(lgeneta);
	}
     }

     // If did leading efficiency, don't do the full efficiency
     return;
  }

  // === Full Efficiencies ===

  // Do efficiency of HLT/GEN
  if(hlteffmode == "GEN" && (cfg->selectBranchMC))
    {
      for(Int_t n = 0;n < NMCpart;n++)
        {
          if(MCpid[n] == mctruthpid)
            {
              for(Int_t m = 0;m < nhlt;m++)
                {
                  Float_t deta = MCeta[n] - hlteta[m];
                  Float_t dphi = MCphi[n] - hltphi[m];
                  Float_t dr = sqrt((deta*deta) + (dphi*dphi));
                  if(dr < drmatch)
                    {
                      /*
                       * Any additional cuts on the numerator for the HLT efficiency (isolation,
                       * matching, tagging, etc.) will go here!
                       */
                      pass = 1;
                    }
                }

              // Fill histograms for eff vs. pT
              if(fabs(MCeta[n]) < etacutforpt)
                {
                  h2->Fill(MCpt[n]);
                  if(pass == 1)
                    h1->Fill(MCpt[n]);
                }

              // Fill histograms for eff vs. eta
              if(MCpt[n] > ptcutforeta)
                {
                  h4->Fill(MCeta[n]);
                  if(pass == 1)
                    h3->Fill(MCeta[n]);
                }
            }
        }
    }

  // Do efficiency of HLT/L1
  else if(hlteffmode == "L1"&& (cfg->selectBranchL1extra))
    {
      for(Int_t n = 0;n < nl1;n++)
	{
	  for(Int_t m = 0;m < nhlt;m++)
	    {
	      Float_t deta = l1eta[n] - hlteta[m];
	      Float_t dphi = l1phi[n] - hltphi[m];
	      Float_t dr = sqrt((deta*deta) + (dphi*dphi));
	      if(dr < drmatch)
		{
		  /*
		   * Any additional cuts on the numerator for the HLT efficiency (isolation,
		   * matching, tagging, etc.) will go here!
		   */
		  pass = 1;
		}
	    }

	  // Book histograms for eff vs. pT
	  if(fabs(l1eta[n]) < etacutforpt)
	    {
	      h2->Fill(l1pt[n]);
	      if(pass == 1)
		h1->Fill(l1pt[n]);
	    }

	  // Book histograms for eff vs. eta
	  if(l1pt[n] > ptcutforeta)
	    {
	      h4->Fill(l1eta[n]);
	      if(pass == 1)
		h3->Fill(l1eta[n]);
	    }
	}
    }
  // Do efficiency of HLT/RECO
  else if(hlteffmode == "RECO" && (cfg->selectBranchReco))
    {
      for(Int_t n = 0;n < nrec;n++)
	{
	  for(Int_t m = 0;m < nhlt;m++)
	    {
	      Float_t deta = recoeta[n] - hlteta[m];
	      Float_t dphi = recophi[n] - hltphi[m];
	      Float_t dr = sqrt((deta*deta) + (dphi*dphi));
	      if(dr < drmatch)
		{
		  /*
		   * Any additional cuts on the numerator for the HLT efficiency (isolation,
		   * matching, tagging, etc.) will go here!
		   */
		  pass = 1;
		}
	    }

	  // Book histograms for eff vs. pT
	  if(fabs(recoeta[n]) < etacutforpt)
	    {
	      h2->Fill(recopt[n]);
	      if(pass == 1)
		h1->Fill(recopt[n]);
	    }

	  // Book histograms for eff vs. eta
	  if(recopt[n] > ptcutforeta)
	    {
	      h4->Fill(recoeta[n]);
	      if(pass == 1)
		h3->Fill(recoeta[n]);
	    }
	}
    }
}


