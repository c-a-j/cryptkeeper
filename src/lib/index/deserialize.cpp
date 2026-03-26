#include <toml++/toml.hpp>
#include <filesystem>

#include "global.hpp"
#include "lib/index/types.hpp"
#include "./util/error.hpp"
#include "./util/logger/logger.hpp"
#include "../path/existence.hpp"
#include "./_internal/walk_path.hpp"
#include "./_internal/types.hpp"
#include "./_internal/vars.hpp"


namespace { 
  using namespace ck::index;
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;

  toml::table parse_file(const fs::path& idx_file) {
    if (!ck::path::file_exists(idx_file)) { 
      logger.debug("Index::deserialize() -> parse_file()");
      throw Error<IndexErrc>{IndexFileNotFound, idx_file.string()};
    }

    toml::table tbl;
    try {
      tbl = toml::parse_file(idx_file.string());
    } catch (const toml::parse_error& e) {
      throw Error<IndexErrc>{InvalidIndexFile, e.what()};
    }

    return tbl;
  };
  
  bool is_valid_uuid(std::string_view uuid) {
    if (uuid.size() != 36) {
      return false;
    }
  
    auto is_hex = [](char c) {
      return ('0' <= c && c <= '9')
        || ('a' <= c && c <= 'f');
    };
  
    for (std::size_t i = 0; i < uuid.size(); ++i) {
      if (i == 8 || i == 13 || i == 18 || i == 23) {
        if (uuid[i] != '-') {
          return false;
        }
        continue;
      }
  
      if (!is_hex(uuid[i])) {
        return false;
      }
    }
  
    if (uuid[14] != '4') {
      return false;
    }
  
    if (uuid[19] != '8' &&
        uuid[19] != '9' &&
        uuid[19] != 'a' &&
        uuid[19] != 'b') {
      return false;
    }
  
    return true;
  }
  
  std::string parse_uuid(const toml::table& tbl, const std::string& file_path) {
    std::optional<std::string> uuid = tbl["uuid"].value<std::string>();
    if (!uuid) {
      std::string msg = file_path + " -> " + "an entry is missing a uuid mapping";
      throw Error<IndexErrc>{CorruptedIndex, msg};
    }
    
    if (!is_valid_uuid(*uuid)) {
      std::string msg = file_path + " -> [" + *uuid + "] " + "is not a valid uuid";
      throw Error<IndexErrc>{CorruptedIndex, msg};
    }
    
    return *uuid;
  }
  
  std::vector<std::string> parse_path(
    const toml::table& tbl, 
    const std::string& file_path,
    const std::string& uuid
  ) {
    std::vector<std::string> path;
    if (auto* arr = tbl["path"].as_array()) {
      path.reserve(arr->size());
      for (const toml::node& n : *arr) {
        std::optional<std::string> part = n.value<std::string>();
        if (!part) {
          std::string msg = file_path + " -> [" + uuid + "] " + "contains non string path element";
          throw Error<IndexErrc>{CorruptedIndex, msg};
        }
        if (part->empty()) {
          std::string msg = file_path + " -> [" + uuid + "] " + "contains empty path element";
          throw Error<IndexErrc>{CorruptedIndex, msg};
        }
        if (part->find('/') != std::string::npos) {
          std::string msg = file_path + " -> [" + uuid + "] " + "contains path element including '/'";
          throw Error<IndexErrc>{CorruptedIndex, msg};
        }
        path.push_back(*part);
      }
    } else {
      std::string msg = file_path + " -> [" + uuid + "] " + "is missing a path";
      throw Error<IndexErrc>{CorruptedIndex, msg};
    }
    
    if (path.empty()) {
      std::string msg = file_path + " -> [" + uuid + "] " + "has empty path";
      throw Error<IndexErrc>{CorruptedIndex, msg};
    }
    
    return path;
  }
  
  IndexObj parse_entry(const toml::node& node, const std::string& file_path) {
    IndexObj obj;

    const toml::table* tbl = node.as_table();
    if (!tbl) { 
      std::string msg = std::string(file_path) 
        + std::string(": all entries must be a toml table");
      throw Error<IndexErrc>{CorruptedIndex, msg}; 
    }

    obj.uuid = parse_uuid(*tbl, file_path);
    obj.path = parse_path(*tbl, file_path, obj.uuid);

    return obj;
  }

}

namespace ck::index { 
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;

  namespace fs = std::filesystem;

  void Index::deserialize(const std::string& vault_path) {
    this->path_ = fs::path(vault_path);
    this->file_ = this->path_ / fs::path(INDEX_FILE);
    toml::table tbl = parse_file(this->file_);

    logger.debug("deserialize() - parsing index file");
    if (toml::array* entries = tbl[IDX_ARR_NAME].as_array()) {
      for (toml::node& n : *entries) {
        IndexObj obj = parse_entry(n, this->file_);
        Node* node = this->break_trail(obj.path);
        node->entry = Entry{obj.uuid};
      }
    } else {
      throw Error<IndexErrc>{InvalidOrEmptyIndexFile, this->file_};
    }
    this->root_.path = this->path_;
  }

  void Index::deserialize(const std::string& alias, const std::string& vault_path) {
    this->deserialize(vault_path);
    this->alias_ = alias;
  }
}
