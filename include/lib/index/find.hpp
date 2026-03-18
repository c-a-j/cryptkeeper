#pragma once
#include <string>

#include "lib/config/types.hpp"
#include "cli/types.hpp"

namespace ck::index {
  void find(const ck::config::VaultConfig&, const ck::cli::ShowArgs&);
}