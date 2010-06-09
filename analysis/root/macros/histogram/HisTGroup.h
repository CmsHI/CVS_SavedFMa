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
    void Save();
    TData * Sum();
    TData * Average();

    // data
    TString name_;
    std::map<TString,TData*> hm_;
    std::map<TString,Double_t> scales_;

    // group properties
    Int_t nbins_;
    Double_t xmin_;
    Double_t xmax_;
    Double_t ymin_;
    Double_t ymax_;

    // group relations
    TData * hSum_;
    TData * hAve_;
};

template <typename TData>
HisTGroup<TData>::HisTGroup(TString name, Int_t n, Double_t xmin, Double_t xmax) :
  name_(name),
  nbins_(n),
  xmin_(xmin),
  xmax_(xmax),
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
  TData * h1 = new TData(hname,hname,nbins_,xmin_,xmax_);
  Add(h1,iname);
}

#endif
