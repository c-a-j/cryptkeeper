#include <toml++/toml.hpp>
#include <filesystem>

#include "global.hpp"
#include "lib/index/types.hpp"
#include "./util/error.hpp"
#include "./_internal/walk_path.hpp"
#include "./_internal/vars.hpp"

namespace ck::index { 
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;

  namespace fs = std::filesystem;

  toml::table parse_file(const fs::path& idx_file) {
    if (!fs::exists(idx_file)) { 
      throw Error<IndexErrc>{IndexFileNotFound, std::string(idx_file)};
    }
    toml::table tbl = toml::parse_file(std::string(idx_file));
    return tbl;
  };

  Entry parse_entry(const toml::table& tbl) {
    Entry obj;
    std::optional<std::string> uuid = tbl["uuid"].value<std::string>();
    if (!uuid) {
      throw Error<IndexErrc>{CorruptedIndex, "an entry is missing a uuid mapping"};
    }
    obj.uuid = *uuid;
    return obj;
  }
}

namespace ck::index {
  void Index::deserialize(const std::string& vault_path) {
    this->path_ = fs::path(vault_path);
    this->file_ = this->path_ / fs::path(INDEX_FILE);
    toml::table tbl = parse_file(this->file_);

    if (auto* entries = tbl[IDX_ARR_NAME].as_array()) {
      for (auto&& n : *entries) {
        auto* entry = n.as_table();
        if (!entry) {
          continue;
        }
        
        Entry e = parse_entry(*entry);
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
