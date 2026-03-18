#pragma once
#include "toml++/toml.hpp"

#include "lib/config/types.hpp"

namespace ck::config {
  toml::table serialize(const Config&);
}