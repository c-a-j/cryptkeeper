#include <nlohmann/json.hpp>

#include "lib/types.hpp"

namespace ck::index { 
  void insert(const ck::config::VaultConfig&, const ck::secret::Secret&);
}