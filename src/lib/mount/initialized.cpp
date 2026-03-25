#include <filesystem>
#include "global.hpp"

#include "../path/existence.hpp"

namespace ck::mount {
  namespace fs = std::filesystem;

  bool initialized(const fs::path& path) { 
    fs::path gpg_id_file = path / fs::path(GPG_ID_FILE);
    if (ck::path::exists(gpg_id_file)) {
      return true; 
    }
    return false;
  }
}

