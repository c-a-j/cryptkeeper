#include <filesystem>
#include <unordered_map>

#include "lib/mount/types.hpp"
#include "util/error.hpp"

#include "../path/parse_path.hpp"
#include "../path/join.hpp"

namespace {
}

namespace ck::mount {
  using ck::util::error::Error;
  using ck::util::error::MountErrc;
  using enum ck::util::error::MountErrc;
  
  ResolvedPath Mounts::resolve(const std::string& path) {
    this->deserialize();
    
    ResolvedPath rp;
    auto parts = ck::path::parse_path(path);

    for (std::size_t n = parts.size(); n > 0; --n) {
      std::string alias = ck::path::join_prefix(parts, n);
      auto it = this->mounts_.find(alias);
      if (it != this->mounts_.end()) {
        rp.alias = alias;
        rp.vault_path = it->second.path;
        rp.relative_path = ck::path::join_suffix(parts, n);
        return rp;
      }
    }
    rp.alias = {};
    rp.vault_path = root_.path;
    rp.relative_path = ck::path::join_suffix(parts, 0);
    return rp;
  }
}
