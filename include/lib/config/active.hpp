#pragma once

#include "lib/types.hpp"

namespace ck::lib::config {
  using namespace ck::types;
  VaultConfig get_active_config(const Config&, const Vault&);
}