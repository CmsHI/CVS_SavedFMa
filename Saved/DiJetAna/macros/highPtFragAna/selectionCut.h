#ifndef SelectionCut_H
#define SelectionCut_H

#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include "TH1.h"
#include <TCut.h>
#include <TString.h>
#include "TMath.h"
#include "TTree.h"
using namespace std;

class selectionCut
{  
  public:
    selectionCut(Double_t et1min=120, Double_t et1max=250);
    ~selectionCut(){}

    // -- ana ranges --
    Double_t CentMin;
    Double_t CentMax;
    vector<Double_t> JEtMin;
    vector<Double_t> JEtMax;
    vector<Double_t> JEtaMin;
    vector<Double_t> JEtaMax;
    Double_t JDPhiMin;
    Double_t AjMin;
    Double_t AjMax;
    // trk
    Double_t TrkPtMin;
    Double_t TrkEtaMax;
    // cone
    Double_t ConeSize;
};

selectionCut::selectionCut(Double_t et1min, Double_t et1max) :
  CentMin(0),
  CentMax(30),
  JEtMin(2),
  JEtMax(2),
  JEtaMin(2),
  JEtaMax(2),
  JDPhiMin(TMath::Pi()*2./3),
  AjMin(0),
  AjMax(1),
  TrkPtMin(4),
  TrkEtaMax(2.4),
  ConeSize(0.5)
{
  for (Int_t j=0; j<2; ++j) {
    JEtMin[j] = et1min;
    JEtMax[j] = et1max;
    JEtMin[j] = -2;
    JEtMax[j] = 2;
  }
}

#endif
