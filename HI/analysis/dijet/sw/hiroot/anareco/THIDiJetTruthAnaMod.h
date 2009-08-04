//
// $Id: THIDiJetTruthAnaMod.h,v 1.12 2009/08/01 02:56:28 frankma Exp $
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
#ifndef HIROOT_THIJetCollection
#include "THIJetCollection.h"
#endif


class THIParticle;
class THIEvent;
class THIGenRecord;
class THIVertex;
class THIParticleArray;
class THIPhotonMCType;

// dijet tree data class
class TTree;
namespace DiJetAna { class TreeDiJetEventData; }

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
   TNtuple                        *fNTJetFF;              //!ntuple with jet based FF
   TNtuple                        *fNTLPartons;           //!ntuple of leading partons in each event
   TNtuple                        *fNTPartonLeading;            //!ntuple of leading partons + matched jetsin each event
   TNtuple                        *fNTJetLeading;            //!ntuple of leading jets + matched partons in each event

   Float_t                         fIsPromPhoton;         //!prompt photon event flag
   Float_t                         fIsIsolated;           //!isolated particle event flag
   THIParticle                    *fAwayParton;           //!away side parton
   THIParticle                    *fNearParton;           //!near side parton
   Float_t                         fAwaySMET;           //!
   Float_t                         fNearSMET;           //!
   const THIParticle                    *fAwayLeading;           //!away side leading particle
   const THIParticle                    *fNearLeading;           //!near side leading particle
   const THIJet                    *fAwayLeadingJet;           //!away side jet
   const THIJet                    *fNearLeadingJet;           //!near side jet
   //--- jet info
   TString                  fJetAArrayName;     //name of array of jet arrays
   Bool_t                   fLoad;              //if true load jet array from tree branch
   THIObjectCollection     *fJetAArray;         //!input array of jet arrays (published or loaded)
   THIJetCollection        *fJetArray;          //!active jet collection from for-loop in Process()
   //--- jet ana
   Float_t                         fDeltaRMatch;           //! Jet matching max dR
   Float_t                         fJetEtMin;              //! Minimum found jet Et accepted
   //--- dijet ana event data
   TTree                           *fEvtTree;
   DiJetAna::TreeDiJetEventData    *fEvtData;


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
   void                           FillLeadNTuple(const THIParticle * nlp, const THIParticle * alp, const THIJet * nlj, const THIJet *alj, TNtuple * lnt);    //! Fill ntuple for leading partons and jets
   void                           FillFragFuncNTuple( const THIParticle *part, Double_t nearEt, Double_t nearEta, Double_t nearPhi, Double_t awayEt, Double_t awayEta, Double_t awayPhi, Double_t awayDPhi, const THIParticle *trigPart, TNtuple *NTuple);
   void                           SetStatOnly(Bool_t b) {fStatOnly = b;}

   // --- Dijet Event Tree ---
   void                           CalcJetVars( const THIParticle* trigPart, const THIParticle * nlp, const THIParticle * alp, const THIJet * nlj, const THIJet *alj, DiJetAna::TreeDiJetEventData *ed);    //! Calc vars for leading partons and jets
   void                           CalcParticleVars( const THIParticle *part, Double_t nearEt, Double_t nearEta, Double_t nearPhi, Double_t awayEt, Double_t awayEta, Double_t awayPhi, Double_t awayDPhi, const THIParticle *trigPart, DiJetAna::TreeDiJetEventData *ed);

public:
   THIDiJetTruthAnaMod(const char *name="DiJetTruthAnaMod", 
                        const char *title="Gamma and matching jet spectra analysis module");
   virtual ~THIDiJetTruthAnaMod() {};
   //--- jet ana related functions
   const char              *GetJetName()              const { return fJetAArrayName; }
   Bool_t                   GetLoadBranch()           const { return fLoad; }
   void                     SetJetName(const char *jetname) { fJetAArrayName = jetname; }
   void                     SetJetDeltaRMatch(Float_t dRMax)   { fDeltaRMatch = dRMax; }
   void                     SetJetEtMin(Float_t EtMin)   { fJetEtMin = EtMin; }
   void                     GetLeadJets(const THIJetCollection * jl);
   void                     PrintJetCol(const THIJetCollection * jetcol);
   const THIJet            *GetMatchedJet(const THIParticle * parton, const THIJetCollection * jetcol, const Float_t dRMax);
   

   ClassDef(THIDiJetTruthAnaMod,1)  //TAM module for gamma-tagged truth matching
};
      
#endif /*HIROOT_THIDiJetTruthAnaMod*/
