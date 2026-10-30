#include <filesystem>
#include <fstream>
#include <string>

#include "cli/error.hpp"

#include "./internal/handlers.hpp"
#include "error.hpp"
#include "global.hpp"
#include "lib/config/config.hpp"
#include "lib/crypto/crypto.hpp"
#include "lib/fio/atomic_write.hpp"
#include "lib/index/index.hpp"
#include "lib/mount/mount.hpp"
#include "lib/path/existence.hpp"
#include "ui/console/console.hpp"

namespace ck::cmd {
namespace fs = std::filesystem;
using ck::error::Error;
using ck::error::InitErrc;
using ck::ui::console::console;
using enum ck::error::InitErrc;
using namespace ck::config;
using ck::mount::mnt;

void init(const ck::cli::Context &_, const ck::cli::InitArgs &args) {
    Config cfg;

    if (!ck::crypto::public_key_exists(args.key_fpr)) {
        throw Error<InitErrc>{KeyNotFound, args.key_fpr};
    }

    fs::path crypt_path;

    if (!args.path) {
        crypt_path = fs::path(home_dir(cfg)) / args.crypt_name;
    } else {
        crypt_path = *args.path;
    }

    bool exists = ck::path::directory_exists(crypt_path);
    if (exists) {
        throw Error<InitErrc>{AlreadyExists, crypt_path.string()};
    }

    const fs::path gpg_id_path = crypt_path / GPG_ID_FILE;
    ck::fio::atomic_write(gpg_id_path, args.key_fpr);

    ck::index::Index idx = ck::index::Index::empty(crypt_path);
    idx.save();

    std::string msg1;
    msg1 += "Crypt " + args.crypt_name + " has been initialized";
    console.info(msg1);

    if (!ck::mount::Mounts::file_exists()) {
        console.info("Welcome to Crypt Keeper - the spooky password manager");
        ck::mount::mnt.chroot(crypt_path);
    }
}
} // namespace ck::cmd
