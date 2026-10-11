#include <filesystem>
#include <unordered_map>

#include "lib/mount/mount.hpp"
#include "util/error.hpp"
#include "lib/path/parse_path.hpp"

#include "../path/internal/join.hpp"

namespace {
}

namespace ck::mount {
  using ck::util::error::Error;
  using ck::util::error::MountErrc;
  using enum ck::util::error::MountErrc;
  
  ResolvedPath Mounts::resolve(const std::string& path) {
    if (this->empty()) {
      this->load();
    }
    
    ResolvedPath rp;
    auto parts = ck::path::parse_path(path);

    for (std::size_t n = parts.size(); n > 0; --n) {
      std::string alias = ck::path::join_prefix(parts, n);
      auto it = this->state_.mounts.find(alias);
      if (it != this->state_.mounts.end()) {
        rp.alias = alias;
        rp.crypt_path = it->second.path;
        rp.relative_path = ck::path::join_suffix(parts, n);
        return rp;
      }
    }
    rp.alias = {};
    rp.crypt_path = this->state_.root.path;
    rp.relative_path = ck::path::join_suffix(parts, 0);
    return rp;
  }
}
