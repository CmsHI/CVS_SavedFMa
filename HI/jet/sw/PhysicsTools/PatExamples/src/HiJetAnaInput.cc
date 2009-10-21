// ana
#include "PhysicsTools/PatExamples/interface/HiJetAnaInput.h"
// stl
#include <map>

using namespace jetana;
using namespace std;

void HiJetAnaInput::LoadJets(TString anatype)
{
  map<TString,int> typecode;
  typecode["patjet"]=0;

  switch (typecode[anatype]) {
    case 0:
      cout << anatype << endl;
      break;
  }
}
