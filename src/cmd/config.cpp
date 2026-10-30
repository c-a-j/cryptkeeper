#include "lib/config/config.hpp"

#include "cli/types.hpp"
#include <CLI/CLI.hpp>

#include "./internal/handlers.hpp"

namespace ck::cmd {
using ck::config::cfg;

void config(const ck::cli::Context &_, const ck::cli::ConfigArgs &args) {
    std::vector<std::string> set_args = args.set_args;
    cfg.load();

    if (set_args.empty()) {
        cfg.print();
        return;
    }

    if (set_args.size() == 1) {
        cfg.print(set_args[0]);
        return;
    }

    if (set_args.size() == 2) {
        cfg.set(set_args);
        cfg.save();
        return;
    }

    if (set_args.size() > 2) {
        throw CLI::ValidationError("config", "too many arguments");
    }
}
} // namespace ck::cmd
