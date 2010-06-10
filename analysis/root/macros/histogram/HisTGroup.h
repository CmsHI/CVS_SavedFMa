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
    HisTGroup(TString name, Int_t n=0, Double_t xmin=0, Double_t xmax=0);
    void Add(TData * t1, TString iname,Double_t sc=-1);
    void Add(TString iname);
    void Add(TFile * inFile, TString hname,TString iname, Double_t sc=-1);
    void Print();
    void Save();
    TData * Sum();
    TData * Average();

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

template <typename TData>
HisTGroup<TData>::HisTGroup(TString name, Int_t n, Double_t xmin, Double_t xmax) :
  name_(name),
  xnbins_(n),
  xmin_(xmin),
  xmax_(xmax),
  ynbins_(-1),
  ymin_(-1),
  ymax_(-1),
  hSum_(0),
  hAve_(0)
{
  TData::SetDefaultSumw2();
}

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
void HisTGroup<TData>::Add(TString iname)
{
  TString hname = TString("h")+name_+iname;
  TData * h1 = new TData(hname,hname,xnbins_,xmin_,xmax_);
  Add(h1,iname);
}

template <typename TData>
void HisTGroup<TData>::Print()
{
  std::cout << "group: " << name_ << std::endl;
  std::cout << "xnbins: " << xnbins_ << " xmin: " << xmin_ << " xmax: " << xmax_ << " hSum_: " << hSum_ << " hAve_: " << hAve_ << std::endl;
  if (ynbins_>0)
    std::cout << "ynbins: " << ynbins_ << " ymin: " << ymin_ << " ymax: " << ymax_ << " hSum_: " << hSum_ << " hAve_: " << hAve_ << std::endl;
}

#endif
