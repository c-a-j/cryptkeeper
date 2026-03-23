#include <string>
#include <filesystem>
#include <fstream>

#include "global.hpp"
#include "./_internal.hpp"
#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/config/types.hpp"
#include "lib/mount/types.hpp"

namespace ck::cmd {
  namespace fs = std::filesystem;
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::InitErrc;
  using enum ck::util::error::InitErrc;
  using namespace ck::config;
    
  void init(const ck::cli::Context& _, const ck::cli::InitArgs& args) {
    Config cfg;
    
    if (!ck::crypto::public_key_exists(args.key_fpr)) {
      throw Error<InitErrc>{KeyNotFound, args.key_fpr};
    }
    
    fs::path vault_path;
    
    if (!args.path) {
      vault_path = fs::path(cfg.home()) / args.vault_name;
    } else {
      vault_path = *args.path;
    }
    
    std::error_code ec;
    bool created = fs::create_directories(vault_path, ec);
    if (ec) {
      throw Error<InitErrc>{CreateDirectoryFailed, ec.message()};
    }
    
    if (!created) {
      throw Error<InitErrc>{AlreadyExists, std::string(vault_path)};
    } 
    
    const fs::path gpg_id_path = vault_path / GPG_ID_FILE;
    std::ofstream gpg_id_file(gpg_id_path, std::ios::out | std::ios::trunc);
    if (!gpg_id_file.is_open()) {
      throw Error<InitErrc>{OpenGpgIdFailed, std::string(gpg_id_path)};
    }
    
    gpg_id_file << args.key_fpr << '\n';
    if (!gpg_id_file) {
      throw Error<InitErrc>{WriteGpgIdFailed, std::string(gpg_id_path)};
    }

    const fs::path idx_path = vault_path / INDEX_FILE;
    std::ofstream idx_file(idx_path, std::ios::out | std::ios::trunc);
    if (!idx_file.is_open()) {
      throw Error<InitErrc>{OpenIndexFailed, std::string(idx_path)};
    }
    
    idx_file << "" << '\n';
    if (!idx_file) {
      throw Error<InitErrc>{WriteIndexFailed, std::string(idx_path)};
    }

    std::string msg1;
    msg1 += "Vault " + args.vault_name + " has been initialized: ";
    std::string msg2;
    msg2 += "use 'ck mount' to mount it";
    
    logger.info(msg1, msg2);
  }
}
