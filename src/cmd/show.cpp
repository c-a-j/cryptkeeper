#include <iostream>

#include "cmd/show.hpp"
#include "lib/index/index.hpp"

namespace ck::show {
  using namespace ck::index;
  using namespace ck::config;
  
  void show(VaultConfig& vcfg, Secret& secret){
    find(vcfg, secret);
  }
}