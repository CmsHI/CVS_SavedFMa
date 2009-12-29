// $Id: PixelsMakerMod.cc,v 1.18 2009/12/22 15:47:44 yjlee Exp $

#include <iostream>
#include "MitQcd/TrackletsAna/interface/PixelsMakerMod.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/PixelHitCol.h"
#include "MitAna/DataTree/interface/CaloTowerCol.h"
#include "MitAna/DataTree/interface/VertexCol.h"
#include <TNtuple.h>
#include <TFile.h>

using namespace std;
using namespace mithig;

ClassImp(mithig::PixelsMakerMod)

//--------------------------------------------------------------------------------------------------
void PixelsMakerMod::PixelEvent::Add(const MCParticle &mc)
{
  // Add MC particle.

  pt[nparticle] = mc.Pt();
  eta[nparticle] = mc.Eta();
  phi[nparticle] = mc.Phi();
  pdg[nparticle] = mc.PdgId();
  chg[nparticle] = (Int_t)mc.Charge();
  if(mc.HasMother()) 
    mother[nparticle] = mc.Mother()->PdgId();
  else 
    mother[nparticle] = -1;

  ++nparticle;
}

//--------------------------------------------------------------------------------------------------
void PixelsMakerMod::PixelEvent::Add(const PixelHit &hit)
{
  // Add pixel hit.

  ThreeVector v(hit.Position());  

  if (hit.Type() == PixelHit::kBLayer1) {
    eta1[nhits1]  = v.Eta();
    phi1[nhits1]  = v.Phi();
    r1[nhits1]    = v.Rho();
    cs1[nhits1]   = hit.SizeY();
    ch1[nhits1]   = hit.Charge();
    flag1[nhits1] = hit.IsAnyPixelOnEdge();
    ++nhits1;
  } else if (hit.Type() == PixelHit::kBLayer2) {
    eta2[nhits2]  = v.Eta();
    phi2[nhits2]  = v.Phi();
    r2[nhits2]    = v.Rho();
    cs2[nhits2]   = hit.SizeY();
    ch2[nhits2]   = hit.Charge();
    flag2[nhits2] = hit.IsAnyPixelOnEdge();
    ++nhits2;
  } else if (hit.Type() == PixelHit::kBLayer3) {
    eta3[nhits3]  = v.Eta();
    phi3[nhits3]  = v.Phi();
    r3[nhits3]    = v.Rho();
    cs3[nhits3]   = hit.SizeY();
    ch3[nhits3]   = hit.Charge();
    flag3[nhits3] = hit.IsAnyPixelOnEdge();
    ++nhits3;
  } else if (hit.Type() == PixelHit::kFNegDisk1 || hit.Type() == PixelHit::kFPosDisk1 ) {
    etaF1[nhitsF1]  = v.Eta();
    phiF1[nhitsF1]  = v.Phi();
    rF1[nhitsF1]    = v.Rho();
    csF1[nhitsF1]   = hit.SizeY();
    chF1[nhitsF1]   = hit.Charge();
    flagF1[nhitsF1] = hit.IsAnyPixelOnEdge();
    ++nhitsF1;
  } else if (hit.Type() == PixelHit::kFNegDisk2 || hit.Type() == PixelHit::kFPosDisk2 ) {
    etaF2[nhitsF2]  = v.Eta();
    phiF2[nhitsF2]  = v.Phi();
    rF2[nhitsF2]    = v.Rho();
    csF2[nhitsF2]   = hit.SizeY();
    chF2[nhitsF2]   = hit.Charge();
    flagF2[nhitsF2] = hit.IsAnyPixelOnEdge();
    ++nhitsF2;
  }
}

//--------------------------------------------------------------------------------------------------
void PixelsMakerMod::PixelEvent::Clear()
{
  // Clear event
  
  nRun      = 0;
  nEv       = 0;
  nLumi     = 0;
  nBX       = 0;
  nhits1    = 0;
  nhits1    = 0;
  nhits2    = 0;
  nhits3    = 0;
  nhitsF1   = 0;
  nhitsF2   = 0;
  nv        = 0;
  nHltBit   = 0;
  nparticle = 0;
  evtType   = 0;
}

//--------------------------------------------------------------------------------------------------
PixelsMakerMod::PixelsMakerMod(const char *name, const char *title, unsigned int nbunch) : 
  BaseMod(name,title),
  fPixelHitName(Names::gkPixelHitBrn),
  fCaloTowerName(Names::gkCaloTowerBrn),
  fHLTBitName(Names::gkHltBitBrn),
  fL1ABitName("L1AlgoBitsBeforeMask"),
  fL1TBitName("L1TechBitsBeforeMask"),
  fL1ABxsName("L1AlgoBitsBxs"),
  fL1TBxsName("L1TechBitsBxs"),
  fMCPartName(Names::gkMCPartBrn),
  fMCVertName("MCVertexes"),
  fMCEvtInfoName(Names::gkMCEvtInfoBrn),
  fIsMC(kFALSE),
  fPixelHitCol(0),
  fHltBits(0),
  fNbunch(nbunch),
  fMCPartCol(0),
  fMCVertCol(0),
  fMCEvtInfo(0),
  fTree(0),
  fFile(0),
  fHfOnly(true)
{
  // Constructor.
}

//------------------------------ helpers -----------------------------------------------
bool PixelsMakerMod::GoodTower(const CaloTower * ct)
{
  bool result=false;
  if (ct->E()>3) {
    if (fHfOnly)
      result=true;
    else if (fabs(ct->Eta())>3.)
      result=true;
  }
  return result;
}

//--------------------------------------------------------------------------------------------------
void PixelsMakerMod::Process()
{
  // Process entries of the tree. For this module, we just load the branches and
  // fill the histograms.

  fPE.Clear();

  // fill event info
  const EventHeader *eh = GetEventHeader();
  fPE.nRun  = eh->RunNum();
  fPE.nEv   = eh->EvtNum();
  fPE.nLumi = eh->LumiSec();
  fPE.nBX   = eh->BunchCrossing();

  // fill pixel info
  LoadEventObject(fPixelHitName, fPixelHitCol);
  if (!fPixelHitCol) {
    SkipEvent();
    return;
  }

  const UInt_t ents = fPixelHitCol->GetEntries();
  for (UInt_t i = 0; i < ents; ++i) {
    const PixelHit *hit = fPixelHitCol->At(i);
    fPE.Add(*hit);
  }


  // fill caloTower info
  LoadEventObject(fCaloTowerName, fCaloTowerCol);
  if (!fCaloTowerCol) {
    SkipEvent();
    return;
  }

  const UInt_t caloTowerEnts = fCaloTowerCol->GetEntries();
  fPE.nHFn = 0;
  fPE.nHFp = 0;
  fPE.SumEsubEp = 0;
  fPE.SumEaddEp = 0;
  fPE.SumEsubEpPos = 0;
  fPE.SumEaddEpPos = 0;
  fPE.SumEsubEpNeg = 0;
  fPE.SumEaddEpNeg = 0;
  if (fIsMC) {
    LoadBranch(fMCEvtInfoName);
    if (fMCEvtInfo) {
      Int_t pid = fMCEvtInfo->ProcessId();
      if (fPE.nEv%200==0)
	printf("Event %d, evtType %d\n",fPE.nEv,pid);
    }
  }
  for (UInt_t i = 0; i < caloTowerEnts; ++i) {
    const CaloTower *ct = fCaloTowerCol->At(i);
    if (ct->Eta()<-3 && ct->E()>3) fPE.nHFn++;
    if (ct->Eta()> 3 && ct->E()>3) fPE.nHFp++;
    // === diffractive study ===
    if (GoodTower(ct)) {
      // calc vars
      Double_t E=ct->E();
      Double_t Ep=ct->E()*cos(ct->Theta());
      if (fPE.nEv%200==0)
	printf("  E: %f, Ep: %f, E-Ep: %f, E+Ep: %f, 0.5ln((E+Ep)/(E-Ep)): %f, Eta: %f\n",E,Ep,E-Ep,E+Ep,0.5*log((E+Ep)/(E-Ep)),ct->Eta());
      fPE.SumEsubEp+=E-Ep;
      fPE.SumEaddEp+=E+Ep;

      // for +z observables
      if (ct->Eta()>0) {
	fPE.SumEsubEpPos+=E-Ep;
	fPE.SumEaddEpPos+=E+Ep;
      }
      // for +z observables
      if (ct->Eta()<0) {
	fPE.SumEsubEpNeg+=E-Ep;
	fPE.SumEaddEpNeg+=E+Ep;
      }
    } // end if good tower
  } // end loop on towers in event
  fPE. evtEta = 0.5*log(fPE.SumEaddEp/fPE.SumEsubEp);


  // fill vertex info
  fPE.vx[0] = -9999; // this one is reserved for MC
  fPE.vy[0] = -9999; // this one is reserved for MC
  fPE.vz[0] = -9999; // this one is reserved for MC
  fPE.nv = 1;
  for (size_t i=0; i<fVertexNames.size(); ++i) {
    LoadEventObject(fVertexNames.at(i).c_str(), fVertexCol[i]);
    fPE.vx[fPE.nv] = -9999;
    fPE.vy[fPE.nv] = -9999;
    fPE.vz[fPE.nv] = -9999;
    if ((!fVertexCol[i]) || (fVertexCol[i]->GetEntries() < 1)) {
      ++fPE.nv;
      continue;
    }
    const Vertex *mainVtx = fVertexCol[i]->At(0);
    Double_t ncount = mainVtx->NTracks();
    for (UInt_t j = 0; j< fVertexCol[i]->GetEntries(); ++j) {
      const Vertex *tmp = fVertexCol[i]->At(j);
      if (ncount<tmp->NTracksFit()) {
        ncount = tmp->NTracksFit();
        mainVtx = tmp;
      }
    }      
    if (mainVtx) {
      fPE.vx[fPE.nv] = mainVtx->X();
      fPE.vy[fPE.nv] = mainVtx->Y();
      fPE.vz[fPE.nv] = mainVtx->Z();
    }
    ++fPE.nv;
  }

  // fill HLT info
  LoadBranch(fHLTBitName);
  fPE.nHltBit = fHltBits->Size();
  for (UInt_t i=0; i<fHltBits->Size(); ++i){
    fPE.hltBit[i] = fHltBits->At(i);
  }


  // fill L1 info
  LoadBranch(fL1ABitName);
  LoadBranch(fL1TBitName);

  fPE.nL1A = fL1ABits->Size();
  for (UInt_t i=0; i<fL1ABits->Size(); ++i){
    fPE.L1A[i] = fL1ABits->TestBit(i);
  }

  fPE.nL1T = fL1TBits->Size();
  for (UInt_t i=0; i<fL1TBits->Size(); ++i){
    fPE.L1T[i] = fL1TBits->TestBit(i);
  }

  if (!fIsMC) {
    LoadBranch(fL1ABxsName);
    LoadBranch(fL1TBxsName);
    for (UInt_t i=0; i<fL1ABits->Size(); ++i){
      for (UInt_t j=0; j<fNbunch; ++j){
	fPE.L1AvsBX[i][j] = (fL1ABitBxs->At(j))->TestBit(i);
      }
    }
    
    for (UInt_t i=0; i<fL1TBits->Size(); ++i){
      for (UInt_t j=0; j<fNbunch; ++j){
	fPE.L1TvsBX[i][j] = (fL1TBitBxs->At(j))->TestBit(i);
      }
    }
  }
  
  // fill MC info
  if (fIsMC) {
    LoadBranch(fMCPartName);
    if (fMCPartCol) {
      const UInt_t ments = fMCPartCol->GetEntries();
      for (UInt_t i = 0; i < ments; ++i) {
        const MCParticle *mc = fMCPartCol->At(i);
        if (mc->Status()!=1)
          continue;
        fPE.Add(*mc);
      }
    }

    LoadBranch(fMCVertName);
    if (fMCVertCol) {
      const Vertex *mainVtx = fMCVertCol->At(0);
      if (mainVtx) {
        fPE.vx[0] = mainVtx->X();
	fPE.vy[0] = mainVtx->Y();
        fPE.vz[0] = mainVtx->Z();
      }
    }
    LoadBranch(fMCEvtInfoName);
    if (fMCEvtInfo) {
      Int_t pid = fMCEvtInfo->ProcessId();
      fPE.evtType = pid;
    }
  }

  if(fTree) 
    fTree->Fill();
}

//--------------------------------------------------------------------------------------------------
void PixelsMakerMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual analysis. Here,
  // we typically initialize histograms and other analysis objects and request
  // branches. For this module, we request a branch of the MitTree.
  
  for (size_t i=0; i<fVertexNames.size(); ++i) {
    ReqEventObject(fVertexNames.at(i).c_str(), fVertexCol[i], kTRUE);
  }
  ReqEventObject(fPixelHitName, fPixelHitCol, kTRUE);
  ReqEventObject(fCaloTowerName, fCaloTowerCol, kTRUE);
  ReqBranch(fHLTBitName, fHltBits);
  ReqBranch(fL1ABitName, fL1ABits);
  ReqBranch(fL1TBitName, fL1TBits);

  if (fIsMC) {
    ReqBranch(fMCPartName,    fMCPartCol);
    ReqBranch(fMCVertName,    fMCVertCol);
    ReqBranch(fMCEvtInfoName, fMCEvtInfo);
  } else {
    ReqBranch(fL1ABxsName, fL1ABitBxs);
    ReqBranch(fL1TBxsName, fL1TBitBxs);
  }

  if (!fFileName.IsNull())
    fFile = TFile::Open(fFileName,"recreate","",6);

  if (!fFile) {
    SendError(kAbortAnalysis,"SlaveBegin","Could not (re-)create file with name %s", 
              fFileName.Data());
    return;
  }

  TDirectory::TContext context(fFile); 
  fTree = new TTree("PixelTree","Tree of Pixel Hits");
  fTree->SetDirectory(fFile);
  fTree->Branch("nRun",&fPE.nRun,"nRun/I");
  fTree->Branch("nEv",&fPE.nEv,"nEv/I");
  fTree->Branch("nLumi",&fPE.nLumi,"nLumi/I");
  fTree->Branch("nBX",&fPE.nBX,"nBX/I");
  fTree->Branch("nhits1",&fPE.nhits1,"nhits1/I");
  fTree->Branch("nhits2",&fPE.nhits2,"nhits2/I");
  fTree->Branch("nhits3",&fPE.nhits3,"nhits3/I");
  fTree->Branch("nhitsF1",&fPE.nhitsF1,"nhitsF1/I");
  fTree->Branch("nhitsF2",&fPE.nhitsF2,"nhitsF2/I");
  fTree->Branch("nv",&fPE.nv,"nv/I");
  fTree->Branch("vx",fPE.vx,"vx[nv]/F");
  fTree->Branch("vy",fPE.vy,"vy[nv]/F");
  fTree->Branch("vz",fPE.vz,"vz[nv]/F");
  fTree->Branch("eta1",fPE.eta1,"eta1[nhits1]/F");
  fTree->Branch("phi1",fPE.phi1,"phi1[nhits1]/F");
  fTree->Branch("r1",fPE.r1,"r1[nhits1]/F");
  fTree->Branch("cs1",fPE.cs1,"cs1[nhits1]/F");
  fTree->Branch("ch1",fPE.ch1,"ch1[nhits1]/F");
  fTree->Branch("fl1",fPE.flag1,"fl1[nhits1]/O");
  fTree->Branch("eta2",fPE.eta2,"eta2[nhits2]/F");
  fTree->Branch("phi2",fPE.phi2,"phi2[nhits2]/F");
  fTree->Branch("r2",fPE.r2,"r2[nhits2]/F");
  fTree->Branch("cs2",fPE.cs2,"cs2[nhits2]/F");
  fTree->Branch("ch2",fPE.ch2,"ch2[nhits2]/F");
  fTree->Branch("fl2",fPE.flag2,"fl2[nhits2]/O");
  fTree->Branch("eta3",fPE.eta3,"eta3[nhits3]/F");
  fTree->Branch("phi3",fPE.phi3,"phi3[nhits3]/F");
  fTree->Branch("r3",fPE.r3,"r3[nhits3]/F");
  fTree->Branch("cs3",fPE.cs3,"cs3[nhits3]/F");
  fTree->Branch("ch3",fPE.ch3,"ch3[nhits3]/F");
  fTree->Branch("fl3",fPE.flag3,"fl3[nhits3]/O");
  fTree->Branch("etaF1",fPE.etaF1,"etaF1[nhitsF1]/F");
  fTree->Branch("phiF1",fPE.phiF1,"phiF1[nhitsF1]/F");
  fTree->Branch("rF1",fPE.rF1,"rF1[nhitsF1]/F");
  fTree->Branch("csF1",fPE.csF1,"csF1[nhitsF1]/F");
  fTree->Branch("chF1",fPE.chF1,"chF1[nhitsF1]/F");
  fTree->Branch("flF1",fPE.flagF1,"flF1[nhitsF1]/O");
  fTree->Branch("etaF2",fPE.etaF2,"etaF2[nhitsF2]/F");
  fTree->Branch("phiF2",fPE.phiF2,"phiF2[nhitsF2]/F");
  fTree->Branch("rF2",fPE.rF2,"rF2[nhitsF2]/F");
  fTree->Branch("csF2",fPE.csF2,"csF2[nhitsF2]/F");
  fTree->Branch("chF2",fPE.chF2,"chF2[nhitsF2]/F");
  fTree->Branch("flF2",fPE.flagF2,"flF2[nhitsF2]/O");
  fTree->Branch("nHltBit",&fPE.nHltBit,"nHltBit/I");
  fTree->Branch("nL1A",&fPE.nL1A,"nL1A/I");
  fTree->Branch("nL1T",&fPE.nL1T,"nL1T/I");
  fTree->Branch("hltBit",fPE.hltBit,"hltBit[nHltBit]/O");
  fTree->Branch("L1A",fPE.L1A,"L1A[nL1A]/O");
  fTree->Branch("L1T",fPE.L1T,"L1T[nL1T]/O");
  fTree->Branch("nHFn",&fPE.nHFn,"nHFn/I");
  fTree->Branch("nHFp",&fPE.nHFp,"nHFp/I");
  // diffractive study
  fTree->Branch("SumEsubEp",&fPE.SumEsubEp,"SumEsubEp/F");
  fTree->Branch("SumEaddEp",&fPE.SumEaddEp,"SumEaddEp/F");
  fTree->Branch("SumEsubEpPos",&fPE.SumEsubEpPos,"SumEsubEpPos/F");
  fTree->Branch("SumEaddEpPos",&fPE.SumEaddEpPos,"SumEaddEpPos/F");
  fTree->Branch("SumEsubEpNeg",&fPE.SumEsubEpNeg,"SumEsubEpNeg/F");
  fTree->Branch("SumEaddEpNeg",&fPE.SumEaddEpNeg,"SumEaddEpNeg/F");
  fTree->Branch("evtEta",&fPE.evtEta,"evtEta/F");

  if (!fIsMC) {
    fTree->Branch("L1AvsBX",fPE.L1AvsBX,Form("L1AvsBX[nL1A][%d]/O",fNbunch));
    fTree->Branch("L1TvsBX",fPE.L1TvsBX,Form("L1TvsBX[nL1T][%d]/O",fNbunch));
  }else{
    fTree->Branch("evtType",&fPE.evtType,"evtType/I");
    fTree->Branch("npart",&fPE.nparticle,"npart/I");
    fTree->Branch("pt",fPE.pt,"pt[npart]/F");
    fTree->Branch("eta",fPE.eta,"eta[npart]/F");
    fTree->Branch("phi",fPE.phi,"phi[npart]/F");
    fTree->Branch("pdg",fPE.pdg,"pdg[npart]/I");
    fTree->Branch("chg",fPE.chg,"chg[npart]/I");
    fTree->Branch("mother",fPE.mother,"mother[npart]/I");
  }
}

//--------------------------------------------------------------------------------------------------
void PixelsMakerMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the analysis. For this
  // module, we dont do anything here.

  if (fTree) {
    TDirectory::TContext context(fFile); 
    fTree->Write();
    fFile->Close();
    delete fFile;
    fTree = 0;
  }
}
