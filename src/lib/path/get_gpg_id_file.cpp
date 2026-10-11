#include <filesystem>

#include "global.hpp"
#include "lib/path/error.hpp"

namespace ck::path {
  namespace fs = std::filesystem;
  using ck::error::Error;
  using ck::error::PathErrc;
  using enum ck::error::PathErrc;

  fs::path get_gpg_id_file(const std::string& crypt_path) {
    if (crypt_path.empty()) {
      throw Error<PathErrc>{CryptUnspecified, "get_key_path()"};
    }
    return fs::path(crypt_path) / GPG_ID_FILE;
  }
}

