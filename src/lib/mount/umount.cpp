#include <filesystem>

#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "lib/mount/mount.hpp"
#include "lib/path/existence.hpp"

#include "../path/internal/path.hpp"

namespace ck::mount {
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::MountErrc;
  using enum ck::util::error::MountErrc;

  void Mounts::umount(const std::string& alias) {
    fs::path mnt_file = ck::path::mount_file();
    if (ck::path::exists(mnt_file)) {
      this->load();
    } else {
      throw Error<MountErrc>{MountFileNotFound, mnt_file.string()};
    }
    if (this->state_.mounts.contains(alias)) {
      this->state_.mounts.erase(alias);
    } else {
      throw Error<MountErrc>{AliasDoesNotExist, alias};
    }
    this->save();
    logger.info(alias + std::string(" has been unmounted"));
    this->print();
  }
}
