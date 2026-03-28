#include <filesystem>

#include "global.hpp"
#include "util/error.hpp"

namespace ck::path {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::PathErrc;
  using enum ck::util::error::PathErrc;

  fs::path get_gpg_id_file(const std::string& vault_path) {
    if (vault_path.empty()) {
      throw Error<PathErrc>{VaultUnspecified, "get_key_path()"};
    }
    return fs::path(vault_path) / GPG_ID_FILE;
  }
}

