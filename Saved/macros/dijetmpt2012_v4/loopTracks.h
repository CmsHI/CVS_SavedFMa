#ifndef LoopTrk_h
#define LoopTrk_h
#include <vector>
#include "loopMpt.h"
using namespace TMath;

class AnaTrk
{
public:
   TString name;
   bool isMC;
   bool trkPhiMode;
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

   std::vector<TrackingCorrections*> vtrkCorr;
   
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
   TH1D * hTrkPt;
   TH1D * hTrkEta;
   TH1D * hTrkCorrPt;
   TH1D * hTrkCorrEta;
   std::vector<TH2D*> vhEffZero;
   std::vector<TH2D*> vhEffNonZero;
   
   AnaTrk(TString myname) :
   name(myname),
   isMC(false),
   trkPhiMode(false),
   subEvtMode(-1),
   particleRecLevel(4), // 0 gen, 1 sim, 2 sim mat, 3 rec mat 4 rec
   maxEntry(-1) // -1 for everything
   {
      cout << "Analyze MPT: " << name << endl;
      
      // Track corrections
      if (!trkPhiMode) {
//          vtrkCorr.push_back(new TrackingCorrections("Forest2_TrkCorrv6","hitrkEffAnalyzer_MergedGeneral"));
//          vtrkCorr.push_back(new TrackingCorrections("Forest2_TrkCorrv6","hitrkEffAnalyzer_MergedGeneral_j1"));
//          vtrkCorr.push_back(new TrackingCorrections("Forest2_TrkCorrv6","hitrkEffAnalyzer_MergedGeneral_j2"));
         vtrkCorr.push_back(new TrackingCorrections("Forest2STA","Forest2_MergedGeneral"));
      } else {
         vtrkCorr.push_back(new TrackingCorrections("Forest2_TrkCorrv6","hitrkEffAnalyzer_MergedGeneral_trkPhi_noJet"));
         vtrkCorr.push_back(new TrackingCorrections("Forest2_TrkCorrv6","hitrkEffAnalyzer_MergedGeneral_trkPhi_j1_50to120"));
         vtrkCorr.push_back(new TrackingCorrections("Forest2_TrkCorrv6","hitrkEffAnalyzer_MergedGeneral_trkPhi_j1_120to200"));
         vtrkCorr.push_back(new TrackingCorrections("Forest2_TrkCorrv6","hitrkEffAnalyzer_MergedGeneral_trkPhi_j1_200to999"));
         vtrkCorr.push_back(new TrackingCorrections("Forest2_TrkCorrv6","hitrkEffAnalyzer_MergedGeneral_trkPhi_j2_50to120"));
         vtrkCorr.push_back(new TrackingCorrections("Forest2_TrkCorrv6","hitrkEffAnalyzer_MergedGeneral_trkPhi_j2_120to200"));
         vtrkCorr.push_back(new TrackingCorrections("Forest2_TrkCorrv6","hitrkEffAnalyzer_MergedGeneral_trkPhi_j2_200to999"));
      }
   }
   
   void Init(TTree * tgj) {
      t=tgj;
      JTSetBranchAddress(tgj,evt,dj);

      for (int i=0; i< vtrkCorr.size(); ++i) {
         if (!trkPhiMode) {
//             vtrkCorr[i]->AddSample("trkcorr/Forest2_TrkCorrv4/trkcorr_hy18dj80_Forest2_TrkCorrv4.root",80);
//             vtrkCorr[i]->AddSample("trkcorr/TrkCorrv8/TrkCorrv8_hy18dj80_10k_icPu5.root",80);
            vtrkCorr[i]->AddSample("trkcorr/TrkCorrv8/TrkCorrv8_hy18dj80_50k_icPu5.root",80);
//             vtrkCorr[i]->AddSample("trkcorr/TrkCorrv8/TrkCorrv8_hy18dj80_icPu5.root",80);
         } else {
            vtrkCorr[i]->AddSample("trkcorr/Forest2_TrkCorrv6/trkcorr_hy18dj80_Forest2_TrkCorrv6.root",80);
         }
         vtrkCorr[i]->smoothLevel_ = 0;
         vtrkCorr[i]->weightSamples_ = false;
         vtrkCorr[i]->Init();
      }
   
      if (outf) outf->cd();

      ///////////////////////////////////
      // Event Distributions
      ///////////////////////////////////
      hCentrality = new TH1D("hCentrality_"+name,";Centrality Bin;",40,0,40);
      hPtHat = new TH1D("hPtHat_"+name,";#hat{p}_{T} (GeV/c);",120,0,600);
      hJetPt2D = new TH2D("hJetPt2D_"+name,";p_{T,1} (GeV/c);p_{T,2} (GeV/c)",100,0,500,100,0,500);
      hJDPhi = new TH1D("hJDPhi_"+name,";#Delta#phi(j1,j2);",40,0,3.14159);
      hAj = new TH1D("hAj_"+name,";A_{J};",nAjBin,AjBins);

      ///////////////////////////////////
      // Tracking Distributions
      ///////////////////////////////////
      TH1D * xBin = (TH1D*)vtrkCorr[0]->ptBin_->Clone("hX"+name);
      xBin->Reset();
//       TH1D * xBin    = new TH1D("hX"+name,"",200,0.5,5.5);
      hGenpPt        = (TH1D*)xBin->Clone("hGenpPt_"+name);
      hTrkPtNoQual   = (TH1D*)xBin->Clone("hTrkPtNoQual_"+name);
      hTrkPt         = (TH1D*)xBin->Clone("hTrkPt_"+name);
      hTrkCorrPt     = (TH1D*)xBin->Clone("hTrkCorrPt_"+name);

      hGenpEta = new TH1D("hGenpEta_"+name,"; #eta;",12,-2.4,2.4);
      hTrkEta = new TH1D("hTrkEta_"+name,"; #eta;",12,-2.4,2.4);
      hTrkCorrEta = new TH1D("hTrkCorrEta_"+name,"; #eta;",12,-2.4,2.4);
      
      ///////////////////////////////////
      // Eff. Distributions
      ///////////////////////////////////
      for (unsigned int i=0; i<3; ++i) {
         vhEffZero.push_back(new TH2D(Form("hEffZero_%d",i),";Jet p_{T} (GeV/c);Trk p_{T} (GeV/c);",100,0,500,50,0,100));
      }
   }

   // Function to Select Event And set event vars
   //----------------------------------------------------------------------------------------
   bool SelEvt() {
      me.pt1 = dj.pt1; me.phi1 = dj.phi1; me.eta1 = dj.eta1;
      me.pt2 = dj.pt2; me.phi2 = dj.phi2; me.eta2 = dj.eta2;

      me.Aj = (me.pt1-me.pt2)/(me.pt1+me.pt2);
      me.jdphi = fabs(deltaPhi(me.phi1,me.phi2));

      if (!isMC&&!evt.anaEvtSel) return false;
      if (isMC&&!evt.offlSel) return false;

      if (evt.cBin<cMin||evt.cBin>=cMax) return false;

      // Jet Selection
      if (me.pt1<minJetPt1 || fabs(me.eta1)>maxJetEta) return false;
      if (minJetPt2>0) {
         if (me.pt2<minJetPt2 || fabs(me.eta2)>maxJetEta) return false;
      }
      if (sigDPhi>0) {
         if (  me.jdphi < sigDPhi) return false;
      }
      
      return true;
   }
   
   // Function to Loop events
   //----------------------------------------------------------------------------------------
   void Loop() {
      int numEvt = t->GetEntries();
      if (maxEntry>0) numEvt = maxEntry;      

      ///////////////////////////////////
      // Main Event Loop
      ///////////////////////////////////
      for (int iEvt=0; iEvt<numEvt; ++iEvt) {
         t->GetEntry(iEvt);
      if (cMax<=12&&iEvt%1000==0) cout <<iEvt<<" / "<<numEvt << " run: " << evt.run << " evt: " << evt.evt << " bin: " << evt.cBin << " nT: " << evt.nT << " trig: " <<  evt.trig << " anaEvtSel: " << evt.anaEvtSel <<endl;

         ///////////////////////////////////
         // Event Selection
         ///////////////////////////////////
         if (!SelEvt()) continue;

         ///////////////////////////////////
         // Event Distributions
         ///////////////////////////////////
         hCentrality->Fill(evt.cBin);
         hPtHat->Fill(evt.pthat);
         hJetPt2D->Fill(me.pt1,me.pt2);
         hJDPhi->Fill(me.jdphi);
         hAj->Fill(me.Aj);
         
         cout << "pt1/eta: " << me.pt1 << "/" << me.eta1 << " pt2/eta2: " << me.pt2 << "/" << me.eta2 << endl;
          
         ///////////////////////////////////
         // Track Loop
         ///////////////////////////////////
         if (particleRecLevel>=3) {
            for (int ip=0; ip<dj.nTrk; ++ip) {
               // Track Selection
               if (dj.trkPt[ip]<minPt) continue;
               if (fabs(dj.trkEta[ip])>maxEta) continue;
               if (dj.trkAlgo[ip]>=4&&!dj.vtrkQual[ip][0]) continue;
               if (particleRecLevel==3&&dj.trkIsFake[ip]) continue;
   
               // Track Variables
               float trkPt = dj.trkPt[ip];
               float trkEta = dj.trkEta[ip];
               float trkPhi = dj.trkPhi[ip];
               float dr1 = deltaR(trkEta,trkPhi,me.eta1,me.phi1);
               float dr2 = deltaR(trkEta,trkPhi,me.eta2,me.phi2);
   
               // Raw Track Distributions
               hTrkPt->Fill(trkPt);
               hTrkEta->Fill(trkEta);
   
               ///////////////////////////////////
               // Tracking Corrections
               ///////////////////////////////////
               double trkcorr[4];
               float trkWt;
               if (me.pt1>40&&dr1<0.5) {
                  trkWt = vtrkCorr[0]->GetCorr(trkPt,trkEta,me.pt1,evt.cBin,trkcorr);
                  if (fabs(trkcorr[0]-1)<0.01) cout << "warning eff=0 for LJ pt eta jet cent: " << trkPt << " " << trkEta << " " << me.pt1 << " c" << evt.cBin << endl;
               } else if (me.pt2>40&&dr2<0.5) {
                  trkWt = vtrkCorr[0]->GetCorr(trkPt,trkEta,me.pt2,evt.cBin,trkcorr);
                  if (fabs(trkcorr[0]-1)<0.01) cout << "warning eff=0 for SubLJ pt eta jet cent: " << trkPt << " " << trkEta << " " << me.pt2 << " c" << evt.cBin << endl;
               } else {
                  trkWt = vtrkCorr[0]->GetCorr(trkPt,trkEta,0,evt.cBin,trkcorr);
                  if (fabs(trkcorr[0]-1)<0.01) cout << "warning eff=0 for NoLJ pt eta jet cent: " << trkPt << " " << trkEta << " " << 0 << " c" << evt.cBin << endl;
               }
               if (particleRecLevel==3) trkWt=1./trkcorr[0];
   
               // Corrected Track Distributions
               hTrkCorrPt->Fill(trkPt,trkWt);
               hTrkCorrEta->Fill(trkEta,trkWt);
            }
         }

         ///////////////////////////////////
         // Sim Loop
         ///////////////////////////////////
         for (int ip=0; ip<dj.nSim; ++ip) {
            // Sim Selection
            if (dj.simPt[ip]<minPt) continue;
            if (fabs(dj.simEta[ip])>maxEta) continue;

            // Sim Variables
            float simPt = dj.simPt[ip];
            float simEta = dj.simEta[ip];
            float simPhi = dj.simPhi[ip];
            float dr1 = deltaR(simEta,simPhi,me.eta1,me.phi1);
            float dr2 = deltaR(simEta,simPhi,me.eta2,me.phi2);

            // Sim Distributions
            hGenpPt->Fill(simPt);
            hGenpEta->Fill(simEta);

            if (particleRecLevel==2&&dj.simHasRec[ip]) {
               ///////////////////////////////////
               // Tracking Corrections
               ///////////////////////////////////
               double trkcorr[4];
               float trkWt;
               if (me.pt1>40&&dr1<0.5) {
                  trkWt = vtrkCorr[0]->GetCorr(simPt,simEta,me.pt1,evt.cBin,trkcorr);
                   if (fabs(trkcorr[0]-1)<0.01) {
                     cout << "warning eff=0 for LJ pt eta jet cent: " << simPt << " " << simEta << " " << me.pt1 << " c" << evt.cBin << endl;
                     vhEffZero[1]->Fill(me.pt1,simPt);
                     }
               } else if (me.pt2>40&&dr2<0.5) {
                  trkWt = vtrkCorr[0]->GetCorr(simPt,simEta,me.pt2,evt.cBin,trkcorr);
                  if (fabs(trkcorr[0]-1)<0.01) {
                     cout << "warning eff=0 for SubLJ pt eta jet cent: " << simPt << " " << simEta << " " << me.pt2 << " c" << evt.cBin << endl;
                     vhEffZero[2]->Fill(me.pt2,simPt);
                  }
               } else {
                  trkWt = vtrkCorr[0]->GetCorr(simPt,simEta,0,evt.cBin,trkcorr);
                  if (fabs(trkcorr[0]-1)<0.01) {
                     cout << "warning eff=0 for NoLJ pt eta jet cent: " << simPt << " " << simEta << " " << 0 << " c" << evt.cBin << endl;
                     vhEffZero[0]->Fill(0.,simPt);
                  }
               }
               // Track Distributions
               hTrkPt->Fill(simPt);
               hTrkEta->Fill(simEta);
               hTrkCorrPt->Fill(simPt,1./trkcorr[0]);
               hTrkCorrEta->Fill(simEta,1./trkcorr[0]);
            }
         }
      } // End of event loop

      cout << "==================================================" << endl;
      cout << "Selected Events: " << hCentrality->GetEntries() << endl;
      cout << "==================================================" << endl;
   }
};
#endif
