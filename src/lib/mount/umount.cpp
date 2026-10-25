#include <filesystem>

#include "lib/mount/error.hpp"
#include "lib/mount/mount.hpp"
#include "lib/path/existence.hpp"
#include "ui/console/console.hpp"

#include "../path/internal/path.hpp"

namespace ck::mount {
using ck::error::Error;
using ck::error::MountErrc;
using ck::ui::console::console;
using enum ck::error::MountErrc;

void Mounts::umount(const std::string &alias) {
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
    console.info(alias + std::string(" has been unmounted"));
    this->print();
}
} // namespace ck::mount
