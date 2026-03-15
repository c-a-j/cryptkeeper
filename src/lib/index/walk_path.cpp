#include <nlohmann/json.hpp>
#include <string>

#include "lib/index/types.hpp"


namespace ck::index { 
  void walk_path(Node* node, Entry& entry, const std::vector<std::string> path) {
    for (std::size_t i = 0; i < path.size(); ++i) {
      const bool is_last = (i + 1 == path.size());
      
      if (node->entry && !is_last){
        throw std::runtime_error("entry already exists in the path");
      }
      
      node = &node->children[path[i]];
    }
    
    if (node->entry) {
      throw std::runtime_error("duplicate path");
    }
    
    if (!node->children.empty()) {
      throw std::runtime_error("path conflict");
    }
    
    node->entry = std::move(entry);
  }
}