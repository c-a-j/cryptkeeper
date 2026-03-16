#pragma once

#include <filesystem>

namespace ck::config {
  namespace fs = std::filesystem;

  fs::path vault_root();
  fs::path app_config_dir();
  fs::path app_config_file();
  void create_config_dir();
}
