#include "lib/config/types.hpp"
#include "./_internal/path.hpp"

namespace ck::config {
  void load_default(Config& cfg, const std::optional<std::string>& vault_name) {
    VaultConfig& obj = cfg.global;
    obj.vault = vault_name;
    obj.directory = vault_root();
    obj.auto_push = false;
    obj.insert_with_editor = false;
  }
}
