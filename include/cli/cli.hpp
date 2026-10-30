#pragma once
#include "cli/types.hpp"
#include <CLI/CLI.hpp>

namespace ck::cli {
CliArgs parse_cli(CLI::App &app, int argc, char **argv);
}
