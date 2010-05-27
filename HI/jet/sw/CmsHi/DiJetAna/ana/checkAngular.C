#include <iostream>
using namespace std;

Float_t deltaPhi(Float_t phi1, Float_t phi2)
{
  Float_t ans = phi2-phi1;
  if (ans>TMath::Pi()) ans=ans-TMath::TwoPi();
  if (ans<-1*TMath::Pi()) ans=ans+TMath::TwoPi();
  return ans;
}

void checkAngular(Float_t nljeta, Float_t nljphi,
    Float_t aljeta, Float_t aljphi,
    Float_t peta, Float_t pphi)
{
  Float_t pndphi, pndeta, padphi, padeta, pndr, padr;
  pndeta=peta-nljeta;
  pndphi=TMath::Abs(deltaPhi(pphi,nljphi));
  pndr = TMath::Sqrt(pndeta*pndeta+pndphi*pndphi);

  padeta=peta-aljeta;
  padphi=TMath::Abs(deltaPhi(pphi,aljphi));
  padr = TMath::Sqrt(padeta*padeta+padphi*padphi);

  cout << "Near: " << endl;
  cout << "NrJetEta|peta: " << nljeta << "|" << peta << endl;
  cout << "pndeta: " << pndeta << endl;
  cout << "NrJetPhi|pphi: " << nljphi << "|" << pphi << endl;
  cout << "pndphi: " << pndphi << endl;
  cout << "pndr:   " << pndr << endl;

  cout << endl << "Away: " << endl;
  cout << "AwJetEta|peta: " << aljeta << "|" << peta << endl;
  cout << "padeta: " << padeta << endl;
  cout << "AwJetPhi|pphi: " << aljphi << "|" << pphi << endl;
  cout << "padphi: " << padphi << endl;
  cout << "padr:   " << padr << endl;
}
