#pragma once

#include "toml++/toml.hpp"
#include <string>
#include "lib/config/types.hpp"


namespace ck::config::codec {
  State deserialize(std::string_view);
  toml::table serialize(const State&);
}
