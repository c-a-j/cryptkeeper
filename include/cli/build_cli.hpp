#pragma once
#include <CLI/CLI.hpp>
#include "lib/types.hpp"

namespace ck::cli {
using namespace ck::types;
  void build_cli(CLI::App&, Vault&, Secret&, Config&, int&);
}