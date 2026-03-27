#include "toml++/toml.hpp"
#include <filesystem>
#include <optional>

#include "util/error.hpp"
#include "util/logger/logger.hpp"
#include "../path/path.hpp"
#include "../path/existence.hpp"
#include "lib/mount/types.hpp"

namespace {
  using namespace ck::mount;
  using ck::util::error::Error;
  using ck::util::error::MountErrc;
  using enum ck::util::error::MountErrc;

  Mount parse_root(const toml::table& tbl, const std::string& file_path) {
    Mount m{};
    for (const auto& field : Mount::fields()) {
      std::optional<std::string> value = tbl[field.key].value<std::string>();
      if (!value) {
        std::string msg = std::string(file_path) + " -> "
          + std::string(field.key) + std::string(" contains a non-string element");
        throw Error<MountErrc>{InvalidMountFile, msg}; 
      }
      m.*(field.member) = *value;
    }

    if (m.path.empty()) {
      std::string msg = std::string(file_path)
        + std::string(" has a root mount with no path");
      throw Error<MountErrc>{InvalidMountFile, msg}; 
    }

    return m;
  }

  std::unordered_map<std::string, Mount> parse_mounts(const toml::array& arr, const std::string& file_path) {
    std::unordered_map<std::string, Mount> mounts;
    for (const toml::node& node : arr) {
      auto* tbl = node.as_table();
  
      if (!tbl) { 
        std::string msg = std::string(file_path) 
          + std::string(" -> every mount must be a valid toml table");
        throw Error<MountErrc>{InvalidMountFile, msg}; 
      }

      Mount m{};

      for (const auto& field : Mount::fields()) {
        std::optional<std::string> value = (*tbl)[field.key].value<std::string>();
        if (!value) {
          std::string msg = std::string(file_path) + " -> "
            + std::string(field.key) + std::string(" contains a non-string element");
          throw Error<MountErrc>{InvalidMountFile, msg}; 
        }
        m.*(field.member) = *value;
      }

      std::optional<std::string> alias = (*tbl)["alias"].value<std::string>();
      if (!alias) {
        std::string msg = std::string(file_path) + " -> "
          + std::string(file_path) + std::string(" contains a mount with no alias");
        throw Error<MountErrc>{NoAlias, msg};
      }

      if (m.path.empty()) {
        std::string msg = std::string(file_path)
          + std::string(" has a mount with no path");
        throw Error<MountErrc>{InvalidMountFile, msg}; 
      }

      mounts[*alias] = m;
    }

    return mounts;
  }
}


namespace ck::mount {
  namespace fs = std::filesystem;
  using ck::util::logger::logger;
  using ck::util::error::Error;
  using ck::util::error::MountErrc;
  using enum ck::util::error::MountErrc;

  void Mounts::deserialize() {
    if (!ck::path::file_exists(this->file_)) {
      logger.debug("Mounts::deserialize()");
      throw Error<MountErrc>{MountFileNotFound, "Initialize a vault or mount an existing one"};
    }

    toml::table mount_toml;
    try {
      mount_toml = toml::parse_file(this->file_.string());
    } catch (const toml::parse_error& e) {
      throw Error<MountErrc>{InvalidMountFile, std::string(e.description())};
    }

    Mount root;
    if (auto* tbl = mount_toml["root"].as_table()) {
      root = parse_root(*tbl, this->file_);
    } else {
      std::string msg = std::string(this->file_) + " does not contain a root mount";
      throw Error<MountErrc>{InvalidMountFile, msg};
    }

    std::unordered_map<std::string, Mount> mounts;
    const auto mount_node = mount_toml["mount"];

    if (mount_node) {
      if (const toml::array* arr = mount_toml["mount"].as_array()) {
        mounts = parse_mounts(*arr, this->file_);
      } else {
        std::string msg = std::string(this->file_) + " -> mount must be an array";
        throw Error<MountErrc>{InvalidMountFile, msg};
      }
    }

    this->root_ = root;
    this->mounts_ = mounts;
  }
}
