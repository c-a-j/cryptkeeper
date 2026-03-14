#include "cmd/insert.hpp"
#include "lib/index/index.hpp"
#include "lib/types.hpp"

#include <iostream>

namespace ck::insert {
  using namespace ck::config;
  void insert(const VaultConfig& cfg, const Secret& secret) {
    ck::index::insert(cfg, secret);
  }
}
