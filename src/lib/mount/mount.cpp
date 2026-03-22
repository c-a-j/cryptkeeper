#include "toml++/toml.hpp"
#include <filesystem>

#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "cli/types.hpp"
#include "../path/path.hpp"
#include "lib/mount/types.hpp"

namespace fs = std::filesystem;

namespace {
  bool initialized(const fs::path& path) { return true; }
  std::string hash() { return "0123456789"; }
}

namespace ck::mount {
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::MountErrc;
  using enum ck::util::error::MountErrc;

  Mounts::Mounts() {}

  Mount& Mounts::root() {
    return this->root_;
  }
  
  const Mount& Mounts::root() const {
    return this->root_;
  }

  std::unordered_map<std::string, Mount>& Mounts::mounts() {
    return this->mounts_;
  }
  
  const std::unordered_map<std::string, Mount>& Mounts::mounts() const {
    return this->mounts_;
  }

  void Mounts::mount(const std::string& path, const std::string& alias) {
    fs::path mnt_file = ck::path::mount_file();
    fs::path vault_path = fs::path(path);
    if (!initialized(vault_path)) { 
      throw Error<MountErrc>{VaultNotInitialized, path};
    }

    if (fs::exists(mnt_file)) {
      deserialize();
    } else {
      this->root_.path = vault_path;
      this->root_.hash = hash();
      this->write();
      return;
    }
    if (this->mounts_.contains(alias)) {
      throw Error<MountErrc>{AliasExists, alias};
    }
    this->mounts_[alias].path = vault_path;
    this->mounts_[alias].hash = hash();
    this->write();
  }

  void Mounts::mount(const std::string& path) {
    std::string alias = fs::path(path).filename().string();
    this->mount(path, alias);
  }

  void Mounts::umount(const std::string& alias) {
    fs::path mnt_file = ck::path::mount_file();
    if (fs::exists(mnt_file)) {
      deserialize();
    } else {
      throw Error<MountErrc>{MountFileNotFound, std::string(mnt_file)};
    }
    if (mounts_.contains(alias)) {
      mounts_.erase(alias);
    } else {
      throw Error<MountErrc>{AliasDoesNotExist, alias};
    }
    this->write();
    logger.info(alias + std::string(" has been unmounted"));
  }

  void Mounts::chroot(const std::string& path) {
    fs::path mnt_file = ck::path::mount_file();
    fs::path vault_path = fs::path(path);

    if (!initialized(vault_path)) { 
      throw Error<MountErrc>{VaultNotInitialized, path};
    }

    if (fs::exists(mnt_file)) {
      this->deserialize();
    }
    this->root_.path = vault_path;
    this->root_.hash = hash();
    this->write();
    return;
  }

  void mount(const ck::cli::Context&, const ck::cli::MountArgs&) {
    Mounts m;
    m.mount("foo", "bar");
    m.mount("foo/bar", "foobar");
    m.mount("fooski/bar", "fooski");
    m.mount("fooski/barski", "barski");
    m.mount("some/other/place", "barski/another/dir/fooski");
    m.umount("fooski");

    m.chroot("new/root");
    m.print();
  }
}
