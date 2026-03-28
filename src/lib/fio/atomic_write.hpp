#pragma once

#include <filesystem>
#include <string>

namespace ck::fio {
  namespace fs = std::filesystem;
  void atomic_write(const fs::path&, const std::string&);
  void atomic_write(const fs::path&, std::span<const std::byte>);
}
