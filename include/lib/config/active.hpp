#pragma once

#include "lib/config/types.hpp"
#include "cli/types.hpp"

namespace ck::config {
  VaultConfig get_active_config(Config&, VaultConfig&, const ck::cli::CmdArgs&);
}