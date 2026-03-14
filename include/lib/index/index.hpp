#pragma once
#include <string>

#include "lib/types.hpp"

namespace ck::index {
  using namespace ck::config;
  using namespace ck::secret;
  void insert(const VaultConfig&, const Secret&);
  void find(const VaultConfig&, const Secret&);
}