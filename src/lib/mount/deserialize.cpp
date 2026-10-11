#include "toml++/toml.hpp"
#include <optional>

#include "lib/mount/error.hpp"
#include "ui/console/console.hpp"
#include "lib/mount/mount.hpp"

namespace {
  using namespace ck::mount;
  using ck::error::Error;
  using ck::error::MountErrc;
  using enum ck::error::MountErrc;

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
      const auto *tbl = node.as_table();
  
      if (tbl == nullptr) { 
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

namespace ck::mount::codec {
  namespace fs = std::filesystem;
  using ck::ui::console::console;
  using ck::error::Error;
  using ck::error::MountErrc;
  using enum ck::error::MountErrc;

  State deserialize(std::string_view toml_text, const std::string& file_path) {
    toml::table mount_toml;
    try {
      mount_toml = toml::parse(toml_text);
    } catch (const toml::parse_error& e) {
      throw Error<MountErrc>{InvalidMountFile, std::string(e.description())};
    }
    
    State state;
    if (auto* tbl = mount_toml["root"].as_table()) {
      state.root = parse_root(*tbl, file_path);
    } else {
      std::string msg = file_path + " does not contain a root mount";
      throw Error<MountErrc>{InvalidMountFile, msg};
    }

    std::unordered_map<std::string, Mount> mounts;
    const auto mount_node = mount_toml["mount"];

    if (mount_node) {
      if (const toml::array* arr = mount_toml["mount"].as_array()) {
        state.mounts = parse_mounts(*arr, file_path);
      } else {
        std::string msg = file_path + " -> mount must be an array";
        throw Error<MountErrc>{InvalidMountFile, msg};
      }
    }

    return state;
  }
}
