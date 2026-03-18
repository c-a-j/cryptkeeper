#include "lib/config/types.hpp"

namespace ck::config {
  void insert_vault(Config& cfg, const std::string& vault_name) {
    if (!cfg.global.vault) cfg.global.vault = vault_name;
    cfg.overrides[vault_name] = VaultConfig{};
  }
}
