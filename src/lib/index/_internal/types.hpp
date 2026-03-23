#include <string>
#include <vector>

#include "lib/index/types.hpp"

namespace ck::index { 
  Node* walk_path(Node*, const std::vector<std::string>);
}
