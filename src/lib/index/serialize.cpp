#include "toml++/toml.hpp"

#include "util/error.hpp"
#include "lib/index/types.hpp"
#include "./_internal/vars.hpp"
#include "./_internal/types.hpp"
#include "../path/get_idx_file.hpp"

namespace { 
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;

  toml::table serialize_entry(const ck::index::IndexObj& obj) {
    toml::table tbl; 
    toml::array path;
    for (const auto& part : obj.path) {
      path.push_back(part);
    }
    tbl.insert_or_assign("path", std::move(path));
    tbl.insert_or_assign("uuid", obj.uuid);
    return tbl;
  }

  void dfs (const ck::index::Node* node, std::vector<std::string>& path, toml::array& out) {
    if (node->entry) {
      ck::index::IndexObj obj;
      obj.path = path;
      obj.uuid = node->entry->uuid;
      out.push_back(serialize_entry(obj));
    }

    for (const auto& [key, child] : node->children) {
      path.push_back(key);
      dfs(&child, path, out);
      path.pop_back();
    }
  }
}

namespace ck::index::codec { 
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  toml::table serialize(const Node* root) {
    toml::table tbl;
    toml::array entries;
    std::vector<std::string> path;
    dfs(root, path, entries);
    tbl.insert(IDX_ARR_NAME, entries);
    return tbl;
  }
}
