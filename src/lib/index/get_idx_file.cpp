#include <nlohmann/json.hpp>
#include <filesystem>

#include "global.hpp"
#include "lib/config/types.hpp"

namespace ck::index { 
  namespace fs = std::filesystem;
  using namespace ck::config;
  
  fs::path get_idx_file(const VaultConfig& cfg) {
    fs::path idx_file = fs::path(*cfg.directory) / fs::path(*cfg.vault) / INDEX_FILE;
    return idx_file;
  }
}