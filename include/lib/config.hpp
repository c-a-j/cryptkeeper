#pragma once
#include <filesystem>

#include "types.hpp"

namespace ck::lib::config {
using namespace ck::types;
  std::filesystem::path vault_root();
  void init_config(Config&, Vault&);
  void load_config(Config&);
  void print_config(Config&, Vault&);
  void set_parameter(Config&, Vault&, std::vector<std::string>);
}
