#include "../HiForest_HEAD/hiForest.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include "TChain.h"
#include "TMath.h"
#include "GammaJet.h"
#include "analyzeTrackingCorrectionBase.h"
using namespace std;
using namespace TMath;

class Isolation{
public:
  float cc1,cc2,cc3,cc4,cc5;
  float cr1,cr2,cr3,cr4,cr5;
  float ct1PtCut20,ct2PtCut20,ct3PtCut20,ct4PtCut20,ct5PtCut20;
  void Set(HiForest * c, int j) {
    cc1=c->photon.cc1[j];
    cc2=c->photon.cc2[j];
    cc3=c->photon.cc3[j];
    cc4=c->photon.cc4[j];
    cc5=c->photon.cc5[j];
    cr1=c->photon.cr1[j];
    cr2=c->photon.cr2[j];
    cr3=c->photon.cr3[j];
    cr4=c->photon.cr4[j];
    cr5=c->photon.cr5[j];
    ct1PtCut20=c->photon.ct1PtCut20[j];
    ct2PtCut20=c->photon.ct2PtCut20[j];
    ct3PtCut20=c->photon.ct3PtCut20[j];
    ct4PtCut20=c->photon.ct4PtCut20[j];
    ct5PtCut20=c->photon.ct5PtCut20[j];
  }
};

void analyzeTrackingCorrectionGJ(
   TString jetAlgo = "akPu3PF",
   TString trkCol = "anaTrack",
   TString inname="/mnt/hadoop/cms/store/user/yenjie/HiForest_v27/Dijet${pthat}_HydjetDrum_v27_mergedV1.root",
   TString outname="trkcorr/test/TrkCorrtest_hy18dj100.root",
   bool isPP=false,
   double samplePtHat=0,
   double ptHatMax=9999,
   double sampleWeight = 1,
   double cutPtTrk=0.5,
   double vzMax = 15,
   int maxEntries = -1,
   double leadingJetPtMin=-1,
   double subleadingJetPtMin=-1,
   double sigDPhi=-1,
   bool genSelMode=false
) {
   TH1::SetDefaultSumw2();
   TString tag=Form("%s_%.0f_%.0f_%.0f_genSelMode%d",jetAlgo.Data(),leadingJetPtMin,subleadingJetPtMin,sigDPhi*1000,genSelMode);
   outname.ReplaceAll(".root",Form("_%s.root",tag.Data()));
   cout << "Input: " << inname << endl;
   cout << "Sample pthat = " << samplePtHat << " ptHatMax = " << ptHatMax << endl;
   cout << "Track pt min = " << cutPtTrk << endl;
   cout << "skim: leading Jet > " << leadingJetPtMin << " subleading > " << subleadingJetPtMin << " dphi > " << sigDPhi  << endl;
   cout << "genSelMode: " << genSelMode << endl;
   cout << "Output: " << outname << endl;

   ///////////////////////////////////////////////////
   // Setup Analysis
   ///////////////////////////////////////////////////
   double cutjetPt = 30;
   double cutjetEta = 2;
   double cutEtaTrk = 2.4;
   double cutphotonEta = 1.44;
   double preCutPhotonEt = 30;
  
   // Define the input file and HiForest
   HiForest * c = new HiForest(inname,jetAlgo.Data());
   c->doTrackCorrections = true;
   c->doTrackingSeparateLeadingSubleading = false;
   c->triggerMode = kPHOTON; // kJET, kPHOTON, kMB
   c->GetEnergyScaleTable("./photonEnergyScaleTable_Hydjet_GammaJet.root");

   // intialize jet variables
   Jets * anajet = &(c->akPu3PF);
//    if (jetAlgo=="akPu3PF") anajet = &(c->akPu3PF);
//    else if (jetAlgo=="akPu3Calo") anajet = &(c->akPu3Calo);
//    else {
//       cout << "Fatal Error: jetalgo " << jetAlgo << " not defined" << endl;
//       exit(1);
//    }
   
   // Output file
   cout << "Output: " << outname << endl;
   TFile *output = new TFile(outname,"recreate");

   EvtSel evt;
   GammaJet gj;
   Isolation isol;

   ///////////////////////////////////////////////////
   // Book Histograms
   ///////////////////////////////////////////////////
   // Tracking Corrections
   TrkCorrHisAna effIter("Forest2Iter",output,30,isPP);
   effIter.jetBins.clear();
   float jetBins[6] = {0,30,60,120,200,1000};
   effIter.jetBins.insert(effIter.jetBins.begin(),jetBins,jetBins+6);
   effIter.DeclareHistograms();

   // basics
   output->cd();
   TH1D * hCent = new TH1D("hCent","",40,0,40);
   TH1D * hVz = new TH1D("hVz","",60,-30,30);
   TH1D * hPtHatBeforeSel = new TH1D("hPtHatBeforeSel","",200,0,1000);
   TH1D * hPtHat = new TH1D("hPtHat","",200,0,1000);
   TH2D * hJetPt2D = new TH2D("hJetPt2D","",100,0,500,100,0,500);
   TH1D * hDPhijg = new TH1D("hDPhijg","",40,0,Pi());
   TH2D * hJPtDPhijg = new TH2D("hJPtDPhijg","",40,0,Pi(),100,0,500);
   TH1D * hXjg = new TH1D("hXjg","",50,0,2);
   TH1D * hSS = new TH1D("hSS","",100,0,0.02);
   TH1D * hSumIso = new TH1D("hSumIso","",100,-50,50);
   TH1D * hGenPhoMom = new TH1D("hGenPhoMom","",200,-100,100);

   // In separate centrality bins
   vector<TH1D*> vhCent;
   vector<TH1D*> vhPtHat;
   vector<TH1D*> vhPtHatBeforeSel;
   vector<TH2D*> vhJetPt2D;
   for (int ib=0; ib<effIter.centBins.size()-1; ++ib) {
      vhCent.push_back(new TH1D(Form("hCent_c%d",ib),
      Form("cbin%dto%d;Centrality Bin;",effIter.centBins[ib],effIter.centBins[ib+1]),40,0,40));
      vhPtHat.push_back(new TH1D(Form("hPtHat_c%d",ib),"",200,0,1000));
      vhPtHatBeforeSel.push_back(new TH1D(Form("hPtHatBeforeSel_c%d",ib),"",200,0,1000));
      vhJetPt2D.push_back(new TH2D(Form("hJetPt2D_c%d",ib),"",100,0,500,100,0,500));
   }

   ///////////////////////////////////////////////////
   // Main loop
   ///////////////////////////////////////////////////
   if (maxEntries<0) maxEntries = c->GetEntries();
   for (int i=0;i<maxEntries;i++) {
      c->GetEntry(i);
      evt.cBin = c->evt.hiBin;
      evt.evtPlane = c->evt.hiEvtPlanes[21];
      evt.nJ = anajet->nref;
      evt.nT = c->track.nTrk;
      evt.offlSel = (c->skim.pcollisionEventSelection > 0);
      if (!c->hasSkimTree) evt.offlSel = (c->evt.hiNtracks>0 && c->evt.hiHFplus>=4 && c->evt.hiHFminus>=4);
      evt.vz = c->track.vz[1];
      evt.sampleWeight = sampleWeight/maxEntries; // for different mc sample, 1 for data
      evt.pthat = anajet->pthat;
      evt.samplePtHat = samplePtHat;

      if (i%1000==0) cout <<i<<" / "<< maxEntries << " pthat: " << evt.pthat << " cBin: " << evt.cBin << " nT: " << evt.nT <<endl;

      // initialize
      int leadingIndex=-1,genLeadingIndex=-1;
      int awayIndex=-1,genAwayIndex=-1;
      gj.clear();
      
      ///////////////////////////////////////////////////////
      // Find Photon
      ///////////////////////////////////////////////////////
      for (int j=0;j<c->photon.nPhotons;j++) {	
         // loose cut!!! 
         if ( c->photon.pt[j]  < preCutPhotonEt ) continue; 
         if ( fabs(c->photon.eta[j]) > cutphotonEta ) continue;
         if (c->isSpike(j)) continue;
         if (!(c->isLoosePhoton(j))) continue;
         
         // sort using corrected photon pt        
         float theCorrPt= c->getCorrEt(j);
//          float theCorrPt= c->photon.pt[j];
         if ( theCorrPt > gj.photonEt) {
           gj.photonEt = theCorrPt;
           leadingIndex = j;
         }
      }


      // set photon
      if (leadingIndex!=-1) {
         gj.photonRawEt=c->photon.pt[leadingIndex];
         gj.photonEta=c->photon.eta[leadingIndex];
         gj.photonPhi=c->photon.phi[leadingIndex];
         gj.hovere=c->photon.hadronicOverEm[leadingIndex];
         gj.sigmaIetaIeta=c->photon.sigmaIetaIeta[leadingIndex];
         gj.sumIsol=(c->photon.cr4[leadingIndex]+c->photon.cc4[leadingIndex]+c->photon.ct4PtCut20[leadingIndex])/0.9;
         gj.genIso   = c->photon.genCalIsoDR04[leadingIndex];
         gj.genMomId = c->photon.genMomId[leadingIndex];
         isol.Set(c,leadingIndex);
         
         // Loop over jet tree to find a away side leading jet
         for (int j=0;j<anajet->nref;j++) {
            if (anajet->jtpt[j]<cutjetPt) continue;
            if (fabs(anajet->jteta[j])>cutjetEta) continue;
            if (fabs(deltaPhi(anajet->jtphi[j],gj.photonPhi))>PiOver2() // require jet to be on away side
               &&anajet->jtpt[j] > gj.lJetPt) {
               gj.lJetPt = anajet->jtpt[j];
               awayIndex = j;
            }
         }

         if (awayIndex !=-1) { // Found an away jet!
            gj.lJetPt = anajet->jtpt[awayIndex];
            gj.lJetEta = anajet->jteta[awayIndex];
            gj.lJetPhi = anajet->jtphi[awayIndex];
         }
      } // end of if leadingIndex

      ///////////////////////////////////////////////////////
      // Skim
      ///////////////////////////////////////////////////////
      if (!evt.offlSel) continue;
      if (samplePtHat>0 && evt.pthat>=ptHatMax) continue;
      if (vzMax>0 && fabs(evt.vz)>vzMax) continue;
      hPtHatBeforeSel->Fill(evt.pthat);
      for (int ib=0; ib<effIter.centBins.size(); ++ib) {
         if(evt.cBin>=effIter.centBins[ib] && evt.cBin<effIter.centBins[ib+1]){
            vhPtHatBeforeSel[ib]->Fill(evt.pthat);
         }
      }

      // kinematic cuts
      if (leadingJetPtMin>0&&gj.photonEt<leadingJetPtMin) continue;
      if (subleadingJetPtMin>0&&gj.lJetPt<subleadingJetPtMin) continue;
      float dphijg = deltaPhi(gj.lJetPhi,gj.photonPhi);
      if (sigDPhi>0&&fabs(dphijg)<sigDPhi) continue;
      
      // additional photon cuts
      bool isAnaPhoton = (gj.sumIsol<1 && gj.sigmaIetaIeta<0.010 && gj.genIso<5 && fabs(gj.genMomId)<=22);
      if (leadingJetPtMin>0&&!isAnaPhoton) continue;
      
      // Fill Baisc Event info
      hCent->Fill(evt.cBin);
      hVz->Fill(evt.vz);
      hPtHat->Fill(evt.pthat);
      hJetPt2D->Fill(gj.photonEt,gj.lJetPt);
      hDPhijg->Fill( fabs(dphijg) );
      hJPtDPhijg->Fill(fabs(dphijg),gj.lJetPt);
      hXjg->Fill(gj.lJetPt/gj.photonEt);
      hSS->Fill(gj.sigmaIetaIeta);
      hSumIso->Fill(gj.sumIsol);
      hGenPhoMom->Fill(gj.genMomId);
      for (int ib=0; ib<effIter.centBins.size(); ++ib) {
         if(evt.cBin>=effIter.centBins[ib] && evt.cBin<effIter.centBins[ib+1]){
            vhCent[ib]->Fill(evt.cBin);
            vhPtHat[ib]->Fill(evt.pthat);
            vhJetPt2D[ib]->Fill(gj.photonEt,gj.lJetPt);
         }
      }

      ///////////////////////////////////////////////////////
      // Tracks
      ///////////////////////////////////////////////////////
      effIter.pt1 = gj.photonEt;
      effIter.eta1 = gj.photonEta;
      effIter.phi1 = gj.photonPhi;
      effIter.pt2 = gj.lJetPt;
      effIter.eta2 = gj.lJetEta;
      effIter.phi2 = gj.lJetPhi;
      // Full Tracks, Pixel Tracks
      Tracks * anaTrks[1] = {&(c->track)};
      for (int it=0; it<anaTrks[0]->nTrk; ++it) {
         // Kinematic Selection
         if (anaTrks[0]->trkPt[it] < cutPtTrk) continue;
         if (fabs(anaTrks[0]->trkEta[it]) > cutEtaTrk) continue;
         if (anaTrks[0]->trkAlgo[it]>=4&&!anaTrks[0]->highPurity[it]) continue; // quality selection

         RecTrack_t r;
         r.ptr = anaTrks[0]->trkPt[it];
         r.etar = anaTrks[0]->trkEta[it];
         r.phir = anaTrks[0]->trkPhi[it];
         r.algo = anaTrks[0]->trkAlgo[it];
         r.nsim = !anaTrks[0]->trkFake[it];
         r.status = 1; // for now correct all tracks
         float dr1 = deltaR(r.etar,r.phir,gj.photonEta,gj.photonPhi);
         float dr2 = deltaR(r.etar,r.phir,gj.lJetEta,gj.lJetPhi);
         r.jet  = 0;
         r.jeta = -99;
         r.jdr  = -99;
         if (samplePtHat>0) {
           if (dr2<0.5&&gj.lJetPt>=cutjetPt) {
              r.jet = gj.lJetPt;
              r.jeta = gj.lJetEta;
              r.jdr = dr2;
           }
         }

         // Fill
         effIter.FillRecHistograms(evt,r);
      }

      ///////////////////////////////////////////////////////
      // SimTracks
      ///////////////////////////////////////////////////////
      for (int ip=0; ip<anaTrks[0]->nParticle; ++ip) {
         if (anaTrks[0]->pPt[ip] < cutPtTrk) continue;
         if (fabs(anaTrks[0]->pEta[ip]) > cutEtaTrk) continue;

         SimTrack_t s;
         s.pts = anaTrks[0]->pPt[ip];
         s.etas = anaTrks[0]->pEta[ip];
         s.phis = anaTrks[0]->pPhi[ip];
         s.status = 1; // for now assume all simtracks as signal tracks
         s.nrec = (anaTrks[0]->pNRec[ip])*(int)(anaTrks[0]->mtrkAlgo[ip]<4||anaTrks[0]->mtrkQual[ip]>0);
//          if (s.pts<1.5) s.nrec = anaTrks[0]->pNRec[ip];
//          else s.nrec = (anaTrks[0]->pNRec[ip])*(anaTrks[0]->mtrkQual[ip]);
         s.acc = anaTrks[0]->pAcc[ip];

         float dr1 = deltaR(s.etas,s.phis,gj.photonEta,gj.photonPhi);
         float dr2 = deltaR(s.etas,s.phis,gj.lJetEta,gj.lJetPhi);
         s.jet = 0;
         s.jeta = -99;
         s.jdr = -99;
         if (samplePtHat>0) {
           if (dr2<0.5&&gj.lJetPt>=cutjetPt) {
              s.jet = gj.lJetPt;
              s.jeta = gj.lJetEta;
              s.jdr = dr2;
           }
         }
         // Fill
         effIter.FillSimHistograms(evt,s);
      }
      // All done
   }

//    effIter.WriteHistograms();
   output->Write();
   output->Close();
   delete c;
}

