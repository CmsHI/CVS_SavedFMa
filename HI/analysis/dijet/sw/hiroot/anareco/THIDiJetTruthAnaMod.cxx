//
// $Id: THIDiJetTruthAnaMod.cxx 
//

#include "THIDiJetTruthAnaMod.h"

#include <Riostream.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TNtuple.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TVector3.h>
#include <TLorentzVector.h>


#include "THIEvent.h"
#include "THIParticle.h"
#include "THIGenPythiaRecord.h"
#include "THIVector32.h"
#include "THIVectorT.h"
#include "THIPhotonMCType.h"
#include "THIMCGammaJetSignalDef.h"
#include "THIMatchedParticles.h"
//#include "THILorentzVector.h"
//#include "THIJet.h"


//////////////////////////////////////////////////////////////////////////
//
// THIDiJetTruthAnaMod
//
// This is the TAM module for finding a dijet and calculating the 
// variables needed for the fragmentation functions and etc.
//
// Authors: C.Roland
//
//
//////////////////////////////////////////////////////////////////////////

ClassImp(THIDiJetTruthAnaMod)

//________________________________________________________________________
THIDiJetTruthAnaMod::THIDiJetTruthAnaMod(const char *name, const char *title) :
   THIAnaBaseMod(name, title),
   fEvent(0), 
   fParticles(0), 
   fVertices(0),
   fGenRecords(0),
   fSigParticles(0),
   fNTECStat(0),
   fNTTruePFF(0),
   fNTTruePFFBG(0),
   fIsPromPhoton(kFALSE),
   fIsIsolated(kFALSE),
   fAwayParton(0),
   fNearParton(0),
   fTrackingEfficiency(0),
   fStatOnly(kFALSE),
   //jet related
   fJetAArrayName(THIJetArray::kCaloJetsName),
   fLoad(kFALSE),
   fJetAArray(0),
   fNearLeadingJet(0),
   fAwayLeadingJet(0),
   //initialize matching radius
   fDeltaRMatch(TMath::Pi()/4)
{
   // Default and user constructor.
   fReso = new TF1("fReso","sqrt(pow(0.017,2)+pow(4.6/x,2)+pow(1.3/sqrt(x),2)+pow(15/x,2))",0,400);
   fGaus = new TF1("fGaus","gaus",-1,3);
   fGaus->SetParameter(0,1);
   fGaus->SetParameter(1,1);
}


//________________________________________________________________________
const THIParticle *THIDiJetTruthAnaMod::GetMother(const THIParticle *p)
{
   //Find mother parton

   if(p->GetStatus()==3){
      return p;//p=21
   }else{
      const THIParticle *mp =  p->GetMother();
      return GetMother(mp);
   }
   return p;
}


void THIDiJetTruthAnaMod::PrintJetCol(const THIJetCollection * jetcol)
{
   printf("Print Jet col: Et  Eta Phi\n");
   for (UInt_t i=0; i<jetcol->GetEntries(); ++i) {
      // get the ith jet
      const THIJet * ijet = jetcol->At(i);
      // Print jet info
      printf("  cand jet: %2u: %6.2f %6.2f %6.2f\n", i, ijet->GetEt(), ijet->GetEta(), ijet->GetPhi());
   }
}

//________________________________________________________________________
const THIParticle *THIDiJetTruthAnaMod::GetTriggerGenParticle()
{
   //Find the highest ET candidate partcle in the vicinity of the ecal cluster

   THIMCGenRecord *gen = dynamic_cast<THIMCGenRecord*>(fGenRecords->At(0));
   fSigParticles = gen->GetParticles();
   UInt_t nump = fSigParticles->GetEntries();

   Double_t etCand = 0;
   Int_t    iCand = -1;
   
   for(UInt_t i=0; i<nump; i++){
      
      const THIParticle *genPart = fSigParticles->At(i);
      if(genPart->GetHepStatus()!=3) continue;
//      if(genPart->GetId()==2212)
//	 printf("beam rap: %f\n",genPart->GetMom().Rapidity());
      // if(genPart->GetPt()<50) continue;
      if(genPart->GetEt()>etCand){
	 etCand = genPart->GetEt();
	 iCand = (Int_t) i;
      }
   }
   
   if(iCand<0){
      printf("No trigger matching particle found");
      return NULL;
   }
//   printf("trig @ %d\n",iCand);   
   return fSigParticles->At(iCand);
}

//________________________________________________________________________
const THIParticle *THIDiJetTruthAnaMod::GetLeading(const THIParticle *trigPart)
{
   //Find the highest ET candidate partcle in the vicinity of a parton

   THIMCGenRecord *gen = dynamic_cast<THIMCGenRecord*>(fGenRecords->At(0));
   fSigParticles = gen->GetParticles();
   UInt_t nump = fSigParticles->GetEntries();

   Double_t etCand = 0;
   Int_t    iCand = -1;
   Double_t idphi;
   for(UInt_t i=0; i<nump; i++){
      
      const THIParticle *genPart = fSigParticles->At(i);
      if(genPart->GetHepStatus()!=1) continue;
      //calcdr
      Double_t dphi = TMath::Abs(genPart->GetMom().DeltaPhi(trigPart->GetMom()));
      if (dphi>(TMath::Pi()/4)) continue;
      if(genPart->GetEt()>etCand){
	 etCand = genPart->GetEt();
	 iCand = (Int_t) i;
	 idphi = dphi;
      }
   }
   
   if(iCand<0){
      printf("No leading particle found");
      return NULL;
   }
   const THIParticle *genPart = fSigParticles->At(iCand);
//   printf("L#%d phi %g tpet: %f tpid: %d id: %d stat %d (%d)et: %g eta %g p(%g|%g|%g) phi %g dphi %g\n",
//	  iCand,
//	  trigPart->GetPhi(),
//	  trigPart->GetEt(),
//	  trigPart->GetId(),
//	  genPart->GetId(),
//	  genPart->GetStatus(),
//	  genPart->GetHepStatus(),
//	  genPart->GetEt(),
//	  genPart->GetEta(),
//	  genPart->GetMom().X(),
//	  genPart->GetMom().Y(),
//	  genPart->GetMom().Z(),
//	  genPart->GetPhi(),
//	  idphi
//	  );
   return fSigParticles->At(iCand);
}

//________________________________________________________________________
void THIDiJetTruthAnaMod::GetLeadJets(const THIJetCollection *jl)
{
   for (int i=0; i<jl->GetEntries(); ++i) {
      float et = jl->At(i)->GetEt();
      printf("jet read out: et: %f\n",et);
   }

   // Search for dijets and if found fill histograms with their properties.

   Double_t phicut = 2.8;  // phi cut for dijets
   Double_t etcut  = 50.;  // et cut for dijets

   //find max Et:
   Double_t maxe = -1.;
   Int_t maxi = -1;
   const UInt_t num = fJetArray->GetEntries();
   for(UInt_t i=0; i<num; i++)
   {
      const THIJet *kj = fJetArray->At(i);
      if(maxe < kj->GetEt()) {maxe = kj->GetEt(); maxi=i;}
   }
   Double_t phi1 = fJetArray->At(maxi)->GetPhi(), phi2, dphi;

   // search for dijet pair on opposite side in Phi
   Double_t maxed = -1., maxdphi=0.;
   Int_t maxid = -1;
   for(UInt_t i=0; i<num; i++)
   {
      const THIJet *kj=fJetArray->At(i);
      phi2=kj->GetPhi();
      dphi = phi1 - phi2;
      while (dphi>=TMath::Pi())dphi-=2.*TMath::Pi();
      while (dphi<-TMath::Pi())dphi+=2.*TMath::Pi();
      dphi=TMath::Abs(dphi);
      // this is a dijet candidate. Choose the max. Et of them.
      if(dphi>phicut && maxed < kj->GetEt()) 
      {
	 maxed=kj->GetEt();
	 maxid=i;
	 maxdphi=dphi;
      }
      HDB(kAnalysis, 4) {
	 if(dphi>phicut)
	    Info("Dijets",
		  "DIJET CANDIDATE: %f %f %f\n",
		  phi1,phi2,dphi);
      }
   }
   if(maxid<0) return;  // no dijet pair found

   // dijet is the two jets with index "maxi" and "maxid".
   HDB(kAnalysis, 4)
      Info("Dijets", "FINAL DIJET: %f %f %f %f\n",
	    fJetArray->At(maxi)->GetPhi(),fJetArray->At(maxi)->GetEt(),
	    fJetArray->At(maxid)->GetPhi(),fJetArray->At(maxid)->GetEt());

   if(fJetArray->At(maxi)->GetEt()<etcut) 
      return;  // do not fill jets below a cut.

   HDB(kAnalysis, 4) 
      Info("Dijets", "Passes the energy cut. \n"); 

   //--- set leading jets ---
   fNearLeadingJet = fJetArray->At(maxi);
   fAwayLeadingJet = fJetArray->At(maxid);
   printf("base for FF: max et's: %f %f\n",fNearLeadingJet->GetEt(),fAwayLeadingJet->GetEt());
}

//________________________________________________________________________
const THIJet *THIDiJetTruthAnaMod::GetMatchedJet(const THIParticle * parton, const THIJetCollection * jetcol, const Float_t dRMax)
{
   // Initialize variables
   Double_t etCand = 0;
   Int_t iCand = -1;

   // Step through match candidates.
   for (UInt_t i=0; i<jetcol->GetEntries(); ++i) {
      // get the ith jet
      const THIJet * ijet = jetcol->At(i);
      // find dR between parton and jet
      Float_t dR = parton->GetMom().DeltaR(ijet->GetMom());
      // cut on dR
      if (dR > dRMax) continue;
      // Get max et jet within the dRMax cone of the parton
      if (ijet->GetEt() > etCand) {
	 etCand = ijet->GetEt();
	 iCand = (Int_t) i;
      }
   }
   // Finished stepping through match candidates. Check if found a matched jet.
   if (iCand < 0) {
      printf("No matched jet found\n");
      return NULL;
   }

   // Print results
   const THIJet * jet = jetcol->At(iCand);
   printf("             Et  Eta Phi\n");
   printf("parton:      %6.2f %6.2f %6.2f\n", parton->GetEt(), parton->GetEta(), parton->GetPhi());
   printf("matched jet: %6.2f %6.2f %6.2f\n", jet->GetEt(), jet->GetEta(), jet->GetPhi());

   // Return results
   return jetcol->At(iCand);
}

//________________________________________________________________________
void THIDiJetTruthAnaMod::GetPartons(const THIParticle *trigPart)
{
   //Find the partons


   //
   //UInt_t nump = fParticles->GetEntries();

   Double_t etNear = 0;
   Int_t    iNear = -1;

   Double_t etAway = 0;
   Int_t    iAway = -1;

   THIMCGenRecord *gen = dynamic_cast<THIMCGenRecord*>(fGenRecords->At(0));
   fSigParticles = gen->GetParticles();
   UInt_t nump = fSigParticles->GetEntries();
   for(UInt_t i=0; i<nump; i++){
      //      const THIParticle *genPart = fSigParticles->At(i);
      const THIParticle *genPart = fSigParticles->At(i);
      
      if(genPart->GetHepStatus()!=3) continue;
      if(genPart->GetId()==2212) continue;
      if(genPart->GetEt()<15) continue;
      Double_t dphi = TMath::Abs(genPart->GetMom().DeltaPhi(trigPart->GetMom()));
      printf("#%d phi %g tpet: %f tpid: %d id: %d stat %d (%d)et: %g eta %g p(%g|%g|%g) phi %g dphi %g\n",
	     i,
	     trigPart->GetPhi(),
	     trigPart->GetEt(),
	     trigPart->GetId(),
	     genPart->GetId(),
	     genPart->GetStatus(),
	     genPart->GetHepStatus(),
	     genPart->GetEt(),
	     genPart->GetEta(),
	     genPart->GetMom().X(),
	     genPart->GetMom().Y(),
	     genPart->GetMom().Z(),
	     genPart->GetPhi(),
	     dphi
	     );
      if(dphi < 0.5*TMath::Pi()) {
	 if(genPart->GetEt()>etNear){
	    etNear = genPart->GetEt();
	    iNear = (Int_t) i;
	 }
      }
      if(dphi > 0.5*TMath::Pi()) {
	 if(genPart->GetEt()>etAway){
	    etAway = genPart->GetEt();
	    iAway = (Int_t) i;
	 }
      }
   }
   printf("iNear = %d iAway = %d\n",iNear,iAway);
   if(iNear>0&&iAway>0){
      fNearParton  = fSigParticles->At(iNear);
      fAwayParton  = fSigParticles->At(iAway);
   }
}

//_______________________________________________________________________
void THIDiJetTruthAnaMod::FillLeadNTuple(const THIParticle * nlp, const THIParticle * alp, const THIJet * nlj, const THIJet *alj, TNtuple * lnt)
{
   //-- get values--
   UInt_t eve = fEvent->GetEventNum();
   UInt_t run = fEvent->GetRunNum();
//   THILorentzVector pcm = nlp->GetMom() + alp->GetMom();
//   Float_t mass = pcm.Mag(); 
//   Float_t cmeta = pcm.Eta();
   TVector3 v1(nlp->Get3Mom().X(),
	 nlp->Get3Mom().Y(),
	 nlp->Get3Mom().Z()
	 );
   TVector3 v2(alp->Get3Mom().X(),
	 alp->Get3Mom().Y(),
	 alp->Get3Mom().Z()
	 );
   TLorentzVector lv1(v1,v1.Mag());
   TLorentzVector lv2(v2,v2.Mag());

   TVector3 vs = v1 +v2;
   //      printf("MASS: np %f ap %f vs %f\n",nlp->GetMom().Mag(),alp->GetMom().Mag(),vs.Mag());
   //      THIVectorT vs = nlp->Get3Mom() + alp->Get3Mom();
   Float_t mass =  TMath::Sqrt(pow(lv1.E()+lv2.E(),2)-pow(vs.Mag(),2));
   //TMath::Sqrt(pow(nlp->GetMom().Mag()+alp->GetMom().Mag(),2)-pow(vs.Mag(),2));
   Float_t cmeta = (nlp->GetEta()+alp->GetEta())/2;
   Double_t dphi = TMath::Abs(alp->GetMom().DeltaPhi(nlp->GetMom()));
   
   //-- set array --
   Float_t fX[100];
   for(Int_t i = 0; i < 100; i++) fX[i] = 0;
   Int_t   fn = 0;
   // Fill basic info
   fX[fn] = run; //0
   fX[++fn] = eve;
   fX[++fn] = mass;
   fX[++fn] = cmeta;;
   fX[++fn] = dphi;
   // Fill leading partons info
   fX[++fn] = nlp->GetId(); //5
   fX[++fn] = nlp->GetStatus();
   fX[++fn] = nlp->GetEt();
   fX[++fn] = nlp->GetEta();
   fX[++fn] = nlp->GetPhi();
   fX[++fn] = alp->GetId(); //10
   fX[++fn] = alp->GetStatus();
   fX[++fn] = alp->GetEt();
   fX[++fn] = alp->GetEta();
   fX[++fn] = alp->GetPhi();
   // Fill leading jets info
   if ( nlj && alj ){
      fX[++fn] = nlj->GetEt(); //15
      fX[++fn] = nlj->GetEta();
      fX[++fn] = nlj->GetPhi();
      fX[++fn] = alj->GetEt(); //18
      fX[++fn] = alj->GetEta();
      fX[++fn] = alj->GetPhi();
      fX[++fn] = TMath::Abs(alj->GetMom().DeltaPhi(nlj->GetMom())); //21 jdphi

      lnt->Fill(fX);
      printf("Run%.0f Evt#%4.0f mass:%7.2f cmeta:%7.2f pdphi: %7.2f jdphi: %7.2f\n", fX[0],fX[1],fX[2],fX[3],fX[4],fX[21]);
      printf("             near id|stat (et|eta|phi)              away id|stat (et|eta|phi\n");
      printf("  lpartons.  1) %7.2f|%7.2f (%7.2f|%7.2f|%7.2f)  2) %7.2f|%7.2f (%7.2f|%7.2f|%7.2f)\n",
	    fX[5],fX[6],fX[7],fX[8],fX[9],
	    fX[10],fX[11],fX[12],fX[13],fX[14]
	    );
      printf("  ljets.     1)                 (%7.2f|%7.2f|%7.2f)  2)                 (%7.2f|%7.2f|%7.2f)\n",
	    fX[15],fX[16],fX[17],fX[18],fX[19],fX[20]
	    );
   }
//   else {
//      fNTLPartons->Fill(run,eve,mass,cmeta,dphi,
//	    nlp->GetId(),nlp->GetStatus(),nlp->GetEt(),nlp->GetEta(),nlp->GetPhi(),
//	    alp->GetId(),alp->GetStatus(),alp->GetEt(),alp->GetEta(),alp->GetPhi()
//	    );
//      printf("Run%d Evt#%d mass:%7.2f cmeta:%7.2f lparton id|stat: %d|%d eta|eta|phi: (%7.2f|%7.2f|%7.2f) aparton id|stat: %d|%d eta|eta|phi: (%7.2f|%7.2f|%7.2f)\n",
//	    run,eve,mass,cmeta,
//	    nlp->GetId(),nlp->GetStatus(),nlp->GetEt(),nlp->GetEta(),nlp->GetPhi(),
//	    alp->GetId(),alp->GetStatus(),alp->GetEt(),alp->GetEta(),alp->GetPhi()
//	    );
//   }
}

//________________________________________________________________________
void THIDiJetTruthAnaMod::FillEClusNTuple(const THIParticle *trigPart)
{
   // Fill Ntuple
   UInt_t eve = fEvent->GetEventNum();
   UInt_t run = fEvent->GetRunNum();

   //run:eve:tpid:tpet:tpeta:tpphi:isprom:npid:npet:npeta:npphi:apid:apet:apeta:apphi:apdphi:ecet:eceta:ecphi:ecz:tpz:tjet:tjeta:tjphi:tjdr:tjdphi:tjde:rjet:rjeta:rjphi:rjdr:rjdphi:rjde

   Float_t fX[100];
   for(Int_t i = 0; i < 100; i++) fX[i] = 0;

   Int_t   fn = 0;

   fX[fn++] = run;
   fX[fn++] = eve;
   
//   printf("trigPart @ %p\n",(void*)trigPart);
//   printf("fNearParton @ %p\n",(void*)fNearParton);
//   printf("fAwayParton @ %p\n",(void*)fAwayParton);
//   printf("fNearLeading @ %p\n",(void*)fNearLeading);
//   printf("fAwayLeading @ %p\n",(void*)fAwayLeading);
//   //"run:eve:npid:npet:npeta:npphi:nplpet:apid:apet:apeta:apphi:apdphi:aplpet"
   //"run:eve:mass:cmeta:
   //         npid:npet:npeta:npphi:nplpet:nplpid:nplpdr:
   //         apid:apet:apeta:apphi:apdphi:aplpet:aplpid:aplpdr"
   if(fNearParton!=0&&fAwayParton!=0){
      TVector3 v1(fNearParton->Get3Mom().X(),
		  fNearParton->Get3Mom().Y(),
		  fNearParton->Get3Mom().Z()
		  );
      TVector3 v2(fAwayParton->Get3Mom().X(),
		  fAwayParton->Get3Mom().Y(),
		  fAwayParton->Get3Mom().Z()
		  );
      TLorentzVector lv1(v1,v1.Mag());
      TLorentzVector lv2(v2,v2.Mag());

      TVector3 vs = v1 +v2;
      //      printf("MASS: np %f ap %f vs %f\n",fNearParton->GetMom().Mag(),fAwayParton->GetMom().Mag(),vs.Mag());
      //      THIVectorT vs = fNearParton->Get3Mom() + fAwayParton->Get3Mom();
      fX[fn++] =  TMath::Sqrt(pow(lv1.E()+lv2.E(),2)-pow(vs.Mag(),2));
	 //TMath::Sqrt(pow(fNearParton->GetMom().Mag()+fAwayParton->GetMom().Mag(),2)-pow(vs.Mag(),2));
      fX[fn++] = (fNearParton->GetEta()+fAwayParton->GetEta())/2;
   }
   else
      fn+=2;
   if(fNearParton!=0){
      fX[fn++] = fNearParton->GetId();
      fX[fn++] = fNearParton->GetEt();
      fX[fn++] = fNearSMET;
      fX[fn++] = fNearParton->GetEta();
      fX[fn++] = fNearParton->GetPhi();
      Double_t dis  =  THIEtaPhi::DeltaR2(fNearParton->GetPhi(), fNearLeading->GetPhi(), fNearParton->GetEta(), fNearLeading->GetEta());
      fX[fn++] = fNearLeading->GetEt();
      fX[fn++] = fNearLeading->GetId();
      fX[fn++] = TMath::Sqrt(dis);
   }
   else
      fn+=7;
   if(fAwayParton!=0){
      fX[fn++] = fAwayParton->GetId();
      fX[fn++] = fAwayParton->GetEt();
      fX[fn++] = fAwaySMET;
      fX[fn++] = fAwayParton->GetEta();
      fX[fn++] = fAwayParton->GetPhi();
      Double_t apdphi = trigPart->GetMom().DeltaPhi(fAwayParton->GetPhi());
      fX[fn++] = apdphi+2*TMath::Pi()*(apdphi<0);
      Double_t dis  =  THIEtaPhi::DeltaR2(fAwayParton->GetPhi(), fAwayLeading->GetPhi(), fAwayParton->GetEta(), fAwayLeading->GetEta());
      fX[fn++] = fAwayLeading->GetEt();
      fX[fn++] = fAwayLeading->GetId();
      fX[fn++] = TMath::Sqrt(dis);
   }
   else
      fn+=5;
   
   fNTECStat->Fill(fX);

}
//________________________________________________________________________
void THIDiJetTruthAnaMod::FillFragFuncNTuple( const THIParticle *part, Double_t nearEt, Double_t nearEta, Double_t nearPhi, Double_t awayEt, Double_t awayEta, Double_t awayPhi,  Double_t awayDPhi, const THIParticle *trigPart, TNtuple *NTuple){
   // Fill FF ntuple

   //   run:eve:mass:cmeta:npid:npet:npeta:npphi:nplpet:nplpid:nplpdr:apid:apet:apeta:apphi:apdphi:aplpet:aplpid:aplpdr:


   Float_t fX[100];
   for(Int_t i = 0; i < 100; i++) fX[i] = 0;
   Int_t   fn = 0;
   Float_t acc = 1;
   UInt_t eve = fEvent->GetEventNum();
   UInt_t run = fEvent->GetRunNum();
   fX[fn++] = run;
   fX[fn++] = eve;
   if(fNearParton!=0&&fAwayParton!=0){
      TVector3 v1(fNearParton->Get3Mom().X(),
		  fNearParton->Get3Mom().Y(),
		  fNearParton->Get3Mom().Z()
		  );
      TVector3 v2(fAwayParton->Get3Mom().X(),
		  fAwayParton->Get3Mom().Y(),
		  fAwayParton->Get3Mom().Z()
		  );
      TLorentzVector lv1(v1,v1.Mag());
      TLorentzVector lv2(v2,v2.Mag());

      TVector3 vs = v1 +v2;
      //      printf("MASS: np %f ap %f vs %f\n",fNearParton->GetMom().Mag(),fAwayParton->GetMom().Mag(),vs.Mag());
      //      THIVectorT vs = fNearParton->Get3Mom() + fAwayParton->Get3Mom();
      fX[fn++] =  TMath::Sqrt(pow(lv1.E()+lv2.E(),2)-pow(vs.Mag(),2));
	 //TMath::Sqrt(pow(fNearParton->GetMom().Mag()+fAwayParton->GetMom().Mag(),2)-pow(vs.Mag(),2));
      fX[fn++] = (fNearParton->GetEta()+fAwayParton->GetEta())/2;
   }
   else
      fn+=2;
   if(fNearParton!=0){
      fX[fn++] = fNearParton->GetId();
      fX[fn++] = fNearParton->GetEt();
      fX[fn++] = fNearSMET;
      fX[fn++] = fNearParton->GetEta();
      fX[fn++] = fNearParton->GetPhi();
      Double_t dis  =  THIEtaPhi::DeltaR2(fNearParton->GetPhi(), fNearLeading->GetPhi(), fNearParton->GetEta(), fNearLeading->GetEta());
      fX[fn++] = fNearLeading->GetEt();
      fX[fn++] = fNearLeading->GetId();
      fX[fn++] = TMath::Sqrt(dis);
   }
   else
      fn+=7;
   if(fAwayParton!=0){
      fX[fn++] = fAwayParton->GetId();
      fX[fn++] = fAwayParton->GetEt();
      fX[fn++] = fAwaySMET;
      fX[fn++] = fAwayParton->GetEta();
      fX[fn++] = fAwayParton->GetPhi();
      Double_t apdphi = trigPart->GetMom().DeltaPhi(fAwayParton->GetPhi());
      fX[fn++] = apdphi+2*TMath::Pi()*(apdphi<0);
      Double_t dis  =  THIEtaPhi::DeltaR2(fAwayParton->GetPhi(), fAwayLeading->GetPhi(), fAwayParton->GetEta(), fAwayLeading->GetEta());
      fX[fn++] = fAwayLeading->GetEt();
      fX[fn++] = fAwayLeading->GetId();
      fX[fn++] = TMath::Sqrt(dis);
   }
   else
      fn+=5;

   //   ppid:ppt:peta:pphi:
   //   pndphi:pndeta:pndr:padphi:padeta:padr:
   //   rec:zn:za
   Double_t pndphi,pndeta,pndr,pndrbg,padphi,padeta,padr,padrbg;
   pndphi = THIEtaPhi::DeltaPhi(part->GetPhi(), nearPhi);
   pndeta  = part->GetEta() - nearEta;
   pndr = THIEtaPhi::DeltaR2(part->GetPhi(), nearPhi, part->GetEta(), nearEta);
   pndrbg = THIEtaPhi::DeltaR2(part->GetPhi(), nearPhi+TMath::Pi()/2, part->GetEta(), nearEta);

   padphi = THIEtaPhi::DeltaPhi(part->GetPhi(), awayPhi);
   padeta = part->GetEta() - awayEta;
   padr = THIEtaPhi::DeltaR2(part->GetPhi(), awayPhi, part->GetEta(), awayEta);
   padrbg = THIEtaPhi::DeltaR2(part->GetPhi(), awayPhi+TMath::Pi()/2, part->GetEta(), awayEta);

   fX[fn++] = part->GetId();
   fX[fn++] = part->GetPt();
   fX[fn++] = part->GetEta();
   fX[fn++] = part->GetPhi();

   fX[fn++] = pndphi+(2*TMath::Pi()*(pndphi<-1*TMath::Pi()/2));
   fX[fn++] = pndeta;
   fX[fn++] = pndr;
   fX[fn++] = pndrbg;
   
   fX[fn++] = padphi+(2*TMath::Pi()*(padphi<-1*TMath::Pi()/2));
   fX[fn++] = padeta;
   fX[fn++] = padr;
   fX[fn++] = padrbg;

   fX[fn++] = acc;
   fX[fn++] = part->GetPt()/nearEt;
   fX[fn++] = part->GetPt()/awayEt;

   NTuple->Fill(fX);
   
}
//________________________________________________________________________
void THIDiJetTruthAnaMod::Process()
{
   // Process entries of the tree 
   // Get the S.C -> Get the matching jet -> Get the variables 
   // for the particles inside the matching jet

   LoadBranch(THIEvent::kEventName);
   LoadBranch(THIGenRecordArray::kRecordsName);
   LoadBranch(THIParticleArray::kParticlesName);
   LoadBranch(THIVertexArray::kVerticesName);

   if(fGenRecords == 0) {
      SendError(kAbortAnalysis, "Process", "THIDiJetTruthAnaMod: Sorry, could not get find gen records.");
      return; 
   }

   if(fParticles == 0) {
      SendError(kAbortAnalysis, "Process", "THIDiJetTruthAnaMod: Sorry, could not get find particles.");
      return; 
   }
   
   const THIParticle *trigPart = GetTriggerGenParticle();
   
   if(trigPart==NULL){
      //      SendError(kAbortAnalysis, "Process", "THIDiJetTruthAnaMod: Sorry, could not get trigger particle.");
      printf("THIDiJetTruthAnaMod: SORRY, could not get trigger particle.\n");
      return;
   }

   fNearParton = 0;
   fAwayParton = 0;
   GetPartons(trigPart);
   if(fNearParton==NULL||
      fNearParton==NULL){
      //      SendError(kAbortAnalysis, "Process", "THIDiJetTruthAnaMod: Sorry, could not get trigger particle.");
      printf("THIDiJetTruthAnaMod: SORRY, could not get partons.\n");
      return;
   }

   THIMCGenRecord *gen = dynamic_cast<THIMCGenRecord*>(fGenRecords->At(0));
   fSigParticles = gen->GetParticles();
   THIGenPythiaRecord *PytRec = dynamic_cast<THIGenPythiaRecord*>(fGenRecords->At(0));
//   printf("xsec: %f\n",PytRec->GetCrossSection());

   if(fAwayParton==0||fNearParton==0||trigPart==0)
      return;
   fAwayLeading = GetLeading(fAwayParton);
   fNearLeading = GetLeading(fNearParton);

//   printf("near @ %d\n",fNearParton);
   Float_t reso = fReso->Eval(fNearParton->GetEt());
   fGaus->SetParameter(2,reso);
   fNearSMET = fNearParton->GetEt()*fGaus->GetRandom();

   reso = fReso->Eval(fAwayParton->GetEt());
   fGaus->SetParameter(2,reso);
   fAwaySMET = fAwayParton->GetEt()*fGaus->GetRandom();

   // Christof's converted ntuple for the leading jets
//   FillEClusNTuple(trigPart);

   if(fStatOnly)
      return;
   // event statistics for normalization--------------------------------

   //--- Loop over the subevents ---
   Int_t numsub = fGenRecords->GetEntries();
//   const UInt_t numsub = fJetAArray->GetEntries();
   for(Int_t ise = 0;ise<numsub; ise++) {
      //--so far no other subevents --
      if (ise>0) continue;

      //--- jet related
      fJetArray = dynamic_cast<THIJetCollection*>(fJetAArray->At(ise));
      if(fJetArray==0) {
	 Error("Process", "Could not get jet collection for entry %d", ise);
	 continue;
      }
      PrintJetCol(fJetArray);

      //-- get the leading jets --
//      GetLeadJets(fJetArray);
      //--- Get the matched jets;
      fNearLeadingJet = GetMatchedJet(fNearParton,fJetArray,fDeltaRMatch);
      fAwayLeadingJet = GetMatchedJet(fAwayParton,fJetArray,fDeltaRMatch);
      if (fNearLeadingJet==0 || fAwayLeadingJet==0) {
	 printf("Since one parton is not matched, skip this event\n");
	 continue;
      }

      // Now we have both the leading partons and jets
      //--- Fill ntuple for leading partons and matching jets ---
      FillLeadNTuple(fNearParton,fAwayParton,fNearLeadingJet,fAwayLeadingJet,fNTPartonLeading);
      //--- Fill ntuple for leading jets and matching partons---
      if (fNearLeadingJet->GetEt() > fAwayLeadingJet->GetEt()) {
	 FillLeadNTuple(fNearParton,fAwayParton,fNearLeadingJet,fAwayLeadingJet,fNTJetLeading);
      }
      else {
	 FillLeadNTuple(fAwayParton,fNearParton,fAwayLeadingJet,fNearLeadingJet,fNTJetLeading);
      }

      // Check the particles inside cone and get variables-----------------
      THIMCGenRecord *gen = dynamic_cast<THIMCGenRecord*>(fGenRecords->At(ise));
      THICollection<THIParticle> *Particles = gen->GetParticles();
      Int_t nump = Particles->GetEntries();
      for(Int_t i=0; i< nump; i++){
	 const THIParticle *p=Particles->At(i);
	 if(p->GetStatus()==1){
	    if(p->GetStatus()==1 && p->GetCharge() != 0){
	       if(TMath::Abs(p->GetEta()) <= 2.5){//found some photons having charge =+-1 ^^
		  if(ise==0){ // away part + sig parts
		     // near part + true jet + sig parts
		     //smear
		     FillFragFuncNTuple(p,
					fNearParton->GetEt(), 
					fNearParton->GetEta(),
					fNearParton->GetPhi(), 
					fAwayParton->GetEt(), 
					fAwayParton->GetEta(), 
					fAwayParton->GetPhi(),
					trigPart->GetMom().DeltaPhi(fAwayParton->GetPhi()), 
					trigPart, fNTTruePFF);
		     /* FillFragFuncNTuple(p,
					fNearParton->GetEt(), 
					fNearParton->GetEta(),
					fNearParton->GetPhi(), 
					fAwayParton->GetEt(), 
					fAwayParton->GetEta(), 
					fAwayParton->GetPhi()+0.5*TMath::Pi(),
					trigPart->GetMom().DeltaPhi(fAwayParton->GetPhi()),
					trigPart, fNTTruePFFBG);
		     		     FillFragFuncNTuple(p,
					fNearParton->GetEt(), 
					fNearParton->GetEta(),
					fNearParton->GetPhi(), 
					fAwayParton->GetEt(), 
					fAwayParton->GetEta(), 
					fAwayParton->GetPhi()-0.5*TMath::Pi(),
					trigPart->GetMom().DeltaPhi(fAwayParton->GetPhi()),
					trigPart, fNTTruePFFBG);
		     */
		  }
	       }
	    }
	 }
      }
   } // done with subevt loop
   printf("Done Fill ntuple\n\n");
}


//________________________________________________________________________
void THIDiJetTruthAnaMod::SlaveBegin()
{
   // Run startup code on the computer (slave) doing the actual
   // analysis. Here, we typically initialize histograms and 
   // other analysis objects and request branches. For this module,
   // we initialize a bunch of histograms.
      
   ReqBranch(THIEvent::kEventName, fEvent);
   ReqBranch(THIParticleArray::kParticlesName, fParticles);
   ReqBranch(THIVertexArray::kVerticesName, fVertices);
   ReqBranch(THIGenRecordArray::kRecordsName, fGenRecords);

   fNTTruePFF = new TNtuple("NTTruePFF","NTTruePFF","run:eve:mass:cmeta:npid:npet:npetsm:npeta:npphi:nplpet:nplpid:nplpdr:apid:apet:apetsm:apeta:apphi:apdphi:aplpet:aplpid:aplpdr:ppid:ppt:peta:pphi:pndphi:pndeta:pndr:pndrbg:padphi:padeta:padr:padrbg:rec:zn:za");
   AddOutput(fNTTruePFF);
   
   fNTTruePFFBG = new TNtuple("NTTruePFFBG","NTTruePFFBG","run:eve:mass:cmeta:npid:npet:npetsm:npeta:npphi:nplpet:nplpid:nplpdr:apid:apet:apetsm:apeta:apphi:apdphi:aplpet:aplpid:aplpdr:ppid:ppt:peta:pphi:pndphi:pndeta:pndr:padphi:padeta:padr:rec:zn:za");
   AddOutput(fNTTruePFFBG);
   
   fNTECStat = new TNtuple("NTECStat","NTECStat","run:eve:mass:cmeta:npid:npet:npetsm:npeta:npphi:nplpet:nplpid:nplpdr:apid:apet:apetsm:apeta:apphi:apdphi:aplpet:aplpid:aplpdr");
   AddOutput(fNTECStat);

   fNTLPartons = new TNtuple("NTLPartons","leading partons","run:eve:mass:cmeta:dphi:nlpid:nlpstat:nlpet:nlpeta:nlpphi:alpid:alpstat:alpet:alpeta:alpphi");
   AddOutput(fNTLPartons);
   
   fNTPartonLeading = new TNtuple("NTPartonLeading","leading partons and matched jets",
	 "run:eve:mass:cmeta:dphi:nlpid:nlpstat:nlpet:nlpeta:nlpphi:alpid:alpstat:alpet:alpeta:alpphi:nljet:nljeta:nljphi:aljet:aljeta:aljphi:jdphi");
   AddOutput(fNTPartonLeading);

   fNTJetLeading = new TNtuple("NTJetLeading","leading partons and matched jets",
	 "run:eve:mass:cmeta:dphi:nlpid:nlpstat:nlpet:nlpeta:nlpphi:alpid:alpstat:alpet:alpeta:alpphi:nljet:nljeta:nljphi:aljet:aljeta:aljphi:jdphi");
   AddOutput(fNTJetLeading);

   // jet related
   if (fLoad) {
      ReqBranch(fJetAArrayName,fJetAArray);
   } else {
      // get published jets
      fJetAArray = dynamic_cast<THIObjectCollection*>
	 (FindPublicObj(fJetAArrayName));
   }

}
