#include "toml++/toml.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <optional>

#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "cli/types.hpp"
#include "../path/path.hpp"
#include "lib/mount/types.hpp"


namespace {
  toml::table create_table(const ck::mount::Mount& m, const std::optional<std::string> alias = std::nullopt) {
    toml::table tbl; 
    tbl.insert_or_assign("path", m.path);
    if (alias) { tbl.insert_or_assign("alias", *alias); }
    tbl.insert_or_assign("hash", m.hash);
    return tbl;
  }

  toml::table serialize(const ck::mount::Mounts& mounts) {
    toml::table tbl; 
    tbl.insert("root", create_table(mounts.root()));

    toml::array mnts;
    for (auto [k, mnt] : mounts.mounts()) {
      mnts.push_back(create_table(mnt, k));
    }
    tbl.insert("mount", mnts);
    return tbl;
  }
}

namespace ck::mount {
  namespace fs = std::filesystem;
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::MountErrc;
  using enum ck::util::error::MountErrc;

  void Mounts::write() {
    ck::path::create_config_dir();
    fs::path mnt_file = ck::path::mount_file();
    bool exists = fs::exists(mnt_file);
    std::ofstream out(mnt_file, std::ios::out | std::ios::trunc);
    toml::table tbl = serialize(*this);
    out << tbl << "\n";
    if (!out) {
      throw Error<MountErrc>{WriteMountFailed, std::string(mnt_file)};
    }
    if (!exists) {
      logger.info("Created new mount file", std::string(mnt_file));
    }
  }
}
