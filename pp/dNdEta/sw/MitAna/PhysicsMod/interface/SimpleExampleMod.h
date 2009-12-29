//--------------------------------------------------------------------------------------------------
// $Id: SimpleExampleMod.h,v 1.7 2009/06/15 15:00:16 loizides Exp $
//
// SimpleExampleMod
//
// This TAM module shows how to use TAM. It takes the MCParticle branch
// and produces the pt and eta distribution of the contained particles.
//
// The module also declares all relevant member functions of a TAModule
// and has extensive comments for each of them, even if for this simple
// example they are not implemented.
//
// More information about TAM in general can be found at
// http://www.cmsaf.mit.edu/twiki/bin/view/Software/TAM
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITANA_PHYSICSMOD_SIMPLEEXAMPLEMOD_H
#define MITANA_PHYSICSMOD_SIMPLEEXAMPLEMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/MCParticleFwd.h"
#include "MitAna/DataTree/interface/CaloTowerFwd.h"
#include "MitAna/DataTree/interface/MCEventInfo.h"

class TH1D;
class TNtuple;
class TFile;

namespace mithep 
{
  class SimpleExampleMod : public BaseMod 
  {
    public:
      SimpleExampleMod(const char *name="SimpleExampleMod", 
                       const char *title="Simple example analysis module");

      const char              *GetPartName()              const { return fPartName; }
      const char              *GetTowerName()              const { return fTowerName; }
      const char              *GetMCEvtName()              const { return fMCEvtName; }
      void                     SetPartName(const char *n)       { fPartName=n; }
      void		       SetUseHfOnly(bool choice)        { fHfOnly=choice; }
      bool                     GoodTower(const CaloTower * ct);

    protected:
      void                     Begin();
      void                     BeginRun();
      void                     EndRun();
      void                     Process();
      void                     SlaveBegin();
      void                     SlaveTerminate();
      void                     Terminate();

      TString                  fPartName;   //branch name of MCParticle collection
      TString                  fTowerName;   //branch name of Calo Tower collection
      TString                  fMCEvtName;
      const MCParticleCol     *fParticles;  //!pointer to generated particle branch
      const CaloTowerCol      *fTowers;  //!pointer to Calo Tower branch
      const MCEventInfo       *fMCEvt;
      TH1D                    *fPtHist;     //!pt histogram
      TH1D                    *fEtaHist;    //!eta histogram
      // diffractive study
      bool                     fHfOnly;
      TH1D                    *fhEUncut;     //!energy histogram
      TH1D                    *fhE;     //!energy histogram
      TH1D                    *fhEp;    //!pz
      TH1D                    *fhEpComb;    //!pz
      TH1D                    *fhEpPos;    //!pz
      TH1D                    *fhEpNeg;    //!pz

      TH1D                    *fhEsubEp;    //!eta histogram
      TH1D                    *fhEaddEp;    //!eta histogram
      TH1D                    *fhEsubEpPos;    //!eta histogram
      TH1D                    *fhEsubEpNeg;    //!eta histogram
      TH1D                    *fhEaddEpPos;    //!eta histogram
      TH1D                    *fhEaddEpNeg;    //!eta histogram

      TH1D                    *fhSumEsubEp;    //!eta histogram
      TH1D                    *fhSumEaddEp;    //!eta histogram
      TH1D                    *fhSumEsubEpPos;    //!eta histogram
      TH1D                    *fhSumEsubEpNeg;    //!eta histogram
      TH1D                    *fhSumEaddEpPos;    //!eta histogram
      TH1D                    *fhSumEaddEpNeg;    //!eta histogram

      TNtuple                 *fntDiffrac;
      TFile                   *fout;
      Int_t                    fEvtNum;

      ClassDef(SimpleExampleMod, 1) // Simple example analysis module
  };
}
#endif
