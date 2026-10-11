#include <string>
#include <vector>

#include "lib/index/index.hpp"

namespace ck::index::tree { 
  Node* break_trail(Node*, const std::vector<std::string>&);
  Node* break_trail(Node*, const std::string&);

  Node* walk_path(Node*, const std::vector<std::string>&);
  Node* walk_path(Node*, const std::string&);

  Node* get_parent(Node*, const std::vector<std::string>&);
  Node* get_parent(Node*, const std::string&);

  bool secret_along_path(Node*, const std::vector<std::string>&);
  bool secret_along_path(Node*, const std::string&);
}
