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
float drbins[ndrbin+1]={0,0.2,0.4,0.6,0.8,1.0,1.2,PiOver2()};
// const int ndrbin=2;
// float drbins[ndrbin+1]={0,0.8,PiOver2()};
// const int ndrbin=1;
// float drbins[ndrbin+1]={0,PiOver2()};

const int ndphibin=6;
float dphibins[ndphibin+1]={0,Pi()/12.,2*Pi()/12.,3*Pi()/12.,4*Pi()/12,5*Pi()/12,PiOver2()};

class MPTEvent {
public:
   float jet1Wt,jet2Wt;
   float pt1,phi1,eta1;
   float pt2,phi2,eta2;
   float jdphi,Aj;
   int n;
   float pt[MAXTRK];
   float eta[MAXTRK];
   float phi[MAXTRK];
   float weight[MAXTRK];

   float x;   
   float x_pt[nptrange];
   float x_pt_dr[nptrange][ndrbin];
   float x_pt_dphi[nptrange][ndphibin];

   float xhem_pt[2][nptrange];
   float xhem_pt_dr[2][nptrange][ndrbin];
   
   float ptbin[nptrange];
   float drbin[ndrbin];
   
   MPTEvent() :
   n(0) {
      for (int i=0; i<nptrange; ++i) {
         ptbin[i] = ptranges[i];
      }
      for (int j=0; j<ndrbin; ++j) {
         drbin[j] = drbins[j];
      }
   }

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
      // separate hemispheres
      for (int h=0; h<2; ++h) {
         for (int i=0; i<nptrange; ++i) {
            xhem_pt[h][i]=0;
            for (int j=0; j<ndrbin; ++j) {
               xhem_pt_dr[h][i][j]=0;
            }
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
         float dr2 = deltaR(candEta,candPhi,eta2,phi2);
         float dphi1 = fabs(deltaPhi(candPhi,phi1));
         float dphi2 = fabs(deltaPhi(candPhi,phi1+Pi()));

         float ptx = -candPt * cos(candPhi-phi1) * candWt;
         
         x += ptx;
//          if (it==0) cout << "add to x: " << ptx << endl;
         for (int i=0; i<nptrange; ++i) {
//             if (it==0) cout << "pt range: " << ptranges[i] << " to " << ptranges[i+1] << endl;
            if (candPt>=ptranges[i] && candPt<ptranges[i+1]) {
               //if (it==0) cout << "pt1, eta1, phi1: " << pt1 << ", " << eta1 << ", " << phi1 << " candPt: " << candPt << " ptx: " << ptx << endl;
               x_pt[i]+= ptx;
               // separate hemispheres
               if (ptx<=0) xhem_pt[0][i] += -ptx;
               if (ptx>0) xhem_pt[1][i] += ptx;
//             if (it==0) cout << "add to x_pt: " << ptx << endl;
               for (int j=0; j<ndrbin; ++j) {
                  float drmin=drbins[j];
                  float drmax=drbins[j+1];
//                   cout << "drbin " << j << ": " << drmin << " to " << drmax << endl;
                  if (j!=ndrbin-1) {
                     if ( (dr1>=drmin && dr1<drmax) ||
                          (dr2>=drmin && dr2<drmax)) {
//                         if (i==0) cout << "drbin " << j << ", dr1: " << dr1 << " dr2: " << dr2 << " ptx: " << ptx << endl;
                        x_pt_dr[i][j] += ptx;
                     }
                  } else {
                     if ( dr1 >=drmin && dr2 >=drmin) {
//                         if (i==0) cout << "drbin rest, dr1: " << dr1 << " dr2: " << dr2 << " ptx: " << ptx << endl;
                        x_pt_dr[i][j] += ptx;
                     }
                  }
                  // separate hemispheres
                  if (ptx<=0 && dr1>=drmin && dr1<drmax) xhem_pt_dr[0][i][j] += -ptx;
                  if (ptx>0  && dr2>=drmin && dr2<drmax) xhem_pt_dr[1][i][j] += ptx;
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
      } // end of for cand
//       cout << "hem1 sum: " << xhem_pt_dr[0][nptrange-1][0] << endl;
//       float ptsum=0, ptdrsum=0;
//       for (int i=0; i<nptrange; ++i) {
//          ptsum+=x_pt[i];
//          for (int j=0; j<ndrbin; ++j) {
//             ptdrsum+=x_pt_dr[i][j];
//          }
//       }
//       cout << "x: " << x << " ptsum: " << ptsum << " ptdrsum: " << ptdrsum << endl;
   }
};

void MPTSetBranchAddress(TTree * tgj, EvtSel & evt, DiJet & gj, MPTEvent & mpt) {
   tgj->Branch("evt",&evt.run,evt.leaves);
   tgj->Branch("jet",&gj.pt1,gj.leaves);
   tgj->Branch("mpt",&mpt.jet1Wt,"jet1Wt/F:jet2Wt");
   tgj->Branch("x",&mpt.x,"x/F");
   tgj->Branch("xpt",mpt.x_pt,Form("xpt[%d]/F",nptrange));
   tgj->Branch("xptdr",mpt.x_pt_dr,Form("xptdr[%d][%d]/F",nptrange,ndrbin));
   tgj->Branch("xptdphi",mpt.x_pt_dphi,Form("xptphi[%d][%d]/F",nptrange,ndphibin));

   // separate hemispheres
   for (int h=0; h<2; ++h) {
      tgj->Branch(Form("xhem%dpt",h+1),mpt.xhem_pt[h],Form("xhem%dpt[%d]/F",h+1,nptrange));
      tgj->Branch(Form("xhem%dptdr",h+1),mpt.xhem_pt_dr[h],Form("xhem%dptdr[%d][%d]/F",h+1,nptrange,ndrbin));
   }

   tgj->Branch("ptbin",mpt.ptbin,Form("ptbin[%d]/F",nptrange));
   tgj->Branch("drbin",mpt.drbin,Form("drbin[%d]/F",ndrbin));   
}

class AnaMPT
{
public:
   TString name;
   bool isMC;
   bool doResCorr;
   bool doJetPhiFlat;
   int subEvtMode;
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
   
   // weights
   TH1D * hJetPhi[2];
   TH2D * hJetWt[2];
   
   // output tree
   TTree * tm;

   AnaMPT(TString myname) :
   name(myname),
   isMC(false),
   doResCorr(false),
   doJetPhiFlat(false),
   subEvtMode(-1),
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
      hAj = new TH1D("hAj"+name,";A_{J};",nAjBin,AjBins);

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
      
      // Weights
      const int njphibin=20;
      float jphibins[njphibin];
      for (int i=0; i<=njphibin; ++i) jphibins[i] = -Pi()+2*Pi()/njphibin*i;
      for (int j=0; j<2; ++j) {
         hJetPhi[j] = new TH1D(Form("hJet%dPhi%s",j+1,+name.Data()),";Leading Jet #phi;",njphibin,jphibins);
         hJetWt[j]  = new TH2D(Form("hJet%dWt%s",j+1,name.Data()),";Aj;Jet #phi;",njphibin,jphibins,nAjBin,AjBins);
      }

      // output tree
      tm = new TTree("t"+name,"dijet mpt tree "+name);
      MPTSetBranchAddress(tm,evt,dj,me);
   }

   void CalcWeights(int maxEntry=-1) {
      int numEvt = t->GetEntries();
      if (maxEntry>0) numEvt = maxEntry;
      for (int iEvt=0; iEvt<numEvt; ++iEvt) {
         t->GetEntry(iEvt);
         // selection
         if (!SelEvt()) continue;

         hJetPhi[0]->Fill(me.phi1);                  
         hJetPhi[1]->Fill(me.phi2);
         hJetWt[0]->Fill(me.phi1,me.Aj);
         hJetWt[1]->Fill(me.phi2,me.Aj);
      }
      
      for (int k=0; k<2; ++k) {
         float averagePhi = hJetPhi[k]->Integral()/hJetPhi[k]->GetNbinsX();
         for (int i=1; i<=hJetPhi[k]->GetNbinsX(); ++i) {
            hJetPhi[k]->SetBinContent(i,averagePhi/hJetPhi[k]->GetBinContent(i));
         }
      }      

      for (int k=0; k<2; ++k) {
         int nxbins = hJetWt[k]->GetNbinsX();
         for (int j=1; j<=hJetWt[k]->GetNbinsY(); ++j) {
            float averagePhi = hJetWt[k]->Integral(1,nxbins,j,j)/nxbins;
            for (int i=1; i<=nxbins; ++i) {
               hJetWt[k]->SetBinContent(i,j,averagePhi/hJetWt[k]->GetBinContent(i,j));
            }
         }
      }      
   }

   bool SelEvt() {
      me.pt1 = dj.pt1; me.phi1 = dj.phi1; me.eta1 = dj.eta1;
      me.pt2 = dj.pt2; me.phi2 = dj.phi2; me.eta2 = dj.eta2;

//          me.pt1 = dj.genjetpt1; me.phi1 = dj.genjetphi1; me.eta1 = dj.genjeteta1;
//          me.pt2 = dj.genjetpt2; me.phi2 = dj.genjetphi2; me.eta2 = dj.genjeteta2;
      me.Aj = (me.pt1-me.pt2)/(me.pt1+me.pt2);
      me.jdphi = fabs(deltaPhi(me.phi1,me.phi2));

      if (!isMC&&!evt.anaEvtSel) return false;

      if (evt.cBin<cMin||evt.cBin>=cMax) return false;

      // Jet Selection
      if (me.pt1<minJetPt1 || fabs(me.eta1)>maxJetEta) return false;
      if (me.pt2<minJetPt2 || fabs(me.eta2)>maxJetEta) return false;
      if (  me.jdphi < sigDPhi) return false;
      
      return true;
   }
   
   void Loop(int maxEntry=-1) {
      int numEvt = t->GetEntries();
      if (maxEntry>0) numEvt = maxEntry;
      
      for (int i=1; i<=hJetWt[0]->GetNbinsX(); ++i) {
         cout << "jet1 2d bin: " << i << " wt: " << hJetWt[0]->GetBinContent(i,hJetWt[0]->GetNbinsY()) << endl;    
      }
      for (int iEvt=0; iEvt<numEvt; ++iEvt) {
         t->GetEntry(iEvt);
         
      if (cMax<=12&&iEvt%1000==0) cout <<iEvt<<" / "<<numEvt << " run: " << evt.run << " evt: " << evt.evt << " bin: " << evt.cBin << " nT: " << evt.nT << " trig: " <<  evt.trig << " anaEvtSel: " << evt.anaEvtSel <<endl;

         // selection
         if (!SelEvt()) continue;

         // Jet Phi Flattening
         me.jet1Wt=1; me.jet2Wt=1;
         if (doJetPhiFlat) {
            int jet1PhiBin = hJetPhi[0]->FindFixBin(me.phi1);
            int jet2PhiBin = hJetPhi[1]->FindFixBin(me.phi2);
            int jetAjBin = hAj->FindBin(me.Aj);
            me.jet1Wt = hJetWt[0]->GetBinContent(jet1PhiBin,jetAjBin);
            me.jet2Wt = hJetWt[1]->GetBinContent(jet2PhiBin,jetAjBin);
//             me.jet1Wt = hJetPhi[0]->GetBinContent(jet1PhiBin);
//             me.jet2Wt = hJetPhi[1]->GetBinContent(jet2PhiBin);
//             if (jet1PhiBin==1&&jetAjBin==hJetWt[0]->GetNbinsY()) cout << "jet1 phi: " << me.phi1 << " bin: " << jet1PhiBin << "," << jetAjBin << " wt: " << me.jet1Wt << endl;
         }
         
         // Fill Basics
         hCentrality->Fill(evt.cBin);
         hJetPt2D->Fill(me.pt1,me.pt2);
         hJDPhi->Fill(me.jdphi);
         hAj->Fill(me.Aj);
         
         // Track loop
         me.n=0;
         for (int ip=0; ip<dj.nTrk; ++ip) {
            if (dj.trkPt[ip]<minPt) continue;
            if (fabs(dj.trkEta[ip])>maxEta) continue;
            hTrkPtNoQual->Fill(dj.trkPt[ip]);
//             if (dj.trkAlgo[ip]<4||dj.vtrkQual[ip][1]) hTrkPtQual3->Fill(dj.trkPt[ip]);
            if (dj.trkAlgo[ip]>=4&&!dj.vtrkQual[ip][0]) continue;
            if (particleRecLevel==3&&dj.trkIsFake[ip]) continue;
            hTrkPt->Fill(dj.trkPt[ip]);
            hTrkEta->Fill(dj.trkEta[ip]);
            // trk correction
//             float trkWt = dj.vtrkWt[ip][0];
            float trkWt = dj.trkWt[ip];

            // correct leading subleading difference in trk eff
            if (doResCorr) {
               if ( cos(dj.trkPhi[ip]-me.phi1)<0 ) {
                  if (me.pt2<80) {
                     if (dj.trkPt[ip]>30) trkWt*=1.2;
                     else if (dj.trkPt[ip]>20) trkWt*=1.1;
                     else if (dj.trkPt[ip]>8) trkWt*=1.05;
                  }
               } else {
                  if (me.pt2<80) {
                     if (dj.trkPt[ip]>30) trkWt*=0.8;
                     else if (dj.trkPt[ip]>20) trkWt*=0.9;
                     else if (dj.trkPt[ip]>8) trkWt*=0.95;
                  }
               }
            }

            hTrkCorrPt->Fill(dj.trkPt[ip],trkWt);
            hTrkCorrEta->Fill(dj.trkEta[ip],trkWt);
            // set mpt input
            if (particleRecLevel>=3) {
                  me.pt[me.n] = dj.trkPt[ip];
                  me.eta[me.n] = dj.trkEta[ip];
                  me.phi[me.n] = dj.trkPhi[ip];
                  if (particleRecLevel==4) me.weight[me.n] = trkWt;
                  if (particleRecLevel==3) me.weight[me.n] = trkWt/(1-dj.trkFak[ip]);
                  ++me.n;
            }
         }

         if (particleRecLevel>=1&&particleRecLevel<3) {
            me.n=0;
            // Sim loop
            for (int ip=0; ip<dj.nSim; ++ip) {
               if (dj.simPt[ip]<minPt) continue;
               if (fabs(dj.simEta[ip])>maxEta) continue;
               if (particleRecLevel==2&&dj.simHasRec[ip]) continue;
               hGenpPt->Fill(dj.simPt[ip]);
               hGenpEta->Fill(dj.simEta[ip]);
               // set mpt input
               me.pt[me.n] = dj.simPt[ip];
               me.eta[me.n] = dj.simEta[ip];
               me.phi[me.n] = dj.simPhi[ip];
               if (particleRecLevel==1) me.weight[me.n] = 1;
               if (particleRecLevel==2) me.weight[me.n] = 1./dj.trkEff[ip];
               ++me.n;
            }
         }else if (particleRecLevel==0){
            me.n=0;
            // Genp loop
            for (int ip=0; ip<dj.nGenp; ++ip) {
               if (dj.genpPt[ip]<minPt) continue;
               if (fabs(dj.genpEta[ip])>maxEta) continue;
               if (subEvtMode==0 && dj.genpSube[ip]!=0) continue;
               if (subEvtMode==1 && dj.genpSube[ip]==0) continue;
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
         hMpt->Fill(me.Aj,me.x);
         for (int i=0; i<nptrange; ++i) {
            vhMptPt[i]->Fill(me.Aj,me.x_pt[i]);
            for (int j=0; j<ndrbin; ++j) {
               vhMptPtDr[i][j]->Fill(me.Aj,me.x_pt_dr[i][j]);
            }
            for (int k=0; k<ndphibin; ++k) {
               vhMptPtDPhi[i][k]->Fill(me.Aj,me.x_pt_dphi[i][k]);
            }
         }
         
         tm->Fill();
      } // End of event loop
      
      SetAliases(tm);
   }

   TString MakeConeVar(TString br="xptdr",float radius=0.8, TString metType="incone", int ipt=nptrange) {
      // 1st find dr bin of the jet cone boundary
      int coneidr=0;
      for (int idr=0; idr<ndrbin; ++idr) {
         if (drbins[idr]>=radius)  {
            coneidr=idr;
            break;
         }
      }
      // 2nd make sum variable
      TString var;
      int idrbeg,idrend;
      if (metType=="incone") {
         idrbeg=0; idrend=coneidr;
      } else {
         idrbeg=coneidr; idrend=ndrbin;
      }
      if (ipt<nptrange) {
         var  = Form("%s[%d][%d]",br.Data(),ipt,idrbeg);
         for (int idr=idrbeg+1; idr<idrend; ++idr) {
            var+= Form("+%s[%d][%d]",br.Data(),ipt,idr);
         }
      } else {
         var = Form("Sum$(%s[][%d])",br.Data(),idrbeg);
         for (int idr=idrbeg+1; idr<idrend; ++idr) {
            var+= Form("+Sum$(%s[][%d])",br.Data(),idr);
         }
      }
      
      return var;
   }
   
   void SetAliases(TTree * tm) {
      TString brs[3] = {"xptdr","xhem1ptdr","xhem2ptdr"};
      TString mptType[2] = {"incone","outcone"};
      for (int h=0; h<3; ++h) {
         for (int i=0;i<nptrange+1;++i) {            
            for (int m=0; m<2; ++m) {
               TString aliasname = "x";
               if (h>0) aliasname += Form("hem%d",h);
               float dr=0.8;
               aliasname += mptType[m]+Form("%.0f",dr*10);
               if (i<nptrange) aliasname+=Form("pt%d",i);
               tm->SetAlias(aliasname,"("+MakeConeVar(brs[h],dr,mptType[m],i)+")");
               if (i==0||i==nptrange) cout << aliasname << ": " << tm->GetAlias(aliasname) << endl;
            }
         }
      }
   }
};
#endif
