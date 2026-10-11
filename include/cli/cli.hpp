#pragma once
#include <CLI/CLI.hpp>

#include "cli/types.hpp"

namespace ck::cli {
  CliArgs parse_cli(CLI::App& app, int argc, char** argv);
}
