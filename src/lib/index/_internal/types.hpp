#pragma once

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

namespace ck::index {
  struct IndexObj {
    std::vector<std::string> path;
    std::string uuid;
  };
  
  struct Entry {
    std::string uuid;
  };
  
  struct Node {
    std::optional<Entry> entry;
    std::unordered_map<std::string, Node> children;
  };
  
  struct Index {
    Node root;
  };
}