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
#include "analyzeDiJetMPT.h"

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

void analyzeDiJetMPT(
                      TString jetAlgo = "icPu5",
                      TString inname="/mnt/hadoop/cms/store/user/velicanu/forest/HiForestDijet_v7.root",
                      TString outname="output-data-DiJet-v7_v0.root",
                      int dataSrcType = 1, // 0 mc, 1 hi, 2 pp 2.76 TeV, 3 pp 7TeV
                      double samplePtHat=0,
                      double sampleWeight = 1, // data: 1, mc: s = 0.62, b = 0.38
                      bool doCentReWeight=false,
                      TString mcfname="",
                      TString datafname="output-data-Photon-v7_v30.root",
                      int makeMixing=0, // 0=default (no mix), 1=make mixing classes 2=mix
                      TString mixfname="output-data-Photon-v7_v30classes.root"
                      )
{
   bool doSkim=true;
   //bool checkDup=( (dataSrcType==1)&&(makeMixing==0||makeMixing==2)&&!inname.Contains("noDuplicate") );
   bool checkDup=false;
   bool doMPT=true, saveAllCands=false;
   bool onlyTrkAlgo4=true, onlyTrkHP=true;
   bool saveParticles=true;
   bool doSimTrk=true;
   outname.ReplaceAll(".root",Form("_%s.root",jetAlgo.Data()));
   mcfname.ReplaceAll(".root",Form("_%s.root",jetAlgo.Data()));
   datafname.ReplaceAll(".root",Form("_%s.root",jetAlgo.Data()));
   mixfname.ReplaceAll(".root",Form("_%s.root",jetAlgo.Data()));
   double cutjetPt = 30;
   double cutjetEta = 5;
   double cutPtTrk=0.5;
   double cutEtaTrk = 8;
   double cutPtPfCand=4;
   // Centrality reweiting
   CentralityReWeight cw(datafname,mcfname,"offlSel&&pt1>120&&pt2>0&&acos(cos(phi2-phi1))>2./3*3.14159");

   // Check for duplicate events
   DuplicateEvents dupEvt(inname);
   if (checkDup) dupEvt.MakeList();
   
   // Define the input file and HiForest
   HiForest *c = new HiForest(inname,"forest",0,0,0,jetAlgo);
   c->doTrackCorrections = 1;
   c->InitTree();
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
   
   TH1D * hTrkPt = new TH1D("hTrkPt",";p_{T} (GeV/c)",nptrange,ptranges);
   TH1D * hTrkCorrPt = new TH1D("hTrkCorrPt",";p_{T} (GeV/c)",nptrange,ptranges);
   TH1D * hGenpPt = new TH1D("hGenpPt",";p_{T} (GeV/c)",nptrange,ptranges);

   EvtSel evt;
   DiJet gj;
   TTree * tgj = new TTree("tgj","dijet jet tree");
   BookGJBranches(tgj,evt,gj);
//   AnaMPT pfmpt("pf",0);
//   AnaMPT pf4mpt("pf4",0,4);
   AnaMPT trkmpt("trk",0);
   AnaMPT genpSigmpt("genpSig",0);
   AnaMPT genpAllmpt("genpAll",0);
   if (doMPT) {
//       pfmpt.Init(tgj);  
      //pf4mpt.Init(tgj);  

      trkmpt.trackingCorrectionTypes.push_back(0); trkmpt.trackingCorrectionNames.push_back("Corr");
      trkmpt.Init(tgj);
      
//       genpSigmpt.Init(tgj);
//       genpAllmpt.Init(tgj);
   }
   
   // mixing classes
   int nCentBin=40;
   //int nEPBin=11;
   int nEPBin=1;
   vector<vector<TTree*> > vtgj(nCentBin,vector<TTree*>(nEPBin));
   vector<vector<EvtSel> > vevt(nCentBin,vector<EvtSel>(nEPBin));
   vector<vector<DiJet> > vgj(nCentBin,vector<DiJet>(nEPBin));
   vector<vector<int> > vmixNEvt(nCentBin,vector<int>(nEPBin));
   vector<vector<int> > vmixEntry(nCentBin,vector<int>(nEPBin));
   if (makeMixing==1) {
      cout << "Mixing step 1: " << endl;
      for (int ib=0; ib<nCentBin; ++ib) {
         for (int e=0; e<nEPBin; ++e) {
            vtgj[ib][e] = new TTree(Form("tgj_%d_%d",ib,e),"gamma jet tree");
            vtgj[ib][e]->Branch("evt",&vevt[ib][e].run,vevt[ib][e].leaves);
            vtgj[ib][e]->Branch("jet",&vgj[ib][e].pt1,vgj[ib][e].leaves);
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
            vtgj[ib][e]->SetBranchAddress("jet",&vgj[ib][e].pt1);
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
   for (int i=0;i<c->GetEntries();i++)
//    for (int i=0;i<1000;i++)
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
      evt.nJ = anajet->nref;
      evt.nT = c->track.nTrk;
      if (c->hasHltTree) evt.trig = (c->hlt.HLT_HIJet80_v1 > 0);
      else evt.trig = true;
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
//      if (dataSrcType==0&&makeMixing==2&&evt.evtPlane<-2) continue;
      if (dataSrcType==2&&!evt.trig) continue;
      if (makeMixing==1&&!evt.offlSel) continue;
      if (doSkim) {
         if (dataSrcType==1&&!evt.anaEvtSel) continue;
//          if (dataSrcType==0&&!evt.offlSel) continue;
//          if (gj.pt1<120) continue;
         if (gj.genjetpt1<120) continue;
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
            gj.inclJetRefResp[gj.nJet] = jetRes.GetSmear(evt.cBin,gj.inclJetRefPt[gj.nJet]);
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
         
         // xcheck with pfcands
         gj.nPf=0;
         for (int it=0; it<pfs.nPFpart; ++it) {
            if (pfs.pfPt[it] < cutPtPfCand) continue;
            if (fabs(pfs.pfEta[it]) > cutEtaTrk) continue;
            gj.pfPt[gj.nPf]=pfs.pfPt[it];
            gj.pfEta[gj.nPf]=pfs.pfEta[it];
            gj.pfPhi[gj.nPf]=pfs.pfPhi[it];
            gj.pfId[gj.nPf]=pfs.pfId[it];
            // find leading pfcand in jet
            float dr = deltaR(pfs.pfEta[it],pfs.pfPhi[it],gj.eta2,gj.phi2);
            if (dr<0.3 && pfs.pfPt[it]>gj.jlpfPt) {
               gj.jlpfPt = pfs.pfPt[it];
               gj.jlpfEta = pfs.pfEta[it];
               gj.jlpfPhi = pfs.pfPhi[it];
               gj.jlpfId = pfs.pfId[it];
            }
            ++gj.nPf;
         }
         
      } // end of if leadingIndex
      
      // mixing classes
      if (makeMixing==1) {
         // mixing classes
         vevt[evt.cBin][evtPlaneBin] = evt;

         vgj[evt.cBin][evtPlaneBin].clear();
         vgj[evt.cBin][evtPlaneBin].pt1 = gj.pt1;
         vgj[evt.cBin][evtPlaneBin].eta1 = gj.eta1;
         vgj[evt.cBin][evtPlaneBin].phi1 = gj.phi1;
         
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
      
      ////////////////////////////////////////
      // If mc, write genjets
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
            float dr1 = deltaR(trkEta,trkPhi,gj.eta1,gj.phi1);
            float dr2 = deltaR(trkEta,trkPhi,gj.eta2,gj.phi2);
            //gj.trkJetDr[gj.nTrk] = dr2;
            ////////////////////////////////////
            // Track efficiency/fake correction
            ////////////////////////////////////
            for (int icorr=0; icorr<3; ++icorr) {
               if (gj.pt1>40&&dr1<0.8) {
                  gj.vtrkWt[gj.nTrk][icorr] = c->trackCorrections[icorr]->GetCorr(trkPt,trkEta,gj.pt1,c->evt.hiBin,trkcorr);
   //                cout << "trk pt,eta,jet,cBin: " << trkPt << "," << trkEta << "," << gj.pt1 << "," << c->evt.hiBin << " eff: " << trkcorr[0] << endl;
               } else if (gj.pt2>40&&dr2<0.8) {
                  gj.vtrkWt[gj.nTrk][icorr] = c->trackCorrections[icorr]->GetCorr(trkPt,trkEta,gj.pt2,c->evt.hiBin,trkcorr);
   //                cout << "trk pt,eta,jet,cBin: " << trkPt << "," << trkEta << "," << gj.pt2 << "," << c->evt.hiBin << " eff: " << trkcorr[0] << endl;
               } else {
                  gj.vtrkWt[gj.nTrk][icorr] = c->trackCorrections[icorr]->GetCorr(trkPt,trkEta,0,c->evt.hiBin,trkcorr);
   //                cout << "trk pt,eta,jet,cBin: " << trkPt << "," << trkEta << "," << 0 << "," << c->evt.hiBin << " eff: " << trkcorr[0] << endl;
               }
               gj.vtrkEff[gj.nTrk][icorr] = trkcorr[0];
               gj.vtrkFak[gj.nTrk][icorr] = trkcorr[1];
            }
            // Fill
            gj.trkWt[gj.nTrk] = gj.vtrkWt[gj.nTrk][0];
            gj.trkEff[gj.nTrk] = gj.vtrkEff[gj.nTrk][0];
            gj.trkFak[gj.nTrk] = gj.vtrkFak[gj.nTrk][0];
            gj.trkNHit[gj.nTrk] = trkNHit;
            gj.trkChi2Norm[gj.nTrk] = trkChi2Norm;
            gj.trkAlgo[gj.nTrk] = trkAlgo;
            gj.trkHP[gj.nTrk] = trkHP;
            gj.vtrkQual[gj.nTrk][0] = anaTrks[iset]->highPurity[it];
            gj.vtrkQual[gj.nTrk][1] = anaTrks[iset]->highPurity[it]&&trkAlgo==4;
            gj.vtrkQual[gj.nTrk][2] = anaTrks[iset]->highPuritySetWithPV[it];
            gj.trkIsFake[gj.nTrk] = anaTrks[iset]->trkFake[it];
            // mpt trk selection
            gj.trkSel[gj.nTrk] = false;
            if (trkNHit<=7||gj.vtrkQual[gj.nTrk][0]) {
               gj.trkSel[gj.nTrk] = true;
               hTrkPt->Fill(trkPt);
               hTrkCorrPt->Fill(trkPt,gj.trkWt[gj.nTrk]);
               // find leading track
               if (trkPt>gj.ltrkPt) {
                  gj.ltrkPt = trkPt;
                  gj.ltrkEta = trkEta;
                  gj.ltrkPhi = trkPhi;
                  gj.ltrkJetDr = dr2;
               }
               // find leading track in jet
               if (dr2<0.3 && trkPt>gj.jltrkPt) {
                  gj.jltrkPt = trkPt;
                  gj.jltrkEta = trkEta;
                  gj.jltrkPhi = trkPhi;
                  gj.jltrkJetDr = dr2;
               }
            }
            ++gj.nTrk;
         }
      }

      ////////////////////////      
      // Gen Particles
      ////////////////////////
      gj.nGenp =0;
      for (int ip=0; ip<c->genparticle.mult; ++ip) {
         if (c->genparticle.pt[ip] < cutPtTrk) continue;
         if (fabs(c->genparticle.eta[ip]) > cutEtaTrk) continue;
         if (abs(int(c->genparticle.chg[ip])) ==0 ) continue;
         // Fill
         hGenpPt->Fill(c->genparticle.pt[ip]);
         gj.genpPt[gj.nGenp] = c->genparticle.pt[ip];
         gj.genpEta[gj.nGenp] = c->genparticle.eta[ip];
         gj.genpPhi[gj.nGenp] = c->genparticle.phi[ip];
         gj.genpCh[gj.nGenp] = c->genparticle.chg[ip];
         gj.genpSube[gj.nGenp] = c->genparticle.sube[ip];
         gj.genpSel[gj.nGenp] = (int(c->genparticle.sube[ip]) ==0);
         ++gj.nGenp;
      }
      
      if (doSimTrk) {
         gj.nSim=0;
         for (int ip=0; ip<anaTrks[0]->nParticle; ++ip) {
            if (anaTrks[0]->pPt[ip] < cutPtTrk) continue;
            if (fabs(anaTrks[0]->pEta[ip]) > cutEtaTrk) continue;
            // Fill
            gj.simPt[gj.nSim] = anaTrks[0]->pPt[ip];
            gj.simEta[gj.nSim] = anaTrks[0]->pEta[ip];
            gj.simPhi[gj.nSim] = anaTrks[0]->pPhi[ip];
            gj.simHasRec[gj.nSim] = (anaTrks[0]->pNRec[ip]==1&&(anaTrks[0]->mtrkAlgo[ip]<4||anaTrks[0]->mtrkQual[ip]>0));
            ++gj.nSim;
         }
      }

      // MPT
      if (doMPT) {
         trkmpt.InputEvent(gj.nTrk,gj.trkPt,gj.trkEta,gj.trkPhi,gj.trkWt,gj.trkSel);
         trkmpt.AnalyzeEvent(gj.pt1,gj.eta1,gj.phi1,gj.pt2,gj.eta2,gj.phi2);
         
//          genpSigmpt.InputEvent(gj.nGenp,gj.genpPt,gj.genpEta,gj.genpPhi,0,gj.genpSel);
//          genpSigmpt.AnalyzeEvent(gj.pt1,gj.eta1,gj.phi1,gj.pt2,gj.eta2,gj.phi2);
//          genpAllmpt.InputEvent(gj.nGenp,gj.genpPt,gj.genpEta,gj.genpPhi);
//          genpAllmpt.AnalyzeEvent(gj.pt1,gj.eta1,gj.phi1,gj.pt2,gj.eta2,gj.phi2);
        }
      
      // All done
      if (!saveParticles) gj.clearParticles();
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

