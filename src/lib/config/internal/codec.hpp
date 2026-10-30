#pragma once

#include <string_view>

#include "toml++/toml.hpp"

#include "lib/config/config.hpp"

namespace ck::config::codec {
State deserialize(std::string_view);
toml::table serialize(const State &);
} // namespace ck::config::codec
