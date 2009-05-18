//
// $Id: THIDiJetTruthAnaMod.h 4883 2007-12-12 18:03:03Z cer $
//

#ifndef HIROOT_THIDiJetTruthAnaMod
#define HIROOT_THIDiJetTruthAnaMod


#ifndef ROOT_TSTRING
#include <TString.h>
#endif
#ifndef ROOT_TAModule
#include <TAModule.h>
#endif


class TH1D;
class TH2D;
class TF1;
class TRandom3;
class TNtuple;
class TProfile;


#ifndef HIROOT_THIAnaBaseMod
#include "THIAnaBaseMod.h"
#endif
#ifndef HIROOT_THICollection
#include "THICollection.h"
#endif
#ifndef HIROOT_THIObjectCollection
#include "THIObjectCollection.h"   
#endif


class THIParticle;
class THIEvent;
class THIGenRecord;
class THIVertex;
class THIParticleArray;
class THIPhotonMCType;


class THIDiJetTruthAnaMod : public THIAnaBaseMod {
protected:

   THIEvent                       *fEvent;                //!event branch
   THICollection<THIParticle>     *fParticles;            //!particle branch
   THICollection<THIVertex>       *fVertices;             //!vertex branch
   THICollection<THIGenRecord>    *fGenRecords;           //!record branch
   THICollection<THIParticle>     *fSigParticles;         //!signal particles

   TNtuple                        *fNTECStat;             //!ntuple with ecal cluster statitics
   TNtuple                        *fNTTruePFF;            //!ntuple with gamma jet and background particles
   TNtuple                        *fNTTruePFFBG;          //!ntuple with gamma jet and background particles
   TNtuple                        *fNTLPartons;           //!ntuple of leading partons in each event

   Float_t                         fIsPromPhoton;         //!prompt photon event flag
   Float_t                         fIsIsolated;           //!isolated particle event flag
   THIParticle                    *fAwayParton;           //!away side parton
   THIParticle                    *fNearParton;           //!near side parton
   Float_t                         fAwaySMET;           //!away side parton 
   Float_t                         fNearSMET;           //!away side parton 
   const THIParticle                    *fAwayLeading;           //!away side parton
   const THIParticle                    *fNearLeading;           //!near side parton

   TF1                            *fTrackingEfficiency;   //! Function parametrizing the tracking efficiency      
   TF1                            *fGaus;                 //! Function parametrizing the tracking efficiency      
   TF1                            *fReso;                 //! Function parametrizing the tracking efficiency      
   Bool_t                          fStatOnly;             //! Write only statistics ntuple
   TRandom3                       *fRand;                 //! Random number generator
   
   const THIParticle              *GetMother(const THIParticle *p);
   void                           Process();
   void                           SlaveBegin();
   const THIParticle             *GetTriggerGenParticle();
   void                           GetPartons(const THIParticle *trigPart);
   const THIParticle             *GetLeading(const THIParticle *trigPart);
   void                           FillEClusNTuple(const THIParticle *trigPart);
   void                           FillFragFuncNTuple( const THIParticle *part, Double_t nearEt, Double_t nearEta, Double_t nearPhi, Double_t awayEt, Double_t awayEta, Double_t awayPhi, Double_t awayDPhi, const THIParticle *trigPart, TNtuple *NTuple);
   void                           SetStatOnly(Bool_t b) {fStatOnly = b;}
public:
   THIDiJetTruthAnaMod(const char *name="DiJetTruthAnaMod", 
                        const char *title="Gamma and matching jet spectra analysis module");
   virtual ~THIDiJetTruthAnaMod() {};
   

   ClassDef(THIDiJetTruthAnaMod,1)  //TAM module for gamma-tagged truth matching
};
      
#endif /*HIROOT_THIDiJetTruthAnaMod*/
