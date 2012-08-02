#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <TMath.h>
using namespace std;

#define PI 3.141592653589

int getNcoll(int cBin=0);

void addOrder(std::string inputFile_, std::string outputFile = "outputOrderAdded.root", float etaCut=1.444, float ptCut = 25, bool is2010Data=false,  bool doTrack = true)
{
   
   bool doTrigCut(false);
   std::string triggerName = "";
   
   const int nMaxPho = 3000;
   
   TFile *inf = new TFile(inputFile_.data());
   
   
  TTree *fChainPhoton;
  
  if ( !is2010Data ) 
     fChainPhoton = (TTree*)inf->Get("multiPhotonAnalyzer/photon");
  else 
     fChainPhoton = (TTree*)inf->Get("NTuples/Analysis");
  
  fChainPhoton->SetName("photon11lhf");


  TTree *fChainHLT    = (TTree*)inf->Get("hltanalysis/HltTree");
  fChainHLT->SetName("fChainHLT341");
  

  TTree *fChainSkim   = (TTree*)inf->Get("skimanalysis/HltTree");
  fChainSkim->SetName("fChainColl611");
  
  TTree *fChainPfjet   = (TTree*)inf->Get("akPu3PFJetAnalyzer/t");
  fChainPfjet->SetName("fChainPfjet511");
  
  TTree *fChainTrack;  
  if ( doTrack) {
     fChainTrack =  (TTree*)inf->Get("anaTrack/trackTree");
     fChainTrack->SetName("fChainTraack233");
  }
  TTree *fChainEvt=0;
  
  if ( !is2010Data ) {
     fChainEvt = (TTree*)inf->Get("hiEvtAnalyzer/HiTree");
       fChainEvt->SetName("fChainEvt234");
  }
  
  TTree *fChainGen = (TTree*)inf->Get("genpana/photon");
  bool isGen(true);
  if ( fChainGen ==0 ) isGen = false;
  
   // now open new root file
   TFile* newfile_data = new TFile(outputFile.data(),"recreate");
   cout << "Output file " << outputFile << endl;
   
   // clone tree

   TTree* newtree = fChainPhoton->CloneTree(0);
   newtree->SetName("yongsunPhotonTree");
   TTree* newtreehlt = fChainHLT->CloneTree(0);
   newtreehlt->SetName("yongsunHltTree");
   TTree* newtreeSkim = fChainSkim->CloneTree(0);
   newtreeSkim->SetName("yongsunSkimTree");
   TTree* newtreePfjet = fChainPfjet->CloneTree(0);
   newtreePfjet->SetName("yongsunPfjetTree");
   TTree* newtreeTrack;
   if ( doTrack){ 
      newtreeTrack = fChainTrack->CloneTree(0);
      newtreeTrack->SetName("yongsunTrackTree");
   }
   TTree* newtreeEvt;
   if ( !is2010Data) {
      newtreeEvt = fChainEvt->CloneTree(0);
      newtreeEvt->SetName("yongsunHiEvt");
   }
   TTree* newtreeGen;
   if ( isGen) {
     newtreeGen = fChainGen->CloneTree(0);
     newtreeGen->SetName("yongsunGen");
   }

   newtree->SetMaxTreeSize(4000000000);
   newtreehlt->SetMaxTreeSize(4000000000);
   newtreeSkim->SetMaxTreeSize(4000000000);
   newtreePfjet->SetMaxTreeSize(4000000000);
   if ( doTrack) newtreeTrack->SetMaxTreeSize(4000000000);
   if ( !is2010Data) {
     newtreeEvt->SetMaxTreeSize(4000000000);
   }
   if ( isGen) 
     newtreeGen->SetMaxTreeSize(4000000000);
   
   Long64_t nentries = fChainPhoton->GetEntries();
   cout << "nentries = " << nentries << endl;
   
   TBranch* b_theTrig;
   Int_t   theTrig;
   if (doTrigCut)  fChainHLT->SetBranchAddress(triggerName.data(), &theTrig, &b_theTrig);
   
   Float_t et[nMaxPho];
   Float_t eta[nMaxPho];
   Float_t phi[nMaxPho];
   Float_t hadronicOverEm[nMaxPho];
   Float_t sigmaIetaIeta[nMaxPho];
   Float_t cc4j[nMaxPho];
   Float_t cr4j[nMaxPho];
   Float_t ct4j20[nMaxPho];
   Int_t           hiBin;
   
   Float_t leadingPt, leadingPhi, leadingEta, lcc4j, lcr4j, lct4j20, lsee, lhoe ;
   
   TBranch *b_et;
   TBranch *b_eta;
   TBranch *b_phi;
   TBranch        *b_hiBin;   //!                                                                                                                                                                                  
   
   int order[nMaxPho];
   float newPt[nMaxPho];
   float locNtrk[nMaxPho];
   
   Int_t cBin;
   Int_t nPhotons;
   TBranch *b_nPhotons;

   int ncoll;
   
   Int_t           nTrk;
   Float_t         trkPt [2000];   //[nTrk]                                                                                                                                                                  
   Float_t         trkEta[2000];   //[nTrk]                                                                                                                                      
   Float_t         trkPhi[2000];   //[nTrk]                                                                                                                                   
   fChainPhoton->SetBranchAddress("pt",et,&b_et);
   fChainPhoton->SetBranchAddress("eta",eta,&b_eta);
   fChainPhoton->SetBranchAddress("phi",phi,&b_phi);
   fChainPhoton->SetBranchAddress("sigmaIetaIeta",sigmaIetaIeta);
   fChainPhoton->SetBranchAddress("cc4j",cc4j);
   fChainPhoton->SetBranchAddress("cr4j",cr4j);
   fChainPhoton->SetBranchAddress("ct4j20",ct4j20);
   fChainPhoton->SetBranchAddress("hadronicOverEm",hadronicOverEm);
   fChainPhoton->SetBranchAddress("nPhotons", &nPhotons, &b_nPhotons);

   if ( doTrack ) {
      fChainTrack->SetBranchAddress("nTrk", &nTrk);
      fChainTrack->SetBranchAddress("trkPt", trkPt);
      fChainTrack->SetBranchAddress("trkEta", trkEta);
      fChainTrack->SetBranchAddress("trkPhi", trkPhi);
      newtree->Branch("locNtrk", &locNtrk,"locNtrk[nPhotons]/F");
   }

   if ( !is2010Data)
      fChainEvt->SetBranchAddress("hiBin", &hiBin, &b_hiBin);
   else
      fChainPhoton->SetBranchAddress("cBin", &hiBin, &b_hiBin);
   
   newtree->Branch("ncoll", &ncoll,"ncoll/I");
   newtree->Branch("order",order,"order[nPhotons]/I");
   newtree->Branch("lPt", &leadingPt,"lpt/F");
   newtree->Branch("lEta", &leadingEta,"leta/F");
   newtree->Branch("lPhi", &leadingPhi,"lphi/F");
   newtree->Branch("lcc4j", &lcc4j,"lcc4j/F");
   newtree->Branch("lcr4j", &lcr4j,"lcr4j/F");
   newtree->Branch("lct4j20", &lct4j20,"lct4j20/F");
   newtree->Branch("lsee", &lsee,"lsee/F");
   newtree->Branch("lhoe", &lhoe,"lhoe/F");
  
   
   
   
   cout << " going into loop" << endl;

   TH1D* hdr = new TH1D("hdr",";dr;Entries",100,-10,10);
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      if (jentry% 10000 == 0) cout <<jentry<<" / "<<nentries<<" "<<setprecision(2)<<(double)jentry/nentries*100<<endl;
      fChainPhoton->GetEntry(jentry);
      fChainHLT->GetEntry(jentry);
      fChainSkim->GetEntry(jentry);
      fChainPfjet->GetEntry(jentry);
      if( doTrack) 
	 fChainTrack->GetEntry(jentry);
      if ( !is2010Data)    
	 fChainEvt->GetEntry(jentry);
      if ( isGen)
	fChainGen->GetEntry(jentry);
      
      if ( (!theTrig) && doTrigCut)   
	continue;    
      
      for (int j=0;j<nPhotons;j++) {
	 order[j] = -1;
	 if ( fabs(eta[j]) < etaCut ) 
	    newPt[j] = et[j]; 
	 else
	    newPt[j] = -et[j] - 1000;
	 
	 locNtrk[j] = -1000;
	 if( doTrack) {
	    if ( (et[j] > 40) && fabs(eta[j])<1.5) {
	       locNtrk[j]=0;
	       for (int jt=0;jt<nTrk;jt++) {
		  //  if ( trkPt[jt] < 2 ) 
		  //	  continue;
		  float dPhi = phi[j] - trkPhi[jt];
		  if (dPhi > PI)  
		     dPhi = dPhi - 2*PI;
		  if (dPhi < -PI) 
		     dPhi = dPhi + 2*PI;
		  float dEta = eta[j] - trkEta[jt];
		  float dR = sqrt( dPhi*dPhi + dEta*dEta);
		  //	       hdr->Fill(dPhi);
		  if ( dR < 0.5 ) 
		     locNtrk[j] = locNtrk[j] + 1;
	       }
	    }
	 }
      }
      
      
      TMath::Sort(nPhotons, newPt, order);
      
      leadingPt = -100;
      leadingEta = -100;
      leadingPhi = -100;
      lcc4j = -100; lcr4j= -100;  lct4j20= -100;  lsee= -100;  lhoe= -100; 
      
      for (int j=0;j<nPhotons;j++) {
	 if ( fabs(eta[j]) > etaCut ) 
	    continue;
         if ( et[j] > leadingPt ) {
	    leadingPt = et[j];
	    leadingEta = eta[j];
	    leadingPhi = phi[j];
	    lcc4j   = cc4j[j];
	    lcr4j   = cr4j[j];
            lct4j20 = ct4j20[j];
	    lhoe    = hadronicOverEm[j];
	    lsee    = sigmaIetaIeta[j];
	 }
      }
      
      if ( leadingPt < ptCut )   continue;
      
      // nColl
      ncoll = getNcoll(hiBin);
       
      newtree->Fill();
      newtreehlt->Fill();
      newtreeSkim->Fill();
      newtreePfjet->Fill();
      if( doTrack)
	 newtreeTrack->Fill();
      if ( !is2010Data)
	 newtreeEvt->Fill();
      if ( isGen )
	newtreeGen->Fill();
   }
   
   
   
   
   //  newtree2->Print();
   newtree->AutoSave();
   newtreehlt->AutoSave();
   newtreeSkim->AutoSave();
   newtreePfjet->AutoSave();
   if( doTrack)
      newtreeTrack->AutoSave();
   if ( !is2010Data)
      newtreeEvt->AutoSave();
   if ( isGen ) 
     newtreeGen->AutoSave(0);
   //   delete newfile_data;
   //hdr->Draw();
   cout << " Done! "<< endl;
   inf->Close();
}




int getNcoll(int cBin) { 
   if (cBin == 0) return  1747.86 ;
   if (cBin == 1) return  1567.53 ;
   if (cBin == 2) return  1388.39 ;
   if (cBin == 3) return  1231.77 ;
   if (cBin == 4) return  1098.2 ;
   if (cBin == 5) return  980.439 ;
   if (cBin == 6) return  861.609 ;
   if (cBin == 7) return  766.042 ;
   if (cBin == 8) return  676.515 ;
   if (cBin == 9) return  593.473 ;
   if (cBin == 10) return  521.912 ;
   if (cBin == 11) return  456.542 ;
   if (cBin == 12) return  398.546 ;
   if (cBin == 13) return  346.647 ;
   if (cBin == 14) return  299.305 ;
   if (cBin == 15) return  258.344 ;
   if (cBin == 16) return  221.216 ;
   if (cBin == 17) return  188.677 ;
   if (cBin == 18) return  158.986 ;
   if (cBin == 19) return  134.7 ;
   if (cBin == 20) return  112.547 ;
   if (cBin == 21) return  93.4537 ;
   if (cBin == 22) return  77.9314 ;
   if (cBin == 23) return  63.5031 ;
   if (cBin == 24) return  52.0469 ;
   if (cBin == 25) return  42.3542 ;
   if (cBin == 26) return  33.9204 ;
   if (cBin == 27) return  27.3163 ;
   if (cBin == 28) return  21.8028 ;
   if (cBin == 29) return  17.2037 ;
   if (cBin == 30) return  13.5881 ;
   if (cBin == 31) return  10.6538 ;
   if (cBin == 32) return  8.35553 ;
   if (cBin == 33) return  6.40891 ;
   if (cBin == 34) return  5.13343 ;
   if (cBin == 35) return  3.73215 ;
   if (cBin == 36) return  3.06627 ;
   if (cBin == 37) return  2.41926 ;
   if (cBin == 38) return  2.11898 ;
   if (cBin == 39) return  1.76953 ;
   return -100000;
}
