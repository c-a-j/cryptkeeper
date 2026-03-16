#include <iostream>

#include "cmd/show.hpp"
#include "lib/config/active.hpp"
#include "lib/index/find.hpp"

namespace ck::show {
  using namespace ck::index;
  using namespace ck::config;
  using namespace ck::secret;
  
  void show(Config& cfg, Vault& vault, const Secret& secret){
    VaultConfig acfg;
    config::get_active_config(cfg, acfg, vault);
    
    find(acfg, secret);
  }
}