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
   fJetAArray(0)
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
      if(genPart->GetId()==2212)
	 printf("beam rap: %f\n",genPart->GetMom().Rapidity());
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
   printf("trig @ %d\n",iCand);   
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
   printf("L#%d phi %g tpet: %f tpid: %d id: %d stat %d (%d)et: %g eta %g p(%g|%g|%g) phi %g dphi %g\n",
	  iCand,
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
	  idphi
	  );
   return fSigParticles->At(iCand);
}

//________________________________________________________________________
void THIDiJetTruthAnaMod::GetLeadJets(const THIJetCollection *jl)
{
   for (int i=0; i<jl->GetEntries(); ++i) {
      float et = jl->At(i)->GetEt();
      printf("jet read out: et: %f\n",et);
   }
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
void THIDiJetTruthAnaMod::FillLeadNTuple()
{
   //-- get values--
   UInt_t eve = fEvent->GetEventNum();
   UInt_t run = fEvent->GetRunNum();
//   THILorentzVector pcm = fNearParton->GetMom() + fAwayParton->GetMom();
//   Float_t mass = pcm.Mag(); 
//   Float_t cmeta = pcm.Eta();
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
   Float_t mass =  TMath::Sqrt(pow(lv1.E()+lv2.E(),2)-pow(vs.Mag(),2));
   //TMath::Sqrt(pow(fNearParton->GetMom().Mag()+fAwayParton->GetMom().Mag(),2)-pow(vs.Mag(),2));
   Float_t cmeta = (fNearParton->GetEta()+fAwayParton->GetEta())/2;
   Double_t dphi = TMath::Abs(fAwayParton->GetMom().DeltaPhi(fNearParton->GetMom()));
   //-- fill nt --
   fNTLPartons->Fill(run,eve,mass,cmeta,dphi,
	 fNearParton->GetId(),fNearParton->GetStatus(),fNearParton->GetEt(),fNearParton->GetEta(),fNearParton->GetPhi(),
	 fAwayParton->GetId(),fAwayParton->GetStatus(),fAwayParton->GetEt(),fAwayParton->GetEta(),fAwayParton->GetPhi()
	 );
   printf("Run%d Evt#%d mass:%f cmeta:%f lparton id|stat: %d|%d eta|eta|phi: (%f|%f|%f) aparton id|stat: %d|%d eta|eta|phi: (%f|%f|%f)\n",
	 run,eve,mass,cmeta,
	 fNearParton->GetId(),fNearParton->GetStatus(),fNearParton->GetEt(),fNearParton->GetEta(),fNearParton->GetPhi(),
	 fAwayParton->GetId(),fAwayParton->GetStatus(),fAwayParton->GetEt(),fAwayParton->GetEta(),fAwayParton->GetPhi()
	 );

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
   
   printf("trigPart @ %p\n",(void*)trigPart);
   printf("fNearParton @ %p\n",(void*)fNearParton);
   printf("fAwayParton @ %p\n",(void*)fAwayParton);
   printf("fNearLeading @ %p\n",(void*)fNearLeading);
   printf("fAwayLeading @ %p\n",(void*)fAwayLeading);
   //"run:eve:npid:npet:npeta:npphi:nplpet:apid:apet:apeta:apphi:apdphi:aplpet"
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
   printf("xsec: %f\n",PytRec->GetCrossSection());

   if(fAwayParton==0||fNearParton==0||trigPart==0)
      return;
   fAwayLeading = GetLeading(fAwayParton);
   fNearLeading = GetLeading(fNearParton);

   printf("near @ %d\n",fNearParton);
   Float_t reso = fReso->Eval(fNearParton->GetEt());
   fGaus->SetParameter(2,reso);
   fNearSMET = fNearParton->GetEt()*fGaus->GetRandom();

   reso = fReso->Eval(fAwayParton->GetEt());
   fGaus->SetParameter(2,reso);
   fAwaySMET = fAwayParton->GetEt()*fGaus->GetRandom();

   FillEClusNTuple(trigPart);

   if(fStatOnly)
      return;
   printf("Done Fill ntuple\n");
   // event statistics for normalization--------------------------------

   //--- Fill ntuple for leading partons ---
   FillLeadNTuple();

   //--- Loop over the subevents ---
   Int_t numsub = fGenRecords->GetEntries();
//   const UInt_t numsub = fJetAArray->GetEntries();
   for(Int_t ise = 0;ise<numsub; ise++) {
      //--so far no other subevents --
      if (ise==0) {
	 //--- jet related
	 for(UInt_t i=0; i<numsub; i++) {
	    fJetArray = 
	       dynamic_cast<THIJetCollection*>(fJetAArray->At(i));

	    if(fJetArray==0) {
	       Error("Process", "Could not get jet collection for entry %d", i);
	       continue;
	    }
	    GetLeadJets(fJetArray);
	 }
	 //---
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
   }
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
   
   // jet related
   if (fLoad) {
      ReqBranch(fJetAArrayName,fJetAArray);
   } else {
      // get published jets
      fJetAArray = dynamic_cast<THIObjectCollection*>
	 (FindPublicObj(fJetAArrayName));
   }

}
