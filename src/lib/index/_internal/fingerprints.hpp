#include "lib/types.hpp"

#include <string>
#include <vector>

namespace ck::index {
  std::vector<std::string> get_fingerprints(const ck::config::VaultConfig&);
}