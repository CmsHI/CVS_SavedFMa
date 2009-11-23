#include <TMatrixD.h>
#include "TString.h"
#include <vector>
#include <iostream>

const Int_t NTYPES = 2;
const Int_t NTRIG = 4;

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

