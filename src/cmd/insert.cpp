#include "./_internal.hpp"
#include "cli/types.hpp"
#include "lib/types.hpp"
#include "lib/config/active.hpp"
#include "lib/index/insert_entry.hpp"

namespace ck::cmd {
  void insert(const ck::cli::Context& ctx, const ck::cli::InsertArgs& args) {
    ck::config::Config cfg;
    ck::config::VaultConfig acfg;
    config::get_active_config(cfg, acfg, args);
    ck::index::insert(acfg, args);
  }
}
