#include "toml++/toml.hpp"
#include <sstream>
#include <filesystem>

#include "util/error.hpp"
#include "lib/index/types.hpp"
#include "./_internal/vars.hpp"
#include "../fs/atomic_write.hpp"
#include "../path/get_idx_file.hpp"

namespace {
  struct IndexObj {
    std::vector<std::string> path;
    std::string uuid;
  };

  toml::table serialize(const IndexObj& obj) {
    toml::table tbl; 
    toml::array path;
    for (const auto& part : obj.path) {
      path.push_back(part);
    }
    tbl.insert_or_assign("path", std::move(path));
    tbl.insert_or_assign("uuid", obj.uuid);
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
    tbl.insert(IDX_ARR_NAME, entries);
    return tbl;
  }
}

namespace ck::index { 
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  
  void Index::write() {
    toml::table tbl = serialize(*this);

    if (this->file_.empty()) {
      throw Error<IndexErrc>{NoPathToIndex, std::string("Index::write()")}; 
    }
    
    std::ostringstream contents;
    contents << tbl << "\n";
    
    ck::fs::atomic_write(this->file_, contents.str());
  }
}
