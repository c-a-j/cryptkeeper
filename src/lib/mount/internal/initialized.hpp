#pragma once
#include <filesystem>

namespace ck::mount {
  bool initialized(const std::filesystem::path&);
}

