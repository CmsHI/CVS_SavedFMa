#include "TreeDiJetEventData.h"
using namespace DiJetAna;

void treedijeteventdataDriver()
{
   TreeDiJetEventData td;
   printf("%f\n",td.nljet);
   printf("%d\n",td.ppid.size());
}
