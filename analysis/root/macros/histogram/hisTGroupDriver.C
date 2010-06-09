#include <iostream>
#include "HisTGroup.h"
#include "TH1D.h"
using namespace std;

void hisTGroupDriver()
{
  HisTGroup<TH1D> hgDj1("Dj1");
  cout << "group: " << hgDj1.name_ << endl;
  cout << "nbins: " << hgDj1.nbins_ << "xmin: " << hgDj1.xmin_ << " xmax: " << hgDj1.xmax_ << " " << hgDj1.hSum_ << " " << hgDj1.hAve_ << endl;
}
