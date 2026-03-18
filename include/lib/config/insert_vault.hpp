#pragma once
#include <string>

#include "lib/config/types.hpp"

namespace ck::config {
  void insert_vault(Config&, const std::string&);
}
