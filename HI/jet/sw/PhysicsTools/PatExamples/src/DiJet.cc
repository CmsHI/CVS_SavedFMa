#include <iostream>
#include "PhysicsTools/PatExamples/interface/DiJet.h"
using namespace std;

DiJet::DiJet()
  : dphi_(-1)
{
  // empty
}

// === Friend Functions ===
ostream& operator <<(ostream& os, const DiJet& dj)
{
  os << "near: " << dj.nj_ << "  away: " << dj.aj_ << "  cm: " << dj.cm_ << "  dphi: " << dj.dphi_ << "  mass:" << dj.cm_.M();
  return os;
}
