#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "util/error.hpp"
#include "./_internal/types.hpp"

namespace {
  std::string join_path(const std::vector<std::string>& components) {
    std::string path;
    bool first = true;
    for (auto component : components) { 
      if (first) {
        path += component;
        first = false;
        continue;
      }
      path += "/" + component; 
    }
    return path;
  }
  
  std::string along_path_msg(const std::vector<std::string>& path, std::size_t i) {
    std::string msg;
    msg += join_path(path);
    msg += " -> the secret '";
    msg += (i > 0) ? path[i-1] : "";
    msg += "' already exists along this path";
    return msg;
  }
}

namespace ck::index { 
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  Node* walk_path(Node* node, const std::vector<std::string> path) {
    for (std::size_t i = 0; i < path.size(); ++i) {
      if (node->entry){
        throw Error<IndexErrc>{PathConflict, along_path_msg(path, i)};
      }
      
      node = &node->children[path[i]];
    }
    
    if (node->entry) {
      throw Error<IndexErrc>{SecretExists, join_path(path)};
    }
    
    if (!node->children.empty()) {
      throw Error<IndexErrc>{PathConflict, join_path(path) + " -> has children"};
    }
    return node;
  }
}
