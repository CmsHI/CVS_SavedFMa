#ifndef HisTGROUP_H
#define HisTGROUP_H

#include <map>
#include <cassert>
#include "TString.h"
#include "TFile.h"
#include <iostream>

template <typename TData>
class HisTGroup
{
  public:
    // methods
    HisTGroup(TString name,
	Int_t xn=-1, Double_t xmin=-1, Double_t xmax=-1,
	Int_t yn=-1, Double_t ymin=-1, Double_t ymax=-1);
    void Add(TData * h1, TString iname,Double_t sc=-1);
    void Add1D(TString iname);
    void Add2D(TString iname);
    void Add(TFile * inFile, TString hname,TString iname, Double_t sc=-1);
    TData * Average();
    void Print();
    void Save();
    TData * Sum();

    // data
    TString name_;
    std::map<TString,TData*> hm_;
    std::map<TString,Double_t> scales_;

    // group properties
    Int_t xnbins_;
    Double_t xmin_;
    Double_t xmax_;
    Int_t ynbins_;
    Double_t ymin_;
    Double_t ymax_;

    // group relations
    TData * hSum_;
    TData * hAve_;
};

// === Constructors ===
template <typename TData>
HisTGroup<TData>::HisTGroup(TString name,
    Int_t xn, Double_t xmin, Double_t xmax,
    Int_t yn, Double_t ymin, Double_t ymax) :
  name_(name),
  xnbins_(xn),
  xmin_(xmin),
  xmax_(xmax),
  ynbins_(yn),
  ymin_(ymin),
  ymax_(ymax),
  hSum_(0),
  hAve_(0)
{
  TData::SetDefaultSumw2();
}

// === Adding to Group ===
template <typename TData>
void HisTGroup<TData>::Add(TData * h1, TString iname, Double_t sc)
{
  assert(h1);
  scales_[iname] = sc;

  if (sc<0) {
    hm_[iname] = h1;
  }

  if (sc>0) {
    TString schname = TString("sc")+h1->GetName();
    TData * hsc = (TData*)h1->Clone(schname);
    hsc->Scale(sc);
    hm_[iname] = hsc;
  }
}

template <typename TData>
void HisTGroup<TData>::Add1D(TString iname)
{
  TString hname = TString("h")+name_+iname;
  TData * h1 = 0;
  if (xnbins_>0)
    h1 = new TData(hname,hname,xnbins_,xmin_,xmax_);
  assert(h1);
  Add(h1,iname);
}

template <typename TData>
void HisTGroup<TData>::Add2D(TString iname)
{
  TString hname = TString("h")+name_+iname;
  TData * h1 = 0;
  if (xnbins_>0 && ynbins_>0)
    h1 = new TData(hname,hname,xnbins_,xmin_,xmax_,ynbins_,ymin_,ymax_);
  assert(h1);
  Add(h1,iname);
}

template <typename TData>
void HisTGroup<TData>::Add(TFile * inFile, TString hname,TString iname, Double_t sc)
{
  assert(inFile);
  TData * h1;
  inFile->GetObject(hname.Data(),h1);
  Add(h1,iname,sc);
}

// === Helper Functions ===
template <typename TData>
void HisTGroup<TData>::Print()
{
  std::cout << "group: " << name_ << std::endl;
  std::cout << "xnbins: " << xnbins_ << " xmin: " << xmin_ << " xmax: " << xmax_ << " hSum_: " << hSum_ << " hAve_: " << hAve_ << std::endl;
  if (ynbins_>0)
    std::cout << "ynbins: " << ynbins_ << " ymin: " << ymin_ << " ymax: " << ymax_ << " hSum_: " << hSum_ << " hAve_: " << hAve_ << std::endl;
}

#endif
