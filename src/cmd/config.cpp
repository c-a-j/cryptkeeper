#include "lib/config/config.hpp"

#include "cli/types.hpp"
#include <CLI/CLI.hpp>

#include "./internal/handlers.hpp"

namespace ck::cmd {

void config(const ck::cli::Context &_, const ck::cli::ConfigArgs &args) {
    std::vector<std::string> set_args = args.set_args;
    ck::config::Config cfg;
    load_cfg(cfg);

    if (set_args.empty()) {
        print_cfg(cfg);
        return;
    }

    if (set_args.size() == 1) {
        print_cfg(cfg, set_args[0]);
        return;
    }

    if (set_args.size() == 2) {
        set_cfg(cfg, set_args);
        save_cfg(cfg);
        return;
    }

    if (set_args.size() > 2) {
        throw CLI::ValidationError("config", "too many arguments");
    }
}
} // namespace ck::cmd
