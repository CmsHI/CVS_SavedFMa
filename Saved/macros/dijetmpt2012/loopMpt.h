#ifndef LoopMpt_h
#define LoopMpt_h
#include <vector>
#include "TMath.h"
#include "TH2.h"
#include "TH3.h"
#include "TTree.h"
#include "commonTool.h"
#include "JetTrkEvent.h"
using namespace TMath;

// MPT Ranges
const int nptrange = 8;
float ptranges[nptrange+1]={0.5,1.0,2,4,8,20,30,50,200};

const int nAjBin = 9;
float AjBins[nAjBin+1] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.55};

const int ndrbin=8;
float drbins[ndrbin+1]={0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,100};

const int ndphibin=6;
float dphibins[ndphibin+1]={0,Pi()/6.,2*Pi()/6.,3*Pi()/6.,4*Pi()/6,5*Pi()/6,Pi()};

class MPTEvent {
public:
   float pt1,phi1,eta1;
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
      n=0;
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
      for (int it=0; it<n; ++it) {
         float candPt  = pt[it];
         float candEta = eta[it];
         float candPhi = phi[it];
         float candWt = weight[it];         
         float dr1 = deltaR(candEta,candPhi,eta1,phi1);
         float dphi1 = fabs(deltaPhi(candPhi,phi1));

         float ptx = candPt * cos(candPhi-phi1) * candWt;
         
         x += ptx;
         
         for (int i=0; i<nptrange; ++i) {
            if (candPt>=ptranges[i] && candPt<ptranges[i+1]) {
               x_pt[i]+= ptx;
               for (int j=0; j<ndrbin; ++j) {
                  if (dr1 >=drbins[j] && dr1<drbins[j+1]) {
                     x_pt_dr[i][j] += ptx;
                  }
               }

               for (int k=0; k<ndphibin; ++k) {
                  if (dphi1 >=dphibins[k] && dphi1<dphibins[k+1]) {
                     x_pt_dphi[i][k] += ptx;
                  }
               }                  
            }
         }
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
   TTree * t;

   EvtSel evt;
   DiJet dj;
   MPTEvent me;
   
   std::vector<float> xbins;
   
   // basics
   TH1D * hCentrality;
   TH2D * hJetPt2D;
   TH1D * hJDPhi;
   TH1D * hAj;
   // mpt
   TH3D * hMpt;
   TH3D * hMptDr;
   TH3D * hMptDPhi;

   AnaMPT(TString myname) :
   name(myname),
   isMC(false)
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
      t=tgj;
      JTSetBranchAddress(tgj,evt,dj);
      
      hCentrality = new TH1D("hCentrality"+name,";Centrality Bin;",40,0,40);
      hJetPt2D = new TH2D("hJetPt2D"+name,";p_{T,1} (GeV/c);p_{T,2} (GeV/c)",60,0,300,60,0,300);
      hJDPhi = new TH1D("hJDPhi"+name,";#Delta#phi(j1,j2);",40,0,3.14159);
      hAj = new TH1D("hAj"+name,";A_{J};",40,0,0.8);

      hMpt = new TH3D(Form("hMpt%s",name.Data()),"",nAjBin,AjBins,xbins.size()-1,&xbins[0],nptrange,ptranges);
      hMptDr = new TH3D(Form("hMptDr%s",name.Data()),"",nAjBin,AjBins,xbins.size()-1,&xbins[0],nptrange,ptranges);
      hMptDPhi = new TH3D(Form("hMptDPhi%s",name.Data()),"",nAjBin,AjBins,xbins.size()-1,&xbins[0],nptrange,ptranges);
   }
   
   void Loop() {
      int numEvt = t->GetEntries();
      for (int iEvt=0; iEvt<numEvt; ++iEvt) {
         t->GetEntry(iEvt);
         
         // selection
      if (iEvt%1000==0) cout <<iEvt<<" / "<<numEvt << " run: " << evt.run << " evt: " << evt.evt << " bin: " << evt.cBin << " nT: " << evt.nT << " trig: " <<  evt.trig << " anaEvtSel: " << evt.anaEvtSel <<endl;
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
      } // End of event loop
   }
};
#endif
