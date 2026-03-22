#include <filesystem>

#include "global.hpp"

namespace ck::index { 
  namespace fs = std::filesystem;
  
  fs::path get_idx_file(const std::string& path) {
    fs::path idx_file = fs::path(path) / INDEX_FILE;
    return idx_file;
  }
}
