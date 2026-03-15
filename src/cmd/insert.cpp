#include "cmd/insert.hpp"
#include "lib/types.hpp"
#include "lib/index/insert_entry.hpp"

namespace ck::insert {
  void insert(const ck::config::VaultConfig& cfg, const ck::secret::Secret& secret) {
    ck::index::insert(cfg, secret);
  }
}
