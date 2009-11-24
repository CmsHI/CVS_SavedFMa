#ifndef SELECTION_DATA
#define SELECTION_DATA

#include <TMatrixD.h>
#include "TString.h"
#include <vector>
#include <iostream>
#include "TFitter.h"

const Int_t NTYPES = 2;
const Int_t NTRIG = 4;

class FitData
{
  public:
    double bestX_;
    double bestY_;
    double searchDist_;
    double errorX_;
    double errorY_;
    TFitter * minimizer_;

    FitData() :
      bestX_(0.22),
      bestY_(0.775),
      searchDist_(0.3),
      errorX_(searchDist_),
      errorY_(searchDist_)
    {/*empty*/}
};

class SelectionData
{
  public:
    // trigger info
    // size: N
    std::vector<TString> trig_;
    std::vector<std::vector<Double_t> > eff_;

    //std::vector<TMatrixD> effTable0_;
    TMatrixD effTable0_;
    TMatrixD effTable1_;
    TMatrixD effSig_;

    // public functions
    void loadInput(const char * infile, TMatrixD & mat);
    void calcEffSigma();

    // --- Friend Functions ---
    friend ostream& operator <<(ostream& outs, const SelectionData& sd);
};

// -------------------- Implementations -----------------------
void SelectionData::loadInput(const char * infname, TMatrixD & mat)
{
  int ncol = 5;
  int nrow = 10;
  mat.ResizeTo(nrow,ncol);

  ifstream inFile(infname);
  // check
  if (!inFile) {
    cerr << "Unable to open " << infname << endl;
    exit(1);
  }
  // read
  TString trigName;
  Double_t effAll;
  Double_t effSD;
  Double_t effDD;
  Double_t effNSD;
  Double_t effND;
  int irow=0;
  while (inFile>>trigName &&
	 inFile>>effAll &&
	 inFile>>effSD &&
	 inFile>>effDD &&
	 inFile>>effNSD &&
	 inFile>>effND
	 ) {
    mat(irow,0)=effAll;
    mat(irow,1)=effSD;
    mat(irow,2)=effDD;
    mat(irow,3)=effNSD;
    mat(irow,4)=effND;
    ++irow;
  }
  //mat.Print();
}

void SelectionData::calcEffSigma()
{
  effTable0_.Print();
  effTable1_.Print();
}

// === Friend Functions ===
ostream& operator <<(ostream& os, const SelectionData& sd)
{
  using namespace std;
  for (UInt_t i=0; i<sd.eff_.size(); ++i) {
    os << sd.trig_[i] << ": ";
    for (UInt_t j=0; j<sd.eff_[i].size(); ++j) {
      os << sd.eff_[i][j] << " ";
    }
    os << std::endl;
  }
  return os;
}

void readInputs(const char * infname, SelectionData & sdata)
{
  ifstream inFile(infname);
  // check
  if (!inFile) {
    cerr << "Unable to open " << infname << endl;
    exit(1);
  }
  // read
  TString trigName;
  Double_t effAll;
  Double_t effSD;
  Double_t effDD;
  Double_t effNSD;
  Double_t effND;
  while (inFile>>trigName &&
	 inFile>>effAll &&
	 inFile>>effSD &&
	 inFile>>effDD &&
	 inFile>>effNSD &&
	 inFile>>effND
	 ) {
    //cout << trigName << ": " << effAll << " " << effSD << " " << effDD << " " << effNSD << " " << effND << endl;
    sdata.trig_.push_back(trigName);
    vector<Double_t> veff;
    if (effAll>1) {
      effAll=effAll*0.01;
      effSD=effSD*0.01;
      effDD=effDD*0.01;
      effNSD=effNSD*0.01;
      effND=effND*0.01;
    }
    veff.push_back(effAll);
    veff.push_back(effSD);
    veff.push_back(effDD);
    veff.push_back(effNSD);
    veff.push_back(effND);
    sdata.eff_.push_back(veff);
  }
  //cout << sdata << endl;
  inFile.close();
}

#endif // SELECTION_DATA
