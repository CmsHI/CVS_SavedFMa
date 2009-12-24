// $Id: SimpleExampleMod.cc,v 1.6 2009/07/10 14:17:08 loizides Exp $

#include "MitAna/PhysicsMod/interface/SimpleExampleMod.h"
#include "MitAna/DataTree/interface/Names.h"
#include "MitAna/DataTree/interface/MCParticleCol.h"
#include "MitAna/DataTree/interface/CaloTower.h"
#include <TH1D.h>
#include <TNtuple.h>
#include <TFile.h>
#include <cmath>

using namespace mithep;
using namespace std;

ClassImp(mithep::SimpleExampleMod)

  //--------------------------------------------------------------------------------------------------
  SimpleExampleMod::SimpleExampleMod(const char *name, const char *title) : 
    BaseMod(name,title),
    fPartName(Names::gkMCPartBrn),
    fTowerName(Names::gkCaloTowerBrn),
    fMCEvtName(Names::gkMCEvtInfoBrn),
    fParticles(0),
    fTowers(0),
    fPtHist(0),
    fEtaHist(0),
    fEvtNum(0)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::Begin()
{
  // Run startup code on the client machine. For this module, we dont
  // do anything here.
}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::BeginRun()
{
  // Run startup code on the client machine. For this module, we dont
  // do anything here.
}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::EndRun()
{
  // Run startup code on the client machine. For this module, we dont
  // do anything here.
}

//------------------------------ helpers -----------------------------------------------
bool SimpleExampleMod::GoodTower(const CaloTower * ct)
{
  bool result=false;
  if (ct->E()>3)
    result=true;
  return result;
}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::Process()
{
  // Process entries of the tree. For this module, we just load
  // the branch and fill the histograms.

  //LoadBranch(GetPartName());
  LoadBranch(GetTowerName());
  LoadBranch(GetMCEvtName());

  Int_t ents=fTowers->GetEntries();
  Double_t SumEsubEp = 0;
  Double_t SumEaddEp = 0;
  Double_t SumEsubEpPos = 0;
  Double_t SumEaddEpPos = 0;
  Double_t SumEsubEpNeg = 0;
  Double_t SumEaddEpNeg = 0;

  printf("Evt %d, evtType: %d\n",fEvtNum,fMCEvt->ProcessId());
  for(Int_t i=0;i<ents;++i) {
    const CaloTower *ct = fTowers->At(i);
    fPtHist->Fill(ct->Pt());
    fEtaHist->Fill(ct->Eta());
    fhEUncut->Fill(ct->E());
    if (GoodTower(ct)) {
      // calc vars
      Double_t E=ct->E();
      Double_t Ep=ct->E()*cos(ct->Theta());

      SumEsubEp+=E-Ep;
      SumEaddEp+=E+Ep;

      // === Fill ===
      fhE->Fill(E);
      fhEp->Fill(Ep);
      fhEsubEp->Fill(E-Ep);
      fhEaddEp->Fill(E+Ep);
      if (fEvtNum%100==0) {
	printf("  E: %f, Ep: %f, E-Ep: %f, E+Ep: %f, 0.5ln((E+Ep)/(E-Ep)): %f, Eta: %f\n",E,Ep,E-Ep,E+Ep,0.5*log((E+Ep)/(E-Ep)),ct->Eta());
      }

      // for +z observables
      if (ct->Eta()>0) {
	fhEpPos->Fill(Ep);
	fhEpComb->Fill(Ep);
	fhEsubEpPos->Fill(E-Ep);
	fhEaddEpPos->Fill(E+Ep);
	SumEsubEpPos+=E-Ep;
	SumEaddEpPos+=E+Ep;
      }

      // for +z observables
      if (ct->Eta()<0) {
	fhEpNeg->Fill(Ep);
	fhEpComb->Fill(-1*Ep);
	fhEsubEpNeg->Fill(E-Ep);
	fhEaddEpNeg->Fill(E+Ep);
	SumEsubEpNeg+=E-Ep;
	SumEaddEpNeg+=E+Ep;
      }
    } // end if good tower
  } // end loop on towers in event

  // fill event level vars
  Double_t evtEta = 0.5*log(SumEaddEp/SumEsubEp);
  if (fEvtNum%100==0)
    printf("  Sum(E-Ep): %f, Sum(E+Ep): %f, 0.5ln((E+Ep)/(E-Ep)): %f\n",SumEsubEp,SumEaddEp,evtEta);
  fntDiffrac->Fill(fEvtNum,fMCEvt->ProcessId(),
      SumEaddEp,SumEsubEp,SumEaddEpPos,SumEsubEpPos,SumEaddEpNeg,SumEsubEpNeg,
      evtEta);
  fhSumEsubEp->Fill(SumEsubEp);
  fhSumEaddEp->Fill(SumEaddEp);
  fhSumEsubEpPos->Fill(SumEsubEpPos);
  fhSumEaddEpPos->Fill(SumEaddEpPos);
  fhSumEsubEpNeg->Fill(SumEsubEpNeg);
  fhSumEaddEpNeg->Fill(SumEaddEpNeg);
  ++fEvtNum;
}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::SlaveBegin()
{
  // Run startup code on the computer (slave) doing the actual
  // analysis. Here, we typically initialize histograms and 
  // other analysis objects and request branches. For this module,
  // we request a branch of the MitTree.

  ReqBranch(GetPartName(), fParticles);
  ReqBranch(GetTowerName(), fTowers);
  ReqBranch(GetMCEvtName(), fMCEvt);

  AddTH1(fPtHist,"hPtHist",";p_{t} [GeV];#",100,0.,25.);
  AddTH1(fEtaHist,"hEtaHist",";#eta;#",160,-8.,8.);

  AddTH1(fhEUncut,"hEUncut",";E [GeV];#",100,0,100);
  AddTH1(fhE,"hE",";E [GeV];#",100,0,100);
  AddTH1(fhEp,"hEp",";Ez [GeV];#",200,-100,100);
  AddTH1(fhEpComb,"hEpComb",";Ez [GeV];#",200,-100,100);
  AddTH1(fhEpPos,"hEpPos",";Ez (z>0) [GeV];#",200,-100,100);
  AddTH1(fhEpNeg,"hEpNeg",";Ez (z<0) [GeV];#",200,-100,100);

  AddTH1(fhEsubEp,"hEsubEp",";E-Ez [GeV]; #",100,-50,50);
  AddTH1(fhEaddEp,"hEaddEp",";E+Ez [GeV]; #",100,-50,50);
  AddTH1(fhEsubEpPos,"hEsubEpPos",";E-Ez (+z) [GeV]; #",100,-50,50);
  AddTH1(fhEaddEpPos,"hEaddEpPos",";E-Ez (+z) [GeV]; #",100,-50,50);
  AddTH1(fhEsubEpNeg,"hEsubEpNeg",";E+Ez (-z) [GeV]; #",100,-50,50);
  AddTH1(fhEaddEpNeg,"hEaddEpNeg",";E+Ez (-z) [GeV]; #",100,-50,50);

  AddTH1(fhSumEsubEp,"hSumEsubEp",";#Sigma E-Ez [GeV]; #",610,-10,600);
  AddTH1(fhSumEaddEp,"hSumEaddEp",";#Sigma E+Ez [GeV]; #",610,-10,600);
  AddTH1(fhSumEsubEpPos,"hSumEsubEpPos",";#Sigma E-Ez (+z) [GeV]; #",610,-10,600);
  AddTH1(fhSumEaddEpPos,"hSumEaddEpPos",";#Sigma E+Ez (+z) [GeV]; #",610,-10,600);
  AddTH1(fhSumEsubEpNeg,"hSumEsubEpNeg",";#Sigma E-Ez (-z) [GeV]; #",610,-10,600);
  AddTH1(fhSumEaddEpNeg,"hSumEaddEpNeg",";#Sigma E+Ez (-z) [GeV]; #",610,-10,600);

  fout = new TFile("diffracAna.root","RECREATE");
  fntDiffrac = new TNtuple("ntDif","ntuple for diffraction ana",
      "evt:evtType:sumAdd:sumSub:sumAddPos:sumSubPos:sumAddNeg:sumSubNeg:Eta");
}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::SlaveTerminate()
{
  // Run finishing code on the computer (slave) that did the
  // analysis. For this module, we dont do anything here.
  fout->Write();
}

//--------------------------------------------------------------------------------------------------
void SimpleExampleMod::Terminate()
{
  // Run finishing code on the client computer. 
  // For this module, we dont do anything here.
}
