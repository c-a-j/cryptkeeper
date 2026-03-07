#pragma once

#include <vector>
#include <string>
#include "lib/types.hpp"

namespace ck::cmd::config {
using namespace ck::types;
  void config(Config&, Vault&, std::vector<std::string>&);
}