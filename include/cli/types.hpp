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
    std::optional<std::string> path;
  };

  struct ConfigArgs {
    std::optional<std::string> vault_name;
    std::vector<std::string> set_args;
  };

  struct InsertArgs {
    std::string path;
    bool pwgen = false;
  };

  struct ShowArgs {
    std::optional<std::string> vault_name;
    std::optional<std::string> path;
  };
  
  struct MountArgs {
    std::optional<std::string> alias;
    std::optional<std::string> path;
    bool list;
  };
  
  struct UmountArgs {
    std::optional<std::string> alias;
  };

  struct ChrootArgs {
    std::optional<std::string> path;
  };
  
  using CmdArgs = std::variant<
    std::monostate,
    InitArgs,
    ConfigArgs,
    InsertArgs,
    ShowArgs,
    MountArgs,
    UmountArgs,
    ChrootArgs
  >;
  
  struct CliArgs {
    RootArgs root;
    CmdArgs cmd;
  };
  
  struct Context {
    RootArgs root_args;
  };
}
