//--------------------------------------------------------------------------------------------------
// $Id: PixelsMakerMod.h,v 1.8 2009/12/22 15:47:44 yjlee Exp $
//
// PixelsMakerMod
//
// This module creates a tree filled with pixel hit information.
// The tree is stored in a regular ROOT file. (Non-Proof safe).
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITQCD_TRACKLETSANA_PIXELSMAKERMOD_H 
#define MITQCD_TRACKLETSANA_PIXELSMAKERMOD_H 

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/CollectionsFwd.h"
#include "MitAna/DataTree/interface/MCEventInfo.h"
#include "MitAna/DataTree/interface/MCParticle.h"
#include "MitAna/DataTree/interface/PixelHit.h"
#include "MitAna/DataTree/interface/TriggerMask.h"
#include "MitAna/DataTree/interface/L1TriggerMaskCol.h"

using namespace mithep;

class TTree;
class TFile;

#define MAXPARTICLES 500000
#define MAXHITS 50000
#define MAXVTX 100
#define MAXHLTBITS 256
#define MAXBX 120

namespace mithig
{
  class PixelsMakerMod : public BaseMod
  {
    public:
      PixelsMakerMod(const char *name="PixelsMakerMod", 
                     const char *title="Pixels maker module",
		     unsigned int nbunch = 5);

      void                       AddVertexName(const char *n)   { fVertexNames.push_back(n); }
      const char                *GetFileName()     const        { return fFileName;          }
      Bool_t                     GetIsMC()         const        { return fIsMC;              }
      const char                *GetPixelHitName() const        { return fPixelHitName;      }
      void                       SetFileName(const char *n)     { fFileName     = n;         }
      void                       SetPixelHitName(const char *n) { fPixelHitName = n;         }
      void                       SetIsMC(Bool_t b)              { fIsMC         = b;         }
      void		         SetUseHfOnly(bool choice)      { fHfOnly=choice;	     }
      bool                       GoodTower(const CaloTower * ct);
      void			 SetSDRelFrac(Float_t f)        { fSDRelFrac     = f;         }

    protected:
      void                       Process();
      void                       SlaveBegin();
      void                       SlaveTerminate();

      struct PixelEvent {
          UInt_t nRun;
          UInt_t nEv;
          UInt_t nLumi;
	  UInt_t nBX;
          Int_t nhits1;
          Int_t nhits2;
          Int_t nhits3;
  	  Int_t nhitsF1;
	  Int_t nhitsF2;
	  Int_t nHFn;
	  Int_t nHFp;
	  // diffractive study
	  Float_t SumEsubEp;
	  Float_t SumEaddEp;
	  Float_t SumEsubEpPos;
	  Float_t SumEaddEpPos;
	  Float_t SumEsubEpNeg;
	  Float_t SumEaddEpNeg;
	  Float_t evtEta;
	  

          Int_t nv;
          Float_t vx[MAXVTX];
          Float_t vy[MAXVTX];
          Float_t vz[MAXVTX];
          Float_t eta1[MAXHITS];
          Float_t phi1[MAXHITS];
          Float_t r1[MAXHITS];
          Float_t cs1[MAXHITS];
          Float_t ch1[MAXHITS];
          Bool_t flag1[MAXHITS];
          Float_t eta2[MAXHITS];
          Float_t phi2[MAXHITS];
          Float_t r2[MAXHITS];
          Float_t cs2[MAXHITS];
          Float_t ch2[MAXHITS];
          Bool_t flag2[MAXHITS];
          Float_t eta3[MAXHITS];
          Float_t phi3[MAXHITS];
          Float_t r3[MAXHITS];
          Float_t cs3[MAXHITS];
          Float_t ch3[MAXHITS];
          Bool_t flag3[MAXHITS];

	Float_t etaF1[MAXHITS];
	Float_t phiF1[MAXHITS];
	Float_t rF1[MAXHITS];
	Float_t csF1[MAXHITS];
	Float_t chF1[MAXHITS];
	Bool_t flagF1[MAXHITS];
	
	Float_t etaF2[MAXHITS];
        Float_t phiF2[MAXHITS];
        Float_t rF2[MAXHITS];
        Float_t csF2[MAXHITS];
        Float_t chF2[MAXHITS];
        Bool_t flagF2[MAXHITS];

          Int_t nHltBit;
	Int_t nL1A;
	Int_t nL1T;

          Bool_t hltBit[MAXHLTBITS];
	Bool_t L1A[MAXHLTBITS];
	Bool_t L1T[MAXHLTBITS];

	Bool_t L1AvsBX[MAXHLTBITS][MAXBX];
        Bool_t L1TvsBX[MAXHLTBITS][MAXBX];

          /* from here mc*/
          Int_t nparticle;
          Float_t pt[MAXPARTICLES];
          Float_t eta[MAXPARTICLES];
          Float_t phi[MAXPARTICLES];

          Int_t pdg[MAXPARTICLES];
	  Int_t mother[MAXPARTICLES];

          Int_t chg[MAXPARTICLES];
          Int_t evtType;

          void Add(const PixelHit &hit);
          void Add(const MCParticle &mc);
          void Clear();
      };

      TString                    fPixelHitName;      //name of pixel hit input collection
      TString                    fCaloTowerName;      //name of caloTower input collection
      TString                    fHLTBitName;        //name of HLT bits
      TString                    fL1ABitName;        //name of L1 Algo bits
      TString                    fL1TBitName;        //name of L1 Technical bits
      TString                  fL1ABxsName;      //l1a bxs branch name
      TString                  fL1TBxsName;      //l1t bxs branch name

      TString                    fFileName;          //file name for pixel tree
      TString                    fMCPartName;        //name of MC particle branch
      TString                    fMCVertName;        //name of MC vertex branch
      TString                    fMCEvtInfoName;     //event info branch name
      Bool_t                     fIsMC;              //== true fill MC info
      std::vector<std::string>   fVertexNames;       //vertex collection names
      const VertexCol           *fVertexCol[MAXVTX]; //!vertex collection
      const PixelHitCol         *fPixelHitCol;       //!pixel hit collection
      const TriggerMask         *fHltBits;           //!hlt trigger bits branch
      const CaloTowerCol           *fCaloTowerCol;   //!CaloTower branch
      const L1TriggerMask         *fL1ABits;           //!hlt trigger bits branch
      const L1TriggerMask         *fL1TBits;           //!hlt trigger bits branch
      const L1TriggerMaskCol  *fL1ABitBxs;       //!l1a bits branch vs bx
      const L1TriggerMaskCol  *fL1TBitBxs;       //!l1t bits branch vs bx
      UInt_t                       fNbunch;       // Number of bunches saved.

      const MCParticleCol       *fMCPartCol;         //!MC particle collection
      const VertexCol           *fMCVertCol;         //!MC vertex collection
      const MCEventInfo         *fMCEvtInfo;         //!MC event info pointer
      TTree                     *fTree;              //!pixel tree
      TFile                     *fFile;              //!pixel file
      PixelEvent                 fPE;                //!pixel event

      // diffractive study
      bool                     fHfOnly;
      Float_t		       fSDRelFrac;

      ClassDef(PixelsMakerMod, 1) // Pixels maker
  };
}
#endif
