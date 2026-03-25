#include <filesystem>

#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "../path/path.hpp"
#include "../path/parse_path.hpp"
#include "../path/existence.hpp"
#include "lib/mount/types.hpp"
#include "lib/config/types.hpp"
#include "./_internal/initialized.hpp"
#include "./_internal/hash.hpp"

namespace ck::mount {
  using ck::config::cfg;
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::MountErrc;
  using enum ck::util::error::MountErrc;

  void Mounts::chroot(const std::string& path) {
    std::vector<std::string> alias_parts = ck::path::parse_path(path);
    fs::path vault_path;
    if (alias_parts.size() == 1) {
      vault_path = fs::path(cfg.home()) / fs::path(path);
    } else {
      vault_path = fs::path(path);
    }

    fs::path mnt_file = ck::path::mount_file();

    if (!initialized(vault_path)) { 
      throw Error<MountErrc>{VaultNotInitialized, path};
    }

    if (ck::path::file_exists(mnt_file)) {
      this->deserialize();
    }
    this->root_.path = vault_path;
    this->root_.hash = hash();
    this->write();
    logger.info("Root vault has been set to " + vault_path.string());
    this->print();
    return;
  }
}
