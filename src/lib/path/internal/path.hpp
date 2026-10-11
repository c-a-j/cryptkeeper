#pragma once

#include <filesystem>

namespace ck::path {
  namespace fs = std::filesystem;
  fs::path crypt_root();
  fs::path config_dir();
  fs::path config_file();
  fs::path mount_dir();
  fs::path mount_file();
  void create_config_dir();
}
