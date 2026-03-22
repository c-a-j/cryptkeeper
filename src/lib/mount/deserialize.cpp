#include "toml++/toml.hpp"
#include <filesystem>
#include <optional>

#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "../path/path.hpp"
#include "lib/mount/types.hpp"


namespace ck::mount {
  namespace fs = std::filesystem;
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::MountErrc;
  using enum ck::util::error::MountErrc;

  void Mounts::deserialize() {
    fs::path path = ck::path::mount_file();
    if (!fs::exists(path)) {
      throw Error<MountErrc>{MountFileNotFound, "Initialize a vault or mount an existing one"};
    }
    auto mount_toml = toml::parse_file(std::string(path));
    if (auto* tbl = mount_toml["root"].as_table()) {
      Mount m{};
      for (const auto& field : Mount::fields()) {
        if (auto v = (*tbl)[field.key].value<std::string>()) {
          m.*(field.member) = *v;
        }
      }
    }

    if (auto* mounts = mount_toml["mount"].as_array()) {
      for (auto&& node : *mounts) {
        auto* tbl = node.as_table();
        if (!tbl) {
          continue;
        }
        Mount m{};

        for (const auto& field : Mount::fields()) {
          if (auto v = (*tbl)[field.key].value<std::string>()) {
            m.*(field.member) = *v;
          }
        }
        auto alias = (*tbl)["alias"].value<std::string>();
        if (!alias) {
          throw Error<MountErrc>{NoAlias, std::string(m.path)};
        }
        this->mounts_[*alias] = m;
      }
    }
  }
}
