#pragma once

#include <string_view>

#include "toml++/toml.hpp"

#include "lib/config/config.hpp"

namespace ck::config::codec {
Config deserialize(std::string_view);
toml::table serialize(const Config &);
} // namespace ck::config::codec
