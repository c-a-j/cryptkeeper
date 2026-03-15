#include <string>
#include <sstream>
#include <vector>

#include "lib/config/parse_key.hpp"
#include "util/error.hpp"

namespace ck::config {
  using ck::util::error::Error;
  using ck::util::error::ConfigErrc;
  using enum ck::util::error::ConfigErrc;

  void parse_key(ConfigKey& cfg_key) {
    std::string part;
    std::vector<std::string> key_parts;
    std::stringstream ss(*cfg_key.key);
    
    while (std::getline(ss, part, '.')) {
      key_parts.push_back(part);
    }
    
    // when user specifies vault (ex -v vault auto_push true)
    if (cfg_key.vault && key_parts.size() == 1) {
      cfg_key.scope = "vaults";
      cfg_key.field = key_parts[0];
    // when user does not specify vault (ex vaults.vault.auto_push true)
    } else if (!cfg_key.vault && key_parts.size() == 3) {
      cfg_key.scope = key_parts[0];
      cfg_key.vault = key_parts[1];
      cfg_key.field = key_parts[2];
    // when user modifies global settings (ex global.auto_push true)
    } else if (!cfg_key.vault && key_parts.size() == 2) {
      cfg_key.scope = key_parts[0];
      cfg_key.field = key_parts[1];
    } else {
      ss.clear();
      ss << "scope = " << cfg_key.scope.value_or("") << ", vault = " << cfg_key.vault.value_or("") << 
        ", field = " << cfg_key.field.value_or("") << "\n";
      throw Error<ConfigErrc>{InvalidSetParameter, ss.str()};
    }
  }
}
