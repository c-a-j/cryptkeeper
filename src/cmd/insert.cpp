#include "cmd/insert.hpp"
#include "lib/types.hpp"
#include "lib/config/active.hpp"
#include "lib/index/insert_entry.hpp"

namespace ck::insert {
  using namespace ck::config;
  using namespace ck::secret;
  void insert(Config& cfg, Vault& vault, const Secret& secret) {
    VaultConfig acfg;
    config::get_active_config(cfg, acfg, vault);
    // secret::wisper(secret);
    ck::index::insert(acfg, secret);
  }
}
