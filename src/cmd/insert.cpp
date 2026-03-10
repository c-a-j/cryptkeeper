#include "cmd/insert.hpp"
#include "lib/secret/secret.hpp"
#include "lib/types.hpp"

#include <iostream>

namespace ck::cmd::insert {
  using namespace ck::types;
  void insert(const VaultConfig& cfg, const Secret& secret) {
    ck::lib::secret::insert(cfg, secret);
  }
}
