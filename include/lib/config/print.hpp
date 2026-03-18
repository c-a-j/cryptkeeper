#pragma once
#include "lib/config/types.hpp"
#include "cli/types.hpp"
#include <string>

// print funcs
namespace ck::config {
  void print_config(const Config& cfg, const ck::cli::ConfigArgs&);
  void print_parameter(const Config& cfg, const ck::cli::ConfigArgs&);
}
