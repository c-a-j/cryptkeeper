#pragma once
#include "toml++/toml.hpp"

#include <filesystem>

#include "lib/mount/mount.hpp"

namespace ck::mount::codec {
  namespace fs = std::filesystem;
  State deserialize(std::string_view, const std::string&);
  toml::table serialize(const ck::mount::State&);
}

