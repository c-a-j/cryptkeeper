#pragma once

#include <filesystem>

namespace ck::fio {
  namespace fs = std::filesystem;
  void sync_dir(const fs::path&);
}
