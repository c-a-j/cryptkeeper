#include "lib/mount/mount.hpp"

#include <filesystem>

#include "../index/internal/tree.hpp"
#include "../path/internal/path.hpp"
#include "./internal/hash.hpp"
#include "./internal/initialized.hpp"
#include "lib/config/config.hpp"
#include "lib/index/index.hpp"
#include "lib/mount/error.hpp"
#include "lib/path/parse_path.hpp"
#include "ui/console/console.hpp"

namespace ck::mount {
using ck::config::cfg;
using ck::error::Error;
using ck::error::MountErrc;
using ck::ui::console::console;
using enum ck::error::MountErrc;

void Mounts::mount(const std::string &path, const std::string &alias) {
    fs::path mnt_file = ck::path::mount_file();
    bool mnt_file_exists = fs::exists(mnt_file);

    if (mnt_file_exists) {
        ResolvedPath rp = this->resolve(alias);
        ck::index::Index idx(rp.crypt_path);

        if (ck::index::tree::secret_along_path(&idx.root(), rp.relative_path)) {
            throw Error<MountErrc>{SecretAlongAlias, alias};
        }
    }

    std::vector<std::string> alias_parts = ck::path::parse_path(path);
    fs::path crypt_path;
    if (alias_parts.size() == 1) {
        crypt_path = fs::path(cfg.home()) / fs::path(path);
    } else {
        crypt_path = fs::path(path);
    }

    if (!initialized(crypt_path)) {
        throw Error<MountErrc>{CryptNotInitialized, path};
    }

    // if the mount file does not exist and the crypt is initialized, the
    // crypt is mounted as the root crypt
    // otherwise, the crypt is mounted at the supplied alias
    if (!mnt_file_exists) {
        this->state_.root.path = crypt_path;
        this->state_.root.hash = hash();
        this->save();
        this->print();
        return;
    }

    if (this->state_.mounts.contains(alias)) {
        throw Error<MountErrc>{AliasExists, alias};
    }

    this->state_.mounts[alias].path = crypt_path;
    this->state_.mounts[alias].hash = hash();
    this->save();
    this->print();
}

void Mounts::mount(const std::string &alias) {
    // this function provides a quick way to mount a crypt that is in
    // cfg.core_.home 'ck mount foo' will attempt to mount a crypt on disk at
    // cfg.core_.home/foo supplying an alias comprised of more than one
    // component, ex foo/bar is not supported
    std::vector<std::string> alias_parts = ck::path::parse_path(alias);
    console.debug("Mounts::mount() - adding a mount with a single argument");
    if (alias_parts.size() > 1) {
        throw Error<MountErrc>{
            InvalidArguments, "a path must be specified for this alias"};
    }

    fs::path crypt_path = fs::path(cfg.home()) / fs::path(alias);
    if (initialized(crypt_path)) {
        this->mount(alias, crypt_path);
        return;
    }
    throw Error<MountErrc>{CryptNotInitialized, crypt_path};
}
} // namespace ck::mount
