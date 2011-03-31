#ifndef Helpers_h
#define Helpers_h
#include "TMath.h"

Double_t deltaPhi(Double_t phi1, Double_t phi2) { 
  Double_t result = phi1 - phi2;
  while (result > TMath::Pi()) result -= 2*TMath::Pi();
  while (result <= -TMath::Pi()) result += 2*TMath::Pi();
  return result;
}

Double_t deltaR2(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2) {
  Double_t deta = eta1 - eta2;
  Double_t dphi = deltaPhi(phi1, phi2);
  return deta*deta + dphi*dphi;
}

Double_t deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2) {
  return TMath::Sqrt(deltaR2 (eta1, phi1, eta2, phi2));
}

#endif // Helpers_h
