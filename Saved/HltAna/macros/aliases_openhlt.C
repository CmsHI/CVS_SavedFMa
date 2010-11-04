#include "TTree.h"

void aliases_openhlt(TTree * HltTree)
{
  HltTree->SetAlias("cent","hiBin*2.5");
}
