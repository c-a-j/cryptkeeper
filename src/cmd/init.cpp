#include <string>
#include <filesystem>
#include <fstream>

#include "cmd/init.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/config/path.hpp"
#include "util/error.hpp"



namespace ck::init {
  namespace fs = std::filesystem;
  using namespace ck::util::error;
  using namespace ck::config;
    
  void init_vault(Config& cfg, Vault& vault) {
    if (!ck::crypto::public_key_exists(vault.key_fpr.value())) {
      throw Error{InitErrc::KeyNotFound, vault.key_fpr.value()};
    }
    
    fs::path dir = vault_root() / vault.name.value();
    
    std::error_code ec;
    bool created = fs::create_directories(dir, ec);
    if (ec) {
      throw Error{InitErrc::CreateDirectoryFailed, ec.message()};
    }
    
    if (!created) {
      throw Error{InitErrc::AlreadyExists, dir};
    } 
    
    const fs::path gpg_id_path = dir / ".gpg-id";
    std::ofstream gpg_id_file(gpg_id_path, std::ios::out | std::ios::trunc);
    if (!gpg_id_file.is_open()) {
      throw Error{InitErrc::OpenGpgIdFailed, std::string(gpg_id_path)};
    }
    
    gpg_id_file << vault.key_fpr.value() << '\n';
    if (!gpg_id_file) {
      throw Error{InitErrc::WriteGpgIdFailed, std::string(gpg_id_path)};
    }
  }
}
