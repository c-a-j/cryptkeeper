#include <CLI/CLI.hpp>
#include "cli/cli.hpp"
#include <string>

#include "cli/types.hpp"
#include "util/logger/logger.hpp"

#include "global.hpp"

namespace ck::cli {
  using ck::util::logger::logger;
  
  namespace {
    struct ParsedCmdArgs {
      InitArgs init;
      ConfigArgs config;
      InsertArgs insert;
      ShowArgs show;
      MountArgs mount;
      UmountArgs umount;
    };
    
    void configure_root(CLI::App& app, RootArgs& args) {
      app.require_subcommand(1, 1);
      app.description(std::string(APP_NAME) + " is a command-line tool for managing encrypted secrets.");
      app.footer("More coming soon");
      
      app.add_flag("--no-color", args.no_color, "Disable ANSI code color insertion");
    } 
    
    CLI::App* add_init(CLI::App& app, ParsedCmdArgs& args) {
      auto* init = app.add_subcommand("init", "initialize a new password store");
      init -> add_option("-v,--vault", args.init.vault_name, "vault name") -> required();
      init -> add_option("-k,--key", args.init.key_fpr, "vault key") -> required();
      init -> add_option("-p,--path", args.init.path, "vault path");
      return init;
    }
      
    CLI::App* add_config(CLI::App& app, ParsedCmdArgs& args) {
      auto* config = app.add_subcommand("config", "Print and edit config file");
      config -> description("Print and edit config file");
      // config -> add_option("-v, --vault", args.config.vault_name, "Set configs for a specific vault");
      config -> add_option("args", args.config.set_args, "Key [value]");
      return config;
    }
      
    CLI::App* add_insert(CLI::App& app, ParsedCmdArgs& args) {
      auto* insert = app.add_subcommand("insert", "insert a new secret");
      insert -> add_flag("--pwgen", args.insert.pwgen, "insert a randomly generated password");
      insert -> add_option("path, -p,--path", args.insert.path, "secret path and name (ex cards/mybank/num") -> required();
      return insert;
    }
    
    CLI::App* add_show(CLI::App& app, ParsedCmdArgs& args) {
      auto* show = app.add_subcommand("show", "Show a secret");
      show -> add_option("path, -p, --path", args.show.path, "Secret path");
      return show;
    }
    
    CLI::App* add_mount(CLI::App& app, ParsedCmdArgs& args) {
      auto* mount = app.add_subcommand("mount", "Mount a vault");
      mount -> add_option("args", args.mount.mount, "Secret path");
      mount -> add_flag("-l, --list", args.mount.list, "List mounts");
      mount -> add_option("-p, --path", args.mount.path, "Secret path");
      return mount;
    }
    
    CLI::App* add_umount(CLI::App& app, ParsedCmdArgs& args) {
      auto* umount = app.add_subcommand("umount", "Unmount a vault");
      umount -> add_option("path, -p, --path", args.mount.path, "Secret path");
      return umount;
    }
  }
  
  CliArgs parse_cli(CLI::App& app, int argc, char** argv) {
    ParsedCmdArgs cargs;
    RootArgs rargs;
    
    configure_root(app, rargs);
    auto* init = add_init(app, cargs);
    auto* config = add_config(app, cargs);
    auto* insert = add_insert(app, cargs);
    auto* show = add_show(app, cargs);
    auto* mount = add_mount(app, cargs);
    auto* umount = add_umount(app, cargs);
    
    CliArgs args = CliArgs{
      .root = RootArgs{},
      .cmd = std::monostate{},
    };
    
    if (argc == 1) {
      std::cout << app.help() << '\n';
      return args;
    }
    
    app.parse(argc, argv);
    
    if (init->parsed()) { 
      args.cmd = cargs.init;
      return args; 
    }
    if (config->parsed()) { 
      args.cmd = cargs.config;
      return args; 
    }
    if (insert->parsed()) { 
      args.cmd = cargs.insert;
      return args; 
    }
    if (show->parsed()) { 
      args.cmd = cargs.show;
      return args; 
    }
    if (mount->parsed()) { 
      args.cmd = cargs.mount;
      return args; 
    }
    if (umount->parsed()) { 
      args.cmd = cargs.mount;
      return args; 
    }
    
    return args;
  }
}