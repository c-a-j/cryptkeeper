#include <iostream>

#include "cmd/show.hpp"
#include "lib/index/find.hpp"

namespace ck::show {
  using namespace ck::index;
  using namespace ck::config;
  using namespace ck::secret;
  
  void show(VaultConfig& vcfg, Secret& secret){
    find(vcfg, secret);
  }
}