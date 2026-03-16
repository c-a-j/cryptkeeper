#include "lib/types.hpp"

namespace ck::config {
  void insert_vault(Config& cfg, const Vault& vault) {
    if (!cfg.global.vault) cfg.global.vault = vault.name;
    cfg.overrides[*vault.name] = VaultConfig{};
  }
}
