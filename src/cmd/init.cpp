#include <string>
#include <filesystem>
#include <fstream>

#include "cmd/init.hpp"
#include "util/error.hpp"

#include "lib/config/insert_vault.hpp"
#include "lib/config/active.hpp"
#include "lib/config/save.hpp"

#include "lib/crypto/crypto.hpp"



namespace ck::init {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::InitErrc;
  using enum ck::util::error::InitErrc;
  using namespace ck::config;
    
  void init(Config& cfg, Vault& vault) {
    
    VaultConfig acfg;
    get_active_config(cfg, acfg, vault);
    
    std::string key_fpr = ck::crypto::fingerprint(vault);
    
    if (!ck::crypto::public_key_exists(key_fpr)) {
      throw Error<InitErrc>{KeyNotFound, vault.key_fpr.value()};
    }
    
    fs::path dir = fs::path(*acfg.directory) / *acfg.vault;
    
    std::error_code ec;
    bool created = fs::create_directories(dir, ec);
    if (ec) {
      throw Error<InitErrc>{CreateDirectoryFailed, ec.message()};
    }
    
    if (!created) {
      throw Error<InitErrc>{AlreadyExists, dir};
    } 
    
    const fs::path gpg_id_path = dir / ".gpg-id";
    std::ofstream gpg_id_file(gpg_id_path, std::ios::out | std::ios::trunc);
    if (!gpg_id_file.is_open()) {
      throw Error<InitErrc>{OpenGpgIdFailed, std::string(gpg_id_path)};
    }
    
    gpg_id_file << vault.key_fpr.value() << '\n';
    if (!gpg_id_file) {
      throw Error<InitErrc>{WriteGpgIdFailed, std::string(gpg_id_path)};
    }
    
    insert_vault(cfg, vault);
    save_config(cfg);
  }
}
