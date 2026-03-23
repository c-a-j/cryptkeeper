#include <string>
#include <vector>

#include "util/error.hpp"
#include "../path/join.hpp"
#include "../path/parse_path.hpp"
#include "lib/index/types.hpp"

namespace {
  std::string along_path_msg(const std::vector<std::string>& path, std::size_t i) {
    std::string msg;
    msg += ck::path::join(path);
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
  Node* Index::break_trail(const std::vector<std::string>& path_parts) {
    Node* node = &this->root_;
    for (std::size_t i = 0; i < path_parts.size(); ++i) {
      if (node->entry){
        throw Error<IndexErrc>{PathConflict, along_path_msg(path_parts, i)};
      }
      
      node = &node->children[path_parts[i]];
    }
    
    if (node->entry) {
      throw Error<IndexErrc>{SecretExists, ck::path::join(path_parts)};
    }
    
    if (!node->children.empty()) {
      throw Error<IndexErrc>{PathConflict, ck::path::join(path_parts) + " -> has children"};
    }
    return node;
  }

  Node* Index::break_trail(const std::string& path) {
    return this->break_trail(ck::path::parse_path(path));
  }

  Node* Index::walk_path(const std::vector<std::string>& path_parts) {
    Node* node = &this->root_;
    for (std::size_t i = 0; i < path_parts.size(); ++i) {
      if (node->entry){
        throw Error<IndexErrc>{PathConflict, along_path_msg(path_parts, i)};
      }
      
      node = &node->children[path_parts[i]];
    }
    
    if (node->entry) {
      throw Error<IndexErrc>{SecretExists, ck::path::join(path_parts)};
    }
    
    if (!node->children.empty()) {
      throw Error<IndexErrc>{PathConflict, ck::path::join(path_parts) + " -> has children"};
    }
    return node;
  }

  Node* Index::walk_path(const std::string& path) {
    return this->walk_path(ck::path::parse_path(path));
  }

  Node* Index::get_parent(const std::vector<std::string>& path_parts) {
    std::vector<std::string> p;
    p.reserve(path_parts.size() - 1);
    for (size_t i = 0; i < path_parts.size() - 1; ++i) {
      p.push_back(path_parts[i]);
    }
    return this->walk_path(p);
  }

  Node* Index::get_parent(const std::string& path) {
    return this->get_parent(ck::path::parse_path(path));
  }
}
