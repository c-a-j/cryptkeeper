#pragma once
#include <nlohmann/json.hpp>
#include <string>

#include "lib/types.hpp"

namespace ck::index { 
  std::optional<std::vector<std::string>> parse_path(const ck::secret::Secret&);
}