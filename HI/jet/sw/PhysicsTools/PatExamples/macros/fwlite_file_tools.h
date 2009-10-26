#include<vector>
#include<string>
#include<fstream>

void MakeInFileList(std::vector<std::string> & vf, const char* flist)
{
  std::ifstream ins(flist);
  char line[1000];
  while (ins>>line) {
    vf.push_back(line);
  }
}

