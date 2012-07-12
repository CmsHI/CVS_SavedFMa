#include "hiForest.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include "TChain.h"
#include "analyzeTrackingCorrection.h"

void analyzeTrackCorrection(
   TString jetAlgo = "icPu5",
   TString inname="/mnt/hadoop/cms/store/user/velicanu/forest/HiForestDijet_v7.root",
   TString outname="output-data-DiJet-v7_v0.root",
   double samplePtHat=0,
   double sampleWeight = 1, // data: 1, mc: s = 0.62, b = 0.38
   bool doCentReWeight=false,
   TString mcfname="",
   TString datafname="output-data-Photon-v7_v30.root"
)
{
   double cutjetPt = 30;
   double cutjetEta = 2;
   double cutPtTrk=0.5;
   double cutEtaTrk = 8;
   double cutPtPfCand=4;
   
   // Define the input file and HiForest
   HiForest *c = new HiForest(inname,"forest",0,0,0,jetAlgo);

   // intialize jet variables
   Jets * anajet = &(c->akPu3PF);   
   
   // Output file
   cout << "Output: " << outname << endl;
   TFile *output = new TFile(outname,"recreate");

   EvtSel evt;
   DiJet gj;

   ///////////////////////////////////////////////////
   // Track Correction Schemes
   ///////////////////////////////////////////////////
   TrkCorrHisAna effMergedGeneral("Forest2_MergedGeneral");
   anaA0.outFile_ = output;
   
   ///////////////////////////////////////////////////
   // Main loop
   ///////////////////////////////////////////////////
   for (int i=0;i<c->GetEntries();i++)
   {
      c->GetEntry(i);
      evt.cBin = c->evt.hiBin;
      if (dataSrcType>1) evt.cBin = 39;
      evt.evtPlane = c->evt.hiEvtPlanes[21];
      evt.nJ = anajet->nref;
      evt.nT = c->track.nTrk;
      evt.offlSel = (c->skim.pcollisionEventSelection > 0);
      if (!c->hasSkimTree) evt.offlSel = (c->evt.hiNtracks>0 && c->evt.hiHFplus>=4 && c->evt.hiHFminus>=4);
      evt.vz = c->track.vz[1];
      evt.sampleWeight = sampleWeight/c->GetEntries(); // for different mc sample, 1 for data
      evt.pthat = anajet->pthat;
      evt.samplePtHat = samplePtHat;

      if (i%1000==0) cout <<i<<" / "<<c->GetEntries() << " run: " << evt.run << " evt: " << evt.evt << " bin: " << evt.cBin << " epbin: " << evtPlaneBin << " nT: " << evt.nT << " trig: " <<  evt.trig << " anaEvtSel: " << evt.anaEvtSel <<endl;

      // initialize
      int leadingIndex=-1,genLeadingIndex=-1;
      int awayIndex=-1,genAwayIndex=-1;
      gj.clear();
      
      // Loop over jets to look for leading jet candidate in the event
      for (int j=0;j<anajet->nref;j++) {
         if (anajet->jtpt[j]<cutjetPt) continue;
         if (fabs(anajet->jteta[j])>cutjetEta) continue;
         if (anajet->jtpt[j] > gj.pt1) {
            gj.pt1 = anajet->jtpt[j];
            leadingIndex = j;
         }
      }

      // Loop over genjets to look for leading genjet candidate in the event
      for (int j=0;j<anajet->ngen;j++) {
         if (anajet->genpt[j]<cutjetPt) continue;
         if (fabs(anajet->geneta[j])>cutjetEta) continue;
         if (anajet->genpt[j] > gj.genjetpt1) {
            gj.genjetpt1=anajet->genpt[j];
            genLeadingIndex=j;
         }
      }
      
      //
      // Skim
      //
      if (!evt.offlSel) continue;
      
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
      // Genjets
      ////////////////////////////////////////
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
         gj.inclGenJetResp[gj.nGenJet] = jetRes.GetSmear(evt.cBin,gj.inclGenJetPt[gj.nGenJet]);
         if (j!=genLeadingIndex && anajet->genpt[j]>gj.genjetpt2) {
            gj.genjetpt2=anajet->genpt[j];
            gj.genjeteta2=anajet->geneta[j];
            gj.genjetphi2=anajet->genphi[j];
         }
         ++gj.nGenJet;
      }

      ///////////////////////////////////////////////////////
      // Tracks
      ///////////////////////////////////////////////////////
      // Full Tracks, Pixel Tracks
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
         r.nim = !anaTrks[0]->trkFake[it];
         r.jet = gj.pt1;
         r.jeta = gj.eta1;
         r.jdr1 = deltaR(r.etar,r.phir,gj.eta1,gj.phi1);
         r.jdr2 = deltaR(r.etar,r.phir,gj.eta2,gj.phi2);
         // Fill
         effMergedGeneral.FillRecHistograms(evt,gj,r);
         
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
         s.nrec = (anaTrks[0]->mtrkQual[ip]>0&&anaTrks[0]->pNRec[ip]<1);
         s.acc = anaTrks[0]->pAcc[ip];
         s.jet = gj.pt1;
         s.jeta = gj.eta1;
         s.jdr1 = deltaR(s.etas,s.phis,gj.eta1,gj.phi1);
         s.jdr2 = deltaR(s.etas,s.phis,gj.eta2,gj.phi2);
         // Fill
         effMergedGeneral.FillSimHistograms(evt,gj,s);
      }
      // All done
   }

   output->Write();
   output->Close();
   delete c;
}

