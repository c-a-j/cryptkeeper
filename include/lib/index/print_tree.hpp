#pragma once

#include "lib/index/types.hpp"
#include "lib/index/find.hpp"

namespace ck::index {
  void print_tree(const Node&, const std::string& = "");
}