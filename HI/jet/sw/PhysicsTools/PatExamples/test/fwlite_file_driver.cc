#include "/home/frankma/work/HI/jet/sw/pat/patanaCMSSW_3_3_1/src/PhysicsTools/PatExamples/macros/fwlite_file_tools.h"
#include "/home/frankma/UserCode/SavedFMa/analysis/cpp/templates/stl_helper_fuctions.h"
#include <vector>
using namespace std;
using namespace mystd;
  
int main(int argc, char* argv[])
{
  if (argc==1) return 0;

  vector<string> vf;
  MakeInFileList(vf,argv[1]);
  print_elements(vf);
  return 0;
}
