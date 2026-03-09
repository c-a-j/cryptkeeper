#include <CLI/CLI.hpp>
#include "cmd/init.hpp"
#include "cmd/insert.hpp"
#include "cmd/get.hpp"
#include "cmd/config.hpp"
#include "cli/cli.hpp"
#include "lib/types.hpp"
#include "lib/config/init.hpp"
#include "lib/config/deserialize.hpp"
#include "lib/config/active.hpp"
#include "global.hpp"

namespace ck::cli {
namespace cmd = ck::cmd;
namespace lib = ck::lib;
using namespace ck::types;
  void build_cli(CLI::App& app) {
    // bool verbose = false;
    // app.add_option("-v, --verbose", verbose, "Path verbose output");
    // 
    app.require_subcommand(1, 1);
    
    app.description(std::string(APP_NAME) + " is a command-line tool for managing encrypted secrets.");
    app.footer("More coming soon");
  } 
    
  void build_config(CLI::App& app, Config& cfg, Vault& vault, std::vector<std::string>& set_args) {
    auto* config = app.add_subcommand("config", "Print and edit config file");
    config -> description("Print and edit config file");
    config -> add_option("args", set_args, "Key [value]");
    config -> add_option("-v, --vault", vault.name, "Set configs for a specific vault");
    config -> callback([&] { 
      lib::config::deserialize(cfg); 
      cmd::config::config(cfg, vault, set_args); 
    });
  }
    
  void build_init(CLI::App& app, Config& cfg, Vault& vault) {
    auto* init = app.add_subcommand("init", "initialize a new password store");
    init -> add_option("-v,--vault", vault.name, "vault name") -> required();
    init -> add_option("-k,--key", vault.key_fpr, "vault key") -> required();
    init -> callback([&] { 
      lib::config::init_config(cfg, vault);
      cmd::init::init_vault(cfg, vault); 
    });
  }
    
  void build_insert(CLI::App& app, Config& cfg, Vault& vault, Secret& secret) {
    auto* insert = app.add_subcommand("insert", "insert a new secret");
    insert -> add_option("-v,--vault", vault.name, "store name");
    insert -> add_option("-k,--key", secret.key_fpr, "encryption key");
    insert -> add_option("path, -p,--path", secret.path, "secret path and name (ex cards/mybank/num") -> required();
    insert -> callback([&] { 
      lib::config::deserialize(cfg);
      VaultConfig vcfg = lib::config::get_active_config(cfg, vault);
      cmd::insert::insert(vcfg, secret); 
    });
  }
  
  void build_get(CLI::App& app, Config& cfg, Vault& vault, Secret& secret) {
    auto* get = app.add_subcommand("get", "Get a secret");
    // std::string key;
    // get -> add_option("key", key, "Secret key path") -> required();
    get -> callback([&] { cmd::get::run_get(); });
  }
}
