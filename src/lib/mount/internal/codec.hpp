#pragma once
#include <filesystem>

#include "toml++/toml.hpp"

#include "lib/mount/mount.hpp"

namespace ck::mount::codec {
namespace fs = std::filesystem;
State deserialize(std::string_view, const std::string &);
toml::table serialize(const ck::mount::State &);
} // namespace ck::mount::codec
