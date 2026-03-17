#include <nlohmann/json.hpp>
#include <string>

#include "./types.hpp"

namespace ck::index { 
  Node* walk_path(Node*, const std::vector<std::string>);
}