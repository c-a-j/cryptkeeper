#include <fstream>
#include <filesystem>

#include "global.hpp"
#include "lib/types.hpp"
#include "cli/types.hpp"
#include "util/error.hpp"

namespace {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  fs::path get_key_path(const std::optional<std::string>& root_dir, const std::optional<std::string>& vault_name) {
    if (!root_dir || !vault_name) {
      throw Error<IndexErrc>{UndefinedOptional, "get_key_path()"};
    }
    fs::path key_path = fs::path(*root_dir) / fs::path(*vault_name) / GPG_ID_FILE;
    return key_path;
  }
}

namespace ck::index {
  using namespace ck::config;
  using ck::util::error::Error;
  using ck::util::error::IndexErrc;
  using enum ck::util::error::IndexErrc;
  namespace fs = std::filesystem;
  
  std::vector<std::string> get_fingerprints(const VaultConfig& acfg) {
    fs::path key_path = get_key_path(acfg.directory, acfg.vault);
    std::ifstream file(key_path);
    if (!file.is_open()) {
      throw Error<IndexErrc>{GpgIdFileNotFound, std::string(key_path)};
    }
    std::vector<std::string> fingerprints;
    std::string buf;
    while (std::getline(file, buf)) {
      fingerprints.push_back(buf);
    }
    return fingerprints;
  } 
}