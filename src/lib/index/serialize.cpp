#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "util/error.hpp"
#include "lib/config/types.hpp"
#include "lib/crypto/random.hpp"


#include "./_internal/get_idx_file.hpp"
#include "./_internal/deserialize.hpp"
#include "./_internal/types.hpp"
#include "./_internal/parse_path.hpp"
#include "./_internal/theme.hpp"
#include "./_internal/types.hpp"


namespace ck::index { 
  using namespace index;
  using namespace ck::config;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  using enum ck::index::OutputComponent;
  using namespace ck::crypto;
  using namespace ck::util::term;
  
  nlohmann::json serialize_obj(const IndexObj& entry) {
    nlohmann::json j = {
      {"path", entry.path},
      {"uuid", entry.uuid},
    };
    return j;
  }
  
  void dfs(const Node& node, std::vector<std::string>& path, nlohmann::json& out) {
    if (node.entry) {
      IndexObj obj;
      obj.path = path;
      obj.uuid = node.entry->uuid;
      out.push_back(serialize_obj(obj));
    }
    
    for (const auto& [key, child] : node.children) {
      path.push_back(key);
      dfs(child, path, out);
      path.pop_back();
    }
  }
  
  nlohmann::json serialize(const Index& idx) {
    nlohmann::json j = nlohmann::json::array();
    std::vector<std::string> path;
    dfs(idx.root, path, j);
    return j;
  }
}