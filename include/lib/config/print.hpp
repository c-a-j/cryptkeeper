#pragma once
#include "lib/types.hpp"
#include <string>

// print funcs
namespace ck::lib::config {
  using namespace ck::types;
  
  void print_config(const Config& cfg, const Vault& vault);
  void print_parameter(const Config& cfg, const Vault& vault, const std::string& key);
}
