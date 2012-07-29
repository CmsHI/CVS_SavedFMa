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
#include "analyzeTrackingCorrection.h"
using namespace std;
using namespace TMath;

void analyzeDiJetMPT(
   TString jetAlgo = "akPu3PF",
   TString trkCol = "anaTrack",
   TString inname="/mnt/hadoop/cms/store/user/yenjie/HiForest_v27/Dijet${pthat}_HydjetDrum_v27_mergedV1.root",
   TString outname="output.root",
   bool isPP=false,
   int dataSrcType = 1, // 0 mc, 1 hi, 2 pp 2.76 TeV, 3 pp 7TeV
   double samplePtHat=0,
   double ptHatMax=9999,
   double sampleWeight = 1, // data: 1, mc: s = 0.62, b = 0.38
   double vzMax = 15,
   int maxEntries = -1,
   double leadingJetPtMin=-1,
   double subleadingJetPtMin=-1,
   double sigDPhi=-1,
   bool genJetMode=false,
   bool doCentReWeight=false,
   TString mcfname="",
   TString datafname="output-data-Photon-v7_v30.root"
                      )
{
   ///////////////////////////////////////////////////
   // Setup Analysis
   ///////////////////////////////////////////////////
   int saveTracks = 1; // 0=none, 1=all, 10=cone
   double cutjetPt = 100;
   double cutjetEta = 2;
   double cutPtTrk=1.;
   double cutEtaTrk = 2.4;
   double trkJetAssoR=0.3;

   TString tag=Form("%s_%.0f_%.0f_%.0f_saveTrk%d_jmin%.0f_tmin%.0f_genJetMode%d_corrppv14",jetAlgo.Data(),leadingJetPtMin,subleadingJetPtMin,sigDPhi*1000,saveTracks,cutjetPt,cutPtTrk,genJetMode);
   outname.ReplaceAll(".root",Form("_%s.root",tag.Data()));
   cout << "Input: " << inname << " isPP: " << isPP << endl;
   cout << "Sample pthat = " << samplePtHat << " ptHatMax = " << ptHatMax << endl;
   cout << "Track pt min = " << cutPtTrk << endl;
   cout << "skim: leading Jet > " << leadingJetPtMin << " subleading > " << subleadingJetPtMin << " dphi > " << sigDPhi  << endl;
   cout << "Genjet mode: " << genJetMode << endl;
   cout << "Output: " << outname << endl;

   // Centrality reweiting
   CentralityReWeight cw(datafname,mcfname,"offlSel&&pt1>100&&pt2>0&&acos(cos(phi2-phi1))>2./3*3.14159");

   // Define the input file and HiForest
//    HiForest * c = new HiForest(inname,trkCol.Data(),0);
   HiForest * c = new HiForest(inname,trkCol.Data(),isPP);
   c->doTrackCorrections = true;
   c->doTrackingSeparateLeadingSubleading = false;
   c->InitTree();

   // intialize jet variables
   Jets * anajet = 0;
   if (jetAlgo=="akPu3PF") anajet = &(c->akPu3PF);
   else if (jetAlgo=="akPu3Calo") anajet = &(c->akPu3Calo);
   else {
      cout << "Fatal Error: jetalgo " << jetAlgo << " not defined" << endl;
      exit(1);
   }
   
   // Output file
   cout << "Output: " << outname << endl;
   TFile *output = new TFile(outname,"recreate");
   if (doCentReWeight&&mcfname!="") {
      cw.Init(); //cw.hCentData->Draw(); cw.hCentMc->Draw("same");
   }

   // basics
   output->cd();
   TH1D * hCent = new TH1D("hCent","",40,0,40);
   TH1D * hVz = new TH1D("hVz","",60,-30,30);
   TH1D * hPtHatBeforeSel = new TH1D("hPtHatBeforeSel","",200,0,1000);
   TH1D * hPtHat = new TH1D("hPtHat","",200,0,1000);
   TH2D * hJetPt2D = new TH2D("hJetPt2D","",100,0,500,100,0,500);
   TH1D * hJDPhi = new TH1D("hJDPhi","",40,0,Pi());
   TH1D * hAj = new TH1D("hAj","",32,0,0.8);   
   TH1D * hTrkPt = new TH1D("hTrkPt",";p_{T} (GeV/c)",200,0,200);
   TH1D * hTrkCorrPt = new TH1D("hTrkCorrPt",";p_{T} (GeV/c)",200,0,200);
   TH1D * hGenpPt = new TH1D("hGenpPt",";p_{T} (GeV/c)",200,0,200);

   EvtSel evt;
   DiJet gj;
   TTree * tgj = new TTree("tgj","dijet jet tree");
   BookGJBranches(tgj,evt,gj);

   // pp triggers
   int HLT_Photon15_CaloIdVL_v1=0;
   int HLT_Photon50_CaloIdVL_v3=0;
   int HLT_Photon50_CaloIdVL_IsoL_v6=0;
   if (dataSrcType==2) {
      c->hltTree->SetBranchAddress("HLT_Photon15_CaloIdVL_v1",&HLT_Photon15_CaloIdVL_v1);
   } else if (dataSrcType==3) {
      c->hltTree->SetBranchAddress("HLT_Photon50_CaloIdVL_v3",&HLT_Photon50_CaloIdVL_v3);
      c->hltTree->SetBranchAddress("HLT_Photon50_CaloIdVL_IsoL_v6",&HLT_Photon50_CaloIdVL_IsoL_v6);
   }

   ///////////////////////////////////////////////////
   // Main loop
   ///////////////////////////////////////////////////
   if (maxEntries<0) maxEntries = c->GetEntries();
   for (int i=0;i<maxEntries;i++) {
      c->GetEntry(i);
      // Event Info
      evt.run = c->hlt.Run;
      evt.evt = c->hlt.Event;
      evt.cBin = c->evt.hiBin;
      if (dataSrcType>1) evt.cBin = 39;
      evt.evtPlane = c->evt.hiEvtPlanes[21];
      evt.nJ = anajet->nref;
      evt.nT = c->track.nTrk;
      evt.trig = (c->hlt.HLT_HIJet80_v1 > 0);
      evt.offlSel = (c->skim.pcollisionEventSelection > 0);
      if (!c->hasSkimTree) evt.offlSel = (c->evt.hiNtracks>0 && c->evt.hiHFplus>=4 && c->evt.hiHFminus>=4);
      evt.noiseFilt = (c->skim.pHBHENoiseFilter > 0);
      if (dataSrcType>1) {
         if (dataSrcType==2) {
            evt.trig = (HLT_Photon15_CaloIdVL_v1>0);
         } else if (dataSrcType==3) {
            evt.trig = (HLT_Photon50_CaloIdVL_v3>0)||(HLT_Photon50_CaloIdVL_IsoL_v6>0);
         }
         evt.offlSel = (c->skim.phfCoincFilter && c->skim.ppurityFractionFilter);
      }
      if (!c->hasHltTree) evt.trig = true;
      evt.anaEvtSel = evt.offlSel && evt.noiseFilt;
      if (dataSrcType>0) evt.anaEvtSel = evt.anaEvtSel && evt.trig;
      evt.vz = c->track.vz[1];
      // Get Centrality Weight
      if (doCentReWeight) evt.weight = cw.GetWeight(evt.cBin);
      else evt.weight = 1;
      evt.npart = getNpart(evt.cBin);
      evt.ncoll = getNcoll(evt.cBin);
      evt.sampleWeight = sampleWeight/maxEntries; // for different mc sample, 1 for data
      evt.pthat = anajet->pthat;
      evt.samplePtHat = samplePtHat;

      if (i%1000==0) cout <<i<<" / "<<maxEntries << " run: " << evt.run << " evt: " << evt.evt << " bin: " << evt.cBin << " nT: " << evt.nT << " trig: " <<  evt.trig << " anaEvtSel: " << evt.anaEvtSel <<endl;

      // initialize
      int leadingIndex=-1,genLeadingIndex=-1;
      int awayIndex=-1,genAwayIndex=-1;
      gj.clear();
      
      ///////////////////////////////////////////////////////
      // Find Leading jets
      ///////////////////////////////////////////////////////
      for (int j=0;j<anajet->nref;j++) {
         if (anajet->jtpt[j]<cutjetPt) continue;
         if (fabs(anajet->jteta[j])>cutjetEta) continue;
	 if ((anajet->trackMax[j]/anajet->jtpt[j])<0.01) continue;
         if (anajet->jtpt[j] > gj.pt1) {
            gj.pt1 = anajet->jtpt[j];
            leadingIndex = j;
         }
      }

      // Found a leading jet which passed basic quality cut!
      if (leadingIndex!=-1) {
         // set leading jet
         gj.pt1raw=anajet->rawpt[leadingIndex];
         gj.eta1=anajet->jteta[leadingIndex];
         gj.phi1=anajet->jtphi[leadingIndex];
         gj.ref1pt = anajet->refpt[leadingIndex];
         gj.ref1eta = anajet->refeta[leadingIndex];
         gj.ref1phi = anajet->refphi[leadingIndex];
         gj.ref1partonpt = anajet->refparton_pt[leadingIndex];
         gj.ref1partonflavor = anajet->refparton_flavor[leadingIndex];
         
         // Loop over jet tree to find a away side leading jet
         gj.nJet=0;
         for (int j=0;j<anajet->nref;j++) {
            if (anajet->jtpt[j]<cutjetPt) continue;
            if (fabs(anajet->jteta[j])>cutjetEta) continue;
            gj.inclJetPt[gj.nJet] = anajet->jtpt[j];
            gj.inclJetEta[gj.nJet] = anajet->jteta[j];
            gj.inclJetPhi[gj.nJet] = anajet->jtphi[j];
            gj.inclJetRefPt[gj.nJet] = anajet->refpt[j];
            gj.inclJetRefPartonPt[gj.nJet] = anajet->refparton_pt[j];
            if (j!=leadingIndex&&anajet->jtpt[j] > gj.pt2) {
               gj.pt2 = anajet->jtpt[j];
               awayIndex = j;
            }
            ++gj.nJet;
         }

         if (awayIndex !=-1) { // Found an away jet!
            gj.pt2raw = anajet->rawpt[awayIndex];
            gj.eta2   = anajet->jteta[awayIndex];
            gj.phi2   = anajet->jtphi[awayIndex];
            gj.deta   = gj.eta2 - gj.eta1;
            gj.dphi   = deltaPhi(gj.phi2,gj.phi1);
            gj.Aj     = (gj.pt1-gj.pt2)/(gj.pt1+gj.pt2);
            gj.ref2pt = anajet->refpt[awayIndex];
            gj.ref2eta = anajet->refeta[awayIndex];
            gj.ref2phi = anajet->refphi[awayIndex];
            gj.ref2partonpt = anajet->refparton_pt[awayIndex];
            gj.ref2partonflavor = anajet->refparton_flavor[awayIndex];
         }         
      } // end of if leadingIndex
      
      ////////////////////////////////////////
      // Find Leading Genjets
      ////////////////////////////////////////
      // Loop over genjets to look for leading genjet candidate in the event
      for (int j=0;j<anajet->ngen;j++) {
         if (anajet->genpt[j]<cutjetPt) continue;
         if (fabs(anajet->geneta[j])>cutjetEta) continue;
         if (anajet->genpt[j] > gj.genjetpt1) {
            gj.genjetpt1=anajet->genpt[j];
            genLeadingIndex=j;
         }
      }

      if (genLeadingIndex!=-1) {
         gj.genjeteta1=anajet->geneta[genLeadingIndex];
         gj.genjetphi1=anajet->genphi[genLeadingIndex];
      }
      // subleading
      gj.nGenJet=0;
      for (int j=0;j<anajet->ngen;j++) {
         if (anajet->genpt[j]<cutjetPt) continue;
         if (fabs(anajet->geneta[j])>cutjetEta) continue;
         gj.inclGenJetPt[gj.nGenJet] = anajet->genpt[j];
         gj.inclGenJetEta[gj.nGenJet] = anajet->geneta[j];
         gj.inclGenJetPhi[gj.nGenJet] = anajet->genphi[j];
         if (j!=genLeadingIndex && anajet->genpt[j]>gj.genjetpt2) {
            gj.genjetpt2=anajet->genpt[j];
            gj.genjeteta2=anajet->geneta[j];
            gj.genjetphi2=anajet->genphi[j];
         }
         ++gj.nGenJet;
      }
      if (genJetMode) {
         gj.pt1 = gj.genjetpt1;
         gj.eta1 = gj.genjeteta1;
         gj.phi1 = gj.genjetphi1;
         gj.pt2 = gj.genjetpt2;
         gj.eta2 = gj.genjeteta2;
         gj.phi2 = gj.genjetphi2;
         gj.dphi   = deltaPhi(gj.phi2,gj.phi1);
      }

      ///////////////////////////////////////////////////////
      // Skim
      ///////////////////////////////////////////////////////
      if (!evt.anaEvtSel) continue;
      if (samplePtHat>0 && evt.pthat>=ptHatMax) continue;
      if (vzMax>0 && fabs(evt.vz)>vzMax) continue;
      hPtHatBeforeSel->Fill(evt.pthat);
      // ensure jet distribution unbiased by pthat turn on
      if (leadingJetPtMin>0&&gj.pt1<leadingJetPtMin) continue;
      if (subleadingJetPtMin>0&&gj.pt2<subleadingJetPtMin) continue;
      if (sigDPhi>0&&fabs(gj.dphi)<sigDPhi) continue;
      
      // Fill Baisc Event info
      hCent->Fill(evt.cBin);
      hVz->Fill(evt.vz);
      hPtHat->Fill(evt.pthat);
      hJetPt2D->Fill(gj.pt1,gj.pt2);
      hJDPhi->Fill(fabs(gj.dphi));
      hAj->Fill(gj.Aj);

      ///////////////////////////////////////////////////////
      // Tracks
      ///////////////////////////////////////////////////////
      double trkcorr[4];
      gj.nTrk=0;
      const int nTrkSet=1;
      Tracks * anaTrks[nTrkSet] = {&(c->track)};
      // Full Tracks, Pixel Tracks
      for (int iset=0; iset<nTrkSet; ++iset) {
         for (int it=0; it<anaTrks[iset]->nTrk; ++it) {
            // Kinematic Selection
            if (anaTrks[iset]->trkPt[it] < cutPtTrk) continue;
            if (fabs(anaTrks[iset]->trkEta[it]) > cutEtaTrk) continue;
            if (!anaTrks[iset]->highPurity[it]) continue;
            
            float trkPt = anaTrks[iset]->trkPt[it];
            float trkEta = anaTrks[iset]->trkEta[it];
            float trkPhi = anaTrks[iset]->trkPhi[it];
            float trkPtErrorNorm = anaTrks[iset]->trkPtError[it]/anaTrks[iset]->trkPt[it];
            float trkNHit = anaTrks[iset]->trkNHit[it];
            float trkChi2Norm = anaTrks[iset]->trkChi2[it]/anaTrks[iset]->trkNlayer[it]/anaTrks[iset]->trkNdof[it];
            float trkDzNorm = anaTrks[iset]->trkDz1[it]/anaTrks[iset]->trkDzError1[it];
            float trkDxyNorm = anaTrks[iset]->trkDxy1[it]/anaTrks[iset]->trkDxyError1[it];
            int trkAlgo = anaTrks[iset]->trkAlgo[it];
            bool trkHP  = (trkPtErrorNorm<0.06&&trkNHit>=13&&trkChi2Norm<0.15&&trkDzNorm<3&&trkDxyNorm<3);
            gj.trkPt[gj.nTrk] = trkPt;
            gj.trkEta[gj.nTrk] = trkEta;
            gj.trkPhi[gj.nTrk] = trkPhi;
            gj.trkWt[gj.nTrk] = c->getTrackCorrection(it);
            float dr1 = deltaR(trkEta,trkPhi,gj.eta1,gj.phi1);
            float dr2 = deltaR(trkEta,trkPhi,gj.eta2,gj.phi2);
            float dr1bkg = deltaR(trkEta,trkPhi,-gj.eta1,gj.phi1);
            float dr2bkg = deltaR(trkEta,trkPhi,-gj.eta2,gj.phi2);
            gj.trkAsso[gj.nTrk] = 0;
            if (dr1<trkJetAssoR) gj.trkAsso[gj.nTrk] = 1;
            else if (dr1bkg<trkJetAssoR) gj.trkAsso[gj.nTrk] = 11;
            else if (dr2<trkJetAssoR) gj.trkAsso[gj.nTrk] = 2;
            else if (dr2bkg<trkJetAssoR) gj.trkAsso[gj.nTrk] = 12;
            // Histogram
            hTrkPt->Fill(trkPt);
            hTrkCorrPt->Fill(trkPt,gj.trkWt[gj.nTrk]);
            // Select what to save
            if (saveTracks==0) continue;
            if (saveTracks==10&&gj.trkAsso[gj.nTrk]==0) continue;

            // Fill
            gj.trkNHit[gj.nTrk] = trkNHit;
            gj.trkChi2Norm[gj.nTrk] = trkChi2Norm;
            gj.trkAlgo[gj.nTrk] = trkAlgo;
            gj.trkHP[gj.nTrk] = trkHP;
            gj.trkIsFake[gj.nTrk] = anaTrks[iset]->trkFake[it];
            ++gj.nTrk;
         }
      }

      gj.nSim=0;
      for (int ip=0; ip<anaTrks[0]->nParticle; ++ip) {
         if (anaTrks[0]->pPt[ip] < cutPtTrk) continue;
         if (fabs(anaTrks[0]->pEta[ip]) > cutEtaTrk) continue;
         float simPt = anaTrks[0]->pPt[ip];
         float simEta = anaTrks[0]->pEta[ip];
         float simPhi = anaTrks[0]->pPhi[ip];
         float dr1 = deltaR(simEta,simPhi,gj.eta1,gj.phi1);
         float dr2 = deltaR(simEta,simPhi,gj.eta2,gj.phi2);
         float dr1bkg = deltaR(simEta,simPhi,-gj.eta1,gj.phi1);
         float dr2bkg = deltaR(simEta,simPhi,-gj.eta2,gj.phi2);
         gj.simAsso[gj.nSim] = 0;
         if (dr1<trkJetAssoR) gj.simAsso[gj.nSim] = 1;
         else if (dr1bkg<trkJetAssoR) gj.simAsso[gj.nSim] = 11;
         else if (dr2<trkJetAssoR) gj.simAsso[gj.nSim] = 2;
         else if (dr2bkg<trkJetAssoR) gj.simAsso[gj.nSim] = 12;
         // Select what to save
         if (saveTracks==0) continue;
         if (saveTracks==10&&gj.simAsso[gj.nSim]==0) continue;

         // Fill
         gj.simPt[gj.nSim] = simPt;
         gj.simEta[gj.nSim] = simEta;
         gj.simPhi[gj.nSim] = simPhi;
         gj.simHasRec[gj.nSim] = (anaTrks[0]->pNRec[ip]==1&&(anaTrks[0]->mtrkAlgo[ip]<4||anaTrks[0]->mtrkQual[ip]>0));
         ++gj.nSim;
      }
      
      // All done
      tgj->Fill();
   }

   output->Write();
   output->Close();
   delete c;
}

