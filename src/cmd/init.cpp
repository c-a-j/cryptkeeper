#include <string>
#include <filesystem>
#include <fstream>

#include "./_internal.hpp"
#include "util/error.hpp"

#include "lib/config/insert_vault.hpp"
#include "lib/config/active.hpp"
#include "lib/config/save.hpp"

#include "lib/crypto/crypto.hpp"

namespace ck::cmd {
  namespace fs = std::filesystem;
  using ck::util::error::Error;
  using ck::util::error::InitErrc;
  using enum ck::util::error::InitErrc;
  using namespace ck::config;
    
  void init(const ck::cli::Context& ctx, const ck::cli::InitArgs& args) {
  //   Config cfg;
    
  //   VaultConfig acfg;
  //   get_active_config(cfg, acfg, args);
    
  //   if (!ck::crypto::public_key_exists(args.key_fpr)) {
  //     throw Error<InitErrc>{KeyNotFound, args.key_fpr};
  //   }
    
  //   fs::path dir = fs::path(*acfg.directory) / *acfg.vault;
    
  //   std::error_code ec;
  //   bool created = fs::create_directories(dir, ec);
  //   if (ec) {
  //     throw Error<InitErrc>{CreateDirectoryFailed, ec.message()};
  //   }
    
  //   if (!created) {
  //     throw Error<InitErrc>{AlreadyExists, dir};
  //   } 
    
  //   const fs::path gpg_id_path = dir / ".gpg-id";
  //   std::ofstream gpg_id_file(gpg_id_path, std::ios::out | std::ios::trunc);
  //   if (!gpg_id_file.is_open()) {
  //     throw Error<InitErrc>{OpenGpgIdFailed, std::string(gpg_id_path)};
  //   }
    
  //   gpg_id_file << args.key_fpr << '\n';
  //   if (!gpg_id_file) {
  //     throw Error<InitErrc>{WriteGpgIdFailed, std::string(gpg_id_path)};
  //   }
    
  //   insert_vault(cfg, args.vault_name);
  //   save_config(cfg);
  }
}
