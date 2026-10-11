#pragma once

#include <string>
#include <filesystem>

namespace ck::fio {
  namespace fs = std::filesystem;
  std::string msg(const fs::path&, int);
}
