#pragma once
#include <string>

#include "lib/types.hpp"

namespace ck::lib::secret {
  using namespace ck::types;
  void insert(const VaultConfig& cfg, const Secret& secret);
}