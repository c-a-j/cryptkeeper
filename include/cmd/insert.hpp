#pragma once

#include "lib/types.hpp"

namespace ck::cmd::insert {
  using namespace ck::types;
  void insert(const VaultConfig&, const Secret&);
}