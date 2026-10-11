#include "toml++/toml.hpp"
#include <optional>

#include "lib/mount/error.hpp"
#include "ui/console/console.hpp"
#include "lib/mount/mount.hpp"

namespace {
  toml::table create_table(const ck::mount::Mount& m, const std::optional<std::string>& alias = std::nullopt) {
    toml::table tbl; 
    tbl.insert_or_assign("path", m.path);
    if (alias) { tbl.insert_or_assign("alias", *alias); }
    tbl.insert_or_assign("hash", m.hash);
    return tbl;
  }
}

namespace ck::mount::codec {
  using ck::ui::console::console;
  using enum ck::error::MountErrc;

  toml::table serialize(const ck::mount::State& state) {
    toml::table tbl; 
    tbl.insert("root", create_table(state.root));

    toml::array mnts;
    for (const auto& [k, mnt] : state.mounts) {
      mnts.push_back(create_table(mnt, k));
    }
    tbl.insert("mount", mnts);
    return tbl;
  }
}
