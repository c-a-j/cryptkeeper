#pragma once

#include <string>
#include <optional>
#include <vector>
#include <variant>

namespace ck::cli {
  struct RootArgs {
    bool no_color = false;
  };
  
  struct InitArgs {
    std::string vault_name;
    std::string key_fpr;
    std::optional<std::string> directory;
  };

  struct ConfigArgs {
    std::optional<std::string> vault_name;
    std::vector<std::string> set_args;
  };

  struct InsertArgs {
    std::optional<std::string> vault_name;
    std::string path;
    bool pwgen = false;
  };

  struct ShowArgs {
    std::optional<std::string> vault_name;
    std::optional<std::string> path;
  };
  
  struct MountArgs {
    std::optional<std::vector<std::string>> mount;
    std::optional<std::string> path;
    std::optional<bool> list;
  };
  
  struct UmountArgs {
    std::optional<std::string> mount_name;
  };
  
  using CmdArgs = std::variant<
    std::monostate,
    InitArgs,
    ConfigArgs,
    InsertArgs,
    ShowArgs,
    MountArgs,
    UmountArgs
  >;
  
  struct CliArgs {
    RootArgs root;
    CmdArgs cmd;
  };
  
  struct Context {
    RootArgs root_args;
  };
}
