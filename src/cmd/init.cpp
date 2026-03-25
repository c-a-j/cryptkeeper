#include <string>
#include <filesystem>
#include <fstream>

#include "global.hpp"
#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/config/types.hpp"
#include "lib/mount/types.hpp"
#include "lib/index/types.hpp"
#include "./_internal.hpp"
#include "../lib/path/existence.hpp"
#include "../lib/fs/atomic_write.hpp"

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
    
    bool exists = ck::path::directory_exists(vault_path);
    if (exists) {
      throw Error<InitErrc>{AlreadyExists, vault_path.string()};
    } 
    
    const fs::path gpg_id_path = vault_path / GPG_ID_FILE;
    ck::fs::atomic_write(gpg_id_path, args.key_fpr);

    ck::index::Index idx = ck::index::Index::empty(vault_path);
    idx.write();

    std::string msg1;
    msg1 += "Vault " + args.vault_name + " has been initialized";
    std::string msg2;
    msg2 += "use 'ck mount' to mount it";
    
    logger.info(msg1, msg2);
  }
}
