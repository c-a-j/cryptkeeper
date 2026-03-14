#pragma once
#include <CLI/CLI.hpp>
#include "lib/types.hpp"

namespace ck::cli {
using namespace ck::config;
using namespace ck::secret;
  void build_cli(CLI::App&);
  void build_config(CLI::App&, Config&, Vault&, std::vector<std::string>&);
  void build_init(CLI::App&, Config&, Vault&);
  void build_insert(CLI::App&, Config&,  Vault&, Secret&);
  void build_show(CLI::App&, Config&, Vault&, Secret&);
}