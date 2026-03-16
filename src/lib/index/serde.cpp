#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "lib/index/types.hpp"
#include "lib/index/walk_path.hpp"
#include "lib/index/get_idx_file.hpp"
#include "lib/types.hpp"


#include "lib/index/theme.hpp"
#include "lib/index/parse_path.hpp"
#include "lib/index/types.hpp"
#include "lib/index/deserialize.hpp"
#include "lib/index/get_idx_file.hpp"

#include "util/error.hpp"
#include "lib/types.hpp"
#include "lib/crypto/random.hpp"


// deserialize
namespace ck::index { 
  namespace fs = std::filesystem;
  using namespace ck::config;
  
  nlohmann::json deserialize_idx(const VaultConfig& vcfg) {
    fs::path idx = get_idx_file(vcfg);
    nlohmann::json j = nlohmann::json::array();
    if (!fs::exists(idx)) { return j; }
    std::ifstream in(idx);
    if (!in) { throw std::runtime_error("failed to open index file"); }
    in >> j;
    return j;
  };
  
  Entry deserialize_entry(const nlohmann::json& j) {
    Entry obj;
    obj.uuid = j["uuid"];
    if (j.contains("key_fpr")) obj.key_fpr = j["key_fpr"];
    return obj;
  }
  
  
  Index deserialize(const VaultConfig& vcfg) {
    Index idx;
    
    nlohmann::json j = deserialize_idx(vcfg);
    
    for (auto& obj : j) {
      Entry entry = deserialize_entry(obj);
      std::vector<std::string> path = obj["path"];
      
      Node* node = &idx.root;
      walk_path(node, entry, path);
    }
    
    return idx;
  }
}


namespace ck::index { 
  using namespace index;
  using namespace ck::config;
  using namespace ck::secret;
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
    if (entry.key_fpr) {
      j["key_fpr"] = *entry.key_fpr;
    }
    return j;
  }
  
  void dfs(const Node& node, std::vector<std::string>& path, nlohmann::json& out) {
    if (node.entry) {
      IndexObj obj;
      obj.path = path;
      obj.uuid = node.entry->uuid;
      obj.key_fpr = node.entry->key_fpr;
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