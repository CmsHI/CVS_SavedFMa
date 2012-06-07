#include "hiForest.h"
#include "MPTUtilities.C"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include "TChain.h"
#include "analyzePhotonJet.h"

class DuplicateEvents {
public:
   DuplicateEvents(TString infname) {
      inf = TFile::Open(infname);
      t = (TTree*)inf->Get("hltanalysis/HltTree");
   };
   ~DuplicateEvents() {
      delete inf;
   }
   void MakeList() {
      cout << "Starting Making List to check for duplicate events" << endl;
      evts.clear();
      occurrence.clear();
      int run,evt;
      t->SetBranchAddress("Run",&run);
      t->SetBranchAddress("Event",&evt);
      for (int i=0;i<t->GetEntries();i++) {
         t->GetEntry(i);
         if (i%100000==0) cout <<i<<" / "<<t->GetEntries() << " run: " << run << " evt: " << evt << endl;
         int occur = (int)FindOccurrences(run,evt);
         if (occur==0) occurrence.push_back(1);
         else occurrence.push_back(2);
         evts.push_back(std::make_pair(run,evt));
      }         
   }
   int FindOccurrences(int run, int evt) {
      int noccur = count(evts.begin(), evts.end(), std::make_pair(run,evt));
      return noccur;
   }
   TFile * inf;
   TTree * t;
   vector<pair<int, int> > evts;
   vector<int> occurrence;
};

void analyzePhotonJet(
                      // mc
                      TString jetAlgo = "akPu3PF",
                      //TString inname="/net/hisrv0001/home/y_alive/scratch1/gammajet/temp/set1Merge/allQCDPhoton50_cent10_merged_highstat.root",
                      //TString outname="output-hy18qcdpho50v2_v30_xsec.root",
                      //int dataSrcType = 0, // 0 mc, 1 hi, 2 pp 2.76 TeV, 3 pp 7TeV
                      //double samplePtHat=50,
                      //double sampleWeight = 0.767, // data: 1, mc: s = 0.62, b = 0.38
                      //bool doCentReWeight=false,
                      //TString mcfname="",
                      //TString datafname="",
                      //int makeMixing=0, // 0=default (no mix), 1=make mixing classes 2=mix
                      //TString mixfname=""
                      // data
                      //TString inname="/d102/velicanu/forest/merged/HiForestPhoton_v7.root",
                      //TString inname="/mnt/hadoop/cms/store/user/frankmalocal/forest/Hi2011ForestPhoton_v7.root",
                      TString inname="/mnt/hadoop/cms/store/user/velicanu/forest/HiForestPhoton-v7-noDuplicate.root",
                      TString outname="output-data-Photon-v7-noDuplicate_v30.root",
                      //TString outname="output-data-Photon-v7_v28classes.root",
                      //TString outname="output-data-Photon-v7_v28mix.root",
                      int dataSrcType = 1, // 0 mc, 1 hi, 2 pp 2.76 TeV, 3 pp 7TeV
                      double samplePtHat=0,
                      double sampleWeight = 1, // data: 1, mc: s = 0.62, b = 0.38
                      //TString inname="/mnt/hadoop/cms/store/user/yinglu/MC_Production/Photon50/HiForest_Tree2/photon50_25k_v2.root",
                      //TString inname="/d102/velicanu/forest/merged/HiForestPhoton_v3.root",
                      //TString outname="output-data-Photon-v3_v10.root",
                      //TString outname="output-hypho50v2_2_v10.root",
                      //TString inname="/net/hidsk0001/d00/scratch/jazzitup/temp/photon50New.root",
                      //TString inname="/d101/kimy/macro/hiPhotonAna2011/rootFiles/photon50_corrCentralityv12.root",
                      //TString outname="output-hypho50v2_50kyongsun_v14_frac62.root",
                      //double sampleWeight = 0.62, // data: 1, mc: s = 0.62, b = 0.38
                      //TString inname="/d101/kimy/macro/hiPhotonAna2011/rootFiles/EM_enriched_Dijet80_60k.root",
                      //TString outname="output-hyuq80em_yongsun60k_v12_frac38.root",
                      //double sampleWeight = 0.38, // data: 1, mc: s = 0.62, b = 0.38
                      //TString inname="/d101/kimy/macro/hiPhotonAna2011/rootFiles/EM_enriched_Dijet120_14k.root",
                      //TString outname="output-hyuq120em_yongsun14k_v12_frac38.root",
                      //double sampleWeight = 1, // data: 1, mc: s = 0.62, b = 0.38
                      //TString inname="/d102/velicanu/forest/merged/HiForestPhoton_v4.root",
                      //TString outname="output-data-Photon-v4_v11.root",
                      bool doCentReWeight=false,
                      TString mcfname="",
                      TString datafname="output-data-Photon-v7_v30.root",
                      int makeMixing=0, // 0=default (no mix), 1=make mixing classes 2=mix
                      TString mixfname="output-data-Photon-v7_v30classes.root"
                      )
{
   bool checkDup=( (dataSrcType==1)&&(makeMixing==0||makeMixing==2)&&!inname.Contains("noDuplicate") );
//   bool checkDup=false;
   bool doMPT=false, saveAllCands=true;
   outname.ReplaceAll(".root",Form("_%s.root",jetAlgo.Data()));
   mcfname.ReplaceAll(".root",Form("_%s.root",jetAlgo.Data()));
   datafname.ReplaceAll(".root",Form("_%s.root",jetAlgo.Data()));
   mixfname.ReplaceAll(".root",Form("_%s.root",jetAlgo.Data()));
   double cutphotonPt = 50; // highest photon trigger is 20, also photon correction valid for photon pt > 40
   double cutjetPt = 0;
   double cutphotonEta = 1.44;
   double cutjetEta = 2;
   double cutPtTrk=4, cutEtaTrk = 2.4;
   if (saveAllCands) cutPtTrk=3;
   // Centrality reweiting
   CentralityReWeight cw(datafname,mcfname,"offlSel&&photonEt>60");

   // Check for duplicate events
   DuplicateEvents dupEvt(inname);
   if (checkDup) dupEvt.MakeList();
   
   // Define the input file and HiForest
   HiForest *c = new HiForest(inname,"forest",0,0,0,jetAlgo);
   c->doTrackCorrections = 1;
   c->InitTree();
   c->GetEnergyScaleTable("photonEnergyScaleTable_lowPt_v6.root");
   // intialize jet variables
   Jets * anajet = &(c->akPu3PF);   
   
   // pfid
   TTree * pfTree = (TTree*) c->inf->Get("pfcandAnalyzer/pfTree");
   PFs pfs;
   if (pfTree) {
      c->CheckTree(pfTree,"PfTree");
      setupPFTree(pfTree,pfs,1);
   }
      
   // Output file
   cout << "Output: " << outname << endl;
   TFile *output = new TFile(outname,"recreate");
   if (doCentReWeight&&mcfname!="") {
      cw.Init(); //cw.hCentData->Draw(); cw.hCentMc->Draw("same");
   }
   
   EvtSel evt;
   GammaJet gj;
   Isolation isol;
   TTree * tgj = new TTree("tgj","gamma jet tree");
   BookGJBranches(tgj,evt,gj,isol);

//   AnaMPT pfmpt("pf",0);
//   AnaMPT pfgenphompt("pfgenpho",0);
//   AnaMPT pf1mpt("pf1",0,1);
//   AnaMPT pf4mpt("pf4",0,4);
//   AnaMPT pf5mpt("pf5",0,5);
//   AnaMPT pfex2mpt("pfex2",2);
   AnaMPT trkmpt("trk",0);
//   AnaMPT genp0mpt("genp0",0);
   if (doMPT) {
//      pfmpt.Init(tgj);  
//      pfgenphompt.Init(tgj);  
//      pf1mpt.Init(tgj);  
//      pf4mpt.Init(tgj);  
//      pf5mpt.Init(tgj);
//      pfex2mpt.Init(tgj);  
      trkmpt.anaDiJet = false;
      trkmpt.doTrackingCorr = false;
      trkmpt.c = c;
      trkmpt.Init(tgj);
//      genp0mpt.Init(tgj);  
   }
   
   // mixing classes
   int nCentBin=40;
   //int nEPBin=11;
   int nEPBin=1;
   vector<vector<TTree*> > vtgj(nCentBin,vector<TTree*>(nEPBin));
   vector<vector<EvtSel> > vevt(nCentBin,vector<EvtSel>(nEPBin));
   vector<vector<GammaJet> > vgj(nCentBin,vector<GammaJet>(nEPBin));
   vector<vector<int> > vmixNEvt(nCentBin,vector<int>(nEPBin));
   vector<vector<int> > vmixEntry(nCentBin,vector<int>(nEPBin));
   if (makeMixing==1) {
      cout << "Mixing step 1: " << endl;
      for (int ib=0; ib<nCentBin; ++ib) {
         for (int e=0; e<nEPBin; ++e) {
            vtgj[ib][e] = new TTree(Form("tgj_%d_%d",ib,e),"gamma jet tree");
            vtgj[ib][e]->Branch("evt",&vevt[ib][e].run,vevt[ib][e].leaves);
            vtgj[ib][e]->Branch("jet",&vgj[ib][e].photonEt,vgj[ib][e].leaves);
            vtgj[ib][e]->Branch("nJet",&vgj[ib][e].nJet,"nJet/I");
            vtgj[ib][e]->Branch("inclJetPt",vgj[ib][e].inclJetPt,"inclJetPt[nJet]/F");
            vtgj[ib][e]->Branch("inclJetEta",vgj[ib][e].inclJetEta,"inclJetEta[nJet]/F");
            vtgj[ib][e]->Branch("inclJetPhi",vgj[ib][e].inclJetPhi,"inclJetPhi[nJet]/F");
         }
      }
      cout << "Branch set for mixing step 1" << endl;
   } else if (makeMixing==2) {
      cout << "Mixing step 2: " << endl;
      TFile * mixf = TFile::Open(mixfname);
      for (int ib=0; ib<nCentBin; ++ib) {
         for (int e=0; e<nEPBin; ++e) {
            vtgj[ib][e] = (TTree*)mixf->Get(Form("tgj_%d_%d",ib,e));
            vtgj[ib][e]->SetBranchAddress("evt",&vevt[ib][e].run);
            vtgj[ib][e]->SetBranchAddress("jet",&vgj[ib][e].photonEt);
            vtgj[ib][e]->SetBranchAddress("nJet",&vgj[ib][e].nJet);
            vtgj[ib][e]->SetBranchAddress("inclJetPt",vgj[ib][e].inclJetPt);
            vtgj[ib][e]->SetBranchAddress("inclJetEta",vgj[ib][e].inclJetEta);
            vtgj[ib][e]->SetBranchAddress("inclJetPhi",vgj[ib][e].inclJetPhi);
            vmixNEvt[ib][e]=vtgj[ib][e]->GetEntries();
         int offset=1;
         vmixEntry[ib][e]=offset;
         cout << " ib" << ib << ", ep" << e << ": " << vmixNEvt[ib][e] << endl;
         }
      }
   }
   
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

   // jet energy studies
   Response jetRes;
   
   ///////////////////////////////////////////////////
   // Main loop
   ///////////////////////////////////////////////////
   //for (int i=0;i<c->GetEntries();i++)
   for (int i=0;i<1000;i++)
   {
      c->GetEntry(i);
      if (pfTree) pfTree->GetEntry(i);
      // check if event is duplicate
      if (checkDup) evt.nOccur = dupEvt.occurrence[i];
      else evt.nOccur = 1;
      // Event Info
      evt.run = c->hlt.Run;
      evt.evt = c->hlt.Event;
      evt.cBin = c->evt.hiBin;
      if (dataSrcType>1) evt.cBin = 39;
      evt.evtPlane = c->evt.hiEvtPlanes[21];
      int evtPlaneBin=nEPBin-1;
//      if (evt.evtPlane>=-TMath::PiOver2()&&evt.evtPlane<=TMath::PiOver2()) {
//         float dEvtPlaneBin=TMath::Pi()/(nEPBin-1);
//         for (int e=0; e<(nEPBin-1); ++e) {
//            if (evt.evtPlane>(-TMath::PiOver2()+e*dEvtPlaneBin) && evt.evtPlane<(-TMath::PiOver2()+(e+1)*dEvtPlaneBin)){
//               evtPlaneBin=e;
//            }
//         }
//      }
      evt.nG = c->photon.nPhotons;
      evt.nJ = anajet->nref;
      evt.nT = c->track.nTrk;
      evt.trig = (c->hlt.HLT_HISinglePhoton40_v2 > 0);
      evt.offlSel = (c->skim.pcollisionEventSelection > 0);
      evt.noiseFilt = (c->skim.pHBHENoiseFilter > 0);
      if (dataSrcType>1) {
         if (dataSrcType==2) {
            evt.trig = (HLT_Photon15_CaloIdVL_v1>0);
         } else if (dataSrcType==3) {
            evt.trig = (HLT_Photon50_CaloIdVL_v3>0)||(HLT_Photon50_CaloIdVL_IsoL_v6>0);
         }
         evt.offlSel = (c->skim.phfCoincFilter && c->skim.ppurityFractionFilter);
      }
      evt.anaEvtSel = evt.trig && evt.offlSel && evt.noiseFilt && evt.nOccur==1;
      if (makeMixing==1) {
         evt.anaEvtSel = evt.offlSel;
      }
      evt.vz = c->track.vz[1];
      // Get Centrality Weight
      if (doCentReWeight) evt.weight = cw.GetWeight(evt.cBin);
      else evt.weight = 1;
      evt.npart = getNpart(evt.cBin);
      evt.ncoll = getNcoll(evt.cBin);
      evt.sampleWeight = sampleWeight/c->GetEntries(); // for different mc sample, 1 for data
      evt.pthat = anajet->pthat;
      evt.samplePtHat = samplePtHat;

      if (i%1000==0) cout <<i<<" / "<<c->GetEntries() << " run: " << evt.run << " evt: " << evt.evt << " bin: " << evt.cBin << " epbin: " << evtPlaneBin << " nT: " << evt.nT << " trig: " <<  evt.trig << " anaEvtSel: " << evt.anaEvtSel <<endl;
      if (dataSrcType==2&&!evt.trig) continue;
      if (makeMixing==1&&!evt.offlSel) continue;
//      if (dataSrcType==0&&makeMixing==2&&evt.evtPlane<-2) continue;

      // initialize
      int leadingIndex=-1;
      int awayIndex=-1;
      gj.clear();

      // Loop over jets to look for leading photon candidate in the event
      for (int j=0;j<c->photon.nPhotons;j++) {
         if (c->photon.pt[j]<cutphotonPt||c->photon.pt[j]>1000) continue;          // photon pT cut
         if (fabs(c->photon.eta[j])>cutphotonEta) continue; // |eta|<1.44
         if (c->isSpike(j)) continue;               // spike removal
         if (!c->isLoosePhoton(j)) continue;         // final cuts in final plot macro execpt photon isol and showershape cut
         if (c->photon.seedTime[j]==0) continue;
         //if (!c->isLooseEGamma(j)) continue;         // final cuts in final plot macro execpt photon isol and showershape cut, include electrons
         // sort using corrected photon pt
         float corrPt=c->getCorrEt(j);
         if (corrPt>gj.photonEt) {
            gj.photonEt = corrPt;
            leadingIndex = j;
         }
      }
      
      //
      // Skim
      //
      if (evt.nOccur!=1) continue;
      if (dataSrcType==1&&!evt.anaEvtSel) continue;
      if (dataSrcType==0&&!evt.offlSel) continue;
      if (gj.photonEt<55) continue;
      
      // If MC, Loop over gen photons to look for leading gen photon candidate in the event
      for(int j = 0; j < c->genp.nPar; ++j){
         if(c->genp.status[j]!=3||c->genp.id[j]!=22) continue; // parton photon selection
         if(c->genp.et[j] < cutphotonPt) continue;
         if(fabs(c->genp.eta[j]) > cutphotonEta) continue;
         if (c->genp.et[j] > gj.genPhoPt ) {
            gj.genPhoPt = c->genp.et[j];
            gj.genPhoEta = c->genp.eta[j];
            gj.genPhoPhi = c->genp.phi[j];
         }
      }

      // Found a leading photon which passed basic quality cut!
      if (leadingIndex!=-1) {
         // set leading photon
         gj.isEle=c->photon.isEle[leadingIndex];
         gj.photonRawEt=c->photon.pt[leadingIndex];
         gj.photonEta=c->photon.eta[leadingIndex];
         gj.photonPhi=c->photon.phi[leadingIndex];
         gj.hovere=c->photon.hadronicOverEm[leadingIndex];
         gj.sigmaIetaIeta=c->photon.sigmaIetaIeta[leadingIndex];
         gj.sumIsol=(c->photon.cr4[leadingIndex]+c->photon.cc4[leadingIndex]+c->photon.ct4PtCut20[leadingIndex]);
         isol.Set(c,leadingIndex);
         gj.refPhoPt = c->photon.genMatchedPt[leadingIndex];
         gj.refPhoEta = c->photon.genMatchedEta[leadingIndex];
         gj.refPhoPhi = c->photon.genMatchedPhi[leadingIndex];
         gj.refPhoFlavor = c->photon.genMomId[leadingIndex];
         gj.genCalIsoDR04 = c->photon.genCalIsoDR04[leadingIndex];
         
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
            gj.inclJetRefResp[gj.nJet] = jetRes.GetSmear(evt.cBin,gj.inclJetRefPt[gj.nJet]);
            if (fabs(deltaPhi(anajet->jtphi[j],c->photon.phi[leadingIndex]))>0.5) {
               if (anajet->jtpt[j]>gj.jetEt) {
                  gj.jetEt = anajet->jtpt[j];
                  awayIndex = j;
               }
            } else { // Loop over jet tree to find a photon matching jet
               if (anajet->jtpt[j]>gj.phoMatJetEt) {
                  gj.phoMatJetEt = anajet->jtpt[j];
                  gj.phoMatJetEta = anajet->jteta[j];
                  gj.phoMatJetPhi = anajet->jtphi[j];
               }
            }
            ++gj.nJet;
         }

         if (awayIndex !=-1) { // Found an away jet!
            double photonEt = c->photon.pt[leadingIndex];
            double jetEt = anajet->jtpt[awayIndex];
            double Agj = (photonEt-jetEt)/(photonEt+jetEt);
            gj.jetEt  = jetEt;
            gj.jetRawEt = anajet->rawpt[awayIndex];
            gj.jetEta = anajet->jteta[awayIndex];
            gj.jetPhi = anajet->jtphi[awayIndex];
            gj.deta = anajet->jteta[awayIndex] - c->photon.eta[leadingIndex];
            gj.dphi = deltaPhi(anajet->jtphi[awayIndex],c->photon.phi[leadingIndex]);
            gj.Aj   = Agj;
            gj.refJetEt = anajet->refpt[awayIndex];
            gj.refJetEta = anajet->refeta[awayIndex];
            gj.refJetPhi = anajet->refphi[awayIndex];
            gj.refPartonPt = anajet->refparton_pt[awayIndex];
            gj.refPartonFlavor = anajet->refparton_flavor[awayIndex];
         }

         // if mc, write genjets
         gj.nGenJet=0;
         for (int j=0;j<anajet->ngen;j++) {
            if (anajet->genpt[j]<cutjetPt) continue;
            if (fabs(anajet->geneta[j])>cutjetEta) continue;
            gj.inclGenJetPt[gj.nGenJet] = anajet->genpt[j];
            gj.inclGenJetEta[gj.nGenJet] = anajet->geneta[j];
            gj.inclGenJetPhi[gj.nGenJet] = anajet->genphi[j];
            gj.inclGenJetResp[gj.nGenJet] = jetRes.GetSmear(evt.cBin,gj.inclGenJetPt[gj.nGenJet]);
            if (anajet->genpt[j]>gj.genJetPt) {
               gj.genJetPt=anajet->genpt[j];
               gj.genJetEta=anajet->geneta[j];
               gj.genJetPhi=anajet->genphi[j];
            }
            ++gj.nGenJet;
         }

         // if mix, overwrite jets from mixed events
         if (makeMixing==2) {
            gj.nJet=0;
            int im=0;
            while (im<40) {
               int ient = (vmixEntry[evt.cBin][evtPlaneBin]) % (vmixNEvt[evt.cBin][evtPlaneBin]);
               //cout << im << " get mix entry: " << ient << endl;
               vtgj[evt.cBin][evtPlaneBin]->GetEntry(ient);
               ++vmixEntry[evt.cBin][evtPlaneBin];
               for (int j=0; j<vgj[evt.cBin][evtPlaneBin].nJet; ++j) {
                  gj.inclJetPt[gj.nJet] = vgj[evt.cBin][evtPlaneBin].inclJetPt[j];
                  gj.inclJetEta[gj.nJet] = vgj[evt.cBin][evtPlaneBin].inclJetEta[j];
                  gj.inclJetPhi[gj.nJet] = vgj[evt.cBin][evtPlaneBin].inclJetPhi[j];
                  ++gj.nJet;
               }
               ++im;
            }
         }
         
         // xcheck with tracks
         gj.nTrk=0;
         for (int it=0; it<c->track.nTrk; ++it) {
            if (c->track.trkPt[it] < cutPtTrk) continue;
            if (fabs(c->track.trkEta[it]) > cutEtaTrk) continue;
            gj.trkPt[gj.nTrk] = c->track.trkPt[it];
            gj.trkEta[gj.nTrk] = c->track.trkEta[it];
            gj.trkPhi[gj.nTrk] = c->track.trkPhi[it];
            gj.trkJetDr[gj.nTrk] = deltaR(gj.trkEta[gj.nTrk],gj.trkPhi[gj.nTrk],gj.jetEta,gj.jetPhi);
            // find leading track
            if (gj.trkPt[gj.nTrk]>gj.ltrkPt) {
               gj.ltrkPt = gj.trkPt[gj.nTrk];
               gj.ltrkEta = gj.trkEta[gj.nTrk];
               gj.ltrkPhi = gj.trkPhi[gj.nTrk];
               gj.ltrkJetDr = gj.trkJetDr[gj.nTrk];
            }
            // find leading track in jet
            if (gj.trkJetDr[gj.nTrk]<0.3 && gj.trkPt[gj.nTrk]>gj.jltrkPt) {
               gj.jltrkPt = gj.trkPt[gj.nTrk];
               gj.jltrkEta = gj.trkEta[gj.nTrk];
               gj.jltrkPhi = gj.trkPhi[gj.nTrk];
               gj.jltrkJetDr = gj.trkJetDr[gj.nTrk];
            }
            ++gj.nTrk;
         }
         
         // xcheck with pfcands
         gj.nPf=0;
         for (int it=0; it<pfs.nPFpart; ++it) {
            if (pfs.pfPt[it] < cutPtTrk) continue;
            if (fabs(pfs.pfEta[it]) > cutEtaTrk) continue;
            gj.pfPt[gj.nPf]=pfs.pfPt[it];
            gj.pfEta[gj.nPf]=pfs.pfEta[it];
            gj.pfPhi[gj.nPf]=pfs.pfPhi[it];
            gj.pfId[gj.nPf]=pfs.pfId[it];
            // find leading pfcand in jet
            float dr = deltaR(pfs.pfEta[it],pfs.pfPhi[it],gj.jetEta,gj.jetPhi);
            if (dr<0.3 && pfs.pfPt[it]>gj.jlpfPt) {
               gj.jlpfPt = pfs.pfPt[it];
               gj.jlpfEta = pfs.pfEta[it];
               gj.jlpfPhi = pfs.pfPhi[it];
               gj.jlpfId = pfs.pfId[it];
            }
            if (gj.pfId[gj.nPf]==4&&gj.pfPt[gj.nPf]>6&&fabs(gj.pfEta[gj.nPf])<1.6&&deltaPhi(gj.pfPhi[gj.nPf],gj.photonPhi)<0.4&&fabs(gj.pfEta[gj.nPf]-gj.photonEta)<0.1) {
               gj.pfPhoPt+=gj.pfPt[gj.nPf]*cos(gj.pfPhi[gj.nPf]-gj.photonPhi);
            }
            ++gj.nPf;
         }
      } // end of if leadingIndex
      
      // mixing classes
      if (makeMixing==1) {
         // mixing classes
         vevt[evt.cBin][evtPlaneBin] = evt;

         vgj[evt.cBin][evtPlaneBin].clear();
         vgj[evt.cBin][evtPlaneBin].photonEt = gj.photonEt;
         vgj[evt.cBin][evtPlaneBin].photonEta = gj.photonEta;
         vgj[evt.cBin][evtPlaneBin].photonPhi = gj.photonPhi;
         vgj[evt.cBin][evtPlaneBin].sigmaIetaIeta = gj.sigmaIetaIeta;
         vgj[evt.cBin][evtPlaneBin].sumIsol = gj.sumIsol;
         
         vgj[evt.cBin][evtPlaneBin].nJet = 0;
         for (int j=0;j<anajet->nref;j++) {
            if (anajet->jtpt[j]<cutjetPt) continue;
            if (fabs(anajet->jteta[j])>cutjetEta) continue;
            // mixing classes
            vgj[evt.cBin][evtPlaneBin].inclJetPt[vgj[evt.cBin][evtPlaneBin].nJet] = anajet->jtpt[j];
            vgj[evt.cBin][evtPlaneBin].inclJetEta[vgj[evt.cBin][evtPlaneBin].nJet] = anajet->jteta[j];
            vgj[evt.cBin][evtPlaneBin].inclJetPhi[vgj[evt.cBin][evtPlaneBin].nJet] = anajet->jtphi[j];
            ++vgj[evt.cBin][evtPlaneBin].nJet;
         }
      }
      
      // MPT
      if (doMPT) {
//         pfmpt.InputEvent(pfs.nPFpart,pfs.pfPt,pfs.pfEta,pfs.pfPhi);
//         pfmpt.AnalyzeEvent(gj.photonEt,gj.photonEta,gj.photonPhi,gj.jetEt,gj.jetEta,gj.jetPhi);
//         
//         pfgenphompt.InputEvent(pfs.nPFpart,pfs.pfPt,pfs.pfEta,pfs.pfPhi);
//         pfgenphompt.AnalyzeEvent(gj.genPhoPt,gj.genPhoEta,gj.genPhoPhi,gj.jetEt,gj.jetEta,gj.jetPhi);
//
//         pf1mpt.InputEvent(pfs.nPFpart,pfs.pfPt,pfs.pfEta,pfs.pfPhi,pfs.pfId);
//         pf1mpt.AnalyzeEvent(gj.photonEt,gj.photonEta,gj.photonPhi,gj.jetEt,gj.jetEta,gj.jetPhi);
//
//         pf4mpt.InputEvent(pfs.nPFpart,pfs.pfPt,pfs.pfEta,pfs.pfPhi,pfs.pfId);
//         pf4mpt.AnalyzeEvent(gj.photonEt,gj.photonEta,gj.photonPhi,gj.jetEt,gj.jetEta,gj.jetPhi);
//
//         pf5mpt.InputEvent(pfs.nPFpart,pfs.pfPt,pfs.pfEta,pfs.pfPhi,pfs.pfId);
//         pf5mpt.AnalyzeEvent(gj.photonEt,gj.photonEta,gj.photonPhi,gj.jetEt,gj.jetEta,gj.jetPhi);
//
//         pfex2mpt.InputEvent(pfs.nPFpart,pfs.pfPt,pfs.pfEta,pfs.pfPhi);
//         pfex2mpt.AnalyzeEvent(gj.photonEt,gj.photonEta,gj.photonPhi,gj.jetEt,gj.jetEta,gj.jetPhi);

         trkmpt.InputEvent(c->track.nTrk,c->track.trkPt,c->track.trkEta,c->track.trkPhi);
         trkmpt.AnalyzeEvent(gj.photonEt,gj.photonEta,gj.photonPhi,gj.jetEt,gj.jetEta,gj.jetPhi);
         
//         genp0mpt.InputEvent(c->genp.nPar,c->genp.et,c->genp.eta,c->genp.phi,0,c->genp.status);
//         genp0mpt.AnalyzeEvent(gj.refPhoPt,gj.photonEta,gj.photonPhi,gj.jetEt,gj.jetEta,gj.jetPhi);
      }
      
      // All done
      tgj->Fill();
      if (makeMixing==1) {
         // mixing classes
         vtgj[evt.cBin][evtPlaneBin]->Fill();
      }
   }

   output->Write();
   output->Close();
   delete c;
}

