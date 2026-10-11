#pragma once

#include "toml++/toml.hpp"
#include <string_view>

#include "lib/config/config.hpp"


namespace ck::config::codec {
  State deserialize(std::string_view);
  toml::table serialize(const State&);
}
