#ifndef LoopMpt_h
#define LoopMpt_h
#include <vector>
#include "TMath.h"
#include "TH2.h"
#include "TH3.h"
#include "TTree.h"
#include "commonTool.h"
#include "JetTrkEvent.h"
#include "Corrector3D.h"
using namespace TMath;

// MPT Ranges
// MPT Ranges
// const int nptrange = 6;
// float ptranges[nptrange+1]={0.5,1.0,2,4,8,20,200};
// const int nptrange = 8;
// float ptranges[nptrange+1]={0.5,1.0,2,4,8,20,50,100,200};
// const int nptrange = 8;
// float ptranges[nptrange+1]={0.5,1.0,2,4,8,20,35,50,200};
const int nptrange = 8;
float ptranges[nptrange+1]={0.5,1.0,2,4,8,20,30,50,200};

// const int nAjBin = 4;
// float AjBins[nAjBin+1] = {0.0001,0.11,0.22,0.33,0.49999};
const int nAjBin = 9;
float AjBins[nAjBin+1] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.55};

const int ndrbin=7;
float drbins[ndrbin+1]={0,0.2,0.4,0.6,0.8,1.0,1.2,100};

const int ndphibin=6;
float dphibins[ndphibin+1]={0,Pi()/12.,2*Pi()/12.,3*Pi()/12.,4*Pi()/12,5*Pi()/12,PiOver2()};

class MPTEvent {
public:
   float pt1,phi1,eta1;
   float pt2,phi2,eta2;
   int n;
   float pt[MAXTRK];
   float eta[MAXTRK];
   float phi[MAXTRK];
   float weight[MAXTRK];

   float x;   
   float x_pt[nptrange];
   float x_pt_dr[nptrange][ndrbin];
   float x_pt_dphi[nptrange][ndphibin];
   
   MPTEvent() :
   n(0)
   {}

   void clear() {
      x=0;
      for (int i=0; i<nptrange; ++i) {
         x_pt[i]=0;
         for (int j=0; j<ndrbin; ++j) {
            x_pt_dr[i][j]=0;
         }
         for (int k=0; k<ndphibin; ++k) {
            x_pt_dphi[i][k]=0;
         }
      }      
   }
   
   void Calc() {
      clear();
//       cout << "n: " << n << endl;
      for (int it=0; it<n; ++it) {
         float candPt  = pt[it];
         float candEta = eta[it];
         float candPhi = phi[it];
         float candWt = weight[it];         
         float dr1 = deltaR(candEta,candPhi,eta1,phi1);
         float dphi1 = fabs(deltaPhi(candPhi,phi1));
         float dr2 = deltaR(candEta,candPhi,eta2,phi2);
         float dphi2 = fabs(deltaPhi(candPhi,phi1+Pi()));

         float ptx = -candPt * cos(candPhi-phi1) * candWt;
         
         x += ptx;
         
         for (int i=0; i<nptrange; ++i) {
            if (candPt>=ptranges[i] && candPt<ptranges[i+1]) {
               x_pt[i]+= ptx;
               for (int j=0; j<ndrbin; ++j) {
                  if ( (dr1 >=drbins[j] && dr1<drbins[j+1]) ||
                       (dr2 >=drbins[j] && dr2<drbins[j+1])
                  ) {
                     x_pt_dr[i][j] += ptx;
                  }
               }

               for (int k=0; k<ndphibin; ++k) {
                  if ( (dphi1 >=dphibins[k] && dphi1<dphibins[k+1]) ||
                       (dphi2 >=dphibins[k] && dphi2<dphibins[k+1])
                  ) {
                     x_pt_dphi[i][k] += ptx;
                  }
               }                  
            }
         }
//          cout << "x: " << x << endl;
      } // end of for cand
   }
};

class AnaMPT
{
public:
   TString name;
   bool isMC;
   int cMin, cMax;
   float minJetPt1,minJetPt2,maxJetEta,sigDPhi;
   float minPt, maxEta;
   int particleRecLevel; // 0 gen, 1 sim, 2 sim mat, 3 rec mat 4 rec
   TTree * t;

   EvtSel evt;
   DiJet dj;
   MPTEvent me;

   TrackingCorrections trkCorr;
   
   std::vector<float> xbins;
   
   TFile * outf;
   
   // basics
   TH1D * hCentrality;
   TH2D * hJetPt2D;
   TH1D * hJDPhi;
   TH1D * hAj;
   // Tracks
   TH1D * hGenpPt;
   TH1D * hGenpEta;
   TH1D * hTrkPtNoQual;
   TH1D * hTrkPtQual3;
   TH1D * hTrkPt;
   TH1D * hTrkEta;
   TH1D * hTrkCorrPt;
   TH1D * hTrkCorrEta;
   // mpt
   TH2D * hMpt;
//    TH3D * hMptPt;
//    TH3D * hMptPtDr;
//    TH3D * hMptPtDPhi;
   TH2D * vhMptPt[nptrange];
   TH2D * vhMptPtDr[nptrange][ndrbin];
   TH2D * vhMptPtDPhi[nptrange][ndphibin];

   AnaMPT(TString myname) :
   name(myname),
   isMC(false),
   trkCorr("Forest2_v19","hitrkEffAnalyzer_MergedGeneral")
   {
      cout << "Analyze MPT: " << name << endl;
      t=0;
      
      // mpt bins
      int nxbins=1600;
      float xmin=-800, xmax=800;
      float dx=(xmax-xmin)/nxbins;
      for (int i=0; i<=nxbins; ++i) xbins.push_back(xmin+dx*i);
   }
   
   void Init(TTree * tgj) {   
      trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj30_Forest2_v19.root",30);
      trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj50_Forest2_v19.root",50);
      trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj80_Forest2_v19.root",80);
      trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj120_Forest2_v19.root",120);
      trkCorr.AddSample("trkcorr/Forest2_v19/trkcorr_hy18dj170_Forest2_v19.root",170);
      trkCorr.smoothLevel_ = 1; 	 
      trkCorr.Init();
   
      t=tgj;
      JTSetBranchAddress(tgj,evt,dj);
      
      if (outf) outf->cd();
      hCentrality = new TH1D("hCentrality"+name,";Centrality Bin;",40,0,40);
      hJetPt2D = new TH2D("hJetPt2D"+name,";p_{T,1} (GeV/c);p_{T,2} (GeV/c)",60,0,300,60,0,300);
      hJDPhi = new TH1D("hJDPhi"+name,";#Delta#phi(j1,j2);",40,0,3.14159);
      hAj = new TH1D("hAj"+name,";A_{J};",40,0,0.8);

//       hGenpPt = new TH1D("hGenpPt"+name,"; p_{T};",100,0,200);
//       hTrkPtNoQual = new TH1D("hTrkPtNoQual"+name,"; p_{T};",100,0,200);
//       hTrkPtQual3 = new TH1D("hTrkPtQual3"+name,"; p_{T};",100,0,200);
//       hTrkPt = new TH1D("hTrkPt"+name,"; p_{T};",100,0,200);
//       hTrkCorrPt = new TH1D("hTrkCorrPt"+name,"; p_{T};",100,0,200);
      hGenpPt = (TH1D*)trkCorr.ptBin_->Clone("hGenpPt"+name);
      hTrkPtNoQual = (TH1D*)trkCorr.ptBin_->Clone("hTrkPtNoQual"+name);
      hTrkPtQual3 = (TH1D*)trkCorr.ptBin_->Clone("hTrkPtQual3"+name);
      hTrkPt = (TH1D*)trkCorr.ptBin_->Clone("hTrkPt"+name);
      hTrkCorrPt = (TH1D*)trkCorr.ptBin_->Clone("hTrkCorrPt"+name);

      hGenpEta = new TH1D("hGenpEta"+name,"; #eta;",48,-2.4,2.4);
      hTrkEta = new TH1D("hTrkEta"+name,"; #eta;",48,-2.4,2.4);
      hTrkCorrEta = new TH1D("hTrkCorrEta"+name,"; #eta;",48,-2.4,2.4);

      hMpt = new TH2D(Form("hMpt%s",name.Data()),";Aj;mpt;",nAjBin,AjBins,xbins.size()-1,&xbins[0]);
//       hMptPt = new TH3D(Form("hMptPt%s",name.Data()),";Aj;mpt;pt",nAjBin,AjBins,xbins.size()-1,&xbins[0],nptrange,ptranges);
//       hMptPtDr = new TH3D(Form("hMptPtDr%s",name.Data()),";Aj;mpt;pt",nAjBin,AjBins,xbins.size()-1,&xbins[0],nptrange,ptranges);
//       hMptPtDPhi = new TH3D(Form("hMptPtDPhi%s",name.Data()),";Aj;mpt;pt",nAjBin,AjBins,xbins.size()-1,&xbins[0],nptrange,ptranges);
      for (int i=0; i<nptrange; ++i) {
         vhMptPt[i] = new TH2D(Form("hMpt%s_pt%d",name.Data(),i),";Aj;mpt",nAjBin,AjBins,xbins.size()-1,&xbins[0]);
         for (int j=0; j<ndrbin; ++j) {
            vhMptPtDr[i][j] = new TH2D(Form("hMpt%s_pt%d_dr%d",name.Data(),i,j),";Aj;mpt",nAjBin,AjBins,xbins.size()-1,&xbins[0]);
         }

         for (int k=0; k<ndphibin; ++k) {
            vhMptPtDPhi[i][k] = new TH2D(Form("hMpt%s_pt%d_dphi%d",name.Data(),i,k),";Aj;mpt",nAjBin,AjBins,xbins.size()-1,&xbins[0]);
         }                  
      }
   }
   
   void Loop(int maxEntry=-1) {
      int numEvt = t->GetEntries();
      if (maxEntry>0) numEvt = maxEntry;
      for (int iEvt=0; iEvt<numEvt; ++iEvt) {
         t->GetEntry(iEvt);
         
         // selection
      if (cMax<=12&&iEvt%1000==0) cout <<iEvt<<" / "<<numEvt << " run: " << evt.run << " evt: " << evt.evt << " bin: " << evt.cBin << " nT: " << evt.nT << " trig: " <<  evt.trig << " anaEvtSel: " << evt.anaEvtSel <<endl;
         if (!isMC&&!evt.anaEvtSel) continue;

         if (evt.cBin<cMin||evt.cBin>=cMax) continue;
         if (dj.pt1<minJetPt1 || fabs(dj.eta1)>maxJetEta) continue;
         if (dj.pt2<minJetPt2 || fabs(dj.eta2)>maxJetEta) continue;
         float jdphi = fabs(deltaPhi(dj.phi1,dj.phi2));
         if (  jdphi < sigDPhi) continue;
         // Fill Basics
         float Aj = (dj.pt1-dj.pt2)/(dj.pt1+dj.pt2);
         hCentrality->Fill(evt.cBin);
         hJetPt2D->Fill(dj.pt1,dj.pt2);
         hJDPhi->Fill(jdphi);
         hAj->Fill(Aj);
         
         me.pt1 = dj.pt1;
         me.phi1 = dj.phi1;
         me.eta1 = dj.eta1;
         me.pt2 = dj.pt2;
         me.phi2 = dj.phi2;
         me.eta2 = dj.eta2;

         // Track loop
         me.n=0;
         for (int ip=0; ip<dj.nTrk; ++ip) {
            if (dj.trkPt[ip]<minPt) continue;
            if (fabs(dj.trkEta[ip])>maxEta) continue;
            hTrkPtNoQual->Fill(dj.trkPt[ip]);
//             if (dj.trkAlgo[ip]<4||dj.vtrkQual[ip][1]) hTrkPtQual3->Fill(dj.trkPt[ip]);
            if (dj.trkAlgo[ip]>=4&&!dj.vtrkQual[ip][0]) continue;
            hTrkPt->Fill(dj.trkPt[ip]);
            hTrkEta->Fill(dj.trkEta[ip]);
            // trk correction
//             float trkWt = dj.vtrkWt[ip][0];
            float trkWt = dj.trkWt[ip];
            hTrkCorrPt->Fill(dj.trkPt[ip],trkWt);
            hTrkCorrEta->Fill(dj.trkEta[ip],trkWt);
            // set mpt input
            if (particleRecLevel>=3) {
                  me.pt[me.n] = dj.trkPt[ip];
                  me.eta[me.n] = dj.trkEta[ip];
                  me.phi[me.n] = dj.trkPhi[ip];
                  me.weight[me.n] = trkWt;
                  ++me.n;
            }
         }

         if (particleRecLevel>=1&&particleRecLevel<3) {
            me.n=0;
            // Sim loop
            for (int ip=0; ip<dj.nSim; ++ip) {
               if (dj.simPt[ip]<minPt) continue;
               if (fabs(dj.simEta[ip])>maxEta) continue;
               hGenpPt->Fill(dj.simPt[ip]);
               hGenpEta->Fill(dj.simEta[ip]);
               // set mpt input
               me.pt[me.n] = dj.simPt[ip];
               me.eta[me.n] = dj.simEta[ip];
               me.phi[me.n] = dj.simPhi[ip];
               me.weight[me.n] = 1;
               ++me.n;
            }
         }else if (particleRecLevel==0){
            me.n=0;
            // Genp loop
            for (int ip=0; ip<dj.nGenp; ++ip) {
               if (dj.genpPt[ip]<minPt) continue;
               if (fabs(dj.genpEta[ip])>maxEta) continue;
               hGenpPt->Fill(dj.genpPt[ip]);
               hGenpEta->Fill(dj.genpEta[ip]);
               // set mpt input
               me.pt[me.n] = dj.genpPt[ip];
               me.eta[me.n] = dj.genpEta[ip];
               me.phi[me.n] = dj.genpPhi[ip];
               me.weight[me.n] = 1;
               ++me.n;
            }
         } // End of if rec level
         me.Calc();
         hMpt->Fill(Aj,me.x);
         for (int i=0; i<nptrange; ++i) {
            vhMptPt[i]->Fill(Aj,me.x_pt[i]);
            for (int j=0; j<ndrbin; ++j) {
               vhMptPtDr[i][j]->Fill(Aj,me.x_pt_dr[i][j]);
            }
            for (int k=0; k<ndphibin; ++k) {
               vhMptPtDPhi[i][k]->Fill(Aj,me.x_pt_dphi[i][k]);
            }
         }
      } // End of event loop
   }
};
#endif
