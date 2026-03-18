#include <nlohmann/json.hpp>

#include "lib/config/types.hpp"

namespace ck::index { 
  void write_idx(const ck::config::VaultConfig&, const nlohmann::json&);
}