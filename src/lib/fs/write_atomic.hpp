#pragma once

#include <filesystem>
#include <string>

namespace ck::fs {
  namespace fs = std::filesystem;
  void write_atomic(const fs::path&, const std::string&);
}
