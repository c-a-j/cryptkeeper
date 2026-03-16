#include "lib/types.hpp"
#include "./path.hpp"

namespace ck::config {
  void load_default(Config& cfg, const Vault& vault) {
    VaultConfig& obj = cfg.global;
    obj.vault = vault.name;
    obj.directory = vault_root();
    obj.auto_push = false;
    obj.insert_with_editor = false;
  }
}
