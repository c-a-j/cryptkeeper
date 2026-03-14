#pragma once

#include "lib/types.hpp"

namespace ck::show {
using namespace ck::secret;
using namespace ck::config;
  void show(VaultConfig&, Secret&);
}