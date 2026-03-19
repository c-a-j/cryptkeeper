#include <CLI/CLI.hpp>

#include "./_internal.hpp"
#include "cli/types.hpp"

#include "lib/config/deserialize.hpp"
#include "lib/config/print.hpp"
#include "lib/config/set.hpp"
#include "lib/config/save.hpp"

#include "lib/config/types.hpp"


namespace ck::cmd {
  namespace foo = ck::config::refactor;
  void config(const ck::cli::Context& ctx, const ck::cli::ConfigArgs& args) {
    ck::config::Config cfg;
    deserialize(cfg); 
    
    foo::Config new_cfg;
    
    std::vector<std::string> set_args = args.set_args;
    
    if (set_args.size() == 0) {
      new_cfg.print();
      return;
    } else if (set_args.size() == 1) {
      new_cfg.print(set_args[0]);
      return;
    } else if (set_args.size() == 2) {
      new_cfg.set(set_args);
      new_cfg.write();
      return;
    } else if (set_args.size() > 2) {
      throw CLI::ValidationError("config", "too many arguments");
    } 
    return;
  }
}
