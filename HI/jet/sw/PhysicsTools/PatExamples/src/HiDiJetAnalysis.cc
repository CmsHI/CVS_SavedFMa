#include "PhysicsTools/PatExamples/interface/HiDiJetAnalysis.h"
// root
#include "TFile.h"
#include "TTree.h"

using namespace std;

namespace jetana
{
  HiDiJetAnalysis::HiDiJetAnalysis()
  {
    TTree * tree_ = new TTree("dijetTree","dijet tree");
    jd_.SetTree(tree_);
  }
} // jetana namespace
