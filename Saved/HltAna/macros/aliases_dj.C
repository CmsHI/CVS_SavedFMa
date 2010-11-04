#include "TTree.h"

void aliases_dj(TTree * djTree)
{
  djTree->SetAlias("pE","ppt*cosh(peta)");
  djTree->SetAlias("Hf","abs(peta)>3");
  djTree->SetAlias("HfP","(pE>3 && peta>3)");
  djTree->SetAlias("HfN","(pE>3 && peta<-3)");

  djTree->SetAlias("NHfP","Sum$(pE>3 && peta>3)");
  djTree->SetAlias("NHfN","Sum$(pE>3 && peta<-3)");
}
