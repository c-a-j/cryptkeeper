#include "toml++/toml.hpp"

#include "../path/internal/get_idx_file.hpp"
#include "./internal/types.hpp"
#include "./internal/vars.hpp"
#include "lib/index/error.hpp"
#include "lib/index/index.hpp"

namespace {
using ck::error::Error;
using ck::error::IndexErrc;
using enum ck::error::IndexErrc;

toml::table serialize_entry(const ck::index::IndexObj &obj) {
    toml::table tbl;
    toml::array path;
    for (const auto &part : obj.path) {
        path.push_back(part);
    }
    tbl.insert_or_assign("path", std::move(path));
    tbl.insert_or_assign("uuid", obj.uuid);
    return tbl;
}

void dfs(
    const ck::index::Node *node,
    std::vector<std::string> &path,
    toml::array &out
) {
    if (node->entry) {
        ck::index::IndexObj obj;
        obj.path = path;
        obj.uuid = node->entry->uuid;
        out.push_back(serialize_entry(obj));
    }

    for (const auto &[key, child] : node->children) {
        path.push_back(key);
        dfs(&child, path, out);
        path.pop_back();
    }
}
} // namespace

namespace ck::index::codec {
using ck::error::Error;
using ck::error::IndexErrc;
using enum ck::error::IndexErrc;
toml::table serialize(const Node *root) {
    toml::table tbl;
    toml::array entries;
    std::vector<std::string> path;
    dfs(root, path, entries);
    tbl.insert(IDX_ARR_NAME, entries);
    return tbl;
}
} // namespace ck::index::codec
