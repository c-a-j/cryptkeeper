#include <filesystem>

#include "lib/types.hpp"
#include "lib/config/path.hpp"
#include "lib/config/deserialize.hpp"
#include "lib/config/save.hpp"

inline constexpr std::string_view GLOBAL_CONFIGS = "global";

namespace ck::lib::config {
  using namespace ck::types;
  namespace fs = std::filesystem;

  void create_default_config(Config& cfg, Vault& vault) {
    VaultConfig& obj = cfg.global;
    obj.vault = *vault.name;
    obj.directory = vault_root();
    obj.auto_push = false;
    obj.insert_with_editor = false;
    cfg.overrides[*vault.name] = VaultConfig{};
  }
  
  void init_config(Config& cfg, Vault& vault) {
    fs::path cfg_file = app_config_file();
    // add new vault if config file already exists
    if (fs::exists(cfg_file)) {
      deserialize(cfg);
      if (vault.name) cfg.overrides[*vault.name] = VaultConfig{};
      save_config(cfg);
      return;
    }
    // create new config
    create_config_dir();
    create_default_config(cfg, vault);
    save_config(cfg);
  }
}
