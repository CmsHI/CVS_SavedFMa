#ifndef SELECTION_DATA
#define SELECTION_DATA

#include <TMatrixD.h>
#include "TString.h"
#include <vector>
#include <iostream>

const Int_t NTYPES = 2;
const Int_t NTRIG = 4;

class FitData
{
  public:
    double bestX_;
    double bestY_;
    double searchDist_;
    FitData() :
      bestX_(0.22),
      bestY_(0.775),
      searchDist_(0.2)
    {/*empty*/}
};

class SelectionData
{
  public:
    // trigger info
    // size: N
    std::vector<TString> trig_;
    std::vector<std::vector<Double_t> > eff_;

    // equations
    // size: N choose s
    std::vector<TMatrixD> coeff_;
    std::vector<TMatrixD> rhs_;
    std::vector<TMatrixD> sol_;

    // --- Friend Functions ---
    friend ostream& operator <<(ostream& outs, const SelectionData& sd);
};

// -------------------- Implementations -----------------------
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
