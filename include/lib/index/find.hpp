#pragma once
#include <string>

#include "lib/types.hpp"

namespace ck::index {
  void find(const ck::config::VaultConfig&, const ck::secret::Secret&);
}