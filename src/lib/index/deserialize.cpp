#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <filesystem>

#include "lib/types.hpp"

#include "./_internal/types.hpp"
#include "./_internal/get_idx_file.hpp"
#include "./_internal/walk_path.hpp"

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
      
      Node* node = walk_path(&idx.root, path);
      node->entry = std::move(entry);
    }
    
    return idx;
  }
}