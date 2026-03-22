#include <toml++/toml.hpp>
#include <filesystem>

#include "global.hpp"
#include "./util/error.hpp"
#include "./_internal/types.hpp"
#include "./_internal/walk_path.hpp"

namespace ck::index { 
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;

  namespace fs = std::filesystem;
  
  toml::table deserialize_idx(const std::string& vault_path) {
    fs::path idx_file = fs::path(vault_path) / fs::path(CONFIG_FILE);
    if (!fs::exists(idx_file)) { 
      throw Error<IndexErrc>{IndexFileNotFound, std::string(idx_file)};
    }
    toml::table tbl = toml::parse_file(std::string(idx_file));
    return tbl;
  };
  
  Entry deserialize_entry(const toml::table& tbl) {
    Entry obj;
    std::optional<std::string> uuid = tbl["uuid"].value<std::string>();
    if (!uuid) {
      throw Error<IndexErrc>{CorruptedIndex, "there is an entry missing a uuid mapping"};
    }
    obj.uuid = *uuid;
    return obj;
  }
}

namespace ck::index {
  void Index::deserialize(const std::string& vault_path) {
    toml::table tbl = deserialize_idx(vault_path);

    // toml::array* entries = tbl["entries"].as_array();
    
    if (auto* entries = tbl["entries"].as_array()) {
      for (auto&& n : *entries) {
        auto* entry = n.as_table();
        if (!entry) {
          continue;
        }
        
        Entry e = deserialize_entry(*entry);
        std::vector<std::string> path;
        if (auto* arr = (*entry)["path"].as_array()) {
          path.reserve(arr->size());
          for (auto&& node : *arr) {
            if (auto part = node.value<std::string>()) {
              path.push_back(*part);
            }
          }
        }
        
        Node* node = walk_path(&this->root_, path);
        node->entry = std::move(e);
      }
    }
  }
}
