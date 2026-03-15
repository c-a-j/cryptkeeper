#include <nlohmann/json.hpp>
#include <string>

#include "lib/index/types.hpp"


namespace ck::index { 
  void walk_path(Node*, Entry&, const std::vector<std::string>);
}