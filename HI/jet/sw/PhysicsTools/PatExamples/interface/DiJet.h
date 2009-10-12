#include "TLorentzVector.h"

class DiJet
{
  public:
    DiJet();

  private:
    TLorentzVector j1_;
    TLorentzVector j2_;
    Double_t dphi_;
};
