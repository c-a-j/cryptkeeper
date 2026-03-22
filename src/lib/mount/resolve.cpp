#include <filesystem>
#include <unordered_map>

#include "lib/mount/types.hpp"
#include "util/error.hpp"

#include "../path/parse_path.hpp"



namespace {
  std::string join_prefix(std::vector<std::string> parts, std::size_t n) {
    std::string out;
    for (std::size_t i = 0; i < n; ++i) {
      if (i > 0) {
        out += "/";
      }
      out += parts[i]; 
    }
    return out;
  }

  std::string join_suffix(std::vector<std::string> parts, std::size_t start) {
    std::string out;
    for (std::size_t i = start; i < parts.size(); ++i) {
      if (i > start) {
        out += "/";
      }
      out += parts[i]; 
    }
    return out;
  }
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
      std::string alias = join_prefix(parts, n);
      auto it = this->mounts_.find(alias);
      if (it != this->mounts_.end()) {
        rp.alias = alias;
        rp.vault_path = it->second.path;
        rp.relative_path = join_suffix(parts, n);
        return rp;
      }
    }
    rp.alias = {};
    rp.vault_path = root_.path;
    rp.relative_path = join_suffix(parts, 0);
    return rp;
  }
}
