#include <string>
#include <vector>

namespace ck::index { 
  // toml index object
  struct IndexObj {
    std::string uuid;
    std::vector<std::string> path;
  };
}
