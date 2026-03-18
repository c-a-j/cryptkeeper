#include <nlohmann/json.hpp>

#include "lib/config/types.hpp"
#include "cli/types.hpp"

namespace ck::index { 
  void insert(const ck::config::VaultConfig&, const ck::cli::InsertArgs&);
}