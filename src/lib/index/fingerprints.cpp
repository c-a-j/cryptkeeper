#include <fstream>
#include <filesystem>

#include "global.hpp"
#include "lib/config/types.hpp"
#include "cli/types.hpp"
#include "util/error.hpp"

namespace {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  fs::path get_key_path(const std::string& vault_path) {
    if (vault_path.empty()) {
      throw Error<IndexErrc>{VaultUnspecified, "get_key_path()"};
    }
    return fs::path(vault_path) / GPG_ID_FILE;
  }
}

namespace ck::index {
  using namespace ck::config;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  namespace fs = std::filesystem;
  
  std::vector<std::string> get_fingerprints(const std::string& vault_path) {
    fs::path key_path = get_key_path(vault_path);
    std::ifstream file(key_path);
    if (!file.is_open()) {
      throw Error<IndexErrc>{GpgIdFileNotFound, key_path.string()};
    }
    std::vector<std::string> fingerprints;
    std::string buf;
    while (std::getline(file, buf)) {
      fingerprints.push_back(buf);
    }
    return fingerprints;
  } 
}
