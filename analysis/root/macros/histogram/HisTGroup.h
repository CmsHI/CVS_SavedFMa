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
    HisTGroup(TString gname, Int_t n=0, Float_t xmin=0, Float_t xmax=0);
    void Add(TString iname);
    void Add(TData * t1, TString iname,Float_t sc=-1);
    void Add(TFile * inFile, TString tname,TString iname, Float_t sc=-1);
    void Save();
    TData * Sum();
    TData * Average();

    // data
    TString name_;
    std::map<TString,TData*> hm_;
    std::map<TString,Float_t> scales_;

    // group properties
    Int_t nbins_;
    Float_t xmin_;
    Float_t xmax_;

    // group relations
    TData * hSum_;
    TData * hAve_;
};

template <typename TData>
HisTGroup<TData>::HisTGroup(TString name, Int_t n, Float_t xmin, Float_t xmax) :
  name_(name),
  nbins_(n),
  xmin_(xmin),
  xmax_(xmax),
  hSum_(0),
  hAve_(0)
{
  TData::SetDefaultSumw2();
}
#endif
