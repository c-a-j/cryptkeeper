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

    // if alias is single path element, it is assumed to refer to a vault in
    // cfg.home
    if (alias_parts.size() == 1) {
      vault_path = fs::path(cfg.home()) / fs::path(path);
    } else {
      vault_path = fs::path(path);
    }

    // nothing to mount if the vault isn't initialized
    if (!initialized(vault_path)) { 
      throw Error<MountErrc>{VaultNotInitialized, path};
    }

    fs::path mnt_file = ck::path::mount_file();
    if (ck::path::file_exists(mnt_file)) {
      this->load();
    }

    // run mount() on a temporary Mounts object to ensure there are no
    // path conflicts with the new root
    State state;
    state.root = {
      .path = vault_path,
      .hash = hash()
    };

    Mounts tmp(state);

    for (const auto& [alias, mount] : this->state_.mounts) {
      try {
        tmp.mount(alias, mount.path);
      } catch (const Error<MountErrc>& _) {
        throw Error<MountErrc>{ChrootConflict, "try removing other mounts first"};
      }
    }

    // set new root
    this->state_.root.path = vault_path;
    this->state_.root.hash = hash();
    this->save();
    logger.info("Root vault has been set to " + vault_path.string());
    this->print();
    return;
  }
}
