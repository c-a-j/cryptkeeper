#include "toml++/toml.hpp"
#include <filesystem>
#include <sstream>
#include <optional>

#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "lib/mount/types.hpp"
#include "../fio/atomic_write.hpp"
#include "../path/path.hpp"
#include "../path/existence.hpp"


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
  using enum ck::util::error::MountErrc;

  void Mounts::write() {
    ck::path::create_config_dir();
    fs::path mnt_file = ck::path::mount_file();
    bool existed = ck::path::file_exists(mnt_file);
    
    std::ostringstream contents;
    toml::table tbl = serialize(*this);
    contents << tbl << "\n";
    
    ck::fio::atomic_write(mnt_file, contents.str());
    
    if (!existed) {
      logger.info("Created new mount file", mnt_file.string());
    }
  }
}
