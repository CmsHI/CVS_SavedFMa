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
   float jet1Wt,jet2Wt,evtWt;
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
      jet1Wt=1.; jet2Wt=1; evtWt=1;
      pt1=0; pt2=0;
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
               x_pt[i]+= ptx;
               // separate hemispheres
               if (ptx<=0) xhem_pt[0][i] += -ptx;
               if (ptx>0) xhem_pt[1][i] += ptx;
               for (int j=0; j<ndrbin; ++j) {
                  float drmin=drbins[j];
                  float drmax=drbins[j+1];
                  if (j!=ndrbin-1) {
                     if ( (dr1>=drmin && dr1<drmax) ||
                          (dr2>=drmin && dr2<drmax)) {
                        x_pt_dr[i][j] += ptx;
                     }
                  } else {
                     if ( dr1 >=drmin && dr2 >=drmin) {
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
   tgj->Branch("mpt",&mpt.jet1Wt,"jet1Wt/F:jet2Wt:evtWt");
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
   int maxEntry;
   TTree * t;

   EvtSel evt;
   DiJet dj;
   MPTEvent me;

   TrackingCorrections trkCorr;
   
   std::vector<float> xbins;
   
   TFile * outf;
   
   // basics
   TH1D * hCentrality;
   TH1D * hPtHat;
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
   TH1D * vhGenpDPhi[10][2];
   TH1D * vhTrkDPhi[10][2];
   TH1D * vhTrkCorrDPhi[10][2];
   TH1D * vhAj[10][2];
   // mpt
   TH2D * hMpt;
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
   particleRecLevel(4), // 0 gen, 1 sim, 2 sim mat, 3 rec mat 4 rec
   maxEntry(-1), // -1 for everything
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
      trkCorr.AddSample("trkcorr/Forest2_v20_v2_large/trkcorr_hy18dj200_Forest2_v20.root",200);
      trkCorr.AddSample("trkcorr/Forest2_v20_v2_large/trkcorr_hy18dj250_Forest2_v20.root",200);
      trkCorr.smoothLevel_ = 1; 	 
      trkCorr.Init();
   
      t=tgj;
      JTSetBranchAddress(tgj,evt,dj);
      
      if (outf) outf->cd();
      ///////////////////////////////////
      // Event Distributions
      ///////////////////////////////////
      hCentrality = new TH1D("hCentrality_"+name,";Centrality Bin;",40,0,40);
      hPtHat = new TH1D("hPtHat_"+name,";#hat{p}_{T} (GeV/c);",120,0,600);
      hJetPt2D = new TH2D("hJetPt2D_"+name,";p_{T,1} (GeV/c);p_{T,2} (GeV/c)",60,0,300,60,0,300);
      hJDPhi = new TH1D("hJDPhi_"+name,";#Delta#phi(j1,j2);",40,0,3.14159);
      hAj = new TH1D("hAj_"+name,";A_{J};",nAjBin,AjBins);

      ///////////////////////////////////
      // Tracking Distributions
      ///////////////////////////////////
      hGenpPt = (TH1D*)trkCorr.ptBin_->Clone("hGenpPt_"+name);
      hGenpPt->Reset();
      hTrkPtNoQual = (TH1D*)trkCorr.ptBin_->Clone("hTrkPtNoQual_"+name);
      hTrkPtNoQual->Reset();
      hTrkPt = (TH1D*)trkCorr.ptBin_->Clone("hTrkPt_"+name);
      hTrkPt->Reset();
      hTrkCorrPt = (TH1D*)trkCorr.ptBin_->Clone("hTrkCorrPt_"+name);
      hTrkCorrPt->Reset();

      hGenpEta = new TH1D("hGenpEta_"+name,"; #eta;",48,-2.4,2.4);
      hTrkEta = new TH1D("hTrkEta_"+name,"; #eta;",48,-2.4,2.4);
      hTrkCorrEta = new TH1D("hTrkCorrEta_"+name,"; #eta;",48,-2.4,2.4);

      ///////////////////////////////////
      // MPT Tracking Closure Studies
      ///////////////////////////////////
      for (int i=0; i<10; ++i) {
         for (int j=0; j<2; ++j) {
            vhGenpDPhi[i][j] = new TH1D(Form("hGenpDPhi_%d_%d_%s",i,j,name.Data()),"",40,0,Pi());
            vhTrkDPhi[i][j] = new TH1D(Form("hTrkDPhi_%d_%d_%s",i,j,name.Data()),"",40,0,Pi());
            vhTrkCorrDPhi[i][j] = new TH1D(Form("hTrkCorrDPhi_%d_%d_%s",i,j,name.Data()),"",40,0,Pi());
            vhAj[i][j] = new TH1D(Form("hAj_%d_%d_%s",i,j,name.Data()),"",40,0,1);
         }
      }
      
      ///////////////////////////////////
      // MPT Distributions
      ///////////////////////////////////
      hMpt = new TH2D(Form("hMpt%s",name.Data()),";Aj;mpt;",nAjBin,AjBins,xbins.size()-1,&xbins[0]);
      for (int i=0; i<nptrange; ++i) {
         vhMptPt[i] = new TH2D(Form("hMpt%s_pt%d",name.Data(),i),";Aj;mpt",nAjBin,AjBins,xbins.size()-1,&xbins[0]);
         for (int j=0; j<ndrbin; ++j) {
            vhMptPtDr[i][j] = new TH2D(Form("hMpt%s_pt%d_dr%d",name.Data(),i,j),";Aj;mpt",nAjBin,AjBins,xbins.size()-1,&xbins[0]);
         }

         for (int k=0; k<ndphibin; ++k) {
            vhMptPtDPhi[i][k] = new TH2D(Form("hMpt%s_pt%d_dphi%d",name.Data(),i,k),";Aj;mpt",nAjBin,AjBins,xbins.size()-1,&xbins[0]);
         }          
      }
      
      ///////////////////////////////////
      // Distributions for reweighting
      ///////////////////////////////////
      const int njphibin=20;
      float jphibins[njphibin];
      for (int i=0; i<=njphibin; ++i) jphibins[i] = -Pi()+2*Pi()/njphibin*i;
      for (int j=0; j<2; ++j) {
         hJetPhi[j] = new TH1D(Form("hJet%dPhi%s",j+1,+name.Data()),";Leading Jet #phi;",njphibin,jphibins);
         hJetWt[j]  = new TH2D(Form("hJet%dWt%s",j+1,name.Data()),";Aj;Jet #phi;",njphibin,jphibins,nAjBin,AjBins);
      }

      ///////////////////////////////////
      // Output Tree
      ///////////////////////////////////
      tm = new TTree("t"+name,"dijet mpt tree "+name);
      MPTSetBranchAddress(tm,evt,dj,me);
   }

   // Function to Select Event And set event vars
   //----------------------------------------------------------------------------------------
   bool SelEvt() {
      me.pt1 = dj.pt1; me.phi1 = dj.phi1; me.eta1 = dj.eta1;
      me.pt2 = dj.pt2; me.phi2 = dj.phi2; me.eta2 = dj.eta2;

//       me.pt1 = dj.genjetpt1; me.phi1 = dj.genjetphi1; me.eta1 = dj.genjeteta1;
//       me.pt2 = dj.genjetpt2; me.phi2 = dj.genjetphi2; me.eta2 = dj.genjeteta2;

      me.Aj = (me.pt1-me.pt2)/(me.pt1+me.pt2);
      me.jdphi = fabs(deltaPhi(me.phi1,me.phi2));

      if (!isMC&&!evt.anaEvtSel) return false;

      if (evt.cBin<cMin||evt.cBin>=cMax) return false;

      // Jet Selection
      if (me.pt1<minJetPt1 || fabs(me.eta1)>maxJetEta) return false;
      if (me.pt2<minJetPt2 || fabs(me.eta2)>maxJetEta) return false;
      if (  me.jdphi < sigDPhi) return false;
      
      // MC pt hat selection
//       if (isMC) {
//          if ( (evt.samplePtHat-50)<0.1 && evt.pthat > 80 ) return false;
//          if ( (evt.samplePtHat-80)<0.1 && evt.pthat > 120 ) return false;
//          if ( (evt.samplePtHat-120)<0.1 && evt.pthat > 200 ) return false;
//       }
      
      return true;
   }

   // Function to Calculate reweighting
   //----------------------------------------------------------------------------------------
   void CalcWeights() {
      int numEvt = t->GetEntries();
      if (maxEntry>0) numEvt = maxEntry;

      ///////////////////////////////////
      // Fill Jet Phi Distributions
      ///////////////////////////////////
      for (int iEvt=0; iEvt<numEvt; ++iEvt) {
         t->GetEntry(iEvt);
         if (cMax<=12&&iEvt%1000==0) cout <<"Calc Weight: " << iEvt<<" / "<<numEvt <<endl;

         ///////////////////////////////////
         // Event Selection (Should be identical to event loop
         ///////////////////////////////////
         if (!SelEvt()) continue;

         hJetPhi[0]->Fill(me.phi1);
         hJetPhi[1]->Fill(me.phi2);
         hJetWt[0]->Fill(me.phi1,me.Aj);
         hJetWt[1]->Fill(me.phi2,me.Aj);
      }
      
      cout << "Calc Event Weight: " << hJetPhi[0]->GetEntries() << endl;

      ///////////////////////////////////
      // Calculate the reweighting factors
      ///////////////////////////////////
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
   
   // Function to Loop events
   //----------------------------------------------------------------------------------------
   void Loop() {
      int numEvt = t->GetEntries();
      if (maxEntry>0) numEvt = maxEntry;      
//       for (int i=1; i<=hJetWt[0]->GetNbinsX(); ++i) {
//          cout << "jet1 2d bin: " << i << " wt: " << hJetWt[0]->GetBinContent(i,hJetWt[0]->GetNbinsY()) << endl;    
//       }

      bool studyEvent[10][2];
      float studyPtMin = 8;

      ///////////////////////////////////
      // Main Event Loop
      ///////////////////////////////////
      for (int iEvt=0; iEvt<numEvt; ++iEvt) {
         t->GetEntry(iEvt);
      if (cMax<=12&&iEvt%1000==0) cout <<iEvt<<" / "<<numEvt << " run: " << evt.run << " evt: " << evt.evt << " bin: " << evt.cBin << " nT: " << evt.nT << " trig: " <<  evt.trig << " anaEvtSel: " << evt.anaEvtSel <<endl;

         me.clear();

         ///////////////////////////////////
         // Event Selection
         ///////////////////////////////////
         if (!SelEvt()) continue;

         ///////////////////////////////////
         // Event Weighting
         ///////////////////////////////////
         // Jet Phi Flattening
         if (doJetPhiFlat) {
            int jet1PhiBin = hJetPhi[0]->FindFixBin(me.phi1);
            int jet2PhiBin = hJetPhi[1]->FindFixBin(me.phi2);
            int jetAjBin = hAj->FindBin(me.Aj);
            me.jet1Wt = hJetWt[0]->GetBinContent(jet1PhiBin,jetAjBin);
            me.jet2Wt = hJetWt[1]->GetBinContent(jet2PhiBin,jetAjBin);
//             if (jet1PhiBin==1&&jetAjBin==hJetWt[0]->GetNbinsY())
//                cout << "jet1 phi: " << me.phi1 << " bin: " << jet1PhiBin << "," << jetAjBin << " wt: " << me.jet1Wt << endl;
         }
         // Sample Weight
//          if (isMC) me.evtWt = evt.sampleWeight;
         
         ///////////////////////////////////
         // Event Distributions
         ///////////////////////////////////
         hCentrality->Fill(evt.cBin,me.evtWt);
         hPtHat->Fill(evt.pthat,me.evtWt);
         hJetPt2D->Fill(me.pt1,me.pt2,me.evtWt);
         hJDPhi->Fill(me.jdphi,me.evtWt);
         hAj->Fill(me.Aj,me.evtWt);
         

         ///////////////////////////////////
         // MPT Tracking Closure Studies
         ///////////////////////////////////
         for (int i=0; i<10; ++i) {
            for (int j=0; j<2; ++j) {
               studyEvent[i][j] = false;
            }
         }

         // 0. Aj
         if (me.Aj<0.1) studyEvent[0][0] = true;
         else if (me.Aj>0.35) studyEvent[0][1] = true;
         // 1. pt1
         if (me.pt1>60&&me.pt1<80) studyEvent[1][0] = true;
         else if (me.pt1>220&&me.pt1<260) studyEvent[1][1] = true;
         // 2. pt2
         if (me.pt2>60&&me.pt2<80) studyEvent[2][0] = true;
         else if (me.pt2>220&&me.pt2<260) studyEvent[2][1] = true;
         // 3. eta1
         if (fabs(me.eta1)<0.8) studyEvent[3][0] = true;
         else studyEvent[3][1] = true;
         // 4. eta2
         if (fabs(me.eta2)<0.8) studyEvent[4][0] = true;
         else studyEvent[4][1] = true;
         // 5. vz
         if (fabs(evt.vz)<5) studyEvent[5][0] = true;
         else studyEvent[5][1] = true;

         for (int i=0; i<10; ++i) {
            for (int j=0; j<2; ++j) {
               if (studyEvent[i][j]) {
                  vhAj[i][j]->Fill(me.Aj,me.evtWt);
               }
            }
         }
         
         ///////////////////////////////////
         // Track Loop
         ///////////////////////////////////
         me.n=0;
         for (int ip=0; ip<dj.nTrk; ++ip) {
            // Track Selection
            if (dj.trkPt[ip]<minPt) continue;
            if (fabs(dj.trkEta[ip])>maxEta) continue;
            hTrkPtNoQual->Fill(dj.trkPt[ip],me.evtWt);
            if (dj.trkAlgo[ip]>=4&&!dj.vtrkQual[ip][0]) continue;
            if (particleRecLevel==3&&dj.trkIsFake[ip]) continue;

            // Track Variables
            float trkPt = dj.trkPt[ip];
            float trkEta = dj.trkEta[ip];
            float trkPhi = dj.trkPhi[ip];
            float dr1 = deltaR(trkEta,trkPhi,me.eta1,me.phi1);
            float dr2 = deltaR(trkEta,trkPhi,me.eta2,me.phi2);
            float dphi1 = fabs(deltaPhi(trkPhi,me.phi1));
            float trkPtp = trkPt * fabs(cos(trkPhi-me.phi1));

            // Raw Track Distributions
            hTrkPt->Fill(trkPt,me.evtWt);
            hTrkEta->Fill(trkEta,me.evtWt);

            ///////////////////////////////////
            // Tracking Corrections
            ///////////////////////////////////
            float trkWt = dj.trkWt[ip];
            if (me.pt1>40&&dr1<0.8) {
               trkWt = trkCorr.GetCorr(trkPt,trkEta,me.pt1,evt.cBin);
            } else if (me.pt2>40&&dr2<0.8) {
               trkWt = trkCorr.GetCorr(trkPt,trkEta,me.pt2,evt.cBin);
            } else {
               trkWt = trkCorr.GetCorr(trkPt,trkEta,0,evt.cBin);
            }

            ///////////////////////////////////
            // Residual Corrections
            ///////////////////////////////////
            if (doResCorr) {
               if ( cos(dj.trkPhi[ip]-me.phi1)<0 ) {
                  if (me.pt2<80) {
                     if (trkPt>30) trkWt*=1.2;
                     else if (trkPt>20) trkWt*=1.1;
                     else if (trkPt>8) trkWt*=1.05;
                  }
               } else {
                  if (me.pt2<80) {
                     if (trkPt>30) trkWt*=0.8;
                     else if (trkPt>20) trkWt*=0.9;
                     else if (trkPt>8) trkWt*=0.95;
                  }
               }
            }

            // Corrected Track Distributions
            hTrkCorrPt->Fill(trkPt,trkWt*me.evtWt);
            hTrkCorrEta->Fill(trkEta,trkWt*me.evtWt);

            ///////////////////////////////////
            // MPT Tracking Closure Studies
            ///////////////////////////////////
            for (int i=0; i<10; ++i) {
               for (int j=0; j<2; ++j) {
                  if (studyEvent[i][j]) {
                     if (trkPt<studyPtMin) continue;
                     vhTrkDPhi[i][j]->Fill(dphi1,trkPtp*me.evtWt);
                     vhTrkCorrDPhi[i][j]->Fill(dphi1,trkPtp*trkWt*me.evtWt);
                  }
               }
            }

            // Set mpt input
            if (particleRecLevel>=3) {
                  me.pt[me.n] = trkPt;
                  me.eta[me.n] = trkEta;
                  me.phi[me.n] = dj.trkPhi[ip];
                  if (particleRecLevel==4) me.weight[me.n] = trkWt;
                  if (particleRecLevel==3) me.weight[me.n] = trkWt/(1-dj.trkFak[ip]);
                  ++me.n;
            }
         }

         if (particleRecLevel>=1&&particleRecLevel<3) {
            ///////////////////////////////////
            // Sim Loop
            ///////////////////////////////////
            me.n=0;
            for (int ip=0; ip<dj.nSim; ++ip) {
               // Sim Selection
               if (dj.simPt[ip]<minPt) continue;
               if (fabs(dj.simEta[ip])>maxEta) continue;
               if (particleRecLevel==2&&dj.simHasRec[ip]) continue;

               // Sim Distributions
               hGenpPt->Fill(dj.simPt[ip],me.evtWt);
               hGenpEta->Fill(dj.simEta[ip],me.evtWt);

               // Set mpt input
               me.pt[me.n] = dj.simPt[ip];
               me.eta[me.n] = dj.simEta[ip];
               me.phi[me.n] = dj.simPhi[ip];
               if (particleRecLevel==1) me.weight[me.n] = 1;
//                if (particleRecLevel==2) me.weight[me.n] = 1./dj.trkEff[ip];
               ++me.n;
            }
         }else if (particleRecLevel==0){
            ///////////////////////////////////
            // Genp Loop
            ///////////////////////////////////
            me.n=0;
            for (int ip=0; ip<dj.nGenp; ++ip) {
               // Genp Selection
               if (dj.genpPt[ip]<minPt) continue;
               if (fabs(dj.genpEta[ip])>maxEta) continue;
               if (subEvtMode==0 && dj.genpSube[ip]!=0) continue;
               if (subEvtMode==1 && dj.genpSube[ip]==0) continue;

               // Genp Variables
               float genpPt = dj.genpPt[ip];
               float genpEta = dj.genpEta[ip];
               float genpPhi = dj.genpPhi[ip];
               float dphi1 = fabs(deltaPhi(genpPhi,me.phi1));
               float genpPtp = genpPt * fabs(cos(genpPhi-me.phi1));

               // Genp Distributions
               hGenpPt->Fill(genpPt,me.evtWt);
               hGenpEta->Fill(genpEta,me.evtWt);

               ///////////////////////////////////
               // MPT Tracking Closure Studies
               ///////////////////////////////////
               for (int i=0; i<10; ++i) {
                  for (int j=0; j<2; ++j) {
                     if (studyEvent[i][j]) {
                        if (genpPt<studyPtMin) continue;
                        vhGenpDPhi[i][j]->Fill(dphi1,genpPtp*me.evtWt);
                     }
                  }
               }

               // Set mpt input
               me.pt[me.n] = genpPt;
               me.eta[me.n] = genpEta;
               me.phi[me.n] = genpPhi;
               me.weight[me.n] = 1;
               ++me.n;
            }
         } // End of if rec level

         ///////////////////////////////////
         // Calculate MPT
         ///////////////////////////////////
         me.Calc();

         ///////////////////////////////////
         // Fill Event Summed Variables
         ///////////////////////////////////
         hMpt->Fill(me.Aj,me.x,me.evtWt);
         for (int i=0; i<nptrange; ++i) {
            vhMptPt[i]->Fill(me.Aj,me.x_pt[i],me.evtWt);
            for (int j=0; j<ndrbin; ++j) {
               vhMptPtDr[i][j]->Fill(me.Aj,me.x_pt_dr[i][j],me.evtWt);
            }
            for (int k=0; k<ndphibin; ++k) {
               vhMptPtDPhi[i][k]->Fill(me.Aj,me.x_pt_dphi[i][k],me.evtWt);
            }
         }
         
         tm->Fill();
      } // End of event loop

      cout << "==================================================" << endl;
      cout << "Selected Events: " << hCentrality->GetEntries() << endl;
      cout << "Weighted Events: " << hCentrality->Integral() << endl;
      cout << "==================================================" << endl;
      
      SetAliases(tm);
   }

   // Function to Build other meaning variables
   //----------------------------------------------------------------------------------------
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
   
   // Function to Set useful aliases
   //----------------------------------------------------------------------------------------
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
