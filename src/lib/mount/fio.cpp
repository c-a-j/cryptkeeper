#include "toml++/toml.hpp"
#include <filesystem>
#include <fstream>

#include "lib/fio/atomic_write.hpp"
#include "lib/mount/error.hpp"
#include "lib/mount/mount.hpp"
#include "lib/path/existence.hpp"
#include "ui/console/console.hpp"

#include "../path/internal/path.hpp"
#include "./internal/codec.hpp"

namespace ck::mount {
namespace fs = std::filesystem;
using ck::error::Error;
using ck::error::MountErrc;
using ck::ui::console::console;
using enum ck::error::MountErrc;

void Mounts::load() {
    const fs::path mnt_file = ck::path::mount_file();

    if (!ck::path::file_exists(mnt_file)) {
        console.debug("ck::mount::load()");
        throw Error<MountErrc>{
            MountFileNotFound, "Initialize a crypt or mount an existing one"};
    }

    std::ifstream in(mnt_file, std::ios::binary);
    std::string text(
        (std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()
    );

    State state = codec::deserialize(text, mnt_file.string());
    this->state_ = state;
}

void Mounts::save() {
    ck::path::create_config_dir();
    fs::path mnt_file = ck::path::mount_file();
    bool existed = ck::path::file_exists(mnt_file);

    std::ostringstream contents;
    toml::table tbl = codec::serialize(this->state_);
    contents << tbl << "\n";

    ck::fio::atomic_write(mnt_file, contents.str());

    if (!existed) {
        console.info("Created new mount file", mnt_file.string());
    }
}
} // namespace ck::mount
