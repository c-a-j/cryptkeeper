#include <filesystem>

#include "lib/config/types.hpp"

namespace ck::index { 
  namespace fs = std::filesystem;
  
  fs::path get_idx_file(const ck::config::VaultConfig&);
}