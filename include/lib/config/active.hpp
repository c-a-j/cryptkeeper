#pragma once

#include "lib/types.hpp"

namespace ck::config {
  VaultConfig get_active_config(Config&, VaultConfig&, Vault&);
}