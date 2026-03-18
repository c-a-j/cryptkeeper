#pragma once
#include "lib/config/types.hpp"
#include "cli/types.hpp"

namespace ck::config {
  void set_parameter(Config&, const ck::cli::ConfigArgs&);
}
