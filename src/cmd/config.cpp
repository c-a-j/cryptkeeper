#include <CLI/CLI.hpp>

#include "cmd/config.hpp"
#include "lib/types.hpp"
#include "lib/config/load.hpp"
#include "lib/config/print.hpp"
#include "lib/config/set.hpp"
#include "lib/config/save.hpp"


namespace ck::cmd::config {
using namespace ck::types;
  using namespace ck::lib::config;
  
  void config(Config& cfg, Vault& vault, std::vector<std::string>& set_args) {
    load_config(cfg);
    
    if (set_args.size() == 0) {
      print_config(cfg, vault);
      return;
    } else if (set_args.size() == 1) {
      print_parameter(cfg, vault, set_args[0]);
      return;
    } else if (set_args.size() == 2) {
      set_parameter(cfg, vault, set_args);
      save_config(cfg);
      return;
    } else if (set_args.size() > 2) {
      throw CLI::ValidationError("config", "too many arguments");
    } 
    return;
  }
}
