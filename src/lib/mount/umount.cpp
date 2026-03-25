#include <filesystem>

#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "../path/path.hpp"
#include "../path/existence.hpp"
#include "lib/mount/types.hpp"

namespace ck::mount {
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::MountErrc;
  using enum ck::util::error::MountErrc;

  void Mounts::umount(const std::string& alias) {
    fs::path mnt_file = ck::path::mount_file();
    if (ck::path::exists(mnt_file)) {
      deserialize();
    } else {
      throw Error<MountErrc>{MountFileNotFound, mnt_file.string()};
    }
    if (mounts_.contains(alias)) {
      mounts_.erase(alias);
    } else {
      throw Error<MountErrc>{AliasDoesNotExist, alias};
    }
    this->write();
    logger.info(alias + std::string(" has been unmounted"));
    this->print();
  }
}
