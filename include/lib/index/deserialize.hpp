#include <nlohmann/json.hpp>

#include "lib/index/types.hpp"
#include "lib/types.hpp"

namespace ck::index { 
  Entry deserialize_entry(const nlohmann::json& j);
  Index deserialize(const ck::config::VaultConfig&);
}