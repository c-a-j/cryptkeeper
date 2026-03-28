#include <CLI/CLI.hpp>

#include "./_internal.hpp"
#include "cli/types.hpp"
#include "lib/config/types.hpp"


namespace ck::cmd {
  using ck::config::cfg;

  void config(const ck::cli::Context& _, const ck::cli::ConfigArgs& args) {
    std::vector<std::string> set_args = args.set_args;
    
    if (set_args.size() == 0) {
      cfg.print();
      return;
    } else if (set_args.size() == 1) {
      cfg.print(set_args[0]);
      return;
    } else if (set_args.size() == 2) {
      cfg.set(set_args);
      cfg.write();
      return;
    } else if (set_args.size() > 2) {
      throw CLI::ValidationError("config", "too many arguments");
    } 
    return;
  }
}
