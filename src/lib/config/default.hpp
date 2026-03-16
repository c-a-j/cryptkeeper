#pragma once
#include "lib/types.hpp"

namespace ck::config {
  void load_default(Config& cfg, const Vault& = {});
}
