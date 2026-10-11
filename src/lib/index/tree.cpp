#include <string>
#include <vector>

#include "lib/index/index.hpp"
#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "lib/path/parse_path.hpp"

#include "../path/internal/join.hpp"

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

namespace ck::index::tree { 
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;

  // used for clearing a trail through the intex tree
  // does not allow secrets anywhere along the path (secrets are always terminal nodes)
  // does not allow terminal node to have children (this is used for inserting entries)
  Node* break_trail(Node* root, const std::vector<std::string>& path_parts) {
    Node* node = root;
    for (std::size_t i = 0; i < path_parts.size(); ++i) {
      if (node->entry){
        logger.debug("break_trail()");
        throw Error<IndexErrc>{PathConflict, along_path_msg(path_parts, i)};
      }
      
      node = &node->children[path_parts[i]];
    }
    
    if (node->entry) {
      logger.debug("Index::break_trail()");
      throw Error<IndexErrc>{SecretExists, ck::path::join(path_parts)};
    }
    
    if (!node->children.empty()) {
      logger.debug("Index::break_trail()");
      throw Error<IndexErrc>{PathConflict, ck::path::join(path_parts) + " -> has children"};
    }
    return node;
  }

  Node* break_trail(Node* root, const std::string& path) {
    return break_trail(root, ck::path::parse_path(path));
  }

  // used for clearing a trail through the intex tree before inserting a mount
  // does not allow secrets anywhere along the path (extra check, Mounts::mount() already checks for this)
  // returns the last node in the path
  Node* walk_path(Node* root, const std::vector<std::string>& path_parts) {
    Node* node = root;
    for (std::size_t i = 0; i < path_parts.size(); ++i) {
      if (node->entry){
        logger.debug("walk_path()");
        throw Error<IndexErrc>{PathConflict, along_path_msg(path_parts, i)};
      }
      node = &node->children[path_parts[i]];
    }
    
    if (node->entry) {
      throw Error<IndexErrc>{SecretExists, ck::path::join(path_parts)};
    }

    return node;
  }

  Node* walk_path(Node* root, const std::string& path) {
    return walk_path(root, ck::path::parse_path(path));
  }

  Node* get_parent(Node* root, const std::vector<std::string>& path_parts) {
    std::vector<std::string> p;
    p.reserve(path_parts.size() - 1);
    for (size_t i = 0; i < path_parts.size() - 1; ++i) {
      p.push_back(path_parts[i]);
    }

    return walk_path(root, p);
  }

  Node* get_parent(Node* root, const std::string& path) {
    return get_parent(root, ck::path::parse_path(path));
  }

  bool secret_along_path(Node* root, const std::vector<std::string>& path_parts) {
    Node* node = root;
    for (const auto& part : path_parts) {
      if (node->entry) { return true; }
      
      node = &node->children[part];
    }
    
    return static_cast<bool>(node->entry);
  }

  bool secret_along_path(Node* root, const std::string& path) {
    return secret_along_path(root, ck::path::parse_path(path));
  }
}
