#include "toml++/toml.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>

#include "util/error.hpp"

#include "./_internal/types.hpp"
#include "../path/get_idx_file.hpp"

namespace {
  struct IndexObj {
    std::vector<std::string> path;
    std::string uuid;
  };

  toml::table serialize(const IndexObj& obj) {
    toml::table tbl; 
    tbl.insert_or_assign("path", obj.path);
    tbl.insert_or_assign("uuid", obj.path);
    return tbl;
  }

  void dfs (const ck::index::Node& node, std::vector<std::string>& path, toml::array& out) {
    if (node.entry) {
      IndexObj obj;
      obj.path = path;
      obj.uuid = node.entry->uuid;
      out.push_back(serialize(obj));
    }

    for (const auto& [key, child] : node.children) {
      path.push_back(key);
      dfs(child, path, out);
      path.pop_back();
    }
  }

  toml::table serialize(const ck::index::Index& idx) {
    toml::table tbl;
    toml::array entries;
    std::vector<std::string> path;
    dfs(idx.root(), path, entries);
    tbl.insert("secret", entries);
    return tbl;
  }
}

namespace ck::index { 
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  
  void Index::write(const std::string& path) {
    fs::path idx = get_idx_file(path);

    toml::table idx_toml = serialize(*this);
    
    std::ofstream out(idx);
    if (!out) { 
      throw Error<IndexErrc>{OpenIndexFailed, std::string(idx)}; 
    }
    
    out << idx_toml << "\n";
  }
}
