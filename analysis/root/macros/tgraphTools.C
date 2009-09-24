#ifndef TGRAPHTOOLS_H
#define TGRAPHTOOLS_H
#endif //TGRAPHTOOLS_H

void tgraphTools() {}

void clearXErrorBar(TGraphAsymmErrors * gr)
{
   for (UInt_t i=0; i< gr->GetN(); ++i) {
      gr->SetPointEXhigh(i,0);
      gr->SetPointEXlow(i,0);
   }
}


