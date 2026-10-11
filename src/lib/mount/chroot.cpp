#include <filesystem>

#include "lib/mount/mount.hpp"
#include "lib/config/config.hpp"
#include "lib/mount/error.hpp"
#include "ui/console/console.hpp"
#include "lib/path/parse_path.hpp"
#include "lib/path/existence.hpp"

#include "../path/internal/path.hpp"
#include "./internal/initialized.hpp"
#include "./internal/hash.hpp"

namespace ck::mount {
  using ck::config::cfg;
  using ck::ui::console::console;
  using ck::error::Error;
  using ck::error::MountErrc;
  using enum ck::error::MountErrc;

  void Mounts::chroot(const std::string& path) {
    std::vector<std::string> alias_parts = ck::path::parse_path(path);
    fs::path crypt_path;

    // if alias is single path element, it is assumed to refer to a crypt in
    // cfg.home
    if (alias_parts.size() == 1) {
      crypt_path = fs::path(cfg.home()) / fs::path(path);
    } else {
      crypt_path = fs::path(path);
    }

    // nothing to mount if the crypt isn't initialized
    if (!initialized(crypt_path)) { 
      throw Error<MountErrc>{CryptNotInitialized, path};
    }

    fs::path mnt_file = ck::path::mount_file();
    if (ck::path::file_exists(mnt_file)) {
      this->load();
    }

    // run mount() on a temporary Mounts object to ensure there are no
    // path conflicts with the new root
    State state;
    state.root = {
      .path = crypt_path,
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
    this->state_.root.path = crypt_path;
    this->state_.root.hash = hash();
    this->save();
    console.info("Root crypt has been set to " + crypt_path.string());
    this->print();
  }
}
