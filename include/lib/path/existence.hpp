#pragma once

#include <filesystem>

namespace ck::path {
  namespace fs = std::filesystem;
  bool file_exists_or_throw(const fs::path&);
  bool file_exists(const fs::path&);
  bool directory_exists_or_throw(const fs::path&);
  bool directory_exists(const fs::path&);
  bool exists_or_throw(const fs::path&);
  bool exists(const fs::path&);
}
